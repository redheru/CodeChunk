//-----------------------------------------------------------------------------
//                             GLUTBase.h
//
// GLUTで描画するためのクラス
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

// 名前空間: NGLUT
// GLUTで描画するためのクラス群
namespace NGLUT
{
// 説明:
// GLUTで描画するための基本クラス
class GLUTBase
{
// 説明:
// 列挙型
public:
	typedef enum
	{
		f_GroundTile,
		f_GroundAxes,
		f_FlagNum
	}Flag;

// 説明:
// 変数
public:
	// フラグ
	bitset<f_FlagNum> flags;

	// 地面クラスのインスタンス
	GLUTGround ground;

// 説明:
// コンストラクタ，デストラクタ
public:
	// デフォルトコンストラクタ
	GLUTBase(){ Initialize();}

// 説明:
// 初期化関数
public:
	void Initialize(){flags.set(this->f_GroundTile);}

// 説明:
// 描画関数
public:
	void Scene()
	{
		// 地面をレンダリングする
		ground.Render( flags[this->f_GroundTile], flags[this->f_GroundAxes] );
	}	
}; // class GLUTBase

} // namespace GLUT

#endif // GLUT_BASE_HEADER__