#ifndef MOTIONUTIL_H
#define MOTIONUTIL_H


#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "../BvhPlayerOnGLUT/DrawMocapData.h" //Math系Libも内部で読み込んでいる
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
	public:	//PoseSimilarity 用の比較演算子
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
		//static klQuat scalarMultipleQuat(klQuat q1, float scale);//クォータニオンをスカラー倍する

	// Convert quat
		static klQuat convertQuatToLnQuat(klQuat qt);//クォータニオン形式から対数クォータニオン形式に変換する
		static klQuat convertLnQuatToQuat(klQuat lnqt);//対数クォータニオン形式からクォータニオン形式に変換する

	// normalize quaternion
		static void normalizeMotion(DrawMocapData& dst, Pose& base, int dst_frame);//モーション1のあるフレームをモーション2のあるフレームの姿勢に基づいて正規化する
		static void normalizeMotion2(DrawMocapData& dst, Pose& base, int dst_frame);//モーション1のあるフレームをモーション2のあるフレームの姿勢に基づいて正規化する

		static void normalizeMocapData(DrawMocapData& src);//Quaternionの180度の反転を除去する
		static void normalizeMocapData2(DrawMocapData& src);//Quaternionのでwがマイナスになっているところを反転させる
		static klMat44 calcTransformBetweenFigures( vector< klVec3> pc1, vector<klVec3> pc2);//座標正規化行列を求める
				
	// lerp, slerp, expMap
		static klVec3 lerp(klVec3& v1, klVec3& v2, float rate);//線形補間をする(ベクトル)
		static klQuat lerp(klQuat& q1, klQuat& q2, float rate);//線形補間をする(クォータニオン)
		static klQuat slerp(klQuat q1, klQuat q2, float rate, bool useInverseCosTheta);//球面線形補間(180度の補正をするかどうか選択可能)
		//static klQuat slerpByDirectXFunc(klQuat src1, klQuat src2, float rate);//球面線形補間(DirectXの関数を使う，180度の補正つき)
		//static klQuat squad(klQuat src0, klQuat src1, klQuat src2, klQuat src3, double rate);//球面線形二次補間(DirectXの関数を使う)
		static klQuat expMap(vector<klQuat> &rots, vector<float> &kernel, bool useInverseCosTheta);//Exponential Map を使った補間(3つ以上のクォータニオンの補間)
		static klQuat calcDifferentialExpMap(klQuat &q0, klQuat &q1, bool useInverseCosTheta);//2つのクォータニオンの差分exponential map の計算

	// Calc Pose Similarity
		static double poseSimilarityBasedJointPos(vector<klVec3>& src1, vector<klVec3>& src2);//姿勢の類似度を求める(位置ベース)
		static double poseSimilarityBasedJointPos(Pose& src1, Pose& src2);//姿勢の類似度を求める(位置ベース)
		static double poseSimilarityBasedJointAngle(Pose& src1, Pose& src2);//姿勢の類似度を求める(関節角度ベース)
		static double velocitySimilarityBasedJointAngle( Pose& src1_mt, Pose& src1_t, int src1_range, Pose& src2_mt, Pose& src2_t, int src2_range);//姿勢同士の速度の類似度を求める(関節角度ベース, 前後のフレームから速度を推定)

		static PoseSimilarity getSimilarity(Pose& src1, Pose& src2);//モーション1のある姿勢とモーション2のある姿勢の類似度を求める(速度はみない)
		static PoseSimilarity getSimilarity(DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, bool useJointAngle = false);//モーション1のある姿勢とモーション2のある姿勢の類似度を求める
		
	// Output
		static void outRotationVecForGnuPlot(DrawMocapData& src);//モーションデータの関節の回転(対数クォータニオン，クォータニオン)を出力する
		static bool outSimilarityGraph(DrawMocapData& src, Pose base, string out_filename = "default.csv");//ベースのポーズを基準として類似度を出力する

	// rotate, translate pose
		static Pose rotatePose(DrawMocapData& src, float angle, int frame = 0);//モーションデータを鉛直軸周り(Z軸周り)にangleだけ回転させる
		static Pose rotatePose(DrawMocapData& src, klQuat rot, int frame = 0);//モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる
		static Pose rotatePose(Pose& src, klQuat rot, int frame = 0);//モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる
		static DrawMocapData rotateMocapData(DrawMocapData& src, float angle, int frame = 0);//モーションデータを鉛直軸周り(Z軸周り)にangleだけ回転させる
		static DrawMocapData rotateMocapData(DrawMocapData& src, klQuat rot, int frame = 0);//モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる

		static DrawMocapData transMocapData(DrawMocapData& src, klVec3 trans);//モーションデータを平行移動させる
		static Pose transPose(DrawMocapData& src, klVec3 trans, int frame=0);//モーションデータを平行移動させる
		static Pose transPose(Pose& src, klVec3 trans);//モーションデータを平行移動させる

	// for Motion Graph
		static string convertFileNameToGraphAnnotation(string name);

	// private method
	private:


	}; //class MotionUtil

}//namespace Motion


#endif