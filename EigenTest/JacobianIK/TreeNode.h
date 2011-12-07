//-----------------------------------------------------------------------------
//                             TreeNode
//
// 木構造クラスの宣言部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef	TREE_NODE_CLASS_HEADER_FILE_
#define	TREE_NODE_CLASS_HEADER_FILE_
#pragma once

#include <vector>

// 説明:
// 木構造クラス
class CTreeNode
{
// 説明:
// 変数
private:
	//! 親ノードへのポインタ
	CTreeNode *m_pParent;
	//! 子ノードへのポインタ列
	std::vector<CTreeNode*> m_vecChildren;

// 説明:
// constructor/destructor
public:
	// デフォルトコンストラクタ
	CTreeNode(void);
	// デストラクタ
	virtual ~CTreeNode();

// 初期化:
// initializer
private:
	// 子ノードも含むノードの破棄
	virtual void Destroy(void);

// 説明:
// attributes
public:
	//-----------------------------------------------------------------------------
	// 説明: 子ノード数取得
	// 引数: 
	// 返り値: 子ノード数
	// その他: 
	//-----------------------------------------------------------------------------
	size_t NumChildren(void) const
		{
			return m_vecChildren.size();
		}

// 説明:
// accessor
public:
	//-----------------------------------------------------------------------------
	// 説明: 親ノード設定
	// 引数: 親ノードへのポインタ
	// 返り値: 
	// その他: 
	//-----------------------------------------------------------------------------
	void SetParent(const CTreeNode *parent)
		{
			m_pParent = const_cast<CTreeNode*>(parent);
		}
	//-----------------------------------------------------------------------------
	// 説明: 親ノード取得
	// 引数: 
	// 返り値: 親ノードへのポインタ 
	// その他: 
	//-----------------------------------------------------------------------------
	CTreeNode* GetParent(void)
		{
			return m_pParent;
		}
	//-----------------------------------------------------------------------------
	// 説明: 親ノード取得(const)
	// 引数: 
	// 返り値: 親ノードへのポインタ(const) 
	// その他: 
	//-----------------------------------------------------------------------------
	const CTreeNode* GetParent(void) const
		{
			return m_pParent;
		}
	// ルートノード取得
	CTreeNode* GetRoot(void);
	// ルートノード取得(const)
	const CTreeNode* GetRoot(void) const;
	// 子ノード取得
	CTreeNode* GetChild(size_t nid);
	// 子ノード取得(const)
	const CTreeNode* GetChild(size_t nid) const;


// 説明:
// operator
public:
	// 子ノード検索
	size_t FindChild(const CTreeNode *node) const;
	// 子ノード追加
	void AddChild(CTreeNode *child);
	// ノード削除
	bool RemoveNode(size_t nid);
	//-----------------------------------------------------------------------------
	// 説明: ノード削除
	// 引数: 
	// 返り値:
	//        true 削除成功
	//        false 削除失敗、もしくは指定ノードが存在しない
	// その他: 
	//-----------------------------------------------------------------------------
	bool RemoveNode(const CTreeNode *node)
		{
			return RemoveNode(FindChild(node));
		}
};

#endif //TREE_NODE_CLASS_HEADER_FILE_
