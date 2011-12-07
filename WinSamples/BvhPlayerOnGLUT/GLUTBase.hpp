#ifndef GLUTBASE_HPP
#define GLUTBASE_HPP

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
#include "GLUTCamera.hpp"
#include "GLUTColor.hpp"
#include "GLUTGround.hpp"
#include "GLUTLight.hpp"
#include "GLUTUtil.hpp"
#include "GLUTView.hpp"

//Math
#include <KlMath/klMath.h>

using namespace std;

namespace GLUT
{

class GLUTBase
{
public:
	typedef enum
	{
		f_GroundTile,
		f_GroundAxes,
		f_FlagNum
	}Flag;
	bitset<f_FlagNum> flags;

// >member
public:
	GLUTGround ground;


private:

// >public method
public:
	GLUTBase()
	{
		Initialize();
	}
	void Initialize()
	{
		flags.set(this->f_GroundTile);
	}


	void Scene()
	{
		//render ground
		ground.Render( flags[this->f_GroundTile], flags[this->f_GroundAxes] );
	}

	//void Begin2D()
	//{
	//	//glPushAttrib(GL_ENABLE_BIT);
	//	glMatrixMode(GL_PROJECTION);
	//	glPushMatrix();
	//	glLoadIdentity();
	//	//gluOrtho2D(0, 1, 0, 1);
	//	glOrtho( 0, 1, 0, 1, -100, 100 );
	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glLoadIdentity();
	//	glDisable(GL_LIGHTING);
	//	
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}

	//void End2D()
	//{
	//	glDisable(GL_BLEND);
	//	glPopMatrix();
	//	glMatrixMode(GL_PROJECTION);
	//	glPopMatrix();
	//	//glPopAttrib();
	//	glMatrixMode(GL_MODELVIEW);
	//	glEnable(GL_LIGHTING);
	//}
	
}; // class GLUTBase

} // namespace GLUT

#endif //GLUTBase.hpp