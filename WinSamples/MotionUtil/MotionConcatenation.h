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

	//// 補間
	static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range = 10);//モーション1からモーション2へ遷移するモーションを作る
	static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int start_src1, int end_src1, int start_src2, int range = 10);//モーション1からモーション2へ遷移するモーションを作る(フレーム指定)
	static void Concatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxConcatenationLength = 30, bool useCenterAlign = true);//モーション1からモーション2へ遷移するモーションを作る(MotionGraph用,Normalizeしない, 遷移する場所も探さない)
	//PipeMotionを使う場合
	void Concatenation3(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range = 10);//モーション1からモーション2へ遷移するモーションを作る(PipeMotion使う,PipeMotionのロードが必要)
	void loadPipeMotions(string mocap_dir, string left_to_right_mocap_dir, string right_to_left_mocap_dir, bool is_recursion, int fps = 60);//接続用のモーションをロードする
	bool getPipeMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int footTrans, int range = 10);//モーション1とモーション2をつなぐモーションを得る

	////モーションの生成
	//つなぎ合わせる部分のモーション生成
	static void makeConcatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum);//モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮しない)
	static void makeConcatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum);//モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮する)
	static void makeConcatenationWithCenterAlign(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum, int maxConcatenationLength);//モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮しない)
	//繋ぎ合わせた全体のモーションの生成
	static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum=0);//モーション1とモーション2とその補間モーションを接続したモーションを作る
	static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int frameNum = 0);//モーション1とモーション2とその補間モーションを接続したモーションを作る

	// 補間(2組のモーション)
	static pair<DrawMocapData, DrawMocapData> Concatenation(DrawMocapData& from1, DrawMocapData& to1, DrawMocapData& from2, DrawMocapData& to2, pair<int,int> start_end_from1, pair<int,int> start_end_to1, pair<int,int> start_end_from2, pair<int,int> start_end_to2,int maxConcatenationLength = 30, bool useCenterAlign = true);//2組のモーション(from1, to1)(from2, to2) を遷移させる

	//補間の補助関数
	static PoseSimilarity getConcatenationPoint( int& src_frame, int& dst_frame, DrawMocapData& src, DrawMocapData& dst, int range);//モーション1からモーション2へ接続するフレームを求める
	static void setConcatenationFrameNum(int& frameNum, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxframenum = 30);//モーション1のあるフレームとモーション2のあるフレームを補間するモーションのフレーム数を求める
	static Pose poseInterpolate(Pose& src1, Pose& src2, float rate);//姿勢1と姿勢2を補間するフレームの姿勢を求める(球面線形補間)
	static int getInfoFootTransition(Pose& src1, Pose& src2);//姿勢1と姿勢2の足の位置関係を得る
	//static Pose poseInterpolate(Pose& src1, Pose& src2, Pose& src0, Pose& src3, double rate);//姿勢1と姿勢2を補間するフレームを求める(球面線形二次補間)

}; //class MotionInterpolation


}//namespace Motion

#endif
