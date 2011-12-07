//-----------------------------------------------------------------------------
//                             EigenMath.h
//
// Eigenライブラリの補助関数群
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

// 名前空間: NEigenMath
// Eigenライブラリの補助関数群
namespace NEigenMath
{

	// 円周率
	static const float EigenPI = 3.141592654f;

	// 度をラジアンに変換する
	float ToRadian(float degree);

	// ラジアンを度に変換する
	float ToDegree(float radian);

	// 2つの3Dベクトルの外積を計算する
	Vector3f* Vector3fCross(Vector3f* pOut, const Vector3f* pV1, const Vector3f* pV2);
	// 指定された行列により3Dベクトルをトランスフォームし、その結果をW=1に射影する
	Vector3f* Vector3fTransformCoord(Vector3f* pOut, const Vector3f* pV, const Matrix4f* pM);

	// 行列を各軸ごとにスケーリングする
	Matrix4f* Matrix4fScaling( Matrix4f *pOut, float sx, float sy, float sz );
	// 回転ベクトルから回転行列を求める
	Matrix4f* Matrix4fRotationAxis( Matrix4f *pOut, const Vector3f *pV, float angle );
	// 移動行列を作成する
	Matrix4f* Matrix4fTranslation( Matrix4f *pOut, float x, float y, float z);
	// クォータニオンから回転行列を作成する
	Matrix4f* Matrix4fRotationQuaternion( Matrix4f *pOut, const Quaternionf* pQ);

	// X軸を回転軸として回転する行列を作成する
	Matrix4f* Matrix4fRotationX( Matrix4f *pOut, float angle);
	// Y軸を回転軸として回転する行列を作成する
	Matrix4f* Matrix4fRotationY( Matrix4f *pOut, float angle);
	// Z軸を回転軸として回転する行列を作成する
	Matrix4f* Matrix4fRotationZ( Matrix4f *pOut, float angle);

	// 回転行列からクォータニオンを作成する
	Quaternionf* QuaternionfRotationMatrix( Quaternionf *pOut, const Matrix4f* pM);

	// あるベクトルからあるベクトルに回転させるクォータニオンを作成する
	Matrix4f* fromTwoVector3f( Matrix4f *pOut, const Vector3f &src, const Vector3f &dist);
};

#endif