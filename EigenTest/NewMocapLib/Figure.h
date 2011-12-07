//-----------------------------------------------------------------------------
//                             Figure.h
//
// キャラクタフィギュアクラス宣言部 
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef HUMAN_FIGURE_CLASS_HEADER_FILE_
#define HUMAN_FIGURE_CLASS_HEADER_FILE_
#pragma once

#include "Joint.h"
#include "MotionData.h"


// 説明:
// キャラクタフィギュアクラス
class CFigure
{
// 説明:
// variables
private:
	//! ルートノード
	CJoint *m_pRoot;
	//! 関節ノードリスト
	std::vector<CJoint*> m_vecJoints;

// 説明:
// constructor/destructor
public:
	// デフォルトコンストラクタ
	CFigure();
	// 初期化子付コンストラクタ
	CFigure(CJoint *root);
	// デストラクタ
	virtual ~CFigure(void);

// 説明:
// initializer
public:
	void Initialize(CJoint *root);

// 説明:
// internal operator
private:
	// スケルトン木構造からのノードリストの作成
	void ListingJoint(CJoint *root);

// 説明:
// attributes
public:
	//-----------------------------------------------------------------------------
	// 説明: フィギュアが利用可能かチェック
    // 引数: 
	// 返り値:
	// その他: 
	//        true 利用可能
	//        false 利用不可
	//-----------------------------------------------------------------------------
	bool IsActive() const;
	// 関節総数の取得
	size_t NumJoints() const;

// 説明:
// accessor
public:
	// ルート位置の設定
	void SetRootPosition(const Vector3f &pos);
	// ルート位置の取得
	Vector3f GetRootPosition() const;
	// ルート方向の設定
	void SetRootOrientation(const Quaternionf &rot);
	// ルート方向の取得
	Quaternionf GetRootOrientation() const;
	// 関節回転量の設定
	void SetJointRotation(size_t jid, const Quaternionf &rot);
	// 関節回転量の取得
	Quaternionf GetJointRotation(size_t jid) const;

	// ルートノードへのポインタの取得
	CJoint* GetRoot();
	// ルートノードへのポインタの取得(const)
	const CJoint* GetRoot() const;
	// 関節ノードへのポインタの取得
	CJoint* GetJoint(size_t jid);
	// 関節ノードへのポインタの取得(const)
	const CJoint* GetJoint(size_t jid) const;

	// 関節ノードへのポインタからノードインデクスの検索
	int FindNode(CJoint *node) const;
	// 関節ノード名からノードインデクスの検索
	int FindNode(const char *name) const;
	// 関節ノードがエフェクタかチェック
	bool IsEffector(size_t jid) const;

// 説明:
// forward kinematics
public:
	// 関節ノードのワールド座標ベクトルの取得
	Vector3f GetWorldPosition(size_t jid) const;

// 説明:
// motion data
public:
	bool SetPose(const CMotionData *mot, size_t frm);
};

#endif //HUMAN_FIGURE_CLASS_HEADER_FILE_
