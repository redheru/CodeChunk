//-----------------------------------------------------------------------------
//                             MotionData.cpp
//
// モーションデータクラス定義部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

//#include "StdAfx.h"
#include "MotionData.h"

//-----------------------------------------------------------------------------
// 説明: デフォルトコンストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CMotionData::CMotionData(void)
{
	m_nJoints = NULL;
	m_nFrames = NULL;
	m_nFrameTime = NULL;
	m_pPosition = NULL;
	m_pRotation = NULL;
}

//-----------------------------------------------------------------------------
// 説明: 初期化子付コンストラクタ
// 引数: 
//       frames [in] フレーム数
//       joints [in] 関節数
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CMotionData::CMotionData(size_t frames, size_t joints)
{
	m_nJoints = NULL;
	m_nFrames = NULL;
	m_nFrameTime = NULL;
	m_pPosition = NULL;
	m_pRotation = NULL;
	Initialize(frames, joints);
}


//-----------------------------------------------------------------------------
// 説明: コピーコンストラクタ
// 引数: 
//       src [in] コピー元
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CMotionData::CMotionData(const CMotionData &src)
{
	m_nJoints = NULL;
	m_nFrames = NULL;
	m_nFrameTime = NULL;
	m_pPosition = NULL;
	m_pRotation = NULL;

	Initialize(src.NumFrames(), src.NumJoints());
	::memcpy(m_pPosition, src.m_pPosition, sizeof(Vector3f) * NumFrames());
	::memcpy(m_pRotation, src.m_pRotation, sizeof(Quaternionf) * NumFrames() * NumJoints()); 
}

//-----------------------------------------------------------------------------
// 説明: デストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CMotionData::~CMotionData(void)
{
	Destroy();
}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
//       frames [in] フレーム数
//       joints [in] 関節数
// 返り値:
//       true 初期化成功
//       false 初期化失敗
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::Initialize(size_t frames, size_t joints)
{
	Destroy();
	return Allocate(frames, joints);
}

//-----------------------------------------------------------------------------
// 説明: 代入演算子
// 引数: 
//       src [in] 代入元
// 返り値:
//        自身への参照
// その他: 
//-----------------------------------------------------------------------------
CMotionData& CMotionData::operator =(const CMotionData &src)
{
	if (!Initialize(src.NumFrames(), src.NumJoints()))
		return *this;

	::memcpy(m_pPosition, src.m_pPosition, sizeof(Vector3f) * NumFrames());
	::memcpy(m_pRotation, src.m_pRotation, sizeof(Quaternionf) * NumFrames() * NumJoints()); 
	return *this;
}


//-----------------------------------------------------------------------------
// 説明: メモリ確保関数
// 引数: 
//      frames [in] フレーム数
//      joints [in] 関節数
// 返り値:
//       true メモリ確保成功
//       false メモリ確保失敗
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::Allocate(size_t frames, size_t joints)
{
	m_pPosition = new Vector3f[frames];
	m_pRotation = new Quaternionf[frames * joints];
	if (m_pRotation == NULL || m_pPosition == NULL)
	{
		Destroy();
		return false;
	}

	m_nJoints = joints;
	m_nFrames = frames;

	Vector3f zerov;
	Quaternionf iq(0, 0, 0, 1);
	for (size_t f = 0;f < m_nFrames;++f)
	{
		POS(f) = zerov;
		for (size_t j = 0;j < m_nJoints;++j)
			ROT(f, j) = iq;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 説明: メモリ解放関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CMotionData::Destroy()
{
	if (m_pPosition != NULL)
	{
		delete[] m_pPosition;
		m_pPosition = NULL;
	}
	if (m_pRotation != NULL)
	{
		delete[] m_pRotation;
		m_pRotation = NULL;
	}
	m_nJoints = 0;
	m_nFrames = 0;
}

//-----------------------------------------------------------------------------
// 説明: データ利用可能状態
// 引数: 
// 返り値:
//       true データ利用可
//       false データ利用不可
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::IsActive() const
{
	return m_pPosition != NULL && m_pRotation != NULL;
}

//-----------------------------------------------------------------------------
// 説明: 関節回転量の設定（クォータニオン）
// 引数: 
//       frm [in] フレーム番号
//       jid [in] 関節インデクス
//       q [in] 回転クォータニオン
// 返り値:
//       true 設定成功
//       false 設定失敗
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::SetRotation(size_t frm, size_t jid, const Quaternionf &q)
{
	if (!IsRange(frm, jid))
		return false;
	ROT(frm, jid) = q;
	return true;
}

//-----------------------------------------------------------------------------
// 説明: 関節回転量の設定（回転行列）
// 引数: 
//       frm [in] フレーム番号
//       jid [in] 関節インデクス
//       m [in] 回転行列
// 返り値:
//       true 設定成功
//       false 設定失敗
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::SetRotation(size_t frm, size_t jid, const Matrix4f &m)
{
	if (!IsRange(frm, jid))
		return false;
	NEigenMath::QuaternionfRotationMatrix(&ROT(frm,jid), &m);
	//AngleAxisf tmp;tmp = Matrix3f(m.block(0,0,3,3));
	//ROT(frm, jid) = Quaternionf(tmp);
	//QuaternionfRotationMatrix(&ROT(frm, jid), &m);
	return true;
}

//-----------------------------------------------------------------------------
// 説明: ルート位置の設定
// 引数: 
//       frm [in] フレーム番号
//       v [in] 位置ベクトル
// 返り値:
//       true 設定成功
//       false 設定失敗
// その他: 
//-----------------------------------------------------------------------------
bool CMotionData::SetPosition(size_t frm, const Vector3f &v)
{
	if (!IsRange(frm))
		return false;
	POS(frm) = v;
	return true;
}

//-----------------------------------------------------------------------------
// 説明: 関節回転クォータニオンの取得
// 引数: 
//       frm [in] フレーム番号
//       jid [in] 関節インデクス
// 返り値:
//       関節回転クォータニオン
// その他: 
//-----------------------------------------------------------------------------
Quaternionf CMotionData::GetRotation(size_t frm, size_t jid) const
{
	if (!IsRange(frm, jid))
		return Quaternionf() = Quaternionf(0.f, 0.0f, 0.f, 1.0f);
	return ROT(frm, jid);
}

//-----------------------------------------------------------------------------
// 説明: 関節回転行列の取得
// 引数: 
//       frm [in] フレーム番号
//       jid [in] 関節インデクス
// 返り値:
//       関節回転行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CMotionData::GetRotationMatrix(size_t frm, size_t jid) const
{
	Matrix4f m = Matrix4f::Identity();
	if (!IsRange(frm, jid))
		return m;
	NEigenMath::Matrix4fRotationQuaternion(&m, &ROT(frm, jid));
	//m.block(0,0,3,3) = Matrix3f( ROT(frm, jid) );
	//Matrix4fRotationQuaternion(&m, &ROT(frm, jid));
	return m;
}


//-----------------------------------------------------------------------------
// 説明: ルート位置ベクトルの取得
// 引数: 
//       frm [in] フレーム番号
// 返り値:
//       ルート位置ベクトル
// その他: 
//-----------------------------------------------------------------------------
Vector3f CMotionData::GetPosition(size_t frm) const
{
	if (!IsRange(frm))
		return Vector3f() = Vector3f::Identity();
	return POS(frm);
}

//-----------------------------------------------------------------------------
// 説明: ルート位置行列の取得
// 引数: 
//       frm [in] フレーム番号
// 返り値:
//       ルート位置行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CMotionData::GetPositionMatrix(size_t frm) const
{
	Matrix4f m = Matrix4f::Identity();
	if (!IsRange(frm))
		return m;
	Vector3f v = POS(frm);
	NEigenMath::Matrix4fTranslation(&m, v.x(), v.y(), v.z());
	//Matrix4fTranslation(&m, v.x, v.y, v.z);
	return m;
}
