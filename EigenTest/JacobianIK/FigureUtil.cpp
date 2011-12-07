#include "FigureUtil.h"

using namespace std;
using namespace Eigen;

//-----------------------------------------------------------------------------
// 説明: ある関節からの関節の位置の取得
// 引数: 
//       figure [in] キャラクタフィギュアクラスのインスタンス
//       dest_jid [in] 目的位置の関節のインデクス
//       root_jid [in] 出発位置の関節インデクス
// 返り値:
//       関節回転行列
// その他: 
//       root_jid が -1 の場合はWorld座標系から
//-----------------------------------------------------------------------------
Vector3f CFigureUtil::GetJointPosition(const CFigure *figure, int dest_jid, int root_jid)
{
	if(root_jid < 0) // 世界座標での位置を求める場合
		return figure->GetWorldPosition(dest_jid);

	const CJoint* destJoint = figure->GetJoint(dest_jid);	
	const CJoint* rootJoint = figure->GetJoint(root_jid);

	Vector3f vTmp(0, 0, 0), vRootPos(0, 0, 0);
	Matrix4f mTrans = Matrix4f::Identity();
   
	mTrans = destJoint->GetOffsetMatrix();
	NEigenMath::Vector3fTransformCoord(&vRootPos, &vTmp, &mTrans);
	for( const CJoint *pNode = destJoint->GetParent(); pNode != NULL; pNode = pNode->GetParent())
	{
		if(pNode == rootJoint)
			break;

		mTrans = pNode->GetTransformMatrix();
		vTmp = vRootPos;
		NEigenMath::Vector3fTransformCoord(&vRootPos, &vTmp, &mTrans);
	}

	return Vector3f(vRootPos.x(), vRootPos.y(), vRootPos.z());
}

//-----------------------------------------------------------------------------
// 説明: 行列からオイラー角表現(ヨー・ピッチ・ロール)への変換
// 引数: 
//       mat [in] 変換対象の行列
//       yaw [out] ヨー（オイラー角・Y軸周り）
//       pitch [out] ピッチ（オイラー角・X軸周り）
//       roll [out] ロール（オイラー角・Z軸周り）
// 返り値:
//       関節回転行列
// その他: 
//       参考文献 (ただしDirectXなので座標系が異なる) http://www.tmps.org/index.php?%A5%AF%A5%A9%A1%BC%A5%BF%A5%CB%A5%AA%A5%F3%B5%D5%B1%BF%C6%B0%B3%D8
//-----------------------------------------------------------------------------
void CFigureUtil::Mat2EulerYPR(const Matrix4f& mat, float& yaw, float& pitch, float& roll)
{
	if( mat(2,1) > 0.998f) // north pole
	{
		yaw = atan2f( mat(0,2), mat(2,2) );
		pitch = static_cast<float>( M_PI / 2.f);
		roll = 0.f;
		return;
	}

	if( mat(2,1) < -0.998f) // north pole
	{
		yaw = atan2f( mat(0,2), mat(2,2) );
		pitch = static_cast<float>( -M_PI / 2.f);
		roll = 0.f;
		return;
	}

	yaw = atan2f( -mat(2,0), mat(2,2) );
	pitch = atan2f( -mat(0,1), mat(1,1) );
	roll = asinf( mat(2,1) );

	return;
}

//-----------------------------------------------------------------------------
// 説明: ヤコビアン行列を求める
// 引数: 
//       figure [in] キャラクタフィギュアクラスのインスタンス
//       dest_jid [in] 目的位置の関節のインデクス
//       root_jid [in] 出発位置の関節インデクス
// 返り値:
//       関節回転行列
// その他: 
//       root_jid が -1 の場合はWorld座標系から
//-----------------------------------------------------------------------------
Matrix4f CFigureUtil::GetJacobian(const CFigure* figure, int dest_jid, int root_jid)
{
	// 必要な情報
	// - 指定のJoint間の回転行列
	// - 指定のJoint間のオフセット
	// - 制約条件？( 7自由度の解析的IKの場合は必要)
	Matrix4f matJac = Matrix4f::Identity();

	std::vector<Matrix4f, aligned_allocator<Matrix4f>> rotMat;
	std::vector<Matrix4f, aligned_allocator<Matrix4f>> offsetMat;
	int linkNum;
	if((linkNum = GetLinkNum(figure, rotMat, offsetMat, dest_jid, root_jid)) > 0 )  //ジョイント間のリンク数を求める
	{
		for( int jid = 0; jid < linkNum; ++jid)
		{
			std::vector<Matrix4f, aligned_allocator<Matrix4f>> dpose(linkNum);
			for( int i = 0; i < linkNum; ++i) // リンクそれぞれの回転行列を代入
				dpose[i] = rotMat[i];

			Matrix4f dm[3], m[3];
			float yaw, pitch, roll;
			Mat2EulerYPR(rotMat[jid], yaw, pitch, roll);   // オイラー角を求める
			DiffMatEulerDomain( dm[0], dm[1], dm[2], roll, pitch, yaw); // それぞれの関節の偏微分を求める

			Matrix4f* Matrix4fRotationZ( &m[0], roll);
			Matrix4f* Matrix4fRotationX( &m[0], pitch);
			Matrix4f* Matrix4fRotationY( &m[0], yaw);

			// 続きはここから。


		}

	}

	return matJac;
}

//-----------------------------------------------------------------------------
// 説明: 指定したジョイント間のリンクの数を求める
// 引数: 
//       rotMat [in] リンクそれぞれの回転行列
//       offsetMat [in] リンクそれぞれのオフセット行列
//       figure [in] キャラクタフィギュアクラスのインスタンス
//       dest_jid [in] 目的位置の関節のインデクス
//       root_jid [in] 出発位置の関節インデクス
// 返り値:
//       リンクの数
// その他: 
//       指定したジョイントも数に含まれる
//-----------------------------------------------------------------------------
int CFigureUtil::GetLinkNum(const CFigure* figure, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& rotMat, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& offsetMat, int dest_jid, int root_jid)
{
	const CJoint* destJoint = figure->GetJoint(dest_jid);	
	const CJoint* rootJoint = figure->GetJoint(root_jid);

	int linkCount = 1;  // 自分自身を含める
	rotMat.push_back(destJoint->GetRotationMatrix());
	offsetMat.push_back(destJoint->GetOffsetMatrix());
	for( const CJoint *pNode = destJoint->GetParent(); pNode != NULL; pNode = pNode->GetParent())
	{
		rotMat.push_back(pNode->GetRotationMatrix());
		offsetMat.push_back(pNode->GetOffsetMatrix());
		linkCount++;

		if(pNode == rootJoint)
			return linkCount;
	}
	return linkCount;  // フィギュアのルートからとなる
}

//-----------------------------------------------------------------------------
// 説明: 各軸に対して偏微分した場合の行列をそれぞれ求める
// 引数: 
//       dmYaw [out] ヨー(Y軸)周りに対して偏微分した場合の行列
//       dmPitch [out] ピッチ(X軸)周りに対して偏微分した場合の行列
//       dmRoll [out] ロール(Z軸)周りに対して偏微分した場合の行列
//       yaw [in] ヨー周りの回転量
//       pitch [in] ピッチ周りの回転量
//       roll [in] ロール周りの回転量
// 返り値:
//       リンクの数
// その他: 
//       指定したジョイントも数に含まれる
//-----------------------------------------------------------------------------
void CFigureUtil::DiffMatEulerDomain(Matrix4f& dmYaw, Matrix4f& dmPitch, Matrix4f& dmRoll, float yaw, float pitch, float roll)
{
	// Y軸周り
	dmYaw = Matrix4f::Identity();
	dmYaw(0,0) = -sinf(yaw);
	dmYaw(0,2) = -cosf(yaw);
	dmYaw(1,1) = 0.f;
	dmYaw(2,0) = cosf(yaw);
	dmYaw(2,2) = -sinf(yaw);

	// X軸周り
	dmPitch = Matrix4f::Identity();
	dmPitch(0,0) = 0.f;
	dmPitch(1,1) = -sinf(pitch);
	dmPitch(1,2) = cosf(pitch);
	dmPitch(2,1) = -cosf(pitch);
	dmPitch(2,2) = -sinf(pitch);

	// Z軸周り
	dmRoll = Matrix4f::Identity();
	dmRoll(0,0) = -sinf(roll);
	dmRoll(0,1) = cosf(roll);
	dmRoll(1,0) = -cosf(roll);
	dmRoll(2,1) = -sinf(roll);
	dmRoll(2,2) = 0.f;
}