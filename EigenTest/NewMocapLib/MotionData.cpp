//-----------------------------------------------------------------------------
//                             MotionData.cpp
//
// ���[�V�����f�[�^�N���X��`��
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
// ����: �f�t�H���g�R���X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
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
// ����: �������q�t�R���X�g���N�^
// ����: 
//       frames [in] �t���[����
//       joints [in] �֐ߐ�
// �Ԃ�l:
// ���̑�: 
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
// ����: �R�s�[�R���X�g���N�^
// ����: 
//       src [in] �R�s�[��
// �Ԃ�l:
// ���̑�: 
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
// ����: �f�X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
CMotionData::~CMotionData(void)
{
	Destroy();
}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
//       frames [in] �t���[����
//       joints [in] �֐ߐ�
// �Ԃ�l:
//       true ����������
//       false ���������s
// ���̑�: 
//-----------------------------------------------------------------------------
bool CMotionData::Initialize(size_t frames, size_t joints)
{
	Destroy();
	return Allocate(frames, joints);
}

//-----------------------------------------------------------------------------
// ����: ������Z�q
// ����: 
//       src [in] �����
// �Ԃ�l:
//        ���g�ւ̎Q��
// ���̑�: 
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
// ����: �������m�ۊ֐�
// ����: 
//      frames [in] �t���[����
//      joints [in] �֐ߐ�
// �Ԃ�l:
//       true �������m�ې���
//       false �������m�ێ��s
// ���̑�: 
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
// ����: ����������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
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
// ����: �f�[�^���p�\���
// ����: 
// �Ԃ�l:
//       true �f�[�^���p��
//       false �f�[�^���p�s��
// ���̑�: 
//-----------------------------------------------------------------------------
bool CMotionData::IsActive() const
{
	return m_pPosition != NULL && m_pRotation != NULL;
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�ʂ̐ݒ�i�N�H�[�^�j�I���j
// ����: 
//       frm [in] �t���[���ԍ�
//       jid [in] �֐߃C���f�N�X
//       q [in] ��]�N�H�[�^�j�I��
// �Ԃ�l:
//       true �ݒ萬��
//       false �ݒ莸�s
// ���̑�: 
//-----------------------------------------------------------------------------
bool CMotionData::SetRotation(size_t frm, size_t jid, const Quaternionf &q)
{
	if (!IsRange(frm, jid))
		return false;
	ROT(frm, jid) = q;
	return true;
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�ʂ̐ݒ�i��]�s��j
// ����: 
//       frm [in] �t���[���ԍ�
//       jid [in] �֐߃C���f�N�X
//       m [in] ��]�s��
// �Ԃ�l:
//       true �ݒ萬��
//       false �ݒ莸�s
// ���̑�: 
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
// ����: ���[�g�ʒu�̐ݒ�
// ����: 
//       frm [in] �t���[���ԍ�
//       v [in] �ʒu�x�N�g��
// �Ԃ�l:
//       true �ݒ萬��
//       false �ݒ莸�s
// ���̑�: 
//-----------------------------------------------------------------------------
bool CMotionData::SetPosition(size_t frm, const Vector3f &v)
{
	if (!IsRange(frm))
		return false;
	POS(frm) = v;
	return true;
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�N�H�[�^�j�I���̎擾
// ����: 
//       frm [in] �t���[���ԍ�
//       jid [in] �֐߃C���f�N�X
// �Ԃ�l:
//       �֐߉�]�N�H�[�^�j�I��
// ���̑�: 
//-----------------------------------------------------------------------------
Quaternionf CMotionData::GetRotation(size_t frm, size_t jid) const
{
	if (!IsRange(frm, jid))
		return Quaternionf() = Quaternionf(0.f, 0.0f, 0.f, 1.0f);
	return ROT(frm, jid);
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�s��̎擾
// ����: 
//       frm [in] �t���[���ԍ�
//       jid [in] �֐߃C���f�N�X
// �Ԃ�l:
//       �֐߉�]�s��
// ���̑�: 
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
// ����: ���[�g�ʒu�x�N�g���̎擾
// ����: 
//       frm [in] �t���[���ԍ�
// �Ԃ�l:
//       ���[�g�ʒu�x�N�g��
// ���̑�: 
//-----------------------------------------------------------------------------
Vector3f CMotionData::GetPosition(size_t frm) const
{
	if (!IsRange(frm))
		return Vector3f() = Vector3f::Identity();
	return POS(frm);
}

//-----------------------------------------------------------------------------
// ����: ���[�g�ʒu�s��̎擾
// ����: 
//       frm [in] �t���[���ԍ�
// �Ԃ�l:
//       ���[�g�ʒu�s��
// ���̑�: 
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
