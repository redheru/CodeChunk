#ifndef GLUTGROUND_HPP
#define GLUTGROUND_HPP


//STL, C++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//GLUT
//#include <GL/glut.h>

//C
#include <time.h>
#include <math.h>

//Math
#include <KlMath/klMatrix.h>
#include <KlMath/klVector.h>

//GLUT
#include "GLUTColor.hpp"
#include "GLUTUtil.hpp"

using namespace std;

namespace GLUT
{

class GLUTGround
{
public:
	GLUTGround()
	{
		alpha = 1.0;
		rows = 10;
		scale = 1.0;
		y_pos = 0.0f;
		axisWidth = 5.0;

		amb = 1.0;
		diffuse = 0.1;
		specular = 0.0;
		colors[0] = klVec4(66.0/255.0, 46.0/255.0, 46.0/255.0, 1.0);
		colors[1] = klVec4(254.0/255.0, 225.0/255.0, 150.0/255.0, 1.0);
	}


///////////////////////////////////
//member
///////////////////////////////////
public:
	klVec4 colors[2];
	double specular;
	double amb;//( colors[(i + j) & 1].r * 0.5, colors[(i + j) & 1].g * 0.5, colors[(i + j) & 1].b * 0.5, 1.0);
	double diffuse;
	int rows;
	double scale;
	float y_pos;
	double alpha;

	double axisWidth;

///////////////////////////////////
//method
///////////////////////////////////
public:
	void Render( bool is_draw_tile = true, bool is_draw_axes = false)
	{
		double length = scale * (double)rows * 2.0;
		glPushMatrix();
		glScaled( scale, scale, scale );
		glNormal3d(0.0, 1.0, 0.0);
		if(is_draw_tile)
		{
			glBegin(GL_QUADS);
			for(int i = -rows; i < rows; i++)
			{
				for(int j = -rows; j < rows; j++)
				{
					GLUTColor::setMaterial(colors[(i+j) & 1], this->specular, this->diffuse, this->amb);

					glVertex3d((GLdouble)j, y_pos, (GLdouble)(i + 1));
					glVertex3d((GLdouble)(j + 1), y_pos, (GLdouble)(i + 1));
					glVertex3d((GLdouble)(j + 1), y_pos, (GLdouble)i);
					glVertex3d((GLdouble)j, y_pos, (GLdouble)i);
				}
			}
			glEnd();
		}
		glPopMatrix();

		double up = 0.02;
		if(is_draw_axes)
		{
			double axis_len = length / 2.0;
			GLUTColor::setMaterial(GLUTColor::red, 0, 0.8, 0.8);
			GLUTUtil::DrawLine(0, up, 0,   axis_len, 0, up,  this->axisWidth);
			
			GLUTColor::setMaterial(GLUTColor::green, 0, 0.8, 0.8);
			GLUTUtil::DrawLine(0, up, 0,   0, axis_len, up,  this->axisWidth);
			
			GLUTColor::setMaterial(GLUTColor::blue, 0, 0.8, 0.8);			
			GLUTUtil::DrawLine(0, up, 0,   0, 0, axis_len,    this->axisWidth);
		}
	}

}; // class GLUTGround

} // namespace GLUT

#endif //GLUTGround.hpp