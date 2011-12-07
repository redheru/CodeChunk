//-----------------------------------------------------------------------------
//                             Joint.cpp
//
// �֐߃N���X��`��
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
// ����: �������q�t�R���X�g���N�^
// ����: 
//       offset [in] �I�t�Z�b�g�x�N�g��
//       parent [in] �e�֐߃m�[�h�ւ̃|�C���^
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint::CJoint(const Vector3f &offset, const CJoint* parent)
{
	m_pszName = NULL;
	Initialize(offset, parent);
}

//-----------------------------------------------------------------------------
// ����: �f�X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint::~CJoint(void)
{
}

//-----------------------------------------------------------------------------
// ����: �m�[�h�̔j��
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void CJoint::Destroy()
{
	if (m_pszName != NULL)
		delete[] m_pszName;
	delete this;
}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
//       offset [in] �I�t�Z�b�g�x�N�g��
//       parent [in] �e�֐߃m�[�h�ւ̃|�C���^
// �Ԃ�l:
// ���̑�: 
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
// ����: �{�[���̏����g�����X�t�H�[���D�x�N�g��(0, 0, 1)�ɑ΂��Ē�`�D
// ����: 
// �Ԃ�l:
//       �g�����X�t�H�[���s��
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetInitialTransform() const
{
	return m_mInit;
}

//-----------------------------------------------------------------------------
// ����: �I�t�Z�b�g�x�N�g���̐ݒ�
// ����: 
//       offset [in] �I�t�Z�b�g�x�N�g��
// �Ԃ�l:
// ���̑�: 
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
// ����: �I�t�Z�b�g�s��̎擾
// ����: 
// �Ԃ�l:
//       �I�t�Z�b�g�s��
// ���̑�: 
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
// ����: ��]�N�H�[�^�j�I���̐ݒ�
// ����: 
//       q [in] ��]�N�H�[�^�j�I��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void CJoint::SetRotation(const Quaternionf &q)
{
	m_qRotation = q;
}

//-----------------------------------------------------------------------------
// ����: �ʒu�x�N�g���̐ݒ�
// ����: 
//       pos [in] �ʒu�x�N�g��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void CJoint::SetPosition(const Vector3f &pos)
{
	m_vPosition = pos;
}

//-----------------------------------------------------------------------------
// ����: ��]�s��̎擾
// ����: 
// �Ԃ�l:
//       ��]�s��
// ���̑�: 
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
// ����: �ʒu�s��̎擾
// ����: 
// �Ԃ�l:
//       �ʒu�s��
// ���̑�: 
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
// ����: �֐߃g�����X�t�H�[���s��̎擾
// ����: 
// �Ԃ�l:
//       �֐߃g�����X�t�H�[���s��
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f CJoint::GetTransformMatrix() const
{
	return GetRotationMatrix() * GetOffsetMatrix() * GetPositionMatrix();
}

//-----------------------------------------------------------------------------
// ����: �e�֐߃m�[�h�̎擾
// ����: 
// �Ԃ�l:
//       �e�֐߃m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
void CJoint::SetParent(const CJoint *node)
{
	CTreeNode::SetParent(node);
}

//-----------------------------------------------------------------------------
// ����: �q�֐߃m�[�h�̒ǉ�
// ����: 
//       node [in] �q�֐߃m�[�h�ւ̃|�C���^
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void CJoint::AddChild(CJoint *node)
{
	CTreeNode::AddChild(node);
}

//-----------------------------------------------------------------------------
// ����: �֐ߖ��̐ݒ�
// ����: 
//       name [in] �֐ߖ�
// �Ԃ�l:
// ���̑�: 
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
