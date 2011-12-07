#include "MotionUtil.h"

using namespace Motion;
using namespace Util;

/**
 * @fn static klQuat scalarMultipleQuat(klQuat q1, float scale)
 * @brief クォータニオンをスカラー倍する
 * @param q1 [in] クォータニオン
 * @param scale [in] スカラー倍する比率
 * @return スカラー倍したクォータニオン
 */
//klQuat MotionUtil::scalarMultipleQuat(klQuat q1, float scale)
//{
//	q1.x = q1.x * scale;
//	q1.y = q1.y * scale;
//	q1.z = q1.z * scale;
//	q1.w = q1.w * scale;
//	return q1;
//}

/**
 * @fn  static klQuat convertQuatToLnQuat(klQuat qt)
 * @brief クォータニオン形式から対数クォータニオン形式に変換する
 * @param qt [in] 変換対象のクォータニオン
 * @return 変換した対数クォータニオン
 */
klQuat MotionUtil::convertQuatToLnQuat(klQuat qt)
{
	if(qt.w > 1.0)
	{
		qt.w = 1.0;
	}

	double theta = acos(qt.w);
	if(theta < 0.0001)
	{
		theta = 0.0001;			
	}

	double isintheta = theta / sinf(theta);
	
	klQuat q;
	q.x = qt.x * isintheta;
	q.y = qt.y * isintheta;
	q.z = qt.z * isintheta;
	q.w = 0;

	return q;
}

/**
 * @fn static klQuat convertLnQuatToQuat(klQuat lnqt)
 * @brief 対数クォータニオン形式からクォータニオン形式に変換する
 * @param lnqt [in] 変換対象の対数クォータニオン
 * @return 変換したクォータニオン
 */
 klQuat MotionUtil::convertLnQuatToQuat(klQuat lnqt)
{
	double theta = sqrtf(lnqt.x * lnqt.x + lnqt.y * lnqt.y + lnqt.z * lnqt.z);

	if(theta < 0.0001)
	{
		theta = 0.0001;			
	}

	double sintheta = sinf(theta) / theta;

	klQuat q;
	q.x = lnqt.x * sintheta;
	q.y = lnqt.y * sintheta;
	q.z = lnqt.z * sintheta;
	q.w = cos(theta);

	return q;
}

/**
 * @fn static klVec3 lerp(klVec3& v1, klVec3& v2, float rate)
 * @brief 線形補間をする(ベクトル)
 * @param v1 [in] 補間対象のベクトル
 * @param v2 [in] 補間対象のベクトル
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then v1*0.7 + v2*0.3)
 * @return 補間したベクトル
 */
 klVec3 MotionUtil::lerp(klVec3& v1, klVec3& v2, float rate)
{
	klVec3 v;
	v.x	= v1.x * (1.0 - rate) + v2.x * rate;
	v.y = v1.y * (1.0 - rate) + v2.y * rate;
	v.z = v1.z * (1.0 - rate) + v2.z * rate;

	return v;
}

/**
 * @fn static klQuat lerp(klQuat& q1, klQuat& q2, float rate)
 * @brief 線形補間をする(クォータニオン)
 * @param q1 [in] 補間対象のクォータニオン
 * @param q2 [in] 補間対象のクォータニオン
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
 * @return 補間したクォータニオン
 */
 klQuat MotionUtil::lerp(klQuat& q1, klQuat& q2, float rate)
{
	klQuat qt = q1 * (1-rate) + q2 * rate;

	return qt.Normalize();
}

/**
 * @fn static klQuat slerp(klQuat q1, klQuat q2, float rate, bool useInverseCosTheta)
 * @brief 球面線形補間(180度の補正をするかどうか選択可能)
 * @param q1  [in]  補間対象のクォータニオン1
 * @param q2  [in]  補間対象のクォータニオン2
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
 * @param useInverseCosTheta [in]  180度の反転処理を入れるかどうか
 * @return 補間結果のクォータニオン
 */
 klQuat MotionUtil::slerp(klQuat q1, klQuat q2, float rate, bool useInverseCosTheta)
{
	//quaternion to return
	klQuat qm; qm.Clear();
	// Calculate angle between them.
	double cosHalfTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

	if (cosHalfTheta < 0 && useInverseCosTheta) { 
		q2.w = -q2.w;
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		cosHalfTheta = -cosHalfTheta;
	}

	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) >= 1.0){
		qm.w = q1.w;
		qm.x = q1.x;
		qm.y = q1.y;
		qm.z = q1.z;
		return qm;
	}
	// Calculate temporary values.
	double halfTheta = acos(cosHalfTheta);
	double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (fabs(sinHalfTheta) < 0.001){ // fabs is floating point absolute
		qm.w = (q1.w * 0.5 + q2.w * 0.5);
		qm.x = (q1.x * 0.5 + q2.x * 0.5);
		qm.y = (q1.y * 0.5 + q2.y * 0.5);
		qm.z = (q1.z * 0.5 + q2.z * 0.5);
		return qm;
	}
	double ratioA = sin((1 - rate) * halfTheta) / sinHalfTheta;
	double ratioB = sin(rate * halfTheta) / sinHalfTheta; 
	//calculate Quaternion.
	qm.w = (q1.w * ratioA + q2.w * ratioB);
	qm.x = (q1.x * ratioA + q2.x * ratioB);
	qm.y = (q1.y * ratioA + q2.y * ratioB);
	qm.z = (q1.z * ratioA + q2.z * ratioB);

	return qm;

	//float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

	//klQuat correct_q2;
	//if(dot < 0.f){
	//	correct_q2 = -q2;
	//	dot = -dot;
	//}else{
	//	correct_q2 = q2;
	//}

	//if(dot >= 1.f)
	//{
	//	dot = 1.f;
	//}


	//float radian = acos(dot);
	//if(fabs(dot) < klVec4::epsilon){ return correct_q2; }
	//if(radian == 0.f){
	//	return correct_q2;
	//}
	//float inverseSin = 1.f / sin(radian);
	//float leftScale = sin((1.f - rate) * radian ) * inverseSin;
	//float rightScale = sin(rate * radian) * inverseSin;
	//return Multi(q1,leftScale) + Multi(correct_q2,rightScale);

	//D3DXQUATERNION pOut, pQ1, pQ2;
	//pQ1.x = q1.x;pQ1.y = q1.y;pQ1.z = q1.z;pQ1.w = q1.w;
	//pQ2.x = q2.x;pQ2.y = q2.y;pQ2.z = q2.z;pQ2.w = q2.w;
	//D3DXQuaternionSlerp( &pOut, &pQ1, &pQ2, rate);

	//klQuat qt;
	//qt.x = pOut.x;qt.y = pOut.y;qt.z = pOut.z;qt.w = pOut.w;

	//return qt;

	//Quaternion qt1(q1.w, q1.x, q1.y, q1.z);
	//Quaternion qt2(q2.w, q2.x, q2.y, q2.z);
	//Quaternion result = slerp(qt1, qt2, rate);

	//klQuat q;
	//q.x = result.i();
	//q.y = result.j();
	//q.z = result.k();
	//q.w = result.r();

	//return q;

	//q1 = normalize(q1);
	//q2 = normalize(q2);

	//double angle = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	//if(angle < 0.0f)
	//{
	//	q1.x = -q1.x;q1.y = -q1.y;q1.z = -q1.z;q1.w = -q1.w;
	//	angle *= -1.0;
	//}

	//double scale, invscale;
	//if ((angle + 1.0f) > 0.05f)
	//{
	//	if ((1.0f - angle) >= 0.05f) // spherical interpolation
	//	{
	//		double theta = acosf(angle);
	//		double invsintheta = 1 / (sinf(theta));
	//		scale = sinf(theta * (1.0f-rate)) * invsintheta;
	//		invscale = sinf(theta * rate) * invsintheta;
	//	}
	//	else // linear interploation
	//	{
	//		scale = 1.0f - rate;
	//		invscale =rate;
	//	}
	//}
	//else
	//{
	//	q2.x = -q1.y; q2.y = q1.x; q2.z = -q1.w; q2.w = q1.z;
	//	scale = sinf(M_PI * (0.5f - rate));
	//	invscale = sinf(M_PI * rate);
	//}

	//return Multi(q1, scale) + Multi(q2 , invscale);

	//q1 = normalize(q1);
	//q2 = normalize(q2);
}

/**
 * @fn static klQuat slerpByDirectXFunc(klQuat src1, klQuat src2, double rate)
 * @brief 球面線形補間(DirectXの関数を使う，180度の補正つき)
 * @param src1    [in]  クォータニオン1
 * @param src2    [in]  クォータニオン2
 * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
 * @return クォータニオン1と2を補間したクォータニオン
 */
// klQuat MotionUtil::slerpByDirectXFunc(klQuat src1, klQuat src2, float rate)
//{
//
//	D3DXQUATERNION Q1 = D3DXQUATERNION(src1.x, src1.y, src1.z, src1.w);
//	D3DXQUATERNION Q2 = D3DXQUATERNION(src2.x, src2.y, src2.z, src2.w);
//	FLOAT time = rate;
//
//	D3DXQUATERNION Qt;
//	D3DXQuaternionSlerp(&Qt, &Q1, &Q2, time);
//
//	klQuat qt;
//	qt.x = Qt.x;
//	qt.y = Qt.y;
//	qt.z = Qt.z;
//	qt.w = Qt.w;
//
//	return qt;
//}

///**
// * @fn static klQuat squad(klQuat src0, klQuat src1, klQuat src2, klQuat src3, double rate)
// * @brief 球面線形二次補間(DirectXの関数を使う)
// * @param src0    [in]  クォータニオン0
// * @param src1    [in]  クォータニオン1
// * @param src2    [in]  クォータニオン2
// * @param src3    [in]  クォータニオン3
// * @param rate [in] 補間の割合(ex，if rate == 0.3 then q1*0.7 + q2*0.3)
// * @return クォータニオン1と2を補間したクォータニオン
// */
// klQuat MotionUtil::squad(klQuat src0, klQuat src1, klQuat src2, klQuat src3, double rate)
//{
//	D3DXQUATERNION Q0 = D3DXQUATERNION(src0.x, src0.y, src0.z, src0.w);
//	D3DXQUATERNION Q1 = D3DXQUATERNION(src1.x, src1.y, src1.z, src1.w);
//	D3DXQUATERNION Q2 = D3DXQUATERNION(src2.x, src2.y, src2.z, src2.w);
//	D3DXQUATERNION Q3 = D3DXQUATERNION(src3.x, src3.y, src3.z, src3.w);
//	D3DXQUATERNION A, B, C, Qt;
//	FLOAT time = rate;
//
//	D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);
//	D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, time);
//
//	klQuat qt;
//	qt.x = Qt.x;
//	qt.y = Qt.y;
//	qt.z = Qt.z;
//	qt.w = Qt.w;
//
//	return qt;
//}

/**
 * @fn static klQuat expMap(const vector<klQuat> &rots, const vector<float> &kernel, bool useInverseCosTheta)
 * @brief Exponential Map を使った補間(3つ以上のクォータニオンの補間)
 * @param rots  [in]  補間対象のクォータニオン
 * @param kernel  [in]  カーネル
 * @param useInverseCosTheta [in]  180度の反転処理を入れるかどうか
 * @return 補間結果のクォータニオン
 */
 klQuat MotionUtil::expMap(vector<klQuat> &rots, vector<float> &kernel, bool useInverseCosTheta)
{
	// 実際に使う場合
	//vector<klQuat> in_q;
	//in_q.push_back(q1);
	//in_q.push_back(q2);

	//vector<float> kernel;
	//kernel.push_back(1 - rate);
	//kernel.push_back(rate);

	//klQuat out_q = interpolate_rotation(in_q, kernel);
	//return out_q;

	// カーネル総和
	float ksum = 0;
	for (unsigned int k = 0;k < kernel.size();k++)
	{
		ksum += kernel[k];
	}

	klQuat qd; 
	klQuat exd;exd.Clear(); exd.w = 0;
	for (unsigned int k = 1;k < kernel.size();k++)
		exd = exd + calcDifferentialExpMap(rots[0], rots[k], useInverseCosTheta) *  kernel[k];
	exd = exd * 1 / ksum;
	qd = convertLnQuatToQuat(exd);
	return rots[0] * qd;
}

/**
 * @fn static klQuat calcDifferentialExpMap(klQuat &q0, klQuat &q1, bool useInverseCosTheta)
 * @brief 2つのクォータニオンの差分exponential map の計算
 * @param q0 [in] クォータニオン0
 * @param q1 [in] クォータニオン1
 * @param useInverseCosTheta [in]  180度の反転処理を入れるかどうか
 * @return 差分exponential map : Ln(q0^{-1} * q1)
 */
 klQuat MotionUtil::calcDifferentialExpMap(klQuat &q0, klQuat &q1, bool useInverseCosTheta)
{
	double cosHalfTheta = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

	if (cosHalfTheta < 0 && useInverseCosTheta) { 
		q1.w = -q1.w;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		cosHalfTheta = -cosHalfTheta;
	}		

	klQuat q0i = !(q0);
	klQuat qd = q0i * q1;

	// 注：D3DXQuaternionLn 関数を用いて exp map を計算する前には，w を正値に正規化する
	//if (qd.w < 0)
	//{
	//	qd.w *= -1.0f;
	//	qd.x *= -1.0f;
	//	qd.y *= -1.0f;
	//	qd.z *= -1.0f;
	//}

	klQuat expm = convertQuatToLnQuat(qd);

	return expm;
}

/**
 * @fn static double poseSimilarityBasedJointPos(Pose& src1, Pose& src2)
 * @brief 姿勢の類似度を求める(位置ベース)
 * @param src1 [in] 姿勢1の位置(ルートからの位置)
 * @param src2 [in] 姿勢2の位置(ルートからの位置)
 * @return 求めた類似度
 */
 double MotionUtil::poseSimilarityBasedJointPos(vector<klVec3>& src1, vector<klVec3>& src2)
{
	double diff = 0;
	int index = 0;

	for(unsigned int i=0; i < src1.size(); i++)
	{
		klVec3 src1_v = src1[i];//.getNodePositionFromBone(i, src1.getRootIndex());
		klVec3 src2_v = src2[i];//.getNodePositionFromBone(i, src2.getRootIndex());
		diff += (src1_v - src2_v).Abs();
		//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		
	}

	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;
	return diff;
}

/**
 * @fn static double poseSimilarityBasedJointPos(Pose& src1, Pose& src2)
 * @brief 姿勢の類似度を求める(位置ベース)
 * @param src1 [in] 姿勢1の位置(ルートからの位置)
 * @param src2 [in] 姿勢2の位置(ルートからの位置)
 * @return 求めた類似度
 */
 double MotionUtil::poseSimilarityBasedJointPos(Pose& src1, Pose& src2)
{
	double diff = 0;
	int index = 0;

	for(int i=1; i < src1.getBoneNum(); i++)
	{
		klVec3 src1_v = src1.getNodePositionFromBone(i, 0);
		klVec3 src2_v = src2.getNodePositionFromBone(i, 0);
		diff += (src1_v - src2_v).Abs();
		//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		
	}

	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;
	return diff;
}


/**
 * @fn static double poseSimilarityBasedJointAngle(Pose& src1, Pose& src2)
 * @brief 姿勢の類似度を求める(関節角度ベース)
 * @param src1 [in] 姿勢1
 * @param src2 [in] 姿勢2
 * @return 求めた類似度
 */
 double MotionUtil::poseSimilarityBasedJointAngle(Pose& src1, Pose& src2)
{
	double diff = 0;

	for(int i=0; i < src1.getBoneNum(); i++)
	{
		string bone_name = src1.getBoneName(i);

		klQuat src1_q = src1.getBoneAngle(i);
		klQuat src2_q = src2.getBoneAngle(i);

		if(src1_q.w < 0){src1_q = !(src1_q); src1_q.w *= -1.0;}
		if(src2_q.w < 0){src2_q = !(src2_q); src2_q.w *= -1.0;}

		klQuat result = !(src1_q) * src2_q;
		double tmp_w = result.w;

		if(tmp_w > 1.0) tmp_w = 1.0;
		else if(tmp_w < -1.0) tmp_w = -1.0;

		double tmp_diff = 2 * acos(tmp_w);
		if( 2*M_PI - tmp_diff < M_PI)
			tmp_diff = 2 * M_PI - tmp_diff;

		tmp_diff /= 2;

		//double tmp_diff = sqrt(result.x*result.x+result.y*result.y+result.z*result.z+result.w*result.w);		


		//parametalize based JING WANG paper
		if(bone_name == "ltibia" || bone_name == "rtibia")
		{
			diff += 1.0 * tmp_diff;
		}else if(bone_name == "lfemur" || bone_name == "rfemur")
		{
			diff += 0.0901 * tmp_diff;
		}else if(bone_name == "lhumerus" || bone_name == "rhumerus")
		{
			diff += 0.7884 * tmp_diff;
		}else if(bone_name == "lradius" || bone_name == "rradius")
		{
			diff += 0.0247 * tmp_diff;
		}

		//diff += tmp_diff;

		//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		
	}

	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;
	return diff;
}

/**
 * @fn static double velocitySimilarityBasedJointAngle( Pose& src1_mt, Pose& src1_t, int src1_range, Pose& src2_mt, Pose& src2_t, int src2_range)
 * @brief 姿勢同士の速度の類似度を求める(関節角度ベース, 前後のフレームから速度を推定)
 * @param src1_mt [in] 姿勢1から -src1_range離れたフレームの姿勢
 * @param src1_t [in] 姿勢1から src1_range離れたフレームの姿勢
 * @param src1_range [in] src1_tの姿勢1からの距離
 * @param src2_mt [in] 姿勢2から -src1_range離れたフレームの姿勢
 * @param src2_t [in] 姿勢2から src1_range離れたフレームの姿勢
 * @param src2_range [in] src2_tの姿勢2からの距離
 * @return 求めた類似度
 */
 double MotionUtil::velocitySimilarityBasedJointAngle( Pose& src1_mt, Pose& src1_t, int src1_range, Pose& src2_mt, Pose& src2_t, int src2_range)
{
	double diff = 0;

	klQuat q_tmp;
	double v_src1, v_src2;
	for(int i = 0; i < src1_mt.getBoneNum(); i++)
	{
		string bone_name = src1_mt.getBoneName(i);

		klQuat src1_mt_q = src1_mt.getBoneAngle(i);
		klQuat src1_t_q = src1_t.getBoneAngle(i);
		klQuat src2_mt_q = src2_mt.getBoneAngle(i);
		klQuat src2_t_q = src2_t.getBoneAngle(i);

		if(src1_mt_q.w < 0){src1_mt_q = !(src1_mt_q); src1_mt_q.w *= -1.0;}
		if(src1_t_q.w < 0){src1_t_q = !(src1_t_q); src1_t_q.w *= -1.0;}
		if(src2_mt_q.w < 0){src2_mt_q = !(src2_mt_q); src2_mt_q.w *= -1.0;}
		if(src2_t_q.w < 0){src2_t_q = !(src2_t_q); src2_t_q.w *= -1.0;}

		//klQuat src1_0, src1_1, src2_0, src2_1;
		//if(src1_range < 0)
		//{
		//	src1_0 = src1_t.getBoneAngle(i);
		//	src1_1 = src1.getBoneAngle(i);
		//}else
		//{
		//	src1_1 = src1_t.getBoneAngle(i);
		//	src1_0 = src1.getBoneAngle(i);
		//}
		//if(src2_range < 0)
		//{
		//	src2_0 = src2_t.getBoneAngle(i);
		//	src2_1 = src2.getBoneAngle(i);
		//}else
		//{
		//	src2_1 = src2_t.getBoneAngle(i);
		//	src2_0 = src2.getBoneAngle(i);
		//}

		double tmp_w = (!(src1_mt_q) * src1_t_q).w;
		if(tmp_w > 1.0) tmp_w = 1.0;
		else if(tmp_w < -1.0) tmp_w = -1.0;
		v_src1 = 2 * acos(tmp_w) / (src1_range * 2);

		tmp_w = (!(src2_mt_q) * src2_t_q).w;
		if(tmp_w > 1.0) tmp_w = 1.0;
		else if(tmp_w < -1.0) tmp_w = -1.0;
		v_src2 = 2 * acos(tmp_w) / (src2_range * 2);

		double tmp_diff = fabs(v_src1 - v_src2);

		// parametalize based JING WANG paper
		if(bone_name == "ltibia" || bone_name == "rtibia")
		{
			diff += 1.0 * tmp_diff;
		}else if(bone_name == "lfemur" || bone_name == "rfemur")
		{
			diff += 0.0901 * tmp_diff;
		}else if(bone_name == "lhumerus" || bone_name == "rhumerus")
		{
			diff += 0.7884 * tmp_diff;
		}else if(bone_name == "lradius" || bone_name == "rradius")
		{
			diff += 0.0247 * tmp_diff;
		}

		//diff += tmp_diff;
	}

	return diff;
}


/**
 * @fn static void normalizeMocapData(DrawMocapData& src)
 * @brief Quaternionの180度の反転を除去する
 * @param src [in, out] 反転を除去する対象モーション，出力モーション
 * @return なし
 */
 void MotionUtil::normalizeMocapData(DrawMocapData& src)
{
	for(int i = 0; i < src.getBoneNum(); i++)
	{
		klQuat qt_p; qt_p = src.getBone_Quat(0, i);
		for(int j =1; j < src.getFrameNum(); j++)
		{

			klQuat qt = src.getBone_Quat(j, i);

			int x_inv = 0;
			int y_inv = 0;
			int z_inv = 0;
			int w_inv = 0;

			if( abs(qt.x - qt_p.x) > abs(qt.x - (-qt_p.x)) && abs(qt_p.x) > 0.05 && abs(qt.x) > 0.05)
			{
				x_inv = 1;

			}
			if( abs(qt.y - qt_p.y) > abs(qt.y - (-qt_p.y)) && abs(qt_p.y) > 0.05 && abs(qt.y) > 0.05)
			{
				y_inv = 1;

			}
			if( abs(qt.z - qt_p.z) > abs(qt.z - (-qt_p.z)) && abs(qt_p.z) > 0.05 && abs(qt.z) > 0.05)
			{
				z_inv = 1;

			}
			if( abs(qt.w - qt_p.w) > abs(qt.w - (-qt_p.w)) && abs(qt_p.w) > 0.05 && abs(qt.w) > 0.05)
			{
				w_inv = 1;
			}

			if(x_inv + y_inv + z_inv + w_inv > 1)
			{
				//if(x_inv)qt.x = -qt.x;
				//if(y_inv)qt.y = -qt.y;
				//if(z_inv)qt.z = -qt.z;
				//if(w_inv)qt.w = -qt.w;
				qt.x = -qt.x;
				qt.y = -qt.y;
				qt.z = -qt.z;
				qt.w = -qt.w;
			}

			src.SetBoneAngle(qt, j, i);

			qt_p = qt;
		}
	}
}

/**
 * @fn static void normalizeMocapData2(DrawMocapData& src)
 * @brief Quaternionのでwがマイナスになっているところを反転させる
 * @param src [in, out] 反転を除去する対象モーション，出力モーション
 * @return なし
 */
 void MotionUtil::normalizeMocapData2(DrawMocapData& src)
{
	for(int i = 0; i < src.getBoneNum(); i++)
	{
		//klQuat qt_p; qt_p = src.getBone_Quat(0, i);
		//if(qt_p.w < 0)
		//{
		//	qt_p.w = -qt_p.w;
		//	qt_p.x = -qt_p.x;
		//	qt_p.y = -qt_p.y;
		//	qt_p.z = -qt_p.z;
		//	src.SetBoneAngle(qt_p, 0, i);
		//}
		for(int j =0; j < src.getFrameNum(); j++)
		{

			klQuat qt = src.getBone_Quat(j, i);

			if(qt.w < 0)
			{
				qt.x = -qt.x;
				qt.y = -qt.y;
				qt.z = -qt.z;
				qt.w = -qt.w;
			}

			src.SetBoneAngle(qt, j, i);
		}
	}
}


/**
 * @fn static klMat44 calcTransformBetweenFigures( vector< klVec3> pc1, vector<klVec3> pc2)
 * @brief 座標正規化行列を求める
 * @param pc1 [in] モーション1の関節位置
 * @param pc2 [in] モーション2の関節位置
 * @return 座標正規化行列
 */
 klMat44 MotionUtil::calcTransformBetweenFigures( vector< klVec3> pc1, vector<klVec3> pc2)
{
	// top_left: theta､ﾗｻｻ､ｹ､・ｰ､ﾎﾊｬｻﾒ､ﾎｺｸ､ﾎｹ爍ﾊｰﾊｲｼﾆｱﾍﾍ｡ﾋ
	float top_left = 0, top_right = 0, bottom_left = 0, bottom_right = 0;
	float ax1 = 0, ax2 = 0, ay1 = 0, ay2 = 0;
	float tx, ty, tz = 0, rz;
	 
	for (unsigned int i = 0;i < pc1.size();++i)
	{
		top_left += pc1[i].x * pc2[i].y - pc2[i].x * pc1[i].y;
		bottom_left += pc1[i].x * pc2[i].x + pc1[i].y * pc2[i].y;
	 
		ax1 += pc1[i].x;
		ay1 += pc1[i].y;
		ax2 += pc2[i].x;
		ay2 += pc2[i].y;
	 
		tz += pc1[i].z - pc2[i].z;
	}
	top_right = (ax1 * ay2 - ax2 * ay1) / static_cast<float>(pc1.size());
	bottom_right = (ax1 * ax2 + ay1 * ay2) / static_cast<float>(pc1.size());
	 
	rz = atan2f(top_left - top_right, bottom_left - bottom_right);
	tx = (ax1 - ax2 * cosf(rz) - ay2 * sinf(rz)) / static_cast<float>(pc1.size());
	ty = (ay1 + ax2 * sinf(rz) - ay2 * cosf(rz)) / static_cast<float>(pc1.size());
	tz /= static_cast<float>(pc1.size());
	 
	//D3DXMATRIX mr, mt;
	//D3DXMatrixRotationZ(&mr, rz);
	//D3DXMatrixTranslation(&mt, ty, tx, tz);
	//mr = mr * mt;

	klMat44 mr;
	mr.SetRot(0.0, 0.0, rz * 180 / M_PI);
	mr.SetPos(ty, tx, tz);
	
	//klMat44 tmp;
	//tmp.m11 = mr._11;tmp.m12 = mr._12;tmp.m13 = mr._13;tmp.m14 = mr._14;
	//tmp.m21 = mr._21;tmp.m22 = mr._22;tmp.m23 = mr._23;tmp.m24 = mr._24;
	//tmp.m31 = mr._31;tmp.m32 = mr._32;tmp.m33 = mr._33;tmp.m34 = mr._34;
	//tmp.m41 = mr._41;tmp.m42 = mr._42;tmp.m43 = mr._43;tmp.m44 = mr._44;

	//tmp.SetPos(klVec3(mr._41, mr._42, mr._43));

	return mr;
}

/**
 * @fn static void outRotationVecForGnuPlot(DrawMocapData& src)
 * @brief モーションデータの関節の回転(対数クォータニオン，クォータニオン)を出力する
 * @param src [in] 出力対象のモーション
 * @return なし
 */
 void MotionUtil::outRotationVecForGnuPlot(DrawMocapData& src)
{
	for(int i = 0; i < src.getBoneNum(); i++)
	{
		string filename = src.getBoneName(i) + "_Quat_" + FileNameUtil::getName(src.getName(), false) + ".dat";
		ofstream fout;fout.open( filename.c_str(), ios::out);
		fout << "#\tX\tY\tZ\tW" << endl;

		klQuat qt_p; qt_p = src.getBone_Quat(0, i);
		fout << "\t" << qt_p.x << "\t" << qt_p.y << "\t" << qt_p.z << "\t" << qt_p.w << endl;
		for(int j =1; j < src.getFrameNum(); j++)
		{

			klQuat qt = src.getBone_Quat(j, i);

			fout << "\t" << qt.x << "\t" << qt.y << "\t" << qt.z << "\t" << qt.w << endl;

			src.SetBoneAngle(qt, j, i);
		}

		fout.close();
	}

	for(int i = 0; i < src.getBoneNum(); i++)
	{
		string filename = src.getBoneName(i) + "_LnQuat_" + FileNameUtil::getName(src.getName(), false) + ".dat";
		ofstream fout;fout.open( filename.c_str(), ios::out);
		fout << "#\tX\tY\tZ" << endl;
		for(int j =0; j < src.getFrameNum(); j++)
		{
			klQuat qt = src.getBone_Quat(j, i);								
			klVec3 vec;
			//double angle;
			klQuat lnqt = MotionUtil::convertQuatToLnQuat(qt);

			fout << "\t" << lnqt.x << "\t" << lnqt.y << "\t" << lnqt.z << endl;
		}

	}

}

/**
 * @fn static DrawMocapData rotateMocapData(DrawMocapData& src, double angle)
 * @brief モーションデータを鉛直軸周り(Z軸周り)にangleだけ回転させる
 * @param src [in] 出力対象のモーション
 * @return 回転させたモーションデータ
 */
 Pose MotionUtil::rotatePose(DrawMocapData& src, float angle, int frame)
{
	Pose dst = src.getPose(frame);

	//原点に移動させる
	klVec3 ini_root_pos = src.getRootPosition(frame);ini_root_pos.z = 0.0;
	dst.SetRootPosition(dst.getRootPosition() - ini_root_pos);

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot;
	ini_rot.FromEuler(0, angle * M_PI / 180, 0);

	dst.SetRootAngle(ini_rot * dst.getRootAngle());
	dst.SetRootPosition(ini_rot.Rotate( dst.getRootPosition()) );

	//元の位置に戻す
	dst.SetRootPosition(dst.getRootPosition() + ini_root_pos);

	return dst;
}

/**
 * @fn static DrawMocapData rotateMocapData(DrawMocapData& src, klQuat rot)
 * @brief モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる
 * @param src [in] 出力対象のモーション
 * @return 回転させたモーションデータ
 */
 Pose MotionUtil::rotatePose(DrawMocapData& src, klQuat rot, int frame)
{
	Pose dst = src.getPose(frame);

	//原点に移動させる
	klVec3 ini_root_pos = src.getRootPosition(frame);ini_root_pos.z = 0.0;
	dst.SetRootPosition(dst.getRootPosition() - ini_root_pos);

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot;
	ini_rot.fromEuler(0, rot.GetYaw(), 0);
	dst.SetRootAngle(ini_rot * dst.getRootAngle());
	dst.SetRootPosition(ini_rot.Rotate(dst.getRootPosition()));

	//元の位置に戻す
	dst.SetRootPosition(dst.getRootPosition() + ini_root_pos);

	return dst;
}

/**
 * @fn static DrawMocapData rotateMocapData(DrawMocapData& src, klQuat rot)
 * @brief モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる
 * @param src [in] 出力対象のモーション
 * @return 回転させたモーションデータ
 */
 Pose MotionUtil::rotatePose(Pose& src, klQuat rot, int frame)
{
	Pose dst = src;

	//原点に移動させる
	klVec3 ini_root_pos = src.getRootPosition();ini_root_pos.z = 0.0;
	dst.SetRootPosition(dst.getRootPosition() - ini_root_pos);

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot;
	ini_rot.FromEuler(0, rot.GetYaw(), 0);
	dst.SetRootAngle(ini_rot * dst.getRootAngle());
	dst.SetRootPosition(ini_rot.Rotate(dst.getRootPosition()));

	//元の位置に戻す
	dst.SetRootPosition(dst.getRootPosition() + ini_root_pos);

	return dst;
}


/**
 * @fn static DrawMocapData rotateMocapData(DrawMocapData& src, double angle)
 * @brief モーションデータを鉛直軸周り(Z軸周り)にangleだけ回転させる
 * @param src [in] 出力対象のモーション
 * @return 回転させたモーションデータ
 */
 DrawMocapData MotionUtil::rotateMocapData(DrawMocapData& src, float angle, int frame )
{
	DrawMocapData dst = src;

	//原点に移動させる
	klVec3 ini_root_pos = dst.getRootPosition(frame);ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot;
	ini_rot.FromEuler(0, angle * M_PI / 180, 0);
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootRotation(ini_rot * dst.getRootRotation(i), i);
		dst.setRootPosition(ini_rot.Rotate(dst.getRootPosition(i)), i);
	}

	//元の位置に戻す
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) + ini_root_pos, i);
	}

	return dst;
}

/**
 * @fn static DrawMocapData rotateMocapData(DrawMocapData& src, klQuat rot)
 * @brief モーションデータを鉛直軸周り(Z軸周り)にrotだけ回転させる
 * @param src [in] 出力対象のモーション
 * @return 回転させたモーションデータ
 */
 DrawMocapData MotionUtil::rotateMocapData(DrawMocapData& src, klQuat rot, int frame)
{
	DrawMocapData dst = src;

	//原点に移動させる
	klVec3 ini_root_pos = dst.getRootPosition(frame);ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot;
	ini_rot.fromEuler(0, rot.GetYaw(), 0);
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootRotation(ini_rot * dst.getRootRotation(i), i);
		dst.setRootPosition(ini_rot.Rotate(dst.getRootPosition(i)), i);
	}

	//元の位置に戻す
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) + ini_root_pos, i);
	}

	return dst;
}


/**
 * @fn static DrawMocapData transMocapData(DrawMocapData& src, double angle)
 * @brief モーションデータを平行移動させる
 * @param src [in] 出力対象のモーション
 * @return 平行移動させたモーションデータ
 */
 DrawMocapData MotionUtil::transMocapData(DrawMocapData& src, klVec3 trans)
{
	DrawMocapData dst = src;

	//trans分だけ移動させる
	klVec3 ini_root_pos = dst.getRootPosition(0);ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) + trans, i);
	}

	return dst;
}

/**
 * @fn static DrawMocapData transMocapData(DrawMocapData& src, double angle)
 * @brief モーションデータを平行移動させる
 * @param src [in] 出力対象のモーション
 * @return 平行移動させたモーションデータ
 */
 Pose MotionUtil::transPose(DrawMocapData& src, klVec3 trans, int frame)
{
	Pose dst = src.getPose(frame);

	//trans分だけ移動させる
	klVec3 ini_root_pos = src.getRootPosition(frame);ini_root_pos.z = 0.0;
	dst.SetRootPosition(dst.getRootPosition() + trans);

	return dst;
}

/**
 * @fn static DrawMocapData transMocapData(DrawMocapData& src, double angle)
 * @brief モーションデータを平行移動させる
 * @param src [in] 出力対象のモーション
 * @return 平行移動させたモーションデータ
 */
 Pose MotionUtil::transPose(Pose& src, klVec3 trans)
{
	Pose dst = src;

	//trans分だけ移動させる
	klVec3 ini_root_pos = src.getRootPosition();ini_root_pos.z = 0.0;
	dst.SetRootPosition(dst.getRootPosition() + trans);

	return dst;
}


/**
 * @fn static PoseSimilarity getSimilarity(DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1のある姿勢とモーション2のある姿勢の類似度を求める(速度はみない)
 * @param src1  [in]　モーション1  
 * @param src2  [in]　モーション2
 * @return 類似度
 */
 PoseSimilarity MotionUtil::getSimilarity(Pose& src1, Pose& src2)
{
	PoseSimilarity sim;
	sim.rootDiff = 0;
	vector<klVec3> pc1; pc1.push_back(src1.getRootPosition());
	vector<klVec3> pc2; pc2.push_back(src2.getRootPosition());
	for(int i=0; i < src1.getBoneNum(); i++)
	{
		pc1.push_back(src1.getNodePosition(i));
		pc2.push_back(src2.getNodePosition(i));
	}
	klMat44 trans = calcTransformBetweenFigures(pc1, pc2);

	klQuat ini_rot = klQuat::fromklMat(trans);
	sim.rootDiff = 2 * acos(ini_rot.w);


	//
	// Velosity similarity 
	//
	sim.velDiff = 0.0;

	//
	// Pose similarity 
	//
	sim.poseDiff = poseSimilarityBasedJointAngle(src1, src2);

	return sim;
}

/**
 * @fn static bool outSimilarityGraph(string out_filename, DrawMocapData& src, Pose base)
 * @brief ベースのポーズを基準として類似度を出力する
 * @param out_filename [in] 出力ファイル名
 * @param src [in] 類似度を測るモーションデータ
 * @param base [in] 類似度のベースとなるポーズ
 * @return ファイル出力できたかどうか
 */
 bool MotionUtil::outSimilarityGraph(DrawMocapData& src, Pose base, string out_filename)
{
	if( FileNameUtil::getExtension( out_filename ) != ".csv" )
	{
		out_filename = FileNameUtil::cutExtension( out_filename ) + ".csv";
	}

	ofstream fout;
	fout.open(out_filename.c_str(), ios::out);

	fout << "Pose_diff" <<  "," << "Root_diff" <<  "," << "Vel_diff" << endl;
	if(!fout.is_open())return false;

	for(int i=0; i < src.getFrameNum(); i++)
	{
		PoseSimilarity sim = 
			getSimilarity( src.getPose(i), base);
		fout << sim.poseDiff <<  "," << sim.rootDiff <<  "," << sim.velDiff << endl;
	}
	fout.close();
	return true;
}

 string MotionUtil::convertFileNameToGraphAnnotation(string name)
{
	name = FileNameUtil::getName(name, false);

	if( name.compare("highkickL") == 0 )
		return "KickHighR";
	if( name.compare("highkickR") == 0 )
		return "KickHighR";
	if( name.compare("middlekickL") == 0 )
		return "KickHighR";
	if( name.compare("middlekickR") == 0 )
		return "KickHighR";
	if( name.compare("lowkickL") == 0 )
		return "KickLowR";
	if( name.compare("lowkickR") == 0)
		return "KickLowR";
	if( name.compare("hookL2") == 0 || name.compare("hookL3") == 0)
		return "HookL";
	if( name.compare("hookR2") == 0 || name.compare("hookR3") == 0)
		return "HookR";
	if( name.compare("jabR1") == 0)
		return "JabR";
	if( name.compare("jabL1") == 0)
		return "JabL";
	if( name.compare("straightR1") == 0)
		return "StraightR";
	if( name.compare("straightL1") == 0)
		return "StraightL";
	if( name.compare("upperR1") == 0)
		return "UpperR";
	if( name.compare("upperL1") == 0)
		return "UpperL";
	if( name.compare("dodge_back") == 0)
		return "DodgeB";
	if( name.compare("dodgeR1") == 0)
		return "SideStepR";
	if( name.compare("dodgeL1") == 0)
		return "SideStepL";

	return "error";
}

/**
 * @fn static PoseSimilarity getSimilarity(DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame)
 * @brief モーション1のある姿勢とモーション2のある姿勢の類似度を求める
 * @param src1  [in]　モーション1  
 * @param src2  [in]　モーション2
 * @param src1_frame  [in]　モーション1のあるフレーム  
 * @param src2_frame  [in]　モーション2のあるフレーム
 * @return 類似度
 */
 PoseSimilarity MotionUtil::getSimilarity(DrawMocapData& src1, DrawMocapData& src2, int src1_frame, int src2_frame, bool useJointAngle)
{
	PoseSimilarity sim;
	sim.rootDiff = 0;
	vector<klVec3> pc1; pc1.push_back(src1.getRootPosition(src1_frame));
	vector<klVec3> pc2; pc2.push_back(src2.getRootPosition(src2_frame));
	for(int i=0; i < src1.getBoneNum(); i++)
	{
		pc1.push_back(src1.getNodePosition(src1_frame, i));
		pc2.push_back(src2.getNodePosition(src2_frame, i));
	}
	klMat44 trans = MotionUtil::calcTransformBetweenFigures(pc1, pc2);

	klQuat ini_rot = klQuat::fromklMat(trans);
	//getQuaternionFromEular<float>(0, getYaw(ini_rot), 0, ini_rot);
	sim.rootDiff = 2 * acos(ini_rot.w);

	//double max_diff = 0;
	//int index = 0;

	//if(onRightFootSupport)
	//{			
	//	klVec3 src1_toe = Converter::toklVec3(src1.getNodePosition(src1.getBoneIndex("rtibia")));
	//	klVec3 src2_toe = Converter::toklVec3(src2.getNodePosition(src2.getBoneIndex("rtibia")));

	//	klVec3 base_root_pos = src1_toe + src2.getRootPosition() - src2_toe;
	//	
	//	this->root_diff = Converter::toklVec3(base_root_pos - src1.getRootPosition()).Abs();

	//}else
	//{

	//	klVec3 src1_toe = Converter::toklVec3(src1.getNodePosition(src1.getBoneIndex("ltibia")));
	//	klVec3 src2_toe = Converter::toklVec3(src2.getNodePosition(src2.getBoneIndex("ltibia")));

	//	klVec3 base_root_pos = src1_toe + src2.getRootPosition() - src2_toe;
	//	
	//	this->root_diff = Converter::toklVec3(base_root_pos - src1.getRootPosition()).Abs();
	//}

	//
	// Velosity similarity 
	//

	int length1, length2;
	if(src1_frame < LENGTH)
	{
		length1 = src1_frame;
	}
	else if(src1_frame >= src1.getFrameNum() - LENGTH)
	{
		length1 = src1.getFrameNum()-1 - src1_frame;
	}else
	{
		length1 = LENGTH;
	}

	if(src2_frame < LENGTH)
	{
		length2 = src2_frame;
	}
	else if(src2_frame >= src2.getFrameNum() - LENGTH)
	{
		length2 = src2.getFrameNum()-1 - src2_frame;
	}else
	{
		length2 = LENGTH;
	}

	sim.velDiff = MotionUtil::velocitySimilarityBasedJointAngle(
		src1.getPose(src1_frame - length1), src1.getPose(src1_frame + length1), length1,
		src2.getPose(src2_frame - length2), src2.getPose(src2_frame + length2), length2
	);

	//v_diff = 0;
	//Pose src1_0 = src1.getPose(src1_frame - LENGTH); // 対象フレームの LENGTH フレーム前を求める
	//Pose src1_1 = src1.getPose(src1_frame);
	//Pose src2_0 = src2.getPose(src2_frame);
	//Pose src2_1 = src2.getPose(src2_frame + LENGTH); // 対象フレームの LENGTH フレーム後を求める
	//klQuat q_tmp;
	//double v_src1, v_src2;
	//for(int i = 0; i < src1.getBoneNum(); i++)
	//{
	//	q_tmp = multiply(inverse(src1_0.getBoneAngle(i)), src1_1.getBoneAngle(i));
	//	v_src1 = 2 * acos(q_tmp.w) / LENGTH;
	//	q_tmp = multiply(inverse(src2_0.getBoneAngle(i)), src2_1.getBoneAngle(i));
	//	v_src2 = 2 * acos(q_tmp.w) / LENGTH;
	//	v_diff += fabs(v_src1 - v_src2);
	//}

	//
	// Pose similarity 
	//
	if(useJointAngle)
	{
		sim.poseDiff = MotionUtil::poseSimilarityBasedJointAngle(src1.getPose(src1_frame), src2.getPose(src2_frame));
	}else
	{
		sim.poseDiff = MotionUtil::poseSimilarityBasedJointPos(src1.getPose(src1_frame), src2.getPose(src2_frame));
	}
	//for(int i=1; i < src1.getBoneNum(); i++)
	//{
	//	klQuat src1_q = src1_1.getBoneAngle(i);
	//	klQuat src2_q = src2_0.getBoneAngle(i);

	//	pose_diff += acos(multiply(inverse(src1_q), src2_q).w);
	//	//cout << "diff : " << src1_v.Angle(src2_v) << " : "<< (src1_v - src2_v).Abs() << ", " << src1.getBoneName(i) << endl;		


	//	//if(MAX_POS_DIFF < (src1_v - src2_v).Abs())
	//	//{
	//	//	//klQuat src1_q = src1.getBoneAngle(i);
	//	//	//klQuat src2_q = src2.getBoneAngle(i);
	//	//	Pose src2_1 = this->src2.getPose(src2_index + LENGTH);
	//	//	q_tmp = multiply(src1.getBoneAngle(i), inverse(src1_1.getBoneAngle(i)));
	//	//	v_src1 = 2 * acos(q_tmp.w) / LENGTH;
	//	//	q_tmp = multiply(src2_0.getBoneAngle(i), inverse(src2.getBoneAngle(i)));
	//	//	v_src2 = 2 * acos(q_tmp.w) / LENGTH;
	//	//	q_tmp = multiply(src1_1.getBoneAngle(i), inverse(src1.getBoneAngle(i)));
	//	//	double dist1 = 2 * acos(q_tmp.w);
	//	//	q_tmp = multiply(src2_1.getBoneAngle(i), inverse(src1.getBoneAngle(i)));
	//	//	double dist2 = 2 * acos(q_tmp.w);

	//	//	if((v_src1 * v_src2 > 0) && (dist1 - dist2 > ROUND ))
	//	//	{
	//	//		this->diff = INFINITE;
	//	//		return;
	//	//	}
	//	//}

	////	if(max_diff < (src1_v - src2_v).Abs())
	////	{
	////		max_diff = (src1_v - src2_v).Abs();
	////		index = i;
	////	}
	//}
	//cout << "max-diff : " << max_diff << ", " << src1.getBoneName(index) << endl;

	return sim;
}

/**
 * @fn static void normalizeMotion(DrawMocapData& dst, DrawMocapData& src, int src_frame, int dst_frame)
 * @brief モーション1のあるフレームをモーション2のあるフレームの姿勢に基づいて正規化する
 * @param dst	  [out]  モーション1
 * @param base    [in]  モーション2のあるフレームの姿勢
 * @param dst_frame [in]  モーション1のあるフレーム
 * @return なし
 */
 void MotionUtil::normalizeMotion(DrawMocapData& dst, Pose& base, int dst_frame)
{
	//基準フレームのときに原点にいるようにする
	klVec3 ini_root_pos = dst.getRootPosition(dst_frame);ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}
	
	//位置を合わせる(x,y方向)
	//向きを合わせる(z軸方向の回転)
	vector<klVec3> pc1; pc1.push_back(dst.getRootPosition(dst_frame));
	vector<klVec3> pc2; pc2.push_back(base.getRootPosition());
	for(int i=0; i < dst.getBoneNum(); i++)
	{
		pc1.push_back(dst.getNodePosition(dst_frame, i));
		pc2.push_back(base.getNodePosition(i));
	}
	klMat44 trans = MotionUtil::calcTransformBetweenFigures(pc2, pc1);

	klVec3 ini_pos = trans.GetPos();//Nino::Converter::toklVec3(dst.getRootPosition(0));
	klQuat ini_rot = klQuat::fromklMat(trans);//multiply(base.getRootAngle(), inverse(dst.getRootRotation(dst_frame)));
	klQuat zero_q;zero_q.Clear();
	//ini_rot.clear();
	ini_rot.fromEuler(0, ini_rot.GetRoll(), 0);
	for(int i =0; i < dst.getFrameNum(); i++)
	{

		dst.setRootRotation(ini_rot * dst.getRootRotation(i), i);
		dst.setRootPosition(ini_rot.Rotate(dst.getRootPosition(i)), i);
		//klVec3 pos = Nino::Converter::toklVec3(dst.getRootPosition(i));
		//klQuat rot = dst.getRootRotation(i);

		//klVec3 correct_pos = Converter::toklVec3(rotate(Converter::toklVec3(pos), inverse(ini_rot))) + ini_pos;
		//klQuat correct_rot = multiply(inverse(ini_rot2), rot);
		//dst.SetRootPos(i, correct_pos.x, -correct_pos.y, pos.z);
		//dst.setRootRotation(correct_rot, i);
	}

	ini_root_pos = dst.getRootPosition(dst_frame) - base.getRootPosition();ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}
	

	//if(this->rightFootonGround)
	//{ // right foot on the ground
	//	Pose p;
	//	Pose p_src1 = src1.getPose(this->src1_start_frame);
	//	//klVec3 src1_pos = p_src1.getRootPosition();
	//	klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("rtibia")));
	//	Pose p_src2 = src2.getPose(this->src2_end_frame);
	//	klVec3 src2_pos = p_src2.getRootPosition();
	//	klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("rtibia")));

	//	klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
	//	klQuat base_root_rot;
	//	getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
	//	
	//	for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
	//	{
	//		p = src2.getPose(i);
	//		p.SetRootPosition(
	//			(p.getRootPosition() - src2_pos) + base_root_pos
	//			);
	//		p.SetRootAngle(
	//			multiply(p.getRootAngle(),inverse(base_root_rot)));
	//		src2_relative.AddPose(p);
	//	}

	//}
	//else
	//{ // left foot on the ground
	//	Pose p;
	//	Pose p_src1 = src1.getPose(this->src1_start_frame);
	//	//klVec3 src1_pos = p_src1.getRootPosition();
	//	klVec3 src1_pos_toe = Converter::toklVec3(p_src1.getNodePosition(p_src1.getBoneIndex("ltibia")));
	//	Pose p_src2 = src2.getPose(this->src2_end_frame);
	//	klVec3 src2_pos = p_src2.getRootPosition();
	//	klVec3 src2_pos_toe = Converter::toklVec3(p_src2.getNodePosition(p_src2.getBoneIndex("ltibia")));

	//	klVec3 base_root_pos = src1_pos_toe + src2_pos - src2_pos_toe;
	//	klQuat base_root_rot;
	//	getQuaternionFromEular<float>(0, getRoll(multiply(p_src1.getRootAngle(), inverse(p_src2.getRootAngle()))), 0, base_root_rot);
	//	
	//	for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
	//	{
	//		p = src2.getPose(i);
	//		p.SetRootPosition(
	//			(p.getRootPosition() - src2_pos) + base_root_pos
	//			);
	//		p.SetRootAngle(
	//			multiply(p.getRootAngle(),inverse(base_root_rot)));
	//		src2_relative.AddPose(p);
	//	}
	//	//Pose p;
	//	//Pose p_src1 = src1.getPose(this->src1_start_frame);
	//	//klVec3 src1_pos = p_src1.getRootPosition();
	//	//Pose p_src2 = src2.getPose(this->src2_end_frame);
	//	//klVec3 src2_pos = p_src2.getRootPosition();

	//	//for(int i = this->src2_end_frame - interpolateFrames; i < src2.getFrameNum(); i++)
	//	//{
	//	//	p = src2.getPose(i);
	//	//	p.SetRootPosition(
	//	//		(p.getRootPosition() - src2_pos) + src1_pos
	//	//		);
	//	//	src2_relative.AddPose(p);
	//	//}	
	//}
}

/**
 * @fn static void normalizeMotion2(DrawMocapData& dst, DrawMocapData& src, int src_frame, int dst_frame)
 * @brief モーション1のあるフレームをモーション2のあるフレームの姿勢に基づいて正規化する
 * @param dst	  [out]  モーション1
 * @param base    [in]  モーション2のあるフレームの姿勢
 * @param dst_frame [in]  モーション1のあるフレーム
 * @return なし
 */
 void MotionUtil::normalizeMotion2(DrawMocapData& dst, Pose& base, int dst_frame)
{
	//基準フレームのときに原点にいるようにする
	klVec3 ini_root_pos = dst.getRootPosition(dst_frame);ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}

	//基準フレームのときのルートの向きを原点に合わせる
	klQuat ini_rot = !(dst.getRootRotation(dst_frame)) * base.getRootAngle();//inverse(base.getRootAngle());
	ini_rot.fromEuler(0, ini_rot.GetRoll(), 0);
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootRotation(ini_rot * dst.getRootRotation(i), i);
		dst.setRootPosition(ini_rot.Rotate(dst.getRootPosition(i)), i);
	}

	ini_root_pos = dst.getRootPosition(dst_frame) - base.getRootPosition();ini_root_pos.z = 0.0;
	for(int i=0; i < dst.getFrameNum(); i++)
	{
		dst.setRootPosition(dst.getRootPosition(i) - ini_root_pos, i);
	}

	return;
}
