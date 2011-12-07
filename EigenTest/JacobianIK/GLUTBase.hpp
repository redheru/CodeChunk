//-----------------------------------------------------------------------------
//                             GLUTBase.h
//
// GLUT�ŕ`�悷�邽�߂̃N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/06 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_BASE_HEADER__
#define GLUT_BASE_HEADER__

//C
#include <time.h>
#include <math.h>

//STL, C++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <bitset>

//GLUT
#include <GL/glew.h>
#include <GL/glut.h>

//GLUT
//#include "GLUTCamera.hpp"
//#include "GLUTColor.hpp"
#include "GLUTGround.hpp"
//#include "GLUTView.hpp"
//#include "GLUTLight.hpp"
//#include "GLUTUtil.hpp"
//#include "GLUTView.hpp"

using namespace std;

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{
// ����:
// GLUT�ŕ`�悷�邽�߂̊�{�N���X
class GLUTBase
{
// ����:
// �񋓌^
public:
	typedef enum
	{
		f_GroundTile,
		f_GroundAxes,
		f_FlagNum
	}Flag;

// ����:
// �ϐ�
public:
	// �t���O
	bitset<f_FlagNum> flags;

	// �n�ʃN���X�̃C���X�^���X
	GLUTGround ground;

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
	GLUTBase(){ Initialize();}

// ����:
// �������֐�
public:
	void Initialize(){flags.set(this->f_GroundTile);}

// ����:
// �`��֐�
public:
	void Scene()
	{
		// �n�ʂ������_�����O����
		ground.Render( flags[this->f_GroundTile], flags[this->f_GroundAxes] );
	}	
}; // class GLUTBase

} // namespace GLUT

#endif // GLUT_BASE_HEADER__