#ifndef MOTIONUTIL_H
#define MOTIONUTIL_H


#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "../BvhPlayerOnGLUT/DrawMocapData.h" //Math�nLib�������œǂݍ���ł���
#include <FileUtil/FileUtil.h>

namespace Motion
{

#define CONNECTIVITY_CHECK 0.25

#define MAX_POSE_DIFF 1.0 // similarity threshold
#define MAX_ROOT_DIFF 0.5 // similarity threshold
#define MAX_VEL_DIFF 0.5 // similarity threshold
#define LENGTH 3 // length to calc average velocity
#define MAX_POS_DIFF 0.05 // length to check direction
#define ROUND 0.01 // length to allow error
#define RANGE 120 // range to check conectivity

using namespace std;

  class PoseSimilarity {
	public:
		double poseDiff;
		double rootDiff;
		double velDiff;
	public:	//PoseSimilarity �p�̔�r���Z�q
		bool operator > (const PoseSimilarity & rhs){return (poseDiff + rootDiff * 3 + velDiff * 5 > rhs.poseDiff + rhs.rootDiff * 3 + rhs.velDiff * 5);}
		bool operator < (const PoseSimilarity & rhs){return (poseDiff + rootDiff * 3 + velDiff * 5 < rhs.poseDiff + rhs.rootDiff * 3 + rhs.velDiff * 5);}
		PoseSimilarity operator + (const PoseSimilarity & rhs){	
			PoseSimilarity result;
			result.poseDiff = poseDiff + rhs.poseDiff;
			result.rootDiff = rootDiff + rhs.rootDiff;
			result.velDiff = velDiff + rhs.velDiff;
			return result;}
		PoseSimilarity(){poseDiff=1000.0;rootDiff=1000.0;velDiff=1000.0;}
  };

	class MotionUtil
	{	

	public:

	// >member
	private:


	// >method
	public:
		//static klQuat scalarMultipleQuat(klQuat q1, float scale);//�N�H�[�^�j�I�����X�J���[�{����

	// Convert quat
		static klQuat convertQuatToLnQuat(klQuat qt);//�N�H�[�^�j�I���`������ΐ��N�H�[�^�j�I���`���ɕϊ�����
		static klQuat convertLnQuatToQuat(klQuat lnqt);//�ΐ��N�H�[�^�j�I���`������N�H�[�^�j�I���`���ɕϊ�����

	// normalize quaternion
		static void normalizeMotion(DrawMocapData& dst, Pose& base, int dst_frame);//���[�V����1�̂���t���[�������[�V����2�̂���t���[���̎p���Ɋ�Â��Đ��K������
		static void normalizeMotion2(DrawMocapData& dst, Pose& base, int dst_frame);//���[�V����1�̂���t���[�������[�V����2�̂���t���[���̎p���Ɋ�Â��Đ��K������

		static void normalizeMocapData(DrawMocapData& src);//Quaternion��180�x�̔��]����������
		static void normalizeMocapData2(DrawMocapData& src);//Quaternion�̂�w���}�C�i�X�ɂȂ��Ă���Ƃ���𔽓]������
		static klMat44 calcTransformBetweenFigures( vector< klVec3> pc1, vector<klVec3> pc2);//���W���K���s������߂�
				
	// lerp, slerp, expMap
		static klVec3 lerp(klVec3& v1, klVec3& v2, float rate);//���`��Ԃ�����(�x�N�g��)
		static klQuat lerp(klQuat& q1, klQuat& q2, float rate);//���`��Ԃ�����(�N�H�[�^�j�I��)
		static klQuat slerp(klQuat q1, klQuat q2, float rate, bool useInverseCosTheta);//���ʐ��`���(180�x�̕␳�����邩�ǂ����I���\)
		//static klQuat slerpByDirectXFunc(klQuat src1, klQuat src2, float rate);//���ʐ��`���(DirectX�̊֐����g���C180�x�̕␳��)
		//static klQuat squad(klQuat src0, klQuat src1, klQuat src2, klQuat src3, double rate);//���ʐ��`�񎟕��(DirectX�̊֐����g��)
		static klQuat expMap(vector<klQuat> &rots, vector<float> &kernel, bool useInverseCosTheta);//Exponential Map ���g�������(3�ȏ�̃N�H�[�^�j�I���̕��)
		static klQuat calcDifferentialExpMap(klQuat &q0, klQuat &q1, bool useInverseCosTheta);//2�̃N�H�[�^�j�I���̍���exponential map �̌v�Z

	// Calc Pose Similarity
		static double poseSimilarityBasedJointPos(vector<klVec3>& src1, vector<klVec3>& src2);//�p���̗ގ��x�����߂�(�ʒu�x�[�X)
		static double poseSimilarityBasedJointPos(Pose& src1, Pose& src2);//�p���̗ގ��x�����߂�(�ʒu�x�[�X)
		static double poseSimilarityBasedJointAngle(Pose& src1, Pose& src2);//�p���̗ގ��x�����߂�(�֐ߊp�x�x�[�X)
		static double velocitySimilarityBasedJointAngle( Pose& src1_mt, Pose& src1_t, int src1_range, Pose& src2_mt, Pose& src2_t, int src2_range);//�p�����m�̑��x�̗ގ��x�����߂�(�֐ߊp�x�x�[�X, �O��̃t���[�����瑬�x�𐄒�)

		static PoseSimilarity getSimilarity(Pose& src1, Pose& src2);//���[�V����1�̂���p���ƃ��[�V����2�̂���p���̗ގ��x�����߂�(���x�݂͂Ȃ�)
		static PoseSimilarity getSimilarity(DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, bool useJointAngle = false);//���[�V����1�̂���p���ƃ��[�V����2�̂���p���̗ގ��x�����߂�
		
	// Output
		static void outRotationVecForGnuPlot(DrawMocapData& src);//���[�V�����f�[�^�̊֐߂̉�](�ΐ��N�H�[�^�j�I���C�N�H�[�^�j�I��)���o�͂���
		static bool outSimilarityGraph(DrawMocapData& src, Pose base, string out_filename = "default.csv");//�x�[�X�̃|�[�Y����Ƃ��ėގ��x���o�͂���

	// rotate, translate pose
		static Pose rotatePose(DrawMocapData& src, float angle, int frame = 0);//���[�V�����f�[�^������������(Z������)��angle������]������
		static Pose rotatePose(DrawMocapData& src, klQuat rot, int frame = 0);//���[�V�����f�[�^������������(Z������)��rot������]������
		static Pose rotatePose(Pose& src, klQuat rot, int frame = 0);//���[�V�����f�[�^������������(Z������)��rot������]������
		static DrawMocapData rotateMocapData(DrawMocapData& src, float angle, int frame = 0);//���[�V�����f�[�^������������(Z������)��angle������]������
		static DrawMocapData rotateMocapData(DrawMocapData& src, klQuat rot, int frame = 0);//���[�V�����f�[�^������������(Z������)��rot������]������

		static DrawMocapData transMocapData(DrawMocapData& src, klVec3 trans);//���[�V�����f�[�^�𕽍s�ړ�������
		static Pose transPose(DrawMocapData& src, klVec3 trans, int frame=0);//���[�V�����f�[�^�𕽍s�ړ�������
		static Pose transPose(Pose& src, klVec3 trans);//���[�V�����f�[�^�𕽍s�ړ�������

	// for Motion Graph
		static string convertFileNameToGraphAnnotation(string name);

	// private method
	private:


	}; //class MotionUtil

}//namespace Motion


#endif