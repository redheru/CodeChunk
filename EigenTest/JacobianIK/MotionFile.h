//-----------------------------------------------------------------------------
//                             MotionFile.h
//
// ���[�V�����f�[�^�ǂݏ����֐��Q�錾��
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

// ���O���: NMotionFile
// ���[�V�����f�[�^�ǂݏ����֐��Q
namespace NMotionFile
{
	// BVH�t�@�C���̃��[�h
	bool LoadBvhFile(const char *fpath, CFigure *fig, CMotionData *mot);
	// ASF-AMC�t�@�C���̃��[�h
	bool LoadAsfAmcFile(const char *asfpath, const char *amcpath, CFigure *fig, CMotionData *mot);

	// BVH�t�@�C���̃Z�[�u
	bool StoreBvhFile(const char *fpath, const CFigure *fig, const CMotionData *mot);
};

#endif //MOTION_FILE_PARSER_NAMESPACE_HEADER_
