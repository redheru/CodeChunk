//-----------------------------------------------------------------------------
//                             TreeNode
//
// �؍\���N���X�̐錾��
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

// ����:
// �؍\���N���X
class CTreeNode
{
// ����:
// �ϐ�
private:
	//! �e�m�[�h�ւ̃|�C���^
	CTreeNode *m_pParent;
	//! �q�m�[�h�ւ̃|�C���^��
	std::vector<CTreeNode*> m_vecChildren;

// ����:
// constructor/destructor
public:
	// �f�t�H���g�R���X�g���N�^
	CTreeNode(void);
	// �f�X�g���N�^
	virtual ~CTreeNode();

// ������:
// initializer
private:
	// �q�m�[�h���܂ރm�[�h�̔j��
	virtual void Destroy(void);

// ����:
// attributes
public:
	//-----------------------------------------------------------------------------
	// ����: �q�m�[�h���擾
	// ����: 
	// �Ԃ�l: �q�m�[�h��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	size_t NumChildren(void) const
		{
			return m_vecChildren.size();
		}

// ����:
// accessor
public:
	//-----------------------------------------------------------------------------
	// ����: �e�m�[�h�ݒ�
	// ����: �e�m�[�h�ւ̃|�C���^
	// �Ԃ�l: 
	// ���̑�: 
	//-----------------------------------------------------------------------------
	void SetParent(const CTreeNode *parent)
		{
			m_pParent = const_cast<CTreeNode*>(parent);
		}
	//-----------------------------------------------------------------------------
	// ����: �e�m�[�h�擾
	// ����: 
	// �Ԃ�l: �e�m�[�h�ւ̃|�C���^ 
	// ���̑�: 
	//-----------------------------------------------------------------------------
	CTreeNode* GetParent(void)
		{
			return m_pParent;
		}
	//-----------------------------------------------------------------------------
	// ����: �e�m�[�h�擾(const)
	// ����: 
	// �Ԃ�l: �e�m�[�h�ւ̃|�C���^(const) 
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const CTreeNode* GetParent(void) const
		{
			return m_pParent;
		}
	// ���[�g�m�[�h�擾
	CTreeNode* GetRoot(void);
	// ���[�g�m�[�h�擾(const)
	const CTreeNode* GetRoot(void) const;
	// �q�m�[�h�擾
	CTreeNode* GetChild(size_t nid);
	// �q�m�[�h�擾(const)
	const CTreeNode* GetChild(size_t nid) const;


// ����:
// operator
public:
	// �q�m�[�h����
	size_t FindChild(const CTreeNode *node) const;
	// �q�m�[�h�ǉ�
	void AddChild(CTreeNode *child);
	// �m�[�h�폜
	bool RemoveNode(size_t nid);
	//-----------------------------------------------------------------------------
	// ����: �m�[�h�폜
	// ����: 
	// �Ԃ�l:
	//        true �폜����
	//        false �폜���s�A�������͎w��m�[�h�����݂��Ȃ�
	// ���̑�: 
	//-----------------------------------------------------------------------------
	bool RemoveNode(const CTreeNode *node)
		{
			return RemoveNode(FindChild(node));
		}
};

#endif //TREE_NODE_CLASS_HEADER_FILE_
