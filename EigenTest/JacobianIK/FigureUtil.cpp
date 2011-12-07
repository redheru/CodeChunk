#include "FigureUtil.h"

using namespace std;
using namespace Eigen;

//-----------------------------------------------------------------------------
// ����: ����֐߂���̊֐߂̈ʒu�̎擾
// ����: 
//       figure [in] �L�����N�^�t�B�M���A�N���X�̃C���X�^���X
//       dest_jid [in] �ړI�ʒu�̊֐߂̃C���f�N�X
//       root_jid [in] �o���ʒu�̊֐߃C���f�N�X
// �Ԃ�l:
//       �֐߉�]�s��
// ���̑�: 
//       root_jid �� -1 �̏ꍇ��World���W�n����
//-----------------------------------------------------------------------------
Vector3f CFigureUtil::GetJointPosition(const CFigure *figure, int dest_jid, int root_jid)
{
	if(root_jid < 0) // ���E���W�ł̈ʒu�����߂�ꍇ
		return figure->GetWorldPosition(dest_jid);

	const CJoint* destJoint = figure->GetJoint(dest_jid);	
	const CJoint* rootJoint = figure->GetJoint(root_jid);

	Vector3f vTmp(0, 0, 0), vRootPos(0, 0, 0);
	Matrix4f mTrans = Matrix4f::Identity();
   
	mTrans = destJoint->GetOffsetMatrix();
	NEigenMath::Vector3fTransformCoord(&vRootPos, &vTmp, &mTrans);
	for( const CJoint *pNode = destJoint->GetParent(); pNode != NULL; pNode = pNode->GetParent())
	{
		if(pNode == rootJoint)
			break;

		mTrans = pNode->GetTransformMatrix();
		vTmp = vRootPos;
		NEigenMath::Vector3fTransformCoord(&vRootPos, &vTmp, &mTrans);
	}

	return Vector3f(vRootPos.x(), vRootPos.y(), vRootPos.z());
}

//-----------------------------------------------------------------------------
// ����: �s�񂩂�I�C���[�p�\��(���[�E�s�b�`�E���[��)�ւ̕ϊ�
// ����: 
//       mat [in] �ϊ��Ώۂ̍s��
//       yaw [out] ���[�i�I�C���[�p�EY������j
//       pitch [out] �s�b�`�i�I�C���[�p�EX������j
//       roll [out] ���[���i�I�C���[�p�EZ������j
// �Ԃ�l:
//       �֐߉�]�s��
// ���̑�: 
//       �Q�l���� (������DirectX�Ȃ̂ō��W�n���قȂ�) http://www.tmps.org/index.php?%A5%AF%A5%A9%A1%BC%A5%BF%A5%CB%A5%AA%A5%F3%B5%D5%B1%BF%C6%B0%B3%D8
//-----------------------------------------------------------------------------
void CFigureUtil::Mat2EulerYPR(const Matrix4f& mat, float& yaw, float& pitch, float& roll)
{
	if( mat(2,1) > 0.998f) // north pole
	{
		yaw = atan2f( mat(0,2), mat(2,2) );
		pitch = static_cast<float>( M_PI / 2.f);
		roll = 0.f;
		return;
	}

	if( mat(2,1) < -0.998f) // north pole
	{
		yaw = atan2f( mat(0,2), mat(2,2) );
		pitch = static_cast<float>( -M_PI / 2.f);
		roll = 0.f;
		return;
	}

	yaw = atan2f( -mat(2,0), mat(2,2) );
	pitch = atan2f( -mat(0,1), mat(1,1) );
	roll = asinf( mat(2,1) );

	return;
}

//-----------------------------------------------------------------------------
// ����: ���R�r�A���s������߂�
// ����: 
//       figure [in] �L�����N�^�t�B�M���A�N���X�̃C���X�^���X
//       dest_jid [in] �ړI�ʒu�̊֐߂̃C���f�N�X
//       root_jid [in] �o���ʒu�̊֐߃C���f�N�X
// �Ԃ�l:
//       �֐߉�]�s��
// ���̑�: 
//       root_jid �� -1 �̏ꍇ��World���W�n����
//-----------------------------------------------------------------------------
Matrix4f CFigureUtil::GetJacobian(const CFigure* figure, int dest_jid, int root_jid)
{
	// �K�v�ȏ��
	// - �w���Joint�Ԃ̉�]�s��
	// - �w���Joint�Ԃ̃I�t�Z�b�g
	// - ��������H( 7���R�x�̉�͓IIK�̏ꍇ�͕K�v)
	Matrix4f matJac = Matrix4f::Identity();

	std::vector<Matrix4f, aligned_allocator<Matrix4f>> rotMat;
	std::vector<Matrix4f, aligned_allocator<Matrix4f>> offsetMat;
	int linkNum;
	if((linkNum = GetLinkNum(figure, rotMat, offsetMat, dest_jid, root_jid)) > 0 )  //�W���C���g�Ԃ̃����N�������߂�
	{
		for( int jid = 0; jid < linkNum; ++jid)
		{
			std::vector<Matrix4f, aligned_allocator<Matrix4f>> dpose(linkNum);
			for( int i = 0; i < linkNum; ++i) // �����N���ꂼ��̉�]�s�����
				dpose[i] = rotMat[i];

			Matrix4f dm[3], m[3];
			float yaw, pitch, roll;
			Mat2EulerYPR(rotMat[jid], yaw, pitch, roll);   // �I�C���[�p�����߂�
			DiffMatEulerDomain( dm[0], dm[1], dm[2], roll, pitch, yaw); // ���ꂼ��̊֐߂̕Δ��������߂�

			Matrix4f* Matrix4fRotationZ( &m[0], roll);
			Matrix4f* Matrix4fRotationX( &m[0], pitch);
			Matrix4f* Matrix4fRotationY( &m[0], yaw);

			// �����͂�������B


		}

	}

	return matJac;
}

//-----------------------------------------------------------------------------
// ����: �w�肵���W���C���g�Ԃ̃����N�̐������߂�
// ����: 
//       rotMat [in] �����N���ꂼ��̉�]�s��
//       offsetMat [in] �����N���ꂼ��̃I�t�Z�b�g�s��
//       figure [in] �L�����N�^�t�B�M���A�N���X�̃C���X�^���X
//       dest_jid [in] �ړI�ʒu�̊֐߂̃C���f�N�X
//       root_jid [in] �o���ʒu�̊֐߃C���f�N�X
// �Ԃ�l:
//       �����N�̐�
// ���̑�: 
//       �w�肵���W���C���g�����Ɋ܂܂��
//-----------------------------------------------------------------------------
int CFigureUtil::GetLinkNum(const CFigure* figure, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& rotMat, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& offsetMat, int dest_jid, int root_jid)
{
	const CJoint* destJoint = figure->GetJoint(dest_jid);	
	const CJoint* rootJoint = figure->GetJoint(root_jid);

	int linkCount = 1;  // �������g���܂߂�
	rotMat.push_back(destJoint->GetRotationMatrix());
	offsetMat.push_back(destJoint->GetOffsetMatrix());
	for( const CJoint *pNode = destJoint->GetParent(); pNode != NULL; pNode = pNode->GetParent())
	{
		rotMat.push_back(pNode->GetRotationMatrix());
		offsetMat.push_back(pNode->GetOffsetMatrix());
		linkCount++;

		if(pNode == rootJoint)
			return linkCount;
	}
	return linkCount;  // �t�B�M���A�̃��[�g����ƂȂ�
}

//-----------------------------------------------------------------------------
// ����: �e���ɑ΂��ĕΔ��������ꍇ�̍s������ꂼ�ꋁ�߂�
// ����: 
//       dmYaw [out] ���[(Y��)����ɑ΂��ĕΔ��������ꍇ�̍s��
//       dmPitch [out] �s�b�`(X��)����ɑ΂��ĕΔ��������ꍇ�̍s��
//       dmRoll [out] ���[��(Z��)����ɑ΂��ĕΔ��������ꍇ�̍s��
//       yaw [in] ���[����̉�]��
//       pitch [in] �s�b�`����̉�]��
//       roll [in] ���[������̉�]��
// �Ԃ�l:
//       �����N�̐�
// ���̑�: 
//       �w�肵���W���C���g�����Ɋ܂܂��
//-----------------------------------------------------------------------------
void CFigureUtil::DiffMatEulerDomain(Matrix4f& dmYaw, Matrix4f& dmPitch, Matrix4f& dmRoll, float yaw, float pitch, float roll)
{
	// Y������
	dmYaw = Matrix4f::Identity();
	dmYaw(0,0) = -sinf(yaw);
	dmYaw(0,2) = -cosf(yaw);
	dmYaw(1,1) = 0.f;
	dmYaw(2,0) = cosf(yaw);
	dmYaw(2,2) = -sinf(yaw);

	// X������
	dmPitch = Matrix4f::Identity();
	dmPitch(0,0) = 0.f;
	dmPitch(1,1) = -sinf(pitch);
	dmPitch(1,2) = cosf(pitch);
	dmPitch(2,1) = -cosf(pitch);
	dmPitch(2,2) = -sinf(pitch);

	// Z������
	dmRoll = Matrix4f::Identity();
	dmRoll(0,0) = -sinf(roll);
	dmRoll(0,1) = cosf(roll);
	dmRoll(1,0) = -cosf(roll);
	dmRoll(2,1) = -sinf(roll);
	dmRoll(2,2) = 0.f;
}