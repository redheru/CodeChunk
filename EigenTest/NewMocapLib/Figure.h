//-----------------------------------------------------------------------------
//                             Figure.h
//
// �L�����N�^�t�B�M���A�N���X�錾�� 
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


// ����:
// �L�����N�^�t�B�M���A�N���X
class CFigure
{
// ����:
// variables
private:
	//! ���[�g�m�[�h
	CJoint *m_pRoot;
	//! �֐߃m�[�h���X�g
	std::vector<CJoint*> m_vecJoints;

// ����:
// constructor/destructor
public:
	// �f�t�H���g�R���X�g���N�^
	CFigure();
	// �������q�t�R���X�g���N�^
	CFigure(CJoint *root);
	// �f�X�g���N�^
	virtual ~CFigure(void);

// ����:
// initializer
public:
	void Initialize(CJoint *root);

// ����:
// internal operator
private:
	// �X�P���g���؍\������̃m�[�h���X�g�̍쐬
	void ListingJoint(CJoint *root);

// ����:
// attributes
public:
	//-----------------------------------------------------------------------------
	// ����: �t�B�M���A�����p�\���`�F�b�N
    // ����: 
	// �Ԃ�l:
	// ���̑�: 
	//        true ���p�\
	//        false ���p�s��
	//-----------------------------------------------------------------------------
	bool IsActive() const;
	// �֐ߑ����̎擾
	size_t NumJoints() const;

// ����:
// accessor
public:
	// ���[�g�ʒu�̐ݒ�
	void SetRootPosition(const Vector3f &pos);
	// ���[�g�ʒu�̎擾
	Vector3f GetRootPosition() const;
	// ���[�g�����̐ݒ�
	void SetRootOrientation(const Quaternionf &rot);
	// ���[�g�����̎擾
	Quaternionf GetRootOrientation() const;
	// �֐߉�]�ʂ̐ݒ�
	void SetJointRotation(size_t jid, const Quaternionf &rot);
	// �֐߉�]�ʂ̎擾
	Quaternionf GetJointRotation(size_t jid) const;

	// ���[�g�m�[�h�ւ̃|�C���^�̎擾
	CJoint* GetRoot();
	// ���[�g�m�[�h�ւ̃|�C���^�̎擾(const)
	const CJoint* GetRoot() const;
	// �֐߃m�[�h�ւ̃|�C���^�̎擾
	CJoint* GetJoint(size_t jid);
	// �֐߃m�[�h�ւ̃|�C���^�̎擾(const)
	const CJoint* GetJoint(size_t jid) const;

	// �֐߃m�[�h�ւ̃|�C���^����m�[�h�C���f�N�X�̌���
	int FindNode(CJoint *node) const;
	// �֐߃m�[�h������m�[�h�C���f�N�X�̌���
	int FindNode(const char *name) const;
	// �֐߃m�[�h���G�t�F�N�^���`�F�b�N
	bool IsEffector(size_t jid) const;

// ����:
// forward kinematics
public:
	// �֐߃m�[�h�̃��[���h���W�x�N�g���̎擾
	Vector3f GetWorldPosition(size_t jid) const;

// ����:
// motion data
public:
	bool SetPose(const CMotionData *mot, size_t frm);
};

#endif //HUMAN_FIGURE_CLASS_HEADER_FILE_
