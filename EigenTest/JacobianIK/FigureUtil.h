//-----------------------------------------------------------------------------
//                             FigureUtil.h
//
// キャラクタフィギュアクラスの機能を拡張するクラス
// 静的関数しか保持しない
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/12/5 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef HUMAN_FIGURE_UTIL_CLASS_HEADER_FILE_
#define HUMAN_FIGURE_UTIL_CLASS_HEADER_FILE_
#pragma once

#include <string>
#include <vector>
#include "Figure.h"

#include <Eigen/StdVector> // EigenをVectorで利用するときに必要
using namespace Eigen;


// 説明:
// キャラクタフィギュアの拡張クラス
class CFigureUtil
{
// 説明:
// 機能拡張の関数
public:
	static Vector3f GetJointPosition(const CFigure* figure, int dest_jid, int root_jid = -1);
	static Vector3f GetJointPosition(const CFigure* figure, std::string destName, std::string rootName = "World")
	{return GetJointPosition(figure, figure->FindNode(destName.c_str()), figure->FindNode(rootName.c_str())); }

	static Matrix4f GetJacobian(const CFigure* figure, int dest_jid, int root_jid = -1);

	static int GetLinkNum(const CFigure* figure, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& rotMat, std::vector<Matrix4f, aligned_allocator<Matrix4f>>& offsetMat, int dest_jid, int root_jid);
	static void Mat2EulerYPR(const Matrix4f& mat, float& yaw, float& pitch, float& roll);
	static void DiffMatEulerDomain(Matrix4f& dmYaw, Matrix4f& dmPitch, Matrix4f& dmRoll, float yaw, float pitch, float roll);

};


#endif //HUMAN_FIGURE_UTIL_CLASS_HEADER_FILE_