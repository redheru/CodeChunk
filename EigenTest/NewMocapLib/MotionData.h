//-----------------------------------------------------------------------------
//                             MotionData.h
//
// ���[�V�����f�[�^�N���X�錾��
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef MOTION_DATA_CLASS_HEADER_
#define MOTION_DATA_CLASS_HEADER__
#pragma once

// Eigen Library
#include "EigenMath.h"

// ����:
// ���[�V�����f�[�^�N���X
class CMotionData
{
// ����:
// variables
private:
	//! �֐ߐ�
	size_t m_nJoints;
	//! �t���[����
	size_t m_nFrames;
	//! �t���[������
	float m_nFrameTime;
	//! ���[�g�ʒu���n��f�[�^
	Vector3f *m_pPosition;
	//! �֐߉�]�p�x���n��f�[�^
	Quaternionf *m_pRotation;

// Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// ����:
// constructor/destructor
public:
	// �f�t�H���g�R���X�g���N�^
	CMotionData(void);
	// �������q�t�R���X�g���N�^
	CMotionData(size_t frames, size_t joints);
	// �R�s�[�R���X�g���N�^
	CMotionData(const CMotionData &src);
	// �f�X�g���N�^
	virtual ~CMotionData(void);

// ����:
// initializer
public:
	// �������֐�
	bool Initialize(size_t frames, size_t joints);
	// ������Z�q
	CMotionData& operator =(const CMotionData &src);

// ����:
// memory allocator
private:
	// �������̈�m��
	bool Allocate(size_t frames, size_t joints);
	// �������̈���
	void Destroy();

// ����:
// attributes
public:
	// �f�[�^���p�\���
	bool IsActive() const;
	//-----------------------------------------------------------------------------
	// ����: ���t���[�����̎擾
    // ����: 
	// �Ԃ�l:
	// ���̑�: 
	//        ���t���[����
	//-----------------------------------------------------------------------------
	size_t NumFrames() const
		{
			return m_nFrames;
		}
	//-----------------------------------------------------------------------------
	// ����: ���֐߃m�[�h�̎擾
    // ����: 
	// �Ԃ�l:
	//        ���֐߃m�[�h��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	size_t NumJoints() const
		{
			return m_nJoints;
		}

// ����:
// accessor
public:  
	// �t���[�����Ԃ̐ݒ�
	bool SetFrameTime(float frmTime){m_nFrameTime = frmTime; return true;}
	// ��]�ʂ̐ݒ�
	bool SetRotation(size_t frm, size_t jid, const Quaternionf &q);
	// ��]�ʂ̐ݒ�
	bool SetRotation(size_t frm, size_t jid, const Matrix4f &m);
	// ���[�g�ʒu�̎w��
	bool SetPosition(size_t frm, const Vector3f &v);

	// �t���[�����Ԃ̎擾
    float GetFrameTime() const {return m_nFrameTime;}
	// ��]�ʂ̎擾
	Quaternionf GetRotation(size_t frm, size_t jid) const;
	// ��]�ʂ̎擾
	Matrix4f GetRotationMatrix(size_t frm, size_t jid) const;
	// ���[�g�ʒu�̎擾
	Vector3f GetPosition(size_t frm) const;
	// ���[�g�ʒu�̎擾
	Matrix4f GetPositionMatrix(size_t frm) const;

// ����:
// macros
private:
	//-----------------------------------------------------------------------------
	// ����: �l�攻��}�N��(�t���[��)
    // ����: 
	//       frm [in] ����Ώۃt���[���ԍ�
	// �Ԃ�l      
	//        true �l���
	//        false �l��O
	// ���̑�: 
	//-----------------------------------------------------------------------------
	bool IsRange(size_t frm) const
		{
			return frm < m_nFrames;
		}
	//-----------------------------------------------------------------------------
	// ����: �l�攻��}�N��(�t���[��&���R�x)
    // ����: 
	//       frm [in] ����Ώۃt���[���ԍ�
	//       jid [in] ����Ώۊ֐߃C���f�N�X
	// �Ԃ�l      
	//        true �l���
	//        false �l��O
	// ���̑�: 
	//-----------------------------------------------------------------------------
	bool IsRange(size_t frm, size_t jid) const
		{
			return frm < m_nFrames && jid < m_nJoints;
		}
	//-----------------------------------------------------------------------------
	// ����: ��]�f�[�^�A�N�Z�X�}�N��
    // ����: 
	//       frm [in] �t���[���ԍ�
	//       jid [in] �֐߃C���f�N�X
	// �Ԃ�l      
	//        ��]�f�[�^�ւ̎Q��
	// ���̑�: 
	//-----------------------------------------------------------------------------
	Quaternionf& ROT(size_t frm, size_t jid)
		{
			return *(m_pRotation + m_nJoints * frm + jid);
		}
	//-----------------------------------------------------------------------------
	// ����: ��]�f�[�^�A�N�Z�X�}�N��(const)
    // ����: 
	//       frm [in] �t���[���ԍ�
	//       jid [in] �֐߃C���f�N�X
	// �Ԃ�l      
	//       ��]�f�[�^�ւ̎Q��(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const Quaternionf& ROT(size_t frm, size_t jid) const
		{
			return *(m_pRotation + m_nJoints * frm + jid);
		}
	//-----------------------------------------------------------------------------
	// ����: ���[�g�ʒu�f�[�^�A�N�Z�X�}�N��
    // ����: 
	//       frm [in] �t���[���ԍ�
	// �Ԃ�l      
	//       �ʒu�f�[�^�ւ̎Q��(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	Vector3f& POS(size_t frm)
		{
			return *(m_pPosition + frm);
		}
	//-----------------------------------------------------------------------------
	// ����: ���[�g�ʒu�f�[�^�A�N�Z�X�}�N��(const)
    // ����: 
	//       frm [in] �t���[���ԍ�
	// �Ԃ�l      
	//       �ʒu�f�[�^�ւ̎Q��(const)
	// ���̑�: 
	//-----------------------------------------------------------------------------
	const Vector3f& POS(size_t frm) const
		{
			return *(m_pPosition + frm);
		}
};

#endif //MOTION_DATA_CLASS_HEADER__
