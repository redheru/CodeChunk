//-----------------------------------------------------------------------------
//                             Joint.cpp
//
// 関節クラス定義部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

//#include "stdafx.h"
#include "Joint.h"

//-----------------------------------------------------------------------------
// 説明: 初期化子付コンストラクタ
// 引数: 
//       offset [in] オフセットベクトル
//       parent [in] 親関節ノードへのポインタ
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CJoint::CJoint(const Vector3f &offset, const CJoint* parent)
{
	m_pszName = NULL;
	Initialize(offset, parent);
}

//-----------------------------------------------------------------------------
// 説明: デストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CJoint::~CJoint(void)
{
}

//-----------------------------------------------------------------------------
// 説明: ノードの破棄
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::Destroy()
{
	if (m_pszName != NULL)
		delete[] m_pszName;
	delete this;
}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
//       offset [in] オフセットベクトル
//       parent [in] 親関節ノードへのポインタ
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::Initialize(Vector3f offset, const CJoint *parent)
{
	SetOffset(offset);
	SetRotation(Quaternionf(0, 0, 0, 1));
	SetPosition(Vector3f(0, 0, 0));
	SetParent(parent);
	if (m_pszName != NULL)
		delete[] m_pszName;
	m_pszName = NULL;
}

//-----------------------------------------------------------------------------
// 説明: ボーンの初期トランスフォーム．ベクトル(0, 0, 1)に対して定義．
// 引数: 
// 返り値:
//       トランスフォーム行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetInitialTransform() const
{
	return m_mInit;
}

//-----------------------------------------------------------------------------
// 説明: オフセットベクトルの設定
// 引数: 
//       offset [in] オフセットベクトル
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::SetOffset(const Vector3f &offset)
{
	m_vOffset = offset;
	float offset_len = offset.norm();

	if (offset_len <= 0)
	{
		m_mInit *= 0;
	}
	else
	{
		Vector3f vz(0, 0, 1), vaxis(0, 0, 0);

		float angle = acosf(offset.z() / offset_len);
		NEigenMath::Vector3fCross(&vaxis, &vz, &offset);
		//D3DXVec3Cross(&vaxis, &vz, &offset);

		NEigenMath::Matrix4fScaling(&m_mInit, 0.2f * offset_len, 0.2f * offset_len, offset_len);
		//m_mInit *= DiagonalMatrix<float,3>( 0.2f * offset_len, 0.2f * offset_len, offset_len);
		m_mInit(3,2) += offset_len / 2.0f;

		Matrix4f tmp;
		//Transform<float, 3, Affine> tmp;
		//tmp = Translation3f(0.f,0.f,0.f) * AngleAxisf(angle, vaxis);
		NEigenMath::Matrix4fRotationAxis(&tmp, &vaxis, angle);
		m_mInit = m_mInit * tmp;
	}
}

//-----------------------------------------------------------------------------
// 説明: オフセット行列の取得
// 引数: 
// 返り値:
//       オフセット行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetOffsetMatrix() const
{
	Matrix4f m;
	//Transform<float, 3, Affine> t = Affine3f::Identity();
	//t.translate(m_vOffset);
	//(Translation<float, 3>(m_vOffset.x(), m_vOffset.y(), m_vOffset.z()));
	NEigenMath::Matrix4fTranslation(&m, m_vOffset.x(), m_vOffset.y(), m_vOffset.z()); 
	return m;
}

//-----------------------------------------------------------------------------
// 説明: 回転クォータニオンの設定
// 引数: 
//       q [in] 回転クォータニオン
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::SetRotation(const Quaternionf &q)
{
	m_qRotation = q;
}

//-----------------------------------------------------------------------------
// 説明: 位置ベクトルの設定
// 引数: 
//       pos [in] 位置ベクトル
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::SetPosition(const Vector3f &pos)
{
	m_vPosition = pos;
}

//-----------------------------------------------------------------------------
// 説明: 回転行列の取得
// 引数: 
// 返り値:
//       回転行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetRotationMatrix() const
{
	Matrix4f m;
	NEigenMath::Matrix4fRotationQuaternion(&m, &m_qRotation);
	//Matrix4f m = Matrix4f::Identity();
	//m.block(0,0,3,3) = Matrix3f(m_qRotation);
	return m;
}

//-----------------------------------------------------------------------------
// 説明: 位置行列の取得
// 引数: 
// 返り値:
//       位置行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetPositionMatrix() const
{
	Matrix4f m;
	NEigenMath::Matrix4fTranslation(&m, m_vPosition.x(), m_vPosition.y(), m_vPosition.z()); 
	//Transform<float, 3, Affine> t = Affine3f::Identity();
	//t.translate(m_vPosition);
	return m;
	//Translation3f(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

//-----------------------------------------------------------------------------
// 説明: 関節トランスフォーム行列の取得
// 引数: 
// 返り値:
//       関節トランスフォーム行列
// その他: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetTransformMatrix() const
{
	return GetRotationMatrix() * GetOffsetMatrix() * GetPositionMatrix();
}

//-----------------------------------------------------------------------------
// 説明: 親関節ノードの取得
// 引数: 
// 返り値:
//       親関節ノードへのポインタ
// その他: 
//-----------------------------------------------------------------------------
void CJoint::SetParent(const CJoint *node)
{
	CTreeNode::SetParent(node);
}

//-----------------------------------------------------------------------------
// 説明: 子関節ノードの追加
// 引数: 
//       node [in] 子関節ノードへのポインタ
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::AddChild(CJoint *node)
{
	CTreeNode::AddChild(node);
}

//-----------------------------------------------------------------------------
// 説明: 関節名の設定
// 引数: 
//       name [in] 関節名
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CJoint::SetName(const char *name)
{
	if (name == NULL)
		return;

	if (m_pszName != NULL)
		delete[] m_pszName;
	if (m_pszName = new char[strlen(name) + 1], m_pszName == NULL)
		return;
	strcpy(m_pszName, name);
}
