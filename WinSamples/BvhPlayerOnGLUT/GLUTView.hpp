#ifndef GLUTVIEW_HPP
#define GLUTVIEW_HPP

//C
#include <math.h>

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
#include <KlMath/klMath.h>

//GLUT
#include "GLUTCamera.hpp"
#include "GLUTLight.hpp"

using namespace std;

namespace GLUT
{

class GLUTView
{

public:
	GLUTView()
	{
		width = 800;
		height = 600;
		widthMin = 0;
		heightMin = 0;

		clearColor = klVec4(0.84, 0.81, 0.75, 1);
		//z_pos = 100;
	}


// >member
public:
	klVec4 clearColor;
	int widthMin;
	int heightMin;
	GLUTCamera camera;
	vector<GLUTLight> lights;

private:
	int width;
	int height;

	int curx, cury;
	//int z_pos;
	klMat44 mat;

	int status[3];//for mouse control
	

// >getter, setter
public:
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	void setClearColor(float R, float G,  float B)
	{
		clearColor.x = R;
		clearColor.y = G;
		clearColor.z = B;
	}


// >method
public:
	void InitiallizeView(int width = 800, int hegiht = 600)
	{
		this->width = width;
		this->height = height;

		Reshape(width, height);
		//matModelView.SetIdentity();

		//mat.LoadIdentity();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixf(mat);
	}
	void AddLight(klVec4 _pos)
	{
		int n = (int)lights.size();
		
		if( n > 7 )
		{
			return;
		}

		GLUTLight temp;
		if( n == 0)
		{
			temp = GLUTLight( GL_LIGHT0 );
		}		
		else if( n == 1)
		{
			temp = GLUTLight( GL_LIGHT1 );
		}
		else if( n == 2)
		{
			glEnable(GL_LIGHT2);
			temp = GLUTLight( GL_LIGHT2 );
		}
		else if( n == 3)
		{
			glEnable(GL_LIGHT3);
			temp = GLUTLight( GL_LIGHT3 );
		}
		else if( n == 4)
		{
			glEnable(GL_LIGHT4);
			temp = GLUTLight( GL_LIGHT4 );
		}
		else if( n == 5)
		{
			glEnable(GL_LIGHT5);
			temp = GLUTLight( GL_LIGHT5 );
		}
		else if( n == 6)
		{
			glEnable(GL_LIGHT6);
			temp = GLUTLight( GL_LIGHT6 );
		}
		else if( n == 7)
		{
			glEnable(GL_LIGHT7);
			temp = GLUTLight( GL_LIGHT7 );
		}
		
		temp.pos = _pos;
		lights.push_back(temp);
	}


	void Reshape(int w, int h)
	{
		if (w < widthMin || h < heightMin) {
			if (w < widthMin) w = widthMin;
			if (h < heightMin) h = heightMin;
		}

		/* ウィンドウ全体をビューポートにする */
		glViewport(0, 0, w, h);

		/* 透視変換行列の指定 */
		glMatrixMode(GL_PROJECTION);

		/* 透視変換行列の初期化 */
		glLoadIdentity();
		gluPerspective(40.0, (double)w / (double)h, 1.0, 100.0);


		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluPerspective(40.0, GLdouble(w) / GLdouble(h), 0.01f, 3000);
		//glViewport(0, 0, w, h);

		width = w; height = h;

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixf(mat);	
	}

	void ApplyClearColor()
	{
		glClearColor(clearColor.x, clearColor.y ,clearColor.z, clearColor.w);
	}

	void ApplyViewPoint()
	{
		camera.MoveCamera();
	}
	
	//set light position
	void ApplyLightsStatus( bool in_shadow = false )
	{
		for(int i = 0; i < (int)lights.size(); i++) lights[i].SetLightStatus( in_shadow );
	}
	
	void ApplyLightsPos()
	{
		for(int i = 0; i < (int)lights.size(); i++) lights[i].SetLightPos();
	}
	void ApplyLightFv(bool in_shadow = false)
	{
		for(int i = 0; i < (int)lights.size(); i++) lights[i].SetLightfv( in_shadow );
	}
    void ApplyLightUseAsSpotLight()
	{
		for(int i = 0; i < (int)lights.size(); i++) lights[i].SetLightAsSpotLight();        
	}

	void ApplyCurrentStatus( bool in_shadow = false, bool is_draw_light_pos = false )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		
		ApplyClearColor();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ApplyViewPoint();

		ApplyLightsStatus( in_shadow );
	}
	
	void ResetView()
	{
		camera.ResetCameraPos();
	}
	
	void MouseFunc(int button, int state, int x, int y)
	{
		switch(button){
		case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN) {
				curx = x;
				cury = y;
				status[0] = 1;
			}
			if( state == GLUT_UP) {
				status[0] = 0;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN) {
				curx = x;
				cury = y;
				status[1] = 1;
			}
			if(state == GLUT_UP) {
				status[1] = 0;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) {
				curx = x;
				cury = y;
				status[2] = 1;
			}
			if(state == GLUT_UP) {
				status[2] = 0;
			}
			break;
		}
	}

	void MouseMove(int x, int y)
	{
		float wsize = (width > height ? height : width);

		if( status[0] == 1 )
		{// left mouse drag to translation
			wsize /= 2;
			float tx = x-curx;
			float ty = y-cury;
			if(tx*tx >= ty*ty)
			{
				camera.TranslateCamera(tx/wsize, 0 );
				
			}
			else
			{
				//camera.TranslateCamera(tx/wsize, 0 );
				camera.TranslateCamera(0, -ty/wsize);
			}
			//camera.TranslateCamera((float)(-(x-curx) /wsize), (float)( -(y-cury)/wsize) );
		}

		if(status[1] == 1)
		{// mid mouse drag to depth direction shift
			wsize /= 3;
			camera.ZoomCamera( (float)(x-curx)/wsize );
		}

		if(status[2] == 1)
		{// right button correspondts to rotation
			wsize /= 200.0;
			float ry = (float)(y-cury) / wsize;
			float rz = -(float)(x-curx) / wsize;
			
			if(fabs((double)ry) > fabs((double)rz))
			{
				camera.RotateCamera( ry, 0  );	
			}
			else
			{
				camera.RotateCamera( 0, rz  );
			}
		}

		curx = x;
		cury = y;
	}

}; // class GLUTView

} // namespace GL

#endif //GLUTVIEW_HPP