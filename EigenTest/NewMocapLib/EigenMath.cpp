//-----------------------------------------------------------------------------
//                             EigenMath.h
//
// Eigenライブラリの補助関数群
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/04 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#include "EigenMath.h"

//-----------------------------------------------------------------------------
// 説明: 度をラジアンに変換する
// 引数: 
//       degree [in] ラジアンに変換する値（度数単位)
// 返り値:
//       ラジアンの単位に変換した値
// その他: 
//-----------------------------------------------------------------------------
float NEigenMath::ToRadian(float degree)
{
	return (degree * (EigenPI / 180.0f));
}

//-----------------------------------------------------------------------------
// 説明: ラジアンを度に変換する
// 引数: 
//       radian [in] 度に変換する値（ラジアン単位)
// 返り値:
//       度数に変換した値
// その他: 
//-----------------------------------------------------------------------------
float NEigenMath::ToDegree(float radian)
{
	return (radian) * (180.0f / EigenPI);
}

//-----------------------------------------------------------------------------
// 説明: 2つの3Dベクトルの外積を計算する
// 引数: 
//       pOut [in,out] 演算結果であるベクトルのポインタ
//       pV1 [in] 処理の基になるベクトルのポインタ
//       pV2 [in] 処理の基になるベクトルのポインタ
// 返り値:
//       2つの3Dベクトルの外積であるベクトルへのポインタ
// その他: 
//-----------------------------------------------------------------------------
Vector3f* NEigenMath::Vector3fCross(Vector3f* pOut, const Vector3f* pV1, const Vector3f* pV2)
{
	*pOut = pV1->cross(*pV2);
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: 行列を各軸ごとにスケーリングする
// 引数: 
//       pOut [in,out] 演算結果を保持する行列のポインタ
//       sx [in] X軸のスケール値
//       sy [in] Y軸のスケール値
//       sz [in] Z軸のスケール値
// 返り値:
//       スケールを変換した行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fScaling( Matrix4f *pOut, float sx, float sy, float sz )
{
	Transform<float, 3, Affine> t; t.matrix() = *pOut;
	t.scale(Vector3f(sx, sy, sz));
	*pOut = t.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: 回転ベクトルから回転行列を求める
// 引数: 
//       pOut [in,out] 演算結果を保持する行列のポインタ
//       pV [in] 軸の角度を識別するベクトルへのポインタ
//       angle [in] 回転の角度(ラジアン単位)
// 返り値:
//       指定された軸を回転軸として回転した行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationAxis( Matrix4f *pOut, const Vector3f *pV, float angle )
{
	Transform<float, 3, Affine> tmp;
	tmp = Translation3f(0.f, 0.f, 0.f) * AngleAxisf(angle, *pV);
	*pOut = tmp.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: 移動行列を作成する
// 引数: 
//       pOut [in,out] 演算結果を保持する行列のポインタ
//       x [in] X軸のオフセット
//       y [in] Y軸のオフセット
//       z [in] Z軸のオフセット
// 返り値:
//       演算結果を持つ行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fTranslation( Matrix4f *pOut, float x, float y, float z)
{
	Transform<float, 3, Affine> t = Affine3f::Identity();
	t.translate(Vector3f(x, y, z));
	*pOut = t.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: クォータニオンから回転行列を作成する
// 引数: 
//       pOut [in,out] 演算結果を保持する行列のポインタ
//       pQ [in] 処理の基になるクォータニオンへのポインタ
// 返り値:
//       ソースのクォータニオンから構築された行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationQuaternion( Matrix4f *pOut, const Quaternionf* pQ)
{
	*pOut = Matrix4f::Identity();
	pOut->block(0,0,3,3) = Matrix3f(*pQ);
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: 回転行列からクォータニオンを作成する
// 引数: 
//       pOut [in,out] 演算結果であるクォータニオンのポインタ
//       pQ [in] 処理の基になる行列へのポインタ
// 返り値:
//       回転用列から作成されたクォータニオンへのポインタ
// その他: 
//-----------------------------------------------------------------------------
Quaternionf* NEigenMath::QuaternionfRotationMatrix( Quaternionf *pOut, const Matrix4f* pM)
{
	AngleAxisf tmp;tmp = Matrix3f(pM->block(0,0,3,3));
	*pOut = Quaternionf(tmp);
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: 指定された行列により3Dベクトルをトランスフォームし、その結果をW=1に射影する
// 引数: 
//       pOut [in,out] 演算結果であるベクトルのポインタ
//       pV [in] 処理の基になるベクトルへのポインタ
//       pM [in] 処理の基になる行列へのポインタ
// 返り値:
//       トランスフォームされたベクトルのポインタ
// その他: 
//-----------------------------------------------------------------------------
Vector3f* NEigenMath::Vector3fTransformCoord(Vector3f* pOut, const Vector3f* pV, const Matrix4f* pM)
{
	Vector4f tmpV = Vector4f(pV->x(), pV->y(), pV->z(), 1.f);
	tmpV = tmpV.transpose() * (*pM);
	*pOut = tmpV.head(3);
	return pOut;
}

//-----------------------------------------------------------------------------
// 説明: X軸を回転軸として回転する行列を作成する
// 引数: 
//       pOut [in,out] 演算結果である行列のポインタ
//       angle [in] 回転の角度(ラジアン単位)
// 返り値:
//       X軸を回転軸として回転した行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationX( Matrix4f *pOut, float angle)
{
	const Vector3f axisX(1.f, 0.f, 0.f);
	return Matrix4fRotationAxis(pOut, &axisX, angle);
}

//-----------------------------------------------------------------------------
// 説明: Y軸を回転軸として回転する行列を作成する
// 引数: 
//       pOut [in,out] 演算結果である行列のポインタ
//       angle [in] 回転の角度(ラジアン単位)
// 返り値:
//       Y軸を回転軸として回転した行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationY( Matrix4f *pOut, float angle)
{
	const Vector3f axisY(0.f, 1.f, 0.f);
	return Matrix4fRotationAxis(pOut, &axisY, angle);
}

//-----------------------------------------------------------------------------
// 説明: Z軸を回転軸として回転する行列を作成する
// 引数: 
//       pOut [in,out] 演算結果である行列のポインタ
//       angle [in] 回転の角度(ラジアン単位)
// 返り値:
//       Z軸を回転軸として回転した行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationZ( Matrix4f *pOut, float angle)
{
	const Vector3f axisZ(0.f, 0.f, 1.f);
	return Matrix4fRotationAxis(pOut, &axisZ, angle);
}

//-----------------------------------------------------------------------------
// 説明: あるベクトルをあるベクトルに変換する行列を求める
// 引数: 
//       pOut [in,out] 演算結果である行列のポインタ
//       src [in] 元のベクトル
//       dst [in] 変換先のベクトル
// 返り値:
//       あるベクトルをあるベクトルに変換する行列へのポインタ
// その他: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::fromTwoVector3f( Matrix4f *pOut, const Vector3f &src, const Vector3f &dist)
{
	Quaternionf q; q = Quaternionf::Identity();
	Matrix4f mat; mat = Matrix4f::Identity();

	Vector3f cross = src.cross(dist);

	//no difference
	if(cross.norm() == 0)
	{
		return pOut = &mat;		
	}
	Vector3f axisVec = cross / cross.norm();
	
	float dot = src.dot(dist);
	float cosTheta =  dot / ( src.norm() * dist.norm() );

	float cosThetaDiv2 = sqrt( 0.5f * (1.f + cosTheta) );
	float sinThetaDiv2 = 0.5f * (1.f - cosTheta);

	if( sinThetaDiv2 >= 0)
	{
		sinThetaDiv2 = sqrt( sinThetaDiv2 );
	}
	else
	{
		sinThetaDiv2 = 0.0;
	}
	
	q.w() = cosThetaDiv2;
	q.x() = axisVec.x() * sinThetaDiv2;
	q.y() = axisVec.y() * sinThetaDiv2;
	q.z() = axisVec.z() * sinThetaDiv2;

	NEigenMath::Matrix4fRotationQuaternion(pOut, &q);
	return pOut;
}
