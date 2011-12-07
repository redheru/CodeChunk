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

#ifndef EIGEN_MATH_NAMESPACE_HEADER_
#define EIGEN_MATH_NAMESPACE_HEADER_

// Eigen Library
#include <Eigen/Dense>
using namespace Eigen;

// ���O���: NEigenMath
// Eigen���C�u�����̕⏕�֐��Q
namespace NEigenMath
{

	// �~����
	static const float EigenPI = 3.141592654f;

	// �x�����W�A���ɕϊ�����
	float ToRadian(float degree);

	// ���W�A����x�ɕϊ�����
	float ToDegree(float radian);

	// 2��3D�x�N�g���̊O�ς��v�Z����
	Vector3f* Vector3fCross(Vector3f* pOut, const Vector3f* pV1, const Vector3f* pV2);
	// �w�肳�ꂽ�s��ɂ��3D�x�N�g�����g�����X�t�H�[�����A���̌��ʂ�W=1�Ɏˉe����
	Vector3f* Vector3fTransformCoord(Vector3f* pOut, const Vector3f* pV, const Matrix4f* pM);

	// �s����e�����ƂɃX�P�[�����O����
	Matrix4f* Matrix4fScaling( Matrix4f *pOut, float sx, float sy, float sz );
	// ��]�x�N�g�������]�s������߂�
	Matrix4f* Matrix4fRotationAxis( Matrix4f *pOut, const Vector3f *pV, float angle );
	// �ړ��s����쐬����
	Matrix4f* Matrix4fTranslation( Matrix4f *pOut, float x, float y, float z);
	// �N�H�[�^�j�I�������]�s����쐬����
	Matrix4f* Matrix4fRotationQuaternion( Matrix4f *pOut, const Quaternionf* pQ);

	// X������]���Ƃ��ĉ�]����s����쐬����
	Matrix4f* Matrix4fRotationX( Matrix4f *pOut, float angle);
	// Y������]���Ƃ��ĉ�]����s����쐬����
	Matrix4f* Matrix4fRotationY( Matrix4f *pOut, float angle);
	// Z������]���Ƃ��ĉ�]����s����쐬����
	Matrix4f* Matrix4fRotationZ( Matrix4f *pOut, float angle);

	// ��]�s�񂩂�N�H�[�^�j�I�����쐬����
	Quaternionf* QuaternionfRotationMatrix( Quaternionf *pOut, const Matrix4f* pM);

	// ����x�N�g�����炠��x�N�g���ɉ�]������N�H�[�^�j�I�����쐬����
	Matrix4f* fromTwoVector3f( Matrix4f *pOut, const Vector3f &src, const Vector3f &dist);
};

#endif