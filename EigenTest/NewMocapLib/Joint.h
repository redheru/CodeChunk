//-----------------------------------------------------------------------------
//                             Joint.h
//
// 関節クラスの宣言部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef HUMAN_JOINT_HEADER_
#define HUMAN_JOINT_HEADER_
#pragma once

#include "TreeNode.h"

#include "EigenMath.h"
//// Eigen Library
//#include <Eigen/Dense>
//using namespace Eigen;

// 説明:
// 関節クラス
class CJoint : public CTreeNode
{
// 説明:
// variables
private:
	//! ノードオフセット
	Vector3f m_vOffset;
	//! 関節回転量
    Quaternionf m_qRotation;
	//! 関節位置
	Vector3f m_vPosition;
	//! ボーンの初期トランスフォーム（オフセット，スケール）
	Matrix4f m_mInit;
	//! 関節ノード名
	char *m_pszName;

 // Eigenライブラリの要素を動的に生成するときに必要
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW


// 説明:
// constructor/destructor
public:
	// 初期化子付コンストラクタ
	CJoint(const Vector3f &offset, const CJoint *parent);
	// デストラクタ
	virtual ~CJoint();

// 説明:
// initializer
public:
	// 初期化関数
	void Initialize(const Vector3f offset, const CJoint *parent);

// 説明:
// accessor
public:
	// ボーンの初期トランスフォームの取得
	Matrix4f GetInitialTransform() const;
	// 関節オフセットベクトルの設定
	void SetOffset(const Vector3f &offset);
	//-----------------------------------------------------------------------------
	// 説明: 関数オフセットベクトルの取得
	// 引数: 
	// 返り値:
	//       オフセットベクトル
	// その他: 
	//-----------------------------------------------------------------------------
	Vector3f GetOffset() const
		{
			return m_vOffset;
		}
	// 関節オフセット行列の取得
	Matrix4f GetOffsetMatrix() const;
	// 回転クォータニオンの設定
	void SetRotation(const Quaternionf &q);
	//-----------------------------------------------------------------------------
	// 説明: 回転クォータニオンの取得
	// 引数: 
	// 返り値:
	//       回転クォータニオン
	// その他: 
	//-----------------------------------------------------------------------------
	Quaternion<float> GetRotation() const
		{
			return m_qRotation;
		}
	// 回転行列の取得
	Matrix4f GetRotationMatrix() const;
	// 位置ベクトルの設定
	void SetPosition(const Vector3f &pos);
	//-----------------------------------------------------------------------------
	// 説明: 位置ベクトルの取得
	// 引数: 
	// 返り値:
	//       位置ベクトル
	// その他: 
	//-----------------------------------------------------------------------------
	Vector3f GetPosition() const
		{
			return m_vPosition;
		}
	// 位置行列の取得
	Matrix4f GetPositionMatrix() const;
	// トランスフォーム行列の取得
	Matrix4f GetTransformMatrix() const;
	// 関節ノード名の設定
	void SetName(const char *name);
	//-----------------------------------------------------------------------------
	// 説明: 関節ノード名の取得
	// 引数: 
	// 返り値:
	//       関節ノード名
	// その他: 
	//-----------------------------------------------------------------------------
	const char* GetName(void) const
		{
			return m_pszName;
		}

// 説明:
// overrides
public:
	// ノードの破棄
	void Destroy();
	//-----------------------------------------------------------------------------
	// 説明: 親関節ノードの取得
	// 引数: 
	// 返り値:
	//       関数ノードへのポインタ
	// その他: 
	//-----------------------------------------------------------------------------
	CJoint* GetParent()
		{
			return static_cast<CJoint*>(CTreeNode::GetParent());
		}
	//-----------------------------------------------------------------------------
	// 説明: 親関節ノードの取得(const)
	// 引数: 
	// 返り値:
	//       関数ノードへのポインタ(const)
	// その他: 
	//-----------------------------------------------------------------------------
	const CJoint* GetParent() const
		{
			return static_cast<const CJoint*>(CTreeNode::GetParent());
		}
	// 親ノードの設定
	void SetParent(const CJoint *node);
	//-----------------------------------------------------------------------------
	// 説明: 子関節ノードの取得
	// 引数: 
	// 返り値:
	//       関数ノードへのポインタ
	// その他: 
	//-----------------------------------------------------------------------------
	CJoint* GetChild(size_t jid)
		{
			return dynamic_cast<CJoint*>(CTreeNode::GetChild(jid));
		}
	//-----------------------------------------------------------------------------
	// 説明: 子関節ノードの取得(const)
	// 引数: 
	// 返り値:
	//       関数ノードへのポインタ(const)
	// その他: 
	//-----------------------------------------------------------------------------
	const CJoint* GetChild(size_t jid) const
		{
			return dynamic_cast<const CJoint*>(CTreeNode::GetChild(jid));
		}
	// 子ノードの追加
	void AddChild(CJoint *node);
	//-----------------------------------------------------------------------------
	// 説明: ルートノードの取得
	// 引数: 
	// 返り値:
	//       ルートノードへのポインタ
	// その他: 
	//-----------------------------------------------------------------------------
	CJoint* GetRoot()
		{
			return dynamic_cast<CJoint*>(CTreeNode::GetRoot());
		}
	//-----------------------------------------------------------------------------
	// 説明: ルートノードの取得(const)
	// 引数: 
	// 返り値:
	//       ルートノードへのポインタ(const)
	// その他: 
	//-----------------------------------------------------------------------------
	const CJoint* GetRoot() const
		{
			return dynamic_cast<const CJoint*>(CTreeNode::GetRoot());
		}
};

#endif // HUMAN_JOINT_HEADER_
