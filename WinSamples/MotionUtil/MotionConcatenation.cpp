#include "MotionConcatenation.h"

using namespace Motion;
using namespace Util;

/**
 * @fn static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2)
 * @brief モーション1からモーション2へ遷移するモーションを作る
 * @param dst  [out] モーション1からモーション2への遷移モーション
 * @param src1 [in]  モーション1
 * @param src2 [in]  モーション2
 * @param range [in]  連結フレームを探す範囲(デフォルト: 10)
 * @return なし
 */
void MotionConcatenation::Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range)
{

	cout << "******* Start Concatenation *******" << endl;

	//
	// モーションの初期化
	//
	dst.LoadVacantMotion(src1.getSkeleton(), src1.getFps());
	//上書きされないようにする
	DrawMocapData src1_t = src1;
	DrawMocapData src2_t = src2;

	//
	// 連結させる場所の決定
	//
	int src1_frame, src2_frame;
	PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);

	//
	// モーション2の正規化
	//
	MotionUtil::normalizeMotion(src2_t, src1_t.getPose(src1_frame), src2_frame);

	//
	// 足の遷移について
	//
	cout << "[Concatenate] footTransiton : " << getInfoFootTransition(src1_t.getPose(src1_frame), src2_t.getPose(src2_frame)) << endl;

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum;
	setConcatenationFrameNum(frameNum, src1_t, src2_t, src1_frame, src2_frame);

	//
	// 補間部分の生成 (SlerpかSquadを使って，補間フレーム以外のreferをなくす)
	//
	DrawMocapData concatenation;
	makeConcatenation(concatenation, src1_t, src2_t, src1_frame, src2_frame, frameNum);

	//
	// モーション1　->　モーション2　の結果生成
	//
	makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);


	//
	// 連結の情報
	//
	cout << "[Concatenate] Concatenate frames : " << frameNum << endl;
	cout << "[Concatenate] Concatenate points : src1 " << src1_frame - src1_t.getFrameNum() << " -> src2 " << src2_frame << endl;
	cout << "[Concatenate] Similarity : "<< sim.poseDiff << ", " << sim.velDiff << ", " << sim.rootDiff <<endl;
	cout << "******* End Concatenation *******" << endl;
	return;
}

/**
 * @fn static void Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int start_src1, int end_src1, int start_src2, int end_src2, int range = 10)
 * @brief モーション1からモーション2へ遷移するモーションを作る
 * @param dst  [out] モーション1からモーション2への遷移モーション
 * @param src1 [in]  モーション1
 * @param src2 [in]  モーション2
 * @param start_src1 [in]  モーション1の開始フレーム
 * @param end_src1 [in]  モーション1の終了フレーム
 * @param start_src1 [in]  モーション2の開始フレーム
 * @param range [in]  連結フレームを探す範囲(デフォルト: 10)
 * @return なし
 */
void MotionConcatenation::Concatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int start_src1, int end_src1, int start_src2, int range)
{

	cout << "******* Start Concatenation *******" << endl;

	//
	// モーションの初期化
	//
	dst.LoadVacantMotion(src1.getSkeleton(), src1.getFps());
	//使う部分のみ抜き出す
	DrawMocapData src1_t = src1;//src1.Trimming(start_src1, end_src1);
	DrawMocapData src2_t = src2;//src2.Trimming(start_src2, end_src2);

	//
	// 連結させる場所の決定
	//
	//int src1_frame, src2_frame;
	//PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);

	//
	// モーション2の正規化
	//
	MotionUtil::normalizeMotion(src2_t, src1_t.getPose(end_src1), start_src2);

	//
	// 足の遷移について
	//
	//cout << "[Concatenate] footTransiton : " << getInfoFootTransition(src1_t.getPose(src1_frame), src2_t.getPose(src2_frame)) << endl;

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum;
	setConcatenationFrameNum(frameNum, src1_t, src2_t, end_src1, start_src2);

	//
	// 補間部分の生成 (補間フレームの前後のPoseもみる)
	//
	DrawMocapData concatenation;
	makeConcatenation(concatenation, src1_t, src2_t, end_src1, start_src2, frameNum);

	src1_t.Trimming(start_src1, end_src1);
	src2_t.Trimming(start_src2, src2_t.getFrameNum()-1);
	//
	// モーション1　->　モーション2　の結果生成
	//
	//makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);
	makeResultMotion(dst, src1_t, concatenation, src2_t);

	//
	// 連結の情報
	//
	cout << "[Concatenate] Concatenate frames : " << frameNum << endl;
	//cout << "[Concatenate] Concatenate points : src1 " << src1_frame - src1_t.getFrameNum() << " -> src2 " << src2_frame << endl;
	//cout << "[Concatenate] Similarity : "<< sim.poseDiff << ", " << sim.velDiff << ", " << sim.rootDiff <<endl;
	cout << "******* End Concatenation *******" << endl;
	return;
}

/**
 * @fn static pair<DrawMocapData, DrawMocapData> Concatenation(DrawMocapData& from1, DrawMocapData& to1, DrawMocapData& from2, DrawMocapData& to2, pair<int,int> start_end1, pair<int,int> start_end2, int range = 10)
 * @brief 2組のモーション(from1, to1)(from2, to2) を遷移させる
 * @param from1  [in] 遷移元モーション
 * @param to1 [in]  遷移先モーション
 * @param from2 [in]  遷移元モーション
 * @param to2 [in]  遷移先モーション
 * @param start_end_from1 [in]  モーションの開始と終了フレーム
 * @param start_end_to1 [in]  モーションの開始と終了フレーム
 * @param start_end_from2 [in]  モーションの開始と終了フレーム
 * @param start_end_to2 [in]  モーションの開始と終了フレーム
 * @param maxConcatenationLength [in]  連結フレームの最大値
 * @return 遷移結果のモーション
 */
pair<DrawMocapData, DrawMocapData> MotionConcatenation::Concatenation(DrawMocapData& from1, DrawMocapData& to1, DrawMocapData& from2, DrawMocapData& to2, pair<int,int> start_end_from1, pair<int,int> start_end_to1, pair<int,int> start_end_from2, pair<int,int> start_end_to2,int maxConcatenationLength, bool useCenterAlign)
{

	cout << "******* Start Concatenation *******" << endl;

	//
	// モーションの初期化
	//
	pair<DrawMocapData,DrawMocapData> dst;	
	dst.first.LoadVacantMotion(from1.getSkeleton(), from1.getFps());
	dst.second.LoadVacantMotion(from2.getSkeleton(), from2.getFps());

	//使う部分のみ抜き出す
	DrawMocapData from1_t = from1;//src1.Trimming(start_src1, end_src1);
	DrawMocapData to1_t = to1;//src2.Trimming(start_src2, end_src2);
	DrawMocapData from2_t = from2;//src1.Trimming(start_src1, end_src1);
	DrawMocapData to2_t = to2;//src2.Trimming(start_src2, end_src2);

	//
	// 連結させる場所の決定
	//
	//int src1_frame, src2_frame;
	//PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);

	//
	// モーション2の正規化
	//
	//normalizeMotion(to1_t, from1_t.getPose(start_end1.second), to1_t);

	//
	// 足の遷移について
	//
	//cout << "[Concatenate] footTransiton : " << getInfoFootTransition(src1_t.getPose(src1_frame), src2_t.getPose(src2_frame)) << endl;

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum1, frameNum2, frameNum;
	setConcatenationFrameNum(frameNum1, from1_t, to1_t, start_end_from1.second, start_end_to1.first, maxConcatenationLength);
	setConcatenationFrameNum(frameNum2, from2_t, to2_t, start_end_from2.second, start_end_to2.first, maxConcatenationLength);
	frameNum = (frameNum1+frameNum2)/2;

	//// 補間してもフレーム数を増やさないようにする(ダンスの場合のみ)
	////
	//// 補間部分の生成 (補間フレームの前後のPoseもみる)
	////
	//DrawMocapData concatenation1, concatenation2;
	//makeConcatenationWithCenterAlign(concatenation1, from1_t, to1_t, start_end_from1.second, start_end_to1.first, frameNum);
	//makeConcatenationWithCenterAlign(concatenation2, from2_t, to2_t, start_end_from2.second, start_end_to2.first, frameNum);

	//from1_t.Trimming(start_end_from1.first, start_end_from1.second);
	//to1_t.Trimming(start_end_to1.first, to1_t.getFrameNum()-1);
	//from2_t.Trimming(start_end_from2.first, start_end_from2.second);
	//to2_t.Trimming(start_end_to2.first, to2_t.getFrameNum()-1);

	////
	//// モーション1　->　モーション2　の結果生成
	////
	////makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);
	//makeResultMotion(dst.first, from1_t, concatenation1, to1_t, frameNum);
	//makeResultMotion(dst.second, from2_t, concatenation2, to2_t, frameNum);

	//
	// 補間部分の生成 (補間フレームの前後のPoseもみる)
	//
	DrawMocapData concatenation1, concatenation2;
	if(useCenterAlign)
	{
		makeConcatenationWithCenterAlign(concatenation1, from1_t, to1_t, start_end_from1.second, start_end_to1.first, frameNum, maxConcatenationLength);
		makeConcatenationWithCenterAlign(concatenation2, from2_t, to2_t, start_end_from2.second, start_end_to2.first, frameNum, maxConcatenationLength);
	}
	else
	{
		makeConcatenation(concatenation1, from1_t, to1_t, start_end_from1.second, start_end_to1.first, frameNum);
		makeConcatenation(concatenation2, from2_t, to2_t, start_end_from2.second, start_end_to2.first, frameNum);
	}

	//from1_t.Trimming(start_end_from1.first, start_end_from1.second);
	//to1_t.Trimming(start_end_to1.first, to1_t.getFrameNum()-1);
	//from2_t.Trimming(start_end_from2.first, start_end_from2.second);
	//to2_t.Trimming(start_end_to2.first, to2_t.getFrameNum()-1);

	//
	// モーション1　->　モーション2　の結果生成
	//
	//makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);
	if(useCenterAlign)
	{
		makeResultMotion(dst.first, from1_t, concatenation1, to1_t, start_end_from1.second, start_end_to1.first, frameNum);
		makeResultMotion(dst.second, from2_t, concatenation2, to2_t, start_end_from2.second, start_end_to2.first, frameNum);
	}
	else
	{
		makeResultMotion(dst.first, from1_t, concatenation1, to1_t, start_end_from1.second, start_end_to1.first);
		makeResultMotion(dst.second, from2_t, concatenation2, to2_t, start_end_from2.second, start_end_to2.first);
	}

	//
	// 連結の情報
	//
	cout << "[Concatenate] Concatenate frames : " << (frameNum1+frameNum2)/2 << endl;
	//cout << "[Concatenate] Concatenate points : src1 " << src1_frame - src1_t.getFrameNum() << " -> src2 " << src2_frame << endl;
	//cout << "[Concatenate] Similarity : "<< sim.poseDiff << ", " << sim.velDiff << ", " << sim.rootDiff <<endl;
	cout << "******* End Concatenation *******" << endl;
	return dst;
}


/**
 * @fn static void Concatenation3(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2)
 * @brief モーション1からモーション2へ遷移するモーションを作る(PipeMotion使う,PipeMotionのロードが必要
 * @param dst  [out] モーション1からモーション2への遷移モーション
 * @param src1 [in]  モーション1
 * @param src2 [in]  モーション2
 * @param range [in]  連結フレームを探す範囲(デフォルト: 10)
 * @return なし
 */
void MotionConcatenation::Concatenation3(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int range)
{
	cout << "******* Start Concatenation *******" << endl;

	//
	// モーションの初期化
	//
	dst.LoadVacantMotion(src1.getSkeleton(), src1.getFps());
	//上書きされないようにする
	DrawMocapData src1_t = src1;
	DrawMocapData src2_t = src2;

	//
	// 連結させる場所の決定
	//
	int src1_frame, src2_frame;
	PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);
	if(sim.poseDiff > MAX_POSE_DIFF 
		|| sim.rootDiff > MAX_ROOT_DIFF
		|| sim.velDiff > MAX_VEL_DIFF)
	{
		int footTrans = getInfoFootTransition(src1_t.getPose(src1_frame), src2_t.getPose(src2_frame));			
		//
		// PipeMotionをつなげる(src1_t <- src1 + PipeMotion)
		//
		getPipeMotion(src1_t, src1, src2, footTrans, range);

		//
		// 連結させる場所の決定
		//
		PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);
	}

	//
	// モーション2の正規化
	//
	MotionUtil::normalizeMotion(src2_t, src1_t.getPose(src1_frame), src2_frame);

	//
	// 足の遷移について
	//
	cout << "[Concatenate] footTransiton : " << getInfoFootTransition(src1_t.getPose(src1_frame), src2_t.getPose(src2_frame)) << endl;

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum;
	setConcatenationFrameNum(frameNum, src1_t, src2_t, src1_frame, src2_frame);

	//
	// 補間部分の生成 (SlerpかSquadを使って，補間フレーム以外のreferをなくす)
	//
	DrawMocapData concatenation;
	makeConcatenation(concatenation, src1_t, src2_t, src1_frame, src2_frame, frameNum);

	//
	// モーション1　->　モーション2　の結果生成
	//
	makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);


	//
	// 連結の情報
	//
	cout << "[Concatenate] Concatenate frames : " << frameNum << endl;
	cout << "[Concatenate] Concatenate points : src1 " << src1_frame - src1_t.getFrameNum() << " -> src2 " << src2_frame << endl;
	cout << "[Concatenate] Similarity : "<< sim.poseDiff << ", " << sim.velDiff << ", " << sim.rootDiff <<endl;
	cout << "******* End Concatenation *******" << endl;
	return;
}

/**
 * @fn static void Concatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1からモーション2へ遷移するモーションを作る(MotionGraph用,Normalizeしない, 遷移する場所も探さない)
 * @param dst  [out] モーション1からモーション2への遷移モーション
 * @param src1 [in]  モーション1
 * @param src2 [in]  モーション2
 * @param maxConcatenationLength [in]  補間モーションの最大長
 * @return なし
 */
void MotionConcatenation::Concatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxConcatenationLength, bool useCenterAlign)
{
	cout << "******* Start Concatenation *******" << endl;
	//
	// モーションの初期化
	//
	//Skeleton dstSkl = *(src1.getSkeleton());
	//dst.LoadVacantMotion(src1.getSkeleton(), src1.getFps());
	//上書きされないようにする
	DrawMocapData src1_t(src1);
	DrawMocapData src2_t(src2);

	//
	// 連結させる場所の決定
	//
	//int src1_frame, src2_frame;
	//PoseSimilarity sim = getConcatenationPoint(src1_frame, src2_frame, src1_t, src2_t, range);

	//
	// モーション2の正規化
	//
	//if(normalizeRootRot)
	//	MotionUtil::normalizeMotion(src2_t, src1_t.getPose(src1_frame), src2_frame);

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum;
	setConcatenationFrameNum(frameNum, src1_t, src2_t, src1_frame, src2_frame, maxConcatenationLength);
	if(frameNum%2 == 0) frameNum++;

	//
	// 補間部分の生成 (SlerpかSquadを使って，補間フレーム以外のreferをなくす)
	//
	DrawMocapData concatenation;
	//concatenation.LoadVacantMotion(&dstSkl, dst.getFps());
	if(useCenterAlign)
		makeConcatenationWithCenterAlign(concatenation, src1_t, src2_t, src1_frame, src2_frame, frameNum, maxConcatenationLength);
	else
		makeConcatenation(concatenation, src1_t, src2_t, src1_frame, src2_frame, frameNum);
	//
	// モーション1　->　モーション2　の結果生成
	//
	//DrawMocapData dammy;
	//makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame);
	if(useCenterAlign)
		makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame, frameNum);
	else
		makeResultMotion(dst, src1_t, concatenation, src2_t, src1_frame, src2_frame, 0);

	//
	// 連結の情報
	//
	cout << "[Concatenate] Concatenate points : src1 " << src1_frame << " -> src2 " << src2_frame << endl;
	//cout << "[Concatenate] Similarity : "<< sim.poseDiff << ", " << sim.velDiff << ", " << sim.rootDiff <<endl;

	cout << "******* End Concatenation *******" << endl;

	return;
}

/**
 * @fn static void getConcatenationPoint( int& src_frame, int& dst_frame, DrawMocapData& src, DrawMocapData& dst, int range)
 * @brief モーション1からモーション2へ接続するフレームを求める
 * @param src_frame [out]  モーション1の接続フレーム
 * @param dst_frame [out]  モーション2の接続フレーム
 * @param src  [in] モーション1
 * @param dst  [in]  モーション2
 * @param range [in]  連結フレームを探す範囲
 * @return 接続フレームの類似度を返す
 */
PoseSimilarity MotionConcatenation::getConcatenationPoint( int& src_frame, int& dst_frame, DrawMocapData& src, DrawMocapData& dst, int range)
{
	PoseSimilarity sim;
	sim.poseDiff = 100;
	sim.velDiff = 100;
	sim.rootDiff = 100;

	//探すフレーム長を調整( frameNum + LENGTH になることを保障する)
	if(src.getFrameNum() < range + LENGTH)
	{
		range = src.getFrameNum();
	}
	if(dst.getFrameNum() < range + LENGTH)
	{
		range = dst.getFrameNum();
	}

	
	for(int i = src.getFrameNum() - range - LENGTH ;i < src.getFrameNum() - LENGTH ; i++)
	{// src の最後から range フレーム分を探す
		//>TODO 足の接地判定を入れる
		for(int j = LENGTH; j < LENGTH + range; j++)
		{// detの LENGTH から LENGTH + range フレーム分を探す
			//>TODO ルートの位置も類似度計算に反映させる
			PoseSimilarity tmp = MotionUtil::getSimilarity(src, dst, i, j);
			if(tmp.poseDiff <= sim.poseDiff
				&& tmp.rootDiff <= sim.rootDiff
				&& tmp.velDiff <= sim.velDiff)
			{
				src_frame = i;
				dst_frame = j;
				sim = tmp;
			}
		}
	}

	//cout << "[Concatenate] similarity : " << poseDiff << ", " << v_diff << endl;

	return sim;

	////(判定接続フレームとして十分似ているか？)
	//if(poseDiff < MAX_POSE_DIFF
	//			&& rootDiff < MAX_ROOT_DIFF
	//			&& v_diff < MAX_VEL_DIFF)
	//{
	//	return true;
	//} 

	//return false;
}

/**
 * @fn static void setConcatenationFrameNum(int& frameNum, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1のあるフレームとモーション2のあるフレームを補間するモーションのフレーム数を求める
 * @param frameNum  [out]  補間モーションのフレーム数
 * @param src1    [in]  モーション1
 * @param src2    [in]  モーション2
 * @param src1_frame [in]  モーション1のあるフレーム
 * @param src2_frame [in]  モーション2のあるフレーム
 * @param maxframenum [in]  最大のフレーム数
 * @return なし
 */
void MotionConcatenation::setConcatenationFrameNum(int& frameNum, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int maxframenum)
{
	Pose src1_0 = src1.getPose(src1_frame - LENGTH);
	Pose src1_1 = src1.getPose(src1_frame);
	Pose src2_0 = src2.getPose(src2_frame);
	Pose src2_1 = src2.getPose(src2_frame + LENGTH);

	double max_diff = 0;
	int max_index = 0;
	double v = 0;

	for(int i=1; i < src1.getBoneNum(); i++)
	{
		// parametalize based JING WANG paper
		if(i == src1.getBoneIndex("rhumerus")
			|| i == src1.getBoneIndex("lhumerus")
			|| i == src1.getBoneIndex("rfemur")
			|| i == src1.getBoneIndex("lfemur")
			|| i == src1.getBoneIndex("rtibia")
			|| i == src1.getBoneIndex("ltibia")
			|| i == src1.getBoneIndex("rradius")
			|| i == src1.getBoneIndex("lradius")
		)
		{
			klVec3 src1_v = src1_1.getNodePositionFromBone(i, 0);
			klVec3 src2_v = src2_0.getNodePositionFromBone(i, 0);
			//diff += (src1_v - src2_v).Abs();
			//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		

			if(max_diff < (src1_v - src2_v).Abs())
			{
				max_diff = (src1_v - src2_v).Abs();
				max_index = i;
			}
		}
	}

	if(max_index > 0)
	{
		v = ((src1_1.getNodePositionFromBone(max_index, 0) - src1_0.getNodePositionFromBone(max_index, 0)).Abs()) / LENGTH;
		v -= ((src2_1.getNodePositionFromBone(max_index, 0) - src2_0.getNodePositionFromBone(max_index, 0)).Abs()) / LENGTH;

		v = fabs(v) / 2;
	}

	//q_tmp = multiply(src1_0.getBoneAngle(max_index), inverse(src2_1.getBoneAngle(max_index)));
	//max_diff = 2 * acos(q_tmp.w);
	if(v < 0.01)
	{
		frameNum = maxframenum;
	}
	else
	{
		frameNum = max_diff / fabs(v);

		if(frameNum > maxframenum)
		{
			frameNum = maxframenum;
		}
	}
	//if(frames > (this->src1.getFrameNum() - this->src1_start_frame))
	//{
	//	frames = this->src1.getFrameNum() - this->src1_start_frame;
	//}
	//if(frames > this->src2_end_frame)
	//{
	//	frames = this->src2_end_frame;
	//}

	//double v = 0;
	//int frame = 0;
	//klQuat q_tmp;
	//for(int i = 0; i < src1_0.getBoneNum(); i++)
	//{
	//	q_tmp = multiply(src1_0.getBoneAngle(i), inverse(src1_1.getBoneAngle(i)));
	//	v = 2 * acos(q_tmp.w);
	//	q_tmp = multiply(src2_0.getBoneAngle(i), inverse(src2_1.getBoneAngle(i)));
	//	v += 2 * acos(q_tmp.w) / 2;

	//	q_tmp = multiply(src1_0.getBoneAngle(i), inverse(src2_1.getBoneAngle(i)));
	//	int t_frame = (2 * acos(q_tmp.w) / v);

	//	if(t_frame > frame){
	//		frame = t_frame;
	//		this->max_diff_bone_name = src1_0.getBoneName(i);
	//	}
	//}

	//return frame;
}

/**
 * @fn static void makeConcatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮しない)
 * @param dst	  [out]  接続モーション
 * @param src1    [in]  モーション1
 * @param src1_frame [in]  モーション1のあるフレーム
 * @param src2    [in]  モーション2
 * @param src2_frame [in]  モーション2のあるフレーム
 * @param frameNum [in]  接続モーションのフレーム数
 * @return なし
 */
void MotionConcatenation::makeConcatenation(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum)
{

	for(int j = 0; j < frameNum; j++)
	{
		//Slerpを使う場合
		dst.AddPose(
				poseInterpolate( src1.getPose( src1_frame ), src2.getPose( src2_frame), (float)(j+1)/(float)(frameNum)
				));
		//Squadを使う場合
		//dst.AddPose(
		//	poseInterpolate( src1.getPose( src1_frame ), src2.getPose( src2_frame ), src1.getPose( src1_frame - LENGTH ),  src2.getPose( src2_frame + LENGTH ), (float)(j+1)/(float)(frameNum)
		//		));
	}
	
	return;
}

/**
 * @fn static void makeConcatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮する)
 * @param dst	  [out]  接続モーション
 * @param src1    [in]  モーション1
 * @param src1_frame [in]  モーション1のあるフレーム
 * @param src2    [in]  モーション2
 * @param src2_frame [in]  モーション2のあるフレーム
 * @param frameNum [in]  接続モーションのフレーム数
 * @return なし
 */
void MotionConcatenation::makeConcatenation2(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum)
{

	for(int j = 0; j < frameNum; j++)
	{
		//Slerpを使う場合
		dst.AddPose(
				poseInterpolate( src1.getPose( src1_frame + j ), src2.getPose( src2_frame - frameNum + j), (float)(j+1)/(float)(frameNum)
				));
		//Squadを使う場合
		//dst.AddPose(
		//	poseInterpolate( src1.getPose( src1_frame ), src2.getPose( src2_frame ), src1.getPose( src1_frame - LENGTH ),  src2.getPose( src2_frame + LENGTH ), (float)(j+1)/(float)(frameNum)
		//		));
	}
	
	return;
}

/**
 * @fn static void makeConcatenationWithCenterAlign(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum)
 * @brief モーション1のあるフレームからモーション2のあるフレームへつなぐモーションを求める(前後のフレームを考慮しない)
 * @param dst	  [out]  接続モーション
 * @param src1    [in]  モーション1
 * @param src1_frame [in]  モーション1のあるフレーム
 * @param src2    [in]  モーション2
 * @param src2_frame [in]  モーション2のあるフレーム
 * @param frameNum [in]  接続モーションのフレーム数
 * @return なし
 */
void MotionConcatenation::makeConcatenationWithCenterAlign(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum, int maxConcatenationLength)
{
	//if(frameNum%2 == 0) frameNum++;//奇数にする
	src1_frame = src1_frame - frameNum/2;
	src2_frame = src2_frame - frameNum/2;

	for(int j = 0; j < frameNum; j++)
	{
		//Slerpを使う場合
		dst.AddPose(
				poseInterpolate( src1.getPose( src1_frame + j), src2.getPose(src2_frame + j), (float)(j+1)/(float)(frameNum)
				));
		//Squadを使う場合
/*		dst.AddPose(
			poseInterpolate( src1.getPose( src1_frame ), src2.getPose( src2_frame ), src1.getPose( src1_frame - LENGTH ),  src2.getPose( src2_frame + LENGTH ), (float)(j+1)/(float)(frameNum)
				));
*/	}
	
	return;
}


/**
 * @fn static void poseInterpolate(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int rate)
 * @brief 姿勢1と姿勢2を補間するフレームを求める(球面線形補間)
 * @param src1    [in]  姿勢1
 * @param src2    [in]  姿勢2
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
 * @return 補間した姿勢
 */
Pose MotionConcatenation::poseInterpolate(Pose& src1, Pose& src2, float rate)
{
	Pose dest;
	dest.LoadVacantPose(src1.skeleton);
	dest.CorrectRoot();

	dest.SetRootPosition(MotionUtil::lerp(src1.getRootPosition(), src2.getRootPosition(), rate));
	dest.SetRootAngle(MotionUtil::slerp(src1.getRootAngle(), src2.getRootAngle(), rate, true));

	dest.SetBoneAngle(MotionUtil::slerp(src1.getBoneAngle(0), src2.getBoneAngle(0), rate, true), 0);
	for(int i=1; i < src1.getBoneNum(); i++)
	{
		//klVec3 src1_t = src1.getNodePosition(i);
		//klVec3 src2_t = src2.getNodePosition(i);

		//dest.SetBoneAngle(MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, false), i);
		//double diff_t1 = (dest.getNodePosition(i) - src1_t).Length()
		//	+ (dest.getNodePosition(i) - src2_t).Length();

		//dest.SetBoneAngle(MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, true), i);
		//double diff_t2 = (dest.getNodePosition(i) - src1_t).Length()
		//	+ (dest.getNodePosition(i) - src2_t).Length();

		klQuat src1_t = src1.getBoneAngle(i);
		klQuat src2_t = src2.getBoneAngle(i);
		//src2_t = inverse(src2_t); src2_t.w = -src2_t.w;

//			double diff_t1 = 2 * acos(multiply(inverse(src1_t), MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, false)).w)
//				+ 2 * acos(multiply(inverse(src2_t), MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, false)).w);

//			double diff_t2 = 2 * acos(multiply(inverse(src1_t), MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, true)).w)
//				+ 2 * acos(multiply(inverse(src2_t), MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), 0.5, true)).w);


//			if(diff_t1 >= diff_t2)
//			{
//				dest.SetBoneAngle(MotionAlignment::SLERP(src1_t, src2_t, rate, true),i);
		//klVec3 tmp1 = src1.getNodePositionFromBone(i, src1.getBoneParentNum(i)).Normalize();
		//klVec3 offset1 = src1.getBoneDirection(i);
		//src1_t = Converter::TwoVec_to_Quat(offset1, tmp1);

		//klVec3 tmp2 = src2.getNodePositionFromBone(i, src2.getBoneParentNum(i)).Normalize();
		//klVec3 offset2 = src2.getBoneDirection(i);
		//src2_t = Converter::TwoVec_to_Quat(offset2, tmp2);

		//vector<klQuat> in_q;
		//in_q.push_back(src1_t);
		//in_q.push_back(src2_t);

		//vector<float> kernel;
		//kernel.push_back(1 - rate);
		//kernel.push_back(rate);

		//dest.SetBoneAngle(MotionAlignment::expMap(in_q, kernel, true), i);

		if(src1_t.w < 0){src1_t = !(src1_t); src1_t.w *= -1.0;}
		if(src2_t.w < 0){src2_t = !(src2_t); src2_t.w *= -1.0;}

		klQuat res = MotionUtil::slerp(src1_t, src2_t, rate, true);
		dest.SetBoneAngle(res,i);
//			}
//			else
//			{
//				dest.SetBoneAngle(MotionAlignment::SLERP(src1.getBoneAngle(i),src2.getBoneAngle(i), rate, false),i);
//			}
	}

	return dest;
}

/**
 * @fn static Pose poseInterpolate(Pose& src1, Pose& src2, Pose& src0, Pose& src3, double rate)
 * @brief 姿勢1と姿勢2を補間するフレームを求める(球面線形二次補間)
 * @param src1    [in]  姿勢1
 * @param src2    [in]  姿勢2
 * @param src0    [in]  姿勢1の前の姿勢
 * @param src3    [in]  姿勢2の前の姿勢
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
 * @return 補間した姿勢
 */
//Pose MotionConcatenation::poseInterpolate(Pose& src1, Pose& src2, Pose& src0, Pose& src3, double rate)
//{
//	Pose dest;
//	dest.LoadVacantPose(src1.skeleton);
//	dest.CorrectRoot();
//
//	dest.SetRootPosition(MotionUtil::lerp(src1.getRootPosition(), src2.getRootPosition(), rate));
//
//	dest.SetRootAngle(MotionUtil::squad(src0.getRootAngle(), src1.getRootAngle(), src2.getRootAngle(), src3.getRootAngle(), rate));
//	for(int i=0; i < src1.getBoneNum(); i++)
//	{
//		dest.SetBoneAngle(MotionUtil::squad(src0.getBoneAngle(i), src1.getBoneAngle(i), src2.getBoneAngle(i), src3.getBoneAngle(i), rate),i);
//	}
//
//	return dest;
//}

/**
 * @fn static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1とモーション2とその補間モーションを接続したモーションを作る
 * @param dst    [out]  接続したモーション
 * @param src1    [in]  モーション1
 * @param concatenation    [in]  補間部分のモーション
 * @param src2    [in]  モーション2
 * @param src1_frame    [in]  モーション1の接続フレーム
 * @param src2_frame    [in]  モーション2の接続フレーム
 * @return なし
 */
void MotionConcatenation::makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int src1_frame, int src2_frame, int frameNum)
{
	dst.ClearFrames();
	dst.setFps(src1.getFps());
	dst.setFrameTime(src1.getFrameTime());

	if(frameNum == 0)
	{//普通につなぐ場合
		for(int i = 0; i < src1_frame; i++)
		{
			dst.AddPose(src1.getPose(i));
		}

		if(concatenation.getFrameNum() > 0)
		{
			for(int j = 0; j < concatenation.getFrameNum(); j++)
			{
				dst.AddPose(concatenation.getPose(j));			
			}
		}

		for(int k = src2_frame; k < src2.getFrameNum(); k++)
		{
			dst.AddPose(src2.getPose(k));
		}
	}
	else
	{//CenterAlignの場合
		for(int i = 0; i < src1_frame - frameNum/2; i++)
		{
			dst.AddPose(src1.getPose(i));
		}

		if(concatenation.getFrameNum() > 0)
		{
			for(int j = 0; j < frameNum; j++)
			{
				dst.AddPose(concatenation.getPose(j));			
			}
		}

		for(int k = src2_frame + frameNum/2; k < src2.getFrameNum(); k++)
		{
			dst.AddPose(src2.getPose(k));
		}
	}

	return;
}

/**
 * @fn static void makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1とモーション2とその補間モーションを接続したモーションを作る
 * @param dst    [out]  接続したモーション
 * @param src1    [in]  モーション1
 * @param concatenation    [in]  補間部分のモーション
 * @param src2    [in]  モーション2
 * @param frameNum    [in]  CenterAlign方式の場合必要
 * @return なし
 */
void MotionConcatenation::makeResultMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& concatenation, DrawMocapData& src2, int frameNum)
{
	dst.ClearFrames();

	//if(frameNum%2 == 0) frameNum++;

	for(int i = 0; i < src1.getFrameNum() - frameNum/2; i++)
	{
		dst.AddPose(src1.getPose(i));
	}

	if(concatenation.getFrameNum() > 0)
	{
		for(int j = 0; j < concatenation.getFrameNum(); j++)
		{
			dst.AddPose(concatenation.getPose(j));			
		}
	}

	for(int k = frameNum/2; k < src2.getFrameNum(); k++)
	{
		dst.AddPose(src2.getPose(k));
	}

	return;
}

/**
 * @fn void loadPipeMotion(string dir_path, bool is_recursion, Skeleton skl, int fps = 60)
 * @brief 接続用のモーションをロードする
 * @param mocap_dir    [in] 接続用モーション全般フォルダ
 * @param left_to_right_mocap_dir    [in] 左足前→右足前の遷移モーションフォルダ
 * @param right_to_left_mocap_dir    [in] 左足前→右足前の遷移モーションフォルダ
 * @param is_recursion    [in]  フォルダ内を再帰的に読み込むか
 * @param skl    [in]  モーションのスケルトン
 * @param fps    [in]  モーションのFPS(デフォルト：60)
 * @return なし
 */
void MotionConcatenation::loadPipeMotions(string mocap_dir, string left_to_right_mocap_dir, string right_to_left_mocap_dir, bool is_recursion, int fps)
{	
	Initialize();
	vector<string> names;
	FileSystemUtil::getFileNameInDirectory(names, mocap_dir, ".V", is_recursion );
	for(unsigned int i=0; i < names.size(); i++)
	{
		DrawMocapData pipe_motion;
		pipe_motion.LoadMotion(names[i]);
		pipe_motions.push_back(pipe_motion);
	}

	FileSystemUtil::getFileNameInDirectory(names, left_to_right_mocap_dir, ".V", is_recursion );
	for(unsigned int i=0; i < names.size(); i++)
	{
		DrawMocapData pipe_motion;
		pipe_motion.LoadMotion(names[i]);
		left_to_right_pipe_motions.push_back(pipe_motion);
	}

	FileSystemUtil::getFileNameInDirectory(names, right_to_left_mocap_dir, ".V", is_recursion );
	for(unsigned int i=0; i < names.size(); i++)
	{
		DrawMocapData pipe_motion;
		pipe_motion.LoadMotion(names[i]);
		right_to_left_pipe_motions.push_back(pipe_motion);
	}


	this->isLoadPipeMotion = true;
}


/**
 * @fn static bool getPipeMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2)
 * @brief モーション1とモーション2をつなぐモーションを得る
 * @param dst    [out] 接続モーション
 * @param src1    [in]  モーション1
 * @param src2    [in]  モーション2
 * @param footTrans    [in]  足の位置関係情報
 * @param range [in]  連結フレームを探す範囲
 * @return 必要かどうかの判定
 */
bool MotionConcatenation::getPipeMotion(DrawMocapData& dst, DrawMocapData& src1, DrawMocapData& src2, int footTrans, int range)
{
	int src1_frame, src2_frame;

	if(!this->isLoadPipeMotion)
	{
		cout << "no loaded pipe motions";
		return false;
	}


	//
	//   src1 motion  ->         pipe motion          ->   src2 motion
	//    src1_frame  -> pipe_frame_1   pipe_frame_2  ->    src2-frame
	int src1_frame_t, pipe_frame1_t, pipe_frame1, pipe_frame2;
	int index = -1;
	double sim_max = 100;//dammy
	PoseSimilarity sim_max_src1_pipe, sim_max_pipe_src2;
	DrawMocapData pipe_motion;


	// 左足前　->　右足前　と　右足前　->　左足前　は区別したデータベースから検索する

	if(footTrans == LeftToRight)
	{
		for(unsigned int i = 0; i < left_to_right_pipe_motions.size(); i++)
		{
			PoseSimilarity sim_src1_pipe = getConcatenationPoint(src1_frame_t, pipe_frame1_t, src1, left_to_right_pipe_motions[i], range);
			PoseSimilarity sim_pipe_src2 = getConcatenationPoint(pipe_frame2, src2_frame, left_to_right_pipe_motions[i], src2, range);
		
			if(sim_max > sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff)
			{
				// src1 -> pipe
				sim_max_src1_pipe.poseDiff = sim_src1_pipe.poseDiff;
				sim_max_src1_pipe.rootDiff = sim_src1_pipe.rootDiff;

				// pipe -> src2
				sim_max_pipe_src2.poseDiff = sim_pipe_src2.poseDiff;
				sim_max_pipe_src2.rootDiff = sim_pipe_src2.rootDiff;

				sim_max = sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff;
				src1_frame = src1_frame_t;
				pipe_frame1 = pipe_frame1_t;
				index = i;
			}
		}
		pipe_motion = left_to_right_pipe_motions[index];
	}
	else if( footTrans == RightToLeft)
	{
		for(unsigned int i = 0; i < right_to_left_pipe_motions.size(); i++)
		{
			PoseSimilarity sim_src1_pipe = getConcatenationPoint(src1_frame_t, pipe_frame1_t, src1, right_to_left_pipe_motions[i], range);
			PoseSimilarity sim_pipe_src2 = getConcatenationPoint(pipe_frame2, src2_frame, right_to_left_pipe_motions[i], src2, range);
		
			if(sim_max > sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff)
			{
				// src1 -> pipe
				sim_max_src1_pipe.poseDiff = sim_src1_pipe.poseDiff;
				sim_max_src1_pipe.rootDiff = sim_src1_pipe.rootDiff;

				// pipe -> src2
				sim_max_pipe_src2.poseDiff = sim_pipe_src2.poseDiff;
				sim_max_pipe_src2.rootDiff = sim_pipe_src2.rootDiff;

				sim_max = sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff;
				src1_frame = src1_frame_t;
				pipe_frame1 = pipe_frame1_t;
				index = i;
			}
		}
		pipe_motion = right_to_left_pipe_motions[index];
	}
	else //if(sim.poseDiff > MAX_POSE_DIFF * 2)
	{//それ以外は制約を厳しくしてやる
		for(unsigned int i = 0; i < pipe_motions.size(); i++)
		{
			PoseSimilarity sim_src1_pipe = getConcatenationPoint(src1_frame_t, pipe_frame1_t, src1, pipe_motions[i], range);
			PoseSimilarity sim_pipe_src2 = getConcatenationPoint(pipe_frame2, src2_frame, pipe_motions[i], src2, range);
	
			if(sim_max > sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff
				/*&& sim_src1_pipe.poseDiff < MAX_POSE_DIFF && sim_pipe_src2.poseDiff < MAX_POSE_DIFF
				&& sim_src1_pipe.rootDiff < MAX_ROOT_DIFF && sim_pipe_src2.rootDiff < MAX_ROOT_DIFF*/)
			{
				// src1 -> pipe
				sim_max_src1_pipe.poseDiff = sim_src1_pipe.poseDiff;
				sim_max_src1_pipe.rootDiff = sim_src1_pipe.rootDiff;

				// pipe -> src2
				sim_max_pipe_src2.poseDiff = sim_pipe_src2.poseDiff;
				sim_max_pipe_src2.rootDiff = sim_pipe_src2.rootDiff;

				sim_max = sim_src1_pipe.poseDiff + sim_pipe_src2.poseDiff + sim_src1_pipe.rootDiff + sim_pipe_src2.rootDiff;
				src1_frame = src1_frame_t;
				pipe_frame1 = pipe_frame1_t;
				index = i;
			}
		}
		pipe_motion = pipe_motions[index];
	}
	//else
	//{
	//	return false;
	//}

	cout << "[Concatenation] pipe motion : file name :" << pipe_motions[index].getName() << endl;
	cout << "[Concatenation] pipe motion : pose diff :" << sim_max_src1_pipe.poseDiff << ", " << sim_max_pipe_src2.poseDiff << endl;
	cout << "[Concatenation] pipe motion : root diff :" << sim_max_src1_pipe.rootDiff << ", " << sim_max_pipe_src2.rootDiff << endl;
	cout << "[Concatenation] pipe motion : footTrans :" << footTrans << endl;

	//PipeMotionとの接続性が悪ければ使わない
	if(sim_max_src1_pipe.poseDiff > MAX_POSE_DIFF || sim_max_pipe_src2.poseDiff > MAX_POSE_DIFF
		|| sim_max_src1_pipe.rootDiff > MAX_ROOT_DIFF || sim_max_pipe_src2.rootDiff > MAX_ROOT_DIFF)
	{
		return false;
	}

	//
	// モーション1 と PipeMotionの接続
	//


	//
	// PipeMotionの正規化
	//
	//sim = getSimilarity(src1, pipe_motion, src1_frame, pipe_frame1);
	//cout << "[Concatenation] pipe motion concatenate: " << sim.poseDiff << ", " << sim.rootDiff << endl;

	MotionUtil::normalizeMotion(pipe_motion, src1.getPose(src1_frame), pipe_frame1);

	//
	// 補間部分のフレーム数を決める
	//
	int frameNum;
	setConcatenationFrameNum(frameNum, src1, pipe_motion, src1_frame, pipe_frame1);

	//
	// 補間部分の生成 (SlerpかSquadを使って，補間フレーム以外のreferをなくす)
	//
	DrawMocapData concatenation;
	makeConcatenation(concatenation, src1, pipe_motion, src1_frame, pipe_frame1, frameNum);

	//
	// モーション1　->　PipeMotion　の結果生成
	//
	makeResultMotion(dst, src1, concatenation, pipe_motion, src1_frame, pipe_frame1);


	//
	// モーション1(PipeMotion追加)とモーション2の接続
	//
	// ここではやらない
			

	return true;
}


/**
 * @fn static int getInfoFootTransition(Pose& src1, Pose& src2)
 * @brief 姿勢1と姿勢2の足の位置関係を得る
 * @param src1    [in]  姿勢1
 * @param src2    [in]  姿勢2
 * @return 足の位置関係
 */
int MotionConcatenation::getInfoFootTransition(Pose& src1, Pose& src2)
{
	klVec3 rfoot_src1_pos = src1.getNodePositionFromRoot(src1.getBoneIndex("rfemur"));
	klVec3 lfoot_src1_pos = src1.getNodePositionFromRoot(src1.getBoneIndex("lfemur"));
	klVec3 rfoot_src2_pos = src2.getNodePositionFromRoot(src2.getBoneIndex("rfemur"));
	klVec3 lfoot_src2_pos = src2.getNodePositionFromRoot(src2.getBoneIndex("lfemur"));

	int src1_state = 0;
	int src2_state = 0;

	//姿勢1
	if(rfoot_src1_pos.x > 0 && lfoot_src1_pos.x < 0)
	{//右足が前
		src1_state = 1;
	}else if(rfoot_src1_pos.x < 0 && lfoot_src1_pos.x > 0)
	{//左足が前
		src1_state = 2;
	}else
	{//それ以外
		src1_state = 0;
	}

	//姿勢2
	if(rfoot_src2_pos.x > 0 && lfoot_src2_pos.x < 0)
	{//右足が前
		src2_state = 1;
	}else if(rfoot_src2_pos.x < 0 && lfoot_src2_pos.x > 0)
	{//左足が前
		src2_state = 2;
	}else
	{//それ以外
		src2_state = 0;
	}

	int result;
	if(src1_state == 0 && src2_state == 0)
	{// それ以外 -> それ以外
		result = MotionConcatenation::OtherToOther;						
	}else if(src1_state == 0 && src2_state == 1)
	{// それ以外 -> 右足前
		result = MotionConcatenation::OtherToRight;
	}else if(src1_state == 0 && src2_state == 2)
	{// それ以外 -> 左足前
		result = MotionConcatenation::OtherToLeft;
	}else if(src1_state == 1 && src2_state == 0)
	{// 右足前 -> それ以外
		result = MotionConcatenation::RightToOther;
	}else if(src1_state == 1 && src2_state == 1)
	{// 右足前 -> 右足前
		result = MotionConcatenation::RightToRight;
	}else if(src1_state == 1 && src2_state == 2)
	{// 右足前 -> 左足前
		result = MotionConcatenation::RightToLeft;
	}else if(src1_state == 2 && src2_state == 0)
	{// 左足前 -> それ以外
		result = MotionConcatenation::LeftToOther;
	}else if(src1_state == 2 && src2_state == 1)
	{// 左足前 -> 右足前
		result = MotionConcatenation::LeftToRight;
	}else if(src1_state == 2 && src2_state == 2)
	{// 左足前 -> 左足前
		result = MotionConcatenation::LeftToLeft;
	}

	return result;
}


//Poseの類似度を求める．
//座標正規化行列に基づく
//static float poseSimilarity(Pose& src1, Pose& src2)
//{
//	vector<klVec3> pc1; pc1.push_back(Nino::Converter::toklVec3(src1.getRootPosition()));
//	vector<klVec3> pc2; pc2.push_back(Nino::Converter::toklVec3(src2.getRootPosition()));
//	for(int i=0; i < src1.getBoneNum(); i++)
//	{
//		pc1.push_back(src1.getNodePosition(i));
//		pc2.push_back(src2.getNodePosition(i));
//	}

//	klMat44 trans = CalcTransformBetweenFigures(pc1, pc2);

//	klVec3 diff;
//	float dist =0.0f;
//	for (int j = 0;j < pc1.size();++j) {
//		diff = trans * pc2[j];
//		diff = pc1[j] - diff;
//		dist += diff.Abs();
//	}
//	return sqrtf(dist / static_cast<float>(pc1.size()));

//}


//static void Interpolation(DrawMocapData& dest, DrawMocapData& src1, DrawMocapData& src2,
//	int src1_start, int src2_goal)
//{
//	this->src1 = src1;
//	this->src2 = src2;
//	this->dest = &dest;

//	this->skl = (src1.getPose(0)).skeleton;
//	this->fps = src1.getFps();

//	this->src1_start_frame = src1_start;
//	this->src2_end_frame = src2_goal;

//	this->canInterpolate = true;

//	if(!src1.getIsLoaded() || !src2.getIsLoaded())
//	{
//		cout << "No loaded" << endl;
//		this->canInterpolate = false;
//		return;
//	}
//	else if((&src1)->getBoneNum() != ((&src2)->getBoneNum()))
//	{
//		cout << "Bone num is different." << endl;
//		this->canInterpolate = false;
//		return;
//	}

//	if( !this->setStartEnd() )
//	{
//		this->canInterpolate = false;
//		return;
//	};

//	this->interpolateFrames = this->setFrameNum();

//	if(this->interpolateFrames + this->src1_start_frame > this->src1.getFrameNum())
//	{
//		cout << "src1 doesn't have frames." << endl;
//		this->canInterpolate = false;
//		return;
//	}
//	else if(this->interpolateFrames > this->src2_end_frame)
//	{
//		cout << "src2 doesn't have frames." << endl;
//		this->canInterpolate = false;
//		return;
//	}

//	if(poseSimilarity(this->src1.getPose(src1_start_frame), this->src2.getPose(src2_end_frame)) > MAX_POSE_DIFF)
//	{
//		cout << "Pose is totally different." << endl;
//		this->canInterpolate = false;
//		return;
//	}
//}

//static bool setStartEnd()
//{
//	double diff;

//	if(this->src1_start_frame + RANGE > this->src1.getFrameNum())
//	{
//		this->src1_range = this->src1.getFrameNum() - LENGTH;
//	}
//	else
//	{
//		this->src1_range = this->src1_start_frame + RANGE;
//	}

//	if(this->src2_end_frame + RANGE > this->src2.getFrameNum())
//	{
//		this->src2_range = this->src2.getFrameNum() - LENGTH;
//	}
//	else
//	{
//		this->src2_range = this->src2_end_frame + RANGE;
//	}

//	for(int j = this->src2_end_frame; j < this->src2_range ; j++)
//	{
//		if(onFly(src2.getPose(j),"rfoot") && !onFly(src2.getPose(j),"lfoot"))
//		{// ??? -> single support by lfoot
//			for(int i = this->src1_start_frame; i < this->src1_range; i++)
//			{
//				if(!onFly(src1.getPose(i), "lfoot"))
//				{ // lfoot support or double support -> lfoot support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, false);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						this->src1_start_frame = i;
//						this->src2_end_frame = j;
//						this->rightFootonGround = false;
//						return true;
//					}
//				}
//			}
//		}
//		else if(onFly(src2.getPose(j), "lfoot") && !onFly(src2.getPose(j),"rfoot"))
//		{// ??? -> single supprt by rfoot
//			for(int i = this->src1_start_frame; i < this->src1_range; i++)
//			{
//				if(!onFly(src1.getPose(i), "rfoot"))
//				{ // rfoot support or double support -> rfoot support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						this->src1_start_frame = i;
//						this->src2_end_frame = j;
//						this->rightFootonGround = true;
//						return true;
//					}
//				}
//			}
//		}
//		else if(!onFly(src2.getPose(j), "lfoot") && !onFly(src2.getPose(j),"rfoot"))
//		{// ??? -> double support
//			for(int i = this->src1_start_frame; i < this->src1_range; i++)
//			{
//				if(onFly(src1.getPose(i), "rfoot") && !onFly(src1.getPose(j),"lfoot"))
//				{ // lfoot supoort -> double support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						this->src1_start_frame = i;
//						this->src2_end_frame = j;
//						this->rightFootonGround = false;
//						return true;
//					}
//				}else if(onFly(src1.getPose(i), "lfoot") && !onFly(src1.getPose(j),"rfoot"))
//				{ // rfoot support -> double support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						this->src1_start_frame = i;
//						this->src2_end_frame = j;
//						this->rightFootonGround = true;
//						return true;
//					}
//				}
//			}
//		}
//	}
//	cout << "no candidate to interpolate." << endl;
//	return false;
//}

//static double poseSimilarity(Pose& src1, Pose& src2)
//{
//	double diff = 0;
//	double max_diff = 0;
//	int index = 0;
//	for(int i=1; i < src1.getBoneNum(); i++)
//	{
//		klVec3 src1_v = src1.getNodePositionFromBone(i, src1.getRootIndex());
//		klVec3 src2_v = src2.getNodePositionFromBone(i, src2.getRootIndex());
//		diff += (src1_v - src2_v).Abs();
//		//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		

//		if(max_diff < (src1_v - src2_v).Abs())
//		{
//			max_diff = (src1_v - src2_v).Abs();
//			index = i;
//		}
//	}

//	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;
//	return diff;
//}

//static void poseSimilarityForStartEnd(Pose& src1, Pose& src2, int src1_index, int src2_index,  bool onRightFootSupport)
//{
//	this->diff = 0;
//	this->rootDiff = 0;

//	double max_diff = 0;
//	int index = 0;

//	if(onRightFootSupport)
//	{			
//		klVec3 src1_toe = Converter::toklVec3(src1.getNodePosition(src1.getBoneIndex("rtibia")));
//		klVec3 src2_toe = Converter::toklVec3(src2.getNodePosition(src2.getBoneIndex("rtibia")));

//		klVec3 base_root_pos = src1_toe + src2.getRootPosition() - src2_toe;
//		
//		this->rootDiff = Converter::toklVec3(base_root_pos - src1.getRootPosition()).Abs();

//	}else
//	{

//		klVec3 src1_toe = Converter::toklVec3(src1.getNodePosition(src1.getBoneIndex("ltibia")));
//		klVec3 src2_toe = Converter::toklVec3(src2.getNodePosition(src2.getBoneIndex("ltibia")));

//		klVec3 base_root_pos = src1_toe + src2.getRootPosition() - src2_toe;
//		
//		this->rootDiff = Converter::toklVec3(base_root_pos - src1.getRootPosition()).Abs();
//	}

//	// velosity similarity 

//	this->v_diff = 0;
//	Pose src1_1 = this->src1.getPose(src1_index + LENGTH);
//	Pose src2_0 = this->src2.getPose(src2_index - LENGTH);

//	klQuat q_tmp;
//	double v_src1, v_src2;
//	for(int i = 0; i < src1.getBoneNum(); i++)
//	{
//		q_tmp = multiply(src1.getBoneAngle(i), inverse(src1_1.getBoneAngle(i)));
//		v_src1 = 2 * acos(q_tmp.w) / LENGTH;
//		q_tmp = multiply(src2_0.getBoneAngle(i), inverse(src2.getBoneAngle(i)));
//		v_src2 = 2 * acos(q_tmp.w) / LENGTH;
//		this->v_diff += fabs(v_src1 - v_src2);
//	}

//	// pose similarity 

//	this->diff = 0;
//	for(int i=1; i < src1.getBoneNum(); i++)
//	{
//		klVec3 src1_v = src1.getNodePositionFromBone(i, src1.getRootIndex());
//		klVec3 src2_v = src2.getNodePositionFromBone(i, src2.getRootIndex());

//		this->diff += (src1_v - src2_v).Abs();
//		//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		


//		if(MAX_POS_DIFF < (src1_v - src2_v).Abs())
//		{
//			//klQuat src1_q = src1.getBoneAngle(i);
//			//klQuat src2_q = src2.getBoneAngle(i);
//			Pose src2_1 = this->src2.getPose(src2_index + LENGTH);
//			q_tmp = multiply(src1.getBoneAngle(i), inverse(src1_1.getBoneAngle(i)));
//			v_src1 = 2 * acos(q_tmp.w) / LENGTH;
//			q_tmp = multiply(src2_0.getBoneAngle(i), inverse(src2.getBoneAngle(i)));
//			v_src2 = 2 * acos(q_tmp.w) / LENGTH;
//			q_tmp = multiply(src1_1.getBoneAngle(i), inverse(src1.getBoneAngle(i)));
//			double dist1 = 2 * acos(q_tmp.w);
//			q_tmp = multiply(src2_1.getBoneAngle(i), inverse(src1.getBoneAngle(i)));
//			double dist2 = 2 * acos(q_tmp.w);

//			if((v_src1 * v_src2 > 0) && (dist1 - dist2 > ROUND ))
//			{
//				this->diff = INFINITE;
//				return;
//			}
//		}

//		if(max_diff < (src1_v - src2_v).Abs())
//		{
//			max_diff = (src1_v - src2_v).Abs();
//			index = i;
//		}
//	}
//	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;
//}

//static int setFrameNum()
//{
//	Pose src1_0 = src1.getPose(this->getStartFrame_src1());
//	Pose src1_1 = src1.getPose(this->getStartFrame_src1() + LENGTH);
//	Pose src2_0 = src2.getPose(this->getEndFrame_src2() - LENGTH);
//	Pose src2_1 = src2.getPose(this->getEndFrame_src2());

//	double max_diff = 0;
//	int max_index = 0;
//	double v;

//	for(int i=1; i < src1.getBoneNum(); i++)
//	{
//		// only clavicle, humerus, femur, tibia use
//		if(i == src1.getBoneIndex("rclavicle")
//			|| i == src1.getBoneIndex("lclavicle")
//			|| i == src1.getBoneIndex("rhumerus")
//			|| i == src1.getBoneIndex("lhumerus")
//			|| i == src1.getBoneIndex("rfemur")
//			|| i == src1.getBoneIndex("lfemur")
//			|| i == src1.getBoneIndex("rtibia")
//			|| i == src1.getBoneIndex("ltibia")
//		)
//		{
//			klVec3 src1_v = src1_0.getNodePositionFromBone(i, src1.getRootIndex());
//			klVec3 src2_v = src2_1.getNodePositionFromBone(i, src2.getRootIndex());
//			//diff += (src1_v - src2_v).Abs();
//			//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		

//			if(max_diff < (src1_v - src2_v).Abs())
//			{
//				max_diff = (src1_v - src2_v).Abs();
//				max_index = i;
//			}
//		}
//	}

//	//klQuat q_tmp;
//	//double w, v;
//	//double max_w = 0;
//	//for(int i = 0; i < src1_0.getBoneNum(); i++)
//	//{
//	//	if(i == src1.getBoneIndex("rclavicle")
//	//		|| i == src1.getBoneIndex("lclavicle")
//	//		|| i == src1.getBoneIndex("rhumerus")
//	//		|| i == src1.getBoneIndex("lhumerus")
//	//		|| i == src1.getBoneIndex("rfemur")
//	//		|| i == src1.getBoneIndex("lfemur")
//	//		|| i == src1.getBoneIndex("rtibia")
//	//		|| i == src1.getBoneIndex("ltibia")
//	//	)
//	//	{
//	//		q_tmp = multiply(src1_0.getBoneAngle(i), inverse(src2_1.getBoneAngle(i)));
//	//		w = 2 * acos(q_tmp.w);

//	//		if(max_w < fabs(w))
//	//		{
//	//			max_index = i;
//	//			max_w = w;
//	//			this->max_diff_bone_name = src1_0.getBoneName(i);
//	//		}
//	//	}
//	//}

//	v = ((src1_1.getNodePositionFromBone(max_index, src1.getRootIndex()) - src1_0.getNodePositionFromBone(max_index, src1.getRootIndex())).Abs()) / LENGTH;
//	v += ((src2_1.getNodePositionFromBone(max_index, src2.getRootIndex()) - src2_0.getNodePositionFromBone(max_index, src2.getRootIndex())).Abs()) / LENGTH;

//	//q_tmp = multiply(src1_0.getBoneAngle(max_index), inverse(src1_1.getBoneAngle(max_index)));
//	//v = 2 * acos(q_tmp.w) / LENGTH;
//	//q_tmp = multiply(src2_0.getBoneAngle(max_index), inverse(src2_1.getBoneAngle(max_index)));
//	//v += 2 * acos(q_tmp.w) / LENGTH;
//	v /= 2;

//	//q_tmp = multiply(src1_0.getBoneAngle(max_index), inverse(src2_1.getBoneAngle(max_index)));
//	//max_diff = 2 * acos(q_tmp.w);
//	int frames = max_diff / v;
//	if(frames > (this->src1.getFrameNum() - this->src1_start_frame))
//	{
//		frames = this->src1.getFrameNum() - this->src1_start_frame;
//	}
//	if(frames > this->src2_end_frame)
//	{
//		frames = this->src2_end_frame;
//	}
//	return frames;

//	//double v = 0;
//	//int frame = 0;
//	//klQuat q_tmp;
//	//for(int i = 0; i < src1_0.getBoneNum(); i++)
//	//{
//	//	q_tmp = multiply(src1_0.getBoneAngle(i), inverse(src1_1.getBoneAngle(i)));
//	//	v = 2 * acos(q_tmp.w);
//	//	q_tmp = multiply(src2_0.getBoneAngle(i), inverse(src2_1.getBoneAngle(i)));
//	//	v += 2 * acos(q_tmp.w) / 2;

//	//	q_tmp = multiply(src1_0.getBoneAngle(i), inverse(src2_1.getBoneAngle(i)));
//	//	int t_frame = (2 * acos(q_tmp.w) / v);

//	//	if(t_frame > frame){
//	//		frame = t_frame;
//	//		this->max_diff_bone_name = src1_0.getBoneName(i);
//	//	}
//	//}

//	//return frame;

//}


//static bool interpolate()
//{
//	if(!this->canInterpolate)
//	{
//		cout << "Can't interpolate" << endl;
//		return false;
//	}

//	dest->LoadVacantMotion(skl, fps);

//	for(int i = 0; i < src1_start_frame; i++)
//	{
//		Pose p = src1.getPose(i);
//		dest->AddPose(p);
//	}

//	setsrc2_relative();

//	for(int j = 0; j < interpolateFrames; j++)
//	{
//		dest->AddPose(
//				poseInterpolate(
//					src1.getPose(this->src1_start_frame + j),
//					src2_relative.getPose(j),
//					(float)(j+1)/(float)(this->interpolateFrames)
//				)					
//			);
//		
//	}

//	for(int i = interpolateFrames; i < src2_relative.getFrameNum(); i++)
//	{
//		dest->AddPose(src2_relative.getPose(i));
//	}

//	return true;
//}

//static klQuat SLERP(klQuat& q1, klQuat& q2, float rate)
//{
//	q1 = normalize(q1);
//	q2 = normalize(q2);

//	float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

//	klQuat correct_q2;
//	if(dot < 0.f){
//		correct_q2 = q2;
//		dot = -dot;
//	}else{
//		correct_q2 = q2;
//	}

//	if(dot >= 1.f)
//	{
//		dot = 1.f;
//	}


//	float radian = acos(dot);
//	if(fabs(dot) < klVec4::epsilon){ return correct_q2; }
//	if(radian == 0.f){
//		return correct_q2;
//	}
//	float inverseSin = 1.f / sin(radian);
//	float leftScale = sin((1.f - rate) * radian ) * inverseSin;
//	float rightScale = sin(rate * radian) * inverseSin;
//	return Multi(q1,leftScale) + Multi(correct_q2,rightScale);
//}

//static klVec3 LERP(klVec3& v1, klVec3& v2, float rate)
//{
//	klVec3 v;
//	v.x	= v1.x * (1.0 - rate) + v2.x * rate;
//	v.y = v1.y * (1.0 - rate) + v2.y * rate;
//	v.z = v1.z * (1.0 - rate) + v2.z * rate;

//	return v;
//}

//static klQuat Multi(klQuat q1, float scale)
//{
//	q1.x = q1.x * scale;
//	q1.y = q1.y * scale;
//	q1.z = q1.z * scale;
//	q1.w = q1.w * scale;
//	return q1;
//}
//
//static bool getsrc2_relative(DrawMocapData& relative)
//{
//	if(!this->canInterpolate)
//	{
//		return false;
//	}

//	if(this->rightFootonGround)
//	{ // right foot on the ground
//		Pose p;
//		Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("rtibia")));
//		Pose p_src2 = src2.getPose(this->src2_end_frame);
//		klVec3 src2_pos = p_src2.getRootPosition();
//		klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("rtibia")));

//		klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
//		klQuat base_root_rot;
//		getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
//		
//		for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		{
//			p = src2.getPose(i);
//			p.SetRootPosition(
//				(p.getRootPosition() - src2_pos) + base_root_pos
//				);
//			p.SetRootAngle(
//				multiply(p.getRootAngle(),inverse(base_root_rot)));
//			relative.AddPose(p);
//		}

//	}
//	else
//	{ // left foot on the ground
//		Pose p;
//		Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("ltibia")));
//		Pose p_src2 = src2.getPose(this->src2_end_frame);
//		klVec3 src2_pos = p_src2.getRootPosition();
//		klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("ltibia")));

//		klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
//		klQuat base_root_rot;
//		getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
//		
//		for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		{
//			p = src2.getPose(i);
//			p.SetRootPosition(
//				(p.getRootPosition() - src2_pos) + base_root_pos
//				);
//			p.SetRootAngle(
//				multiply(p.getRootAngle(),inverse(base_root_rot)));
//			src2_relative.AddPose(p);
//		}
//		//Pose p;
//		//Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		//Pose p_src2 = src2.getPose(this->src2_end_frame);
//		//klVec3 src2_pos = p_src2.getRootPosition();

//		//for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		//{
//		//	p = src2.getPose(i);
//		//	p.SetRootPosition(
//		//		(p.getRootPosition() - src2_pos) + src1_pos
//		//		);
//		//	src2_relative.AddPose(p);
//		//}
//	}
//	return true;
//}

//static void setsrc2_relative()
//{
//	if(this->rightFootonGround)
//	{ // right foot on the ground
//		Pose p;
//		Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("rtibia")));
//		Pose p_src2 = src2.getPose(this->src2_end_frame);
//		klVec3 src2_pos = p_src2.getRootPosition();
//		klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("rtibia")));

//		klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
//		klQuat base_root_rot;
//		getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
//		
//		for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		{
//			p = src2.getPose(i);
//			p.SetRootPosition(
//				(p.getRootPosition() - src2_pos) + base_root_pos
//				);
//			p.SetRootAngle(
//				multiply(p.getRootAngle(),inverse(base_root_rot)));
//			src2_relative.AddPose(p);
//		}

//	}
//	else
//	{ // left foot on the ground
//		Pose p;
//		Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("ltibia")));
//		Pose p_src2 = src2.getPose(this->src2_end_frame);
//		klVec3 src2_pos = p_src2.getRootPosition();
//		klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("ltibia")));

//		klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
//		klQuat base_root_rot;
//		getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
//		
//		for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		{
//			p = src2.getPose(i);
//			p.SetRootPosition(
//				(p.getRootPosition() - src2_pos) + base_root_pos
//				);
//			p.SetRootAngle(
//				multiply(p.getRootAngle(),inverse(base_root_rot)));
//			src2_relative.AddPose(p);
//		}
//		//Pose p;
//		//Pose p_src1 = src1.getPose(this->src1_start_frame);
//		//klVec3 src1_pos = p_src1.getRootPosition();
//		//Pose p_src2 = src2.getPose(this->src2_end_frame);
//		//klVec3 src2_pos = p_src2.getRootPosition();

//		//for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
//		//{
//		//	p = src2.getPose(i);
//		//	p.SetRootPosition(
//		//		(p.getRootPosition() - src2_pos) + src1_pos
//		//		);
//		//	src2_relative.AddPose(p);
//		//}
//	}

//}

//static bool onFly(Pose p, string name)
//{
//	klVec3 foot = Converter::toklVec3(p.getNodePosition(p.getBoneIndex(name)));
//	if(foot.z > ((p.getBoneOffset("rfoot")).Abs() / 2))
//	{
//		return true;
//	}else
//	{
//		return false;
//	}

//}

//static void outAllPath(string filename)
//{
//	ofstream fout;
//	fout.open(filename.c_str(), std::ios::out);
//	if( fout.is_open()==false)
//	{
//		std::cout << "Can't open file." << std::endl;
//		exit(1);
//	}

//	fout << "#PATH" << endl;

//	double diff;

//	for(int j = LENGTH; j < this->src2.getFrameNum() - LENGTH ; j++)
//	{
//		cout << j << " / "<< this->src2.getFrameNum() << endl;
//		if(onFly(src2.getPose(j),"rfoot") && !onFly(src2.getPose(j),"lfoot"))
//		{// ??? -> single support by lfoot
//			double min = 100.0;
//			int i_min = 0;
//			int j_min = 0;
//			bool cluster = false;
//			for(int i = LENGTH; i < this->src1.getFrameNum() - LENGTH; i++)
//			{
//				if(!onFly(src1.getPose(i), "lfoot"))
//				{ // lfoot support or double support -> lfoot support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, false);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						double tmp = this->diff / MAX_POSE_DIFF + this->rootDiff / MAX_POSE_DIFF + this->v_diff / MAX_VEL_DIFF ;
//						if(min > tmp)
//						{
//							min = tmp;
//							i_min = i;
//							j_min = j;
//						}
//						cluster = true;
//					}else
//					{
//						if(cluster)
//						{
//							fout << i_min << "->" << j_min << " : " << min << endl;
//							i_min = 0;
//							j_min = 0;
//							min = 100.0;
//						}
//						cluster = false;
//					}
//				}else
//				{
//					if(cluster)
//					{
//						fout << i_min << "->" << j_min << " : " << min << endl;
//						i_min = 0;
//						j_min = 0;
//						min = 100.0;
//					}
//					cluster = false;
//				}
//			}
//		}
//		else if(onFly(src2.getPose(j), "lfoot") && !onFly(src2.getPose(j),"rfoot"))
//		{// ??? -> single supprt by rfoot
//			double min = 100.0;
//			int i_min = 0;
//			int j_min = 0;
//			bool cluster = false;
//			for(int i = LENGTH; i < this->src1.getFrameNum() - LENGTH; i++)
//			{
//				if(!onFly(src1.getPose(i), "rfoot"))
//				{ // rfoot support or double support -> rfoot support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						double tmp = this->diff / MAX_POSE_DIFF + this->rootDiff / MAX_POSE_DIFF + this->v_diff / MAX_VEL_DIFF ;
//						if(min > tmp)
//						{
//							min = tmp;
//							i_min = i;
//							j_min = j;
//						}
//						cluster = true;
//					}else
//					{
//						if(cluster)
//						{
//							fout << i_min << "->" << j_min << " : " << min << endl;
//							i_min = 0;
//							j_min = 0;
//							min = 100.0;
//						}
//						cluster = false;
//					}
//				}else
//				{
//					if(cluster)
//					{
//						fout << i_min << "->" << j_min << " : " << min << endl;
//						i_min = 0;
//						j_min = 0;
//						min = 100.0;
//					}
//					cluster = false;
//				}
//			}
//		}
//		else if(!onFly(src2.getPose(j), "lfoot") && !onFly(src2.getPose(j),"rfoot"))
//		{// ??? -> double support
//			double min = 0.0;
//			int i_min = 0;
//			int j_min = 0;
//			bool cluster = false;
//			for(int i = LENGTH; i < this->src1.getFrameNum() - LENGTH; i++)
//			{
//				if(onFly(src1.getPose(i), "rfoot") && !onFly(src1.getPose(j),"lfoot"))
//				{ // lfoot supoort -> double support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						double tmp = this->diff / MAX_POSE_DIFF + this->rootDiff / MAX_POSE_DIFF + this->v_diff / MAX_VEL_DIFF ;
//						if(min > tmp)
//						{
//							min = tmp;
//							i_min = i;
//							j_min = j;
//						}
//						cluster = true;
//					}else
//					{
//						if(cluster)
//						{
//							fout << i_min << "->" << j_min << " : " << min << endl;
//							i_min = 0;
//							j_min = 0;
//							min = 100.0;
//						}
//						cluster = false;
//					}
//				}else if(onFly(src1.getPose(i), "lfoot") && !onFly(src1.getPose(j),"rfoot"))
//				{ // rfoot support -> double support
//					poseSimilarityForStartEnd(src1.getPose(i), src2.getPose(j), i, j, true);
//					if(this->diff < MAX_POSE_DIFF
//						&& this->rootDiff < MAX_ROOT_DIFF
//						&& this->v_diff < MAX_VEL_DIFF)
//					{
//						double tmp = this->diff / MAX_POSE_DIFF + this->rootDiff / MAX_POSE_DIFF + this->v_diff / MAX_VEL_DIFF ;
//						if(min > tmp)
//						{
//							min = tmp;
//							i_min = i;
//							j_min = j;
//						}
//						cluster = true;
//					}else
//					{
//						if(cluster)
//						{
//							fout << i_min << "->" << j_min << " : " << min << endl;
//							i_min = 0;
//							j_min = 0;
//							min = 100.0;
//						}
//						cluster = false;
//					}
//				}else
//				{
//					if(cluster)
//					{
//						fout << i_min << "->" << j_min << " : " << min << endl;
//						i_min = 0;
//						j_min = 0;
//						min = 100.0;
//					}
//					cluster = false;
//				}
//			}
//		}
//	}

//	cout << "finish "<< filename << endl;

//	fout.close();
//}

////////////////////////
// getter
////////////////////////	
//static int getStartFrame_src1()
//{
//	return src1_start_frame;
//}
//static int getEndFrame_src2()
//{
//	return src2_end_frame;
//}

//static int getInterpolationFrameNum()
//{
//	return interpolateFrames;
//}

//static double getSimilarity()
//{
//	return poseSimilarity(this->src1.getPose(src1_start_frame), this->src2.getPose(src2_end_frame));
//}

//static string getDiffBoneName()
//{
//	return max_diff_bone_name;
//}

//static bool getcanInterpolate()
//{
//	return canInterpolate;
//}
