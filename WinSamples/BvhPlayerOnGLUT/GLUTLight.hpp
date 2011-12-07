#ifndef GLUTLIGHT_HPP
#define GLUTLIGHT_HPP

//C
#include <math.h>
#include <time.h>

//STL, C++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//GLUT
#include <GL/glew.h>
#include <GL/glut.h>

//Math
#include <KlMath/klMatrix.h>
#include <KlMath/klVector.h>

//GLUT
#include "GLUTUtil.hpp"

using namespace std;

namespace GLUT
{


class GLUTLight
{

public:


///////////////////////////////////
// member
///////////////////////////////////
public:
	klVec4 specular;
	klVec4 diffuse;
	klVec4 ambient;
	klVec4 diffuse_shadow;
	klVec4 specular_shadow;
	klVec4 ambient_shadow;

	klVec4 pos;// 平行光線にするために、pos.w = 0にする！
	GLenum lightName;
	

///////////////////////////////////
// method
///////////////////////////////////
public:
	GLUTLight(){ Init(); }

	GLUTLight(GLenum light_name)
	{
		Init();
		lightName = light_name;
        SetLightAsSpotLight();
	}
	void Init()
	{
		pos = klVec4(1, 10, 2, 1);

		specular = klVec4( 0.50, 0.50, 0.50, 1.0 );
		diffuse = klVec4( 1.0,  1.0, 1.0, 1.0 );
		ambient = klVec4( 0.65, 0.65, 0.65, 1.0 );

		specular_shadow = klVec4( 0.0,  0.0, 0.0, 1.0 );
		diffuse_shadow = klVec4( 0.7, 0.7, 0.7, 1.0 );
		ambient_shadow = klVec4( 0.65, 0.65, 0.65, 1.0 );

		//specular = klVec4( 1.0, 1.0, 1.0, 1.0 );
		//diffuse = klVec4( 1.0,  1.0, 1.0, 1.0 );
		//ambient = klVec4( 0.0, 0.0, 0.0, 0.0 );

		//specular_shadow = klVec4( 0.5,  0.5, 0.5, 1.0 );
		//diffuse_shadow = klVec4( 0.5, 0.5, 0.5, 1.0 );
		//ambient_shadow = klVec4( 0.5, 0.5, 0.5, 1.0 );
		
	}


	void SetLightStatus( bool in_shadow = false )
	{
		SetLightPos();
		SetLightfv( in_shadow );
	}

	void SetLightfv( bool in_shadow = false  )
	{	
		//glLightfv(lightName, GL_AMBIENT, ambient );
		if( in_shadow )// 影の中の場合
		{	
			glLightfv(lightName, GL_AMBIENT, ambient_shadow );
		    glLightfv(lightName, GL_DIFFUSE, diffuse_shadow );
		    glLightfv(lightName, GL_SPECULAR, specular_shadow );
		}
		else// 日向の場合
		{
			glLightfv(lightName, GL_AMBIENT, ambient );
		    glLightfv(lightName, GL_DIFFUSE, diffuse );
		    glLightfv(lightName, GL_SPECULAR, specular );
		}

		//if( is_draw_light_pos )
		//{
		//	NinoGL::Util::DrawSphere( klVec3(pos.x,pos.y,pos.z), 1.0, 50 );
		//}
	}

	void SetLightPos()
	{
		glLightfv(lightName, GL_POSITION, pos );

	}

    void SetLightAsSpotLight()
	{
		//pos = klVec4(3.0, 7.0, 3.0, 1.0); //wには0以外を入れるとスポットライトになる
		GLfloat lightDir[] = {0.f, -1.0f, 0.f, 1.f};
		glLightfv( lightName, GL_SPOT_DIRECTION, lightDir);
		glLightf( lightName, GL_SPOT_CUTOFF, 30.f);
		glLightf( lightName, GL_SPOT_EXPONENT, 20.0f );
	}

}; // class GLUTLight

}// namespace GLUT

#endif //GLUTLIGHT_HPP