//-----------------------------------------------------------------------------
//                             Joint.h
//
// �֐߃N���X�̐錾��
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

// ����:
// �֐߃N���X
class CJoint : public CTreeNode
{
// ����:
// variables
private:
	//! �m�[�h�I�t�Z�b�g
	Vector3f m_vOffset;
	//! �֐߉�]��
    Quaternionf m_qRotation;
	//! �֐߈ʒu
	Vector3f m_vPosition;
	//! �{�[���̏����g�����X�t�H�[���i�I�t�Z�b�g�C�X�P�[���j
	Matrix4f m_mInit;
	//! �֐߃m�[�h��
	char *m_pszName;

 // Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW


// ����:
// constructor/destructor
public:
	// �������q�t�R���X�g���N�^
	CJoint(const Vector3f &offset, const CJoint *parent);
	// �f�X�g���N�^
	virtual ~CJoint();

// ����:
// initializer
public:
	// �������֐�
	void Initialize(const Vector3f offset, const CJoint *parent);

// ����:
// accessor
public:
	// �{�[���̏����g�����X�t�H�[���̎擾
	Matrix4f GetInitialTransform() const;
	// �֐߃I�t�Z�b�g�x�N�g���̐ݒ�
	void SetOffset(const Vector3f &offset);
	//-----------------------------------------------------------------------------
	// ����: �֐��I�t�Z�b�g�x�N�g���̎擾
	// ����: 
	// �Ԃ�l:
	//       �I�t�Z�b�g�x�N�g��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	Vector3f GetOffset() const
		{
			return m_vOffset;
		}
	// �֐߃I�t�Z�b�g�s��̎擾
	Matrix4f GetOffsetMatrix() const;
	// ��]�N�H�[�^�j�I���̐ݒ�
	void SetRotation(const Quaternionf &q);
	//-----------------------------------------------------------------------------
	// ����: ��]�N�H�[�^�j�I���̎擾
	// ����: 
	// �Ԃ�l:
	//       ��]�N�H�[�^�j�I��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	Quaternion<float> GetRotation() const
		{
			return m_qRotation;
		}
	// ��]�s��̎擾
	Matrix4f GetRotationMatrix() const;
	// �ʒu�x�N�g���̐ݒ�
	void SetPosition(const Vector3f &pos);
	//-----------------------------------------------------------------------------
	// ����: �ʒu�x�N�g���̎擾
	// ����: 
	// �Ԃ�l:
	//       �ʒu�x�N�g��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	Vector3f GetPosition() const
		{
			return m_vPosition;
		}
	// �ʒu�s��̎擾
	Matrix4f GetPositionMatrix() const;
	// �g�����X�t�H�[���s��̎擾
	Matrix4f GetTransformMatrix() const;
	// �֐߃m�[�h���̐ݒ�
	void SetName(const char *name);
	//-----------------------------------------------------------------------------
	// ����: �֐߃m�[�h���̎擾
	// ����: 
	// �Ԃ�l:
	//       �֐߃m�[�h��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const char* GetName(void) const
		{
			return m_pszName;
		}

// ����:
// overrides
public:
	// �m�[�h�̔j��
	void Destroy();
	//-----------------------------------------------------------------------------
	// ����: �e�֐߃m�[�h�̎擾
	// ����: 
	// �Ԃ�l:
	//       �֐��m�[�h�ւ̃|�C���^
	// ���̑�: 
	//-----------------------------------------------------------------------------
	CJoint* GetParent()
		{
			return static_cast<CJoint*>(CTreeNode::GetParent());
		}
	//-----------------------------------------------------------------------------
	// ����: �e�֐߃m�[�h�̎擾(const)
	// ����: 
	// �Ԃ�l:
	//       �֐��m�[�h�ւ̃|�C���^(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const CJoint* GetParent() const
		{
			return static_cast<const CJoint*>(CTreeNode::GetParent());
		}
	// �e�m�[�h�̐ݒ�
	void SetParent(const CJoint *node);
	//-----------------------------------------------------------------------------
	// ����: �q�֐߃m�[�h�̎擾
	// ����: 
	// �Ԃ�l:
	//       �֐��m�[�h�ւ̃|�C���^
	// ���̑�: 
	//-----------------------------------------------------------------------------
	CJoint* GetChild(size_t jid)
		{
			return dynamic_cast<CJoint*>(CTreeNode::GetChild(jid));
		}
	//-----------------------------------------------------------------------------
	// ����: �q�֐߃m�[�h�̎擾(const)
	// ����: 
	// �Ԃ�l:
	//       �֐��m�[�h�ւ̃|�C���^(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const CJoint* GetChild(size_t jid) const
		{
			return dynamic_cast<const CJoint*>(CTreeNode::GetChild(jid));
		}
	// �q�m�[�h�̒ǉ�
	void AddChild(CJoint *node);
	//-----------------------------------------------------------------------------
	// ����: ���[�g�m�[�h�̎擾
	// ����: 
	// �Ԃ�l:
	//       ���[�g�m�[�h�ւ̃|�C���^
	// ���̑�: 
	//-----------------------------------------------------------------------------
	CJoint* GetRoot()
		{
			return dynamic_cast<CJoint*>(CTreeNode::GetRoot());
		}
	//-----------------------------------------------------------------------------
	// ����: ���[�g�m�[�h�̎擾(const)
	// ����: 
	// �Ԃ�l:
	//       ���[�g�m�[�h�ւ̃|�C���^(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const CJoint* GetRoot() const
		{
			return dynamic_cast<const CJoint*>(CTreeNode::GetRoot());
		}
};

#endif // HUMAN_JOINT_HEADER_
