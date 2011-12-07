#ifndef MOTIONCONCATENATION_H
#define MOTIONCONCATENATION_H

#include "MotionUtil.h"
//#include "../NinoUtil/FileName.hpp"

namespace Motion
{

using namespace std;

class MotionConcatenation
{

public:

	typedef enum
	{
		OtherToOther,
		OtherToRight,
		OtherToLeft,
		RightToOther,
		RightToRight,
		RightToLeft,
		LeftToOther,
		LeftToRight,
		LeftToLeft,
		FragNum
	}FootTransition;

	void Initialize()
	{
		pipe_motions.clear();
		this->isLoadPipeMotion = false;
	}

	//constructor
	MotionConcatenation(){
		Initialize();
	}

	~MotionConcatenation(){}


// >member
private:
	vector<DrawMocapData> pipe_motions;
	vector<DrawMocapData> right_to_left_pipe_motions;
	vector<DrawMocapData> left_to_right_pipe_motions;
	bool isLoadPipeMotion;


	//DrawMocapData src2;
	//DrawMocapData src2_relative;
	//DrawMocapData* dest;

	//Skeleton* skl;
	//int fps;

	//int src1_start_frame;
	//int src2_end_frame;
	//int interpolateFrames;

	//bool canInterpolate;
	//bool rightFootonGround;

	//string max_diff_bone_name;

	//double root_diff;
	//double v_diff;
	//double diff;

	//int src1_range;
	//int src2_range;
	
// >method
public:

	//// ���
	static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range = 10);//���[�V����1���烂�[�V����2�֑J�ڂ��郂�[�V���������
	static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int start_src1, int end_src1, int start_src2, int range = 10);//���[�V����1���烂�[�V����2�֑J�ڂ��郂�[�V���������(�t���[���w��)
	static void Concatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxConcatenationLength = 30, bool useCenterAlign = true);//���[�V����1���烂�[�V����2�֑J�ڂ��郂�[�V���������(MotionGraph�p,Normalize���Ȃ�, �J�ڂ���ꏊ���T���Ȃ�)
	//PipeMotion���g���ꍇ
	void Concatenation3(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range = 10);//���[�V����1���烂�[�V����2�֑J�ڂ��郂�[�V���������(PipeMotion�g��,PipeMotion�̃��[�h���K�v)
	void loadPipeMotions(string mocap_dir, string left_to_right_mocap_dir, string right_to_left_mocap_dir, bool is_recursion, int fps = 60);//�ڑ��p�̃��[�V���������[�h����
	bool getPipeMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int footTrans, int range = 10);//���[�V����1�ƃ��[�V����2���Ȃ����[�V�����𓾂�

	////���[�V�����̐���
	//�Ȃ����킹�镔���̃��[�V��������
	static void makeConcatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum);//���[�V����1�̂���t���[�����烂�[�V����2�̂���t���[���ւȂ����[�V���������߂�(�O��̃t���[�����l�����Ȃ�)
	static void makeConcatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum);//���[�V����1�̂���t���[�����烂�[�V����2�̂���t���[���ւȂ����[�V���������߂�(�O��̃t���[�����l������)
	static void makeConcatenationWithCenterAlign(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum, int maxConcatenationLength);//���[�V����1�̂���t���[�����烂�[�V����2�̂���t���[���ւȂ����[�V���������߂�(�O��̃t���[�����l�����Ȃ�)
	//�q�����킹���S�̂̃��[�V�����̐���
	static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum=0);//���[�V����1�ƃ��[�V����2�Ƃ��̕�ԃ��[�V������ڑ��������[�V���������
	static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int frameNum = 0);//���[�V����1�ƃ��[�V����2�Ƃ��̕�ԃ��[�V������ڑ��������[�V���������

	// ���(2�g�̃��[�V����)
	static pair<DrawMocapData, DrawMocapData> Concatenation(DrawMocapData& from1, DrawMocapData& to1, DrawMocapData& from2, DrawMocapData& to2, pair<int,int> start_end_from1, pair<int,int> start_end_to1, pair<int,int> start_end_from2, pair<int,int> start_end_to2,int maxConcatenationLength = 30, bool useCenterAlign = true);//2�g�̃��[�V����(from1, to1)(from2, to2) ��J�ڂ�����

	//��Ԃ̕⏕�֐�
	static PoseSimilarity getConcatenationPoint( int& src_frame, int& dst_frame, DrawMocapData& src, DrawMocapData& dst, int range);//���[�V����1���烂�[�V����2�֐ڑ�����t���[�������߂�
	static void setConcatenationFrameNum(int& frameNum, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxframenum = 30);//���[�V����1�̂���t���[���ƃ��[�V����2�̂���t���[�����Ԃ��郂�[�V�����̃t���[���������߂�
	static Pose poseInterpolate(Pose& src1, Pose& src2, float rate);//�p��1�Ǝp��2���Ԃ���t���[���̎p�������߂�(���ʐ��`���)
	static int getInfoFootTransition(Pose& src1, Pose& src2);//�p��1�Ǝp��2�̑��̈ʒu�֌W�𓾂�
	//static Pose poseInterpolate(Pose& src1, Pose& src2, Pose& src0, Pose& src3, double rate);//�p��1�Ǝp��2���Ԃ���t���[�������߂�(���ʐ��`�񎟕��)

}; //class MotionInterpolation


}//namespace Motion

#endif
