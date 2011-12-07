//-----------------------------------------------------------------------------
//                             EigenMath.h
//
// Eigen���C�u�����̕⏕�֐��Q
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/04 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#include "EigenMath.h"

//-----------------------------------------------------------------------------
// ����: �x�����W�A���ɕϊ�����
// ����: 
//       degree [in] ���W�A���ɕϊ�����l�i�x���P��)
// �Ԃ�l:
//       ���W�A���̒P�ʂɕϊ������l
// ���̑�: 
//-----------------------------------------------------------------------------
float NEigenMath::ToRadian(float degree)
{
	return (degree * (EigenPI / 180.0f));
}

//-----------------------------------------------------------------------------
// ����: ���W�A����x�ɕϊ�����
// ����: 
//       radian [in] �x�ɕϊ�����l�i���W�A���P��)
// �Ԃ�l:
//       �x���ɕϊ������l
// ���̑�: 
//-----------------------------------------------------------------------------
float NEigenMath::ToDegree(float radian)
{
	return (radian) * (180.0f / EigenPI);
}

//-----------------------------------------------------------------------------
// ����: 2��3D�x�N�g���̊O�ς��v�Z����
// ����: 
//       pOut [in,out] ���Z���ʂł���x�N�g���̃|�C���^
//       pV1 [in] �����̊�ɂȂ�x�N�g���̃|�C���^
//       pV2 [in] �����̊�ɂȂ�x�N�g���̃|�C���^
// �Ԃ�l:
//       2��3D�x�N�g���̊O�ςł���x�N�g���ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Vector3f* NEigenMath::Vector3fCross(Vector3f* pOut, const Vector3f* pV1, const Vector3f* pV2)
{
	*pOut = pV1->cross(*pV2);
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: �s����e�����ƂɃX�P�[�����O����
// ����: 
//       pOut [in,out] ���Z���ʂ�ێ�����s��̃|�C���^
//       sx [in] X���̃X�P�[���l
//       sy [in] Y���̃X�P�[���l
//       sz [in] Z���̃X�P�[���l
// �Ԃ�l:
//       �X�P�[����ϊ������s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fScaling( Matrix4f *pOut, float sx, float sy, float sz )
{
	Transform<float, 3, Affine> t; t.matrix() = *pOut;
	t.scale(Vector3f(sx, sy, sz));
	*pOut = t.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: ��]�x�N�g�������]�s������߂�
// ����: 
//       pOut [in,out] ���Z���ʂ�ێ�����s��̃|�C���^
//       pV [in] ���̊p�x�����ʂ���x�N�g���ւ̃|�C���^
//       angle [in] ��]�̊p�x(���W�A���P��)
// �Ԃ�l:
//       �w�肳�ꂽ������]���Ƃ��ĉ�]�����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationAxis( Matrix4f *pOut, const Vector3f *pV, float angle )
{
	Transform<float, 3, Affine> tmp;
	tmp = Translation3f(0.f, 0.f, 0.f) * AngleAxisf(angle, *pV);
	*pOut = tmp.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: �ړ��s����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂ�ێ�����s��̃|�C���^
//       x [in] X���̃I�t�Z�b�g
//       y [in] Y���̃I�t�Z�b�g
//       z [in] Z���̃I�t�Z�b�g
// �Ԃ�l:
//       ���Z���ʂ����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fTranslation( Matrix4f *pOut, float x, float y, float z)
{
	Transform<float, 3, Affine> t = Affine3f::Identity();
	t.translate(Vector3f(x, y, z));
	*pOut = t.matrix();
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: �N�H�[�^�j�I�������]�s����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂ�ێ�����s��̃|�C���^
//       pQ [in] �����̊�ɂȂ�N�H�[�^�j�I���ւ̃|�C���^
// �Ԃ�l:
//       �\�[�X�̃N�H�[�^�j�I������\�z���ꂽ�s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationQuaternion( Matrix4f *pOut, const Quaternionf* pQ)
{
	*pOut = Matrix4f::Identity();
	pOut->block(0,0,3,3) = Matrix3f(*pQ);
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: ��]�s�񂩂�N�H�[�^�j�I�����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂł���N�H�[�^�j�I���̃|�C���^
//       pQ [in] �����̊�ɂȂ�s��ւ̃|�C���^
// �Ԃ�l:
//       ��]�p�񂩂�쐬���ꂽ�N�H�[�^�j�I���ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Quaternionf* NEigenMath::QuaternionfRotationMatrix( Quaternionf *pOut, const Matrix4f* pM)
{
	AngleAxisf tmp;tmp = Matrix3f(pM->block(0,0,3,3));
	*pOut = Quaternionf(tmp);
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: �w�肳�ꂽ�s��ɂ��3D�x�N�g�����g�����X�t�H�[�����A���̌��ʂ�W=1�Ɏˉe����
// ����: 
//       pOut [in,out] ���Z���ʂł���x�N�g���̃|�C���^
//       pV [in] �����̊�ɂȂ�x�N�g���ւ̃|�C���^
//       pM [in] �����̊�ɂȂ�s��ւ̃|�C���^
// �Ԃ�l:
//       �g�����X�t�H�[�����ꂽ�x�N�g���̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Vector3f* NEigenMath::Vector3fTransformCoord(Vector3f* pOut, const Vector3f* pV, const Matrix4f* pM)
{
	Vector4f tmpV = Vector4f(pV->x(), pV->y(), pV->z(), 1.f);
	tmpV = tmpV.transpose() * (*pM);
	*pOut = tmpV.head(3);
	return pOut;
}

//-----------------------------------------------------------------------------
// ����: X������]���Ƃ��ĉ�]����s����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂł���s��̃|�C���^
//       angle [in] ��]�̊p�x(���W�A���P��)
// �Ԃ�l:
//       X������]���Ƃ��ĉ�]�����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationX( Matrix4f *pOut, float angle)
{
	const Vector3f axisX(1.f, 0.f, 0.f);
	return Matrix4fRotationAxis(pOut, &axisX, angle);
}

//-----------------------------------------------------------------------------
// ����: Y������]���Ƃ��ĉ�]����s����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂł���s��̃|�C���^
//       angle [in] ��]�̊p�x(���W�A���P��)
// �Ԃ�l:
//       Y������]���Ƃ��ĉ�]�����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationY( Matrix4f *pOut, float angle)
{
	const Vector3f axisY(0.f, 1.f, 0.f);
	return Matrix4fRotationAxis(pOut, &axisY, angle);
}

//-----------------------------------------------------------------------------
// ����: Z������]���Ƃ��ĉ�]����s����쐬����
// ����: 
//       pOut [in,out] ���Z���ʂł���s��̃|�C���^
//       angle [in] ��]�̊p�x(���W�A���P��)
// �Ԃ�l:
//       Z������]���Ƃ��ĉ�]�����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::Matrix4fRotationZ( Matrix4f *pOut, float angle)
{
	const Vector3f axisZ(0.f, 0.f, 1.f);
	return Matrix4fRotationAxis(pOut, &axisZ, angle);
}

//-----------------------------------------------------------------------------
// ����: ����x�N�g��������x�N�g���ɕϊ�����s������߂�
// ����: 
//       pOut [in,out] ���Z���ʂł���s��̃|�C���^
//       src [in] ���̃x�N�g��
//       dst [in] �ϊ���̃x�N�g��
// �Ԃ�l:
//       ����x�N�g��������x�N�g���ɕϊ�����s��ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f* NEigenMath::fromTwoVector3f( Matrix4f *pOut, const Vector3f &src, const Vector3f &dist)
{
	Quaternionf q; q = Quaternionf::Identity();
	Matrix4f mat; mat = Matrix4f::Identity();

	Vector3f cross = src.cross(dist);

	//no difference
	if(cross.norm() == 0)
	{
		return pOut = &mat;		
	}
	Vector3f axisVec = cross / cross.norm();
	
	float dot = src.dot(dist);
	float cosTheta =  dot / ( src.norm() * dist.norm() );

	float cosThetaDiv2 = sqrt( 0.5f * (1.f + cosTheta) );
	float sinThetaDiv2 = 0.5f * (1.f - cosTheta);

	if( sinThetaDiv2 >= 0)
	{
		sinThetaDiv2 = sqrt( sinThetaDiv2 );
	}
	else
	{
		sinThetaDiv2 = 0.0;
	}
	
	q.w() = cosThetaDiv2;
	q.x() = axisVec.x() * sinThetaDiv2;
	q.y() = axisVec.y() * sinThetaDiv2;
	q.z() = axisVec.z() * sinThetaDiv2;

	NEigenMath::Matrix4fRotationQuaternion(pOut, &q);
	return pOut;
}
