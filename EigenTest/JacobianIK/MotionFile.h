//-----------------------------------------------------------------------------
//                             MotionFile.h
//
// モーションデータ読み書き関数群宣言部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/04 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef MOTION_FILE_PARSER_NAMESPACE_HEADER_
#define MOTION_FILE_PARSER_NAMESPACE_HEADER_

#include "Figure.h"
#include "MotionData.h"

// 名前空間: NMotionFile
// モーションデータ読み書き関数群
namespace NMotionFile
{
	// BVHファイルのロード
	bool LoadBvhFile(const char *fpath, CFigure *fig, CMotionData *mot);
	// ASF-AMCファイルのロード
	bool LoadAsfAmcFile(const char *asfpath, const char *amcpath, CFigure *fig, CMotionData *mot);

	// BVHファイルのセーブ
	bool StoreBvhFile(const char *fpath, const CFigure *fig, const CMotionData *mot);
};

#endif //MOTION_FILE_PARSER_NAMESPACE_HEADER_
