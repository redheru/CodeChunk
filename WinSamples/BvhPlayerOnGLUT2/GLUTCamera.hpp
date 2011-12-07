#ifndef GLUTCAMERA_HPP
#define GLUTCAMERA_HPP

//STL, C++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//C
#include <time.h>
#include <math.h>

//GLUT
#include <GL/glew.h>
#include <GL/glut.h>

//Math
#include <KlMath/klMath.h>

using namespace std;
using namespace KlMath;

namespace GLUT
{

class GLUTCamera
{
public:
	GLUTCamera()
	{
		ResetCameraPos();
		camState = CameraState(0);
	}



// >member
public:
	//rigid	
	klVec3 upDirection;
	klVec3 eyePos;
	klVec3 centerPos;

	typedef enum
	{
		cam_Front,
		cam_Right,
		cam_Back,
		cam_Num
	}CameraState;
	CameraState camState;

// >method
public:
	void DisplayCurrentCameraState(string path = "no_output")
	{
		if(path != "no_output")
		{
			ofstream ofs(path.c_str());
			ofs << "upDirection = klVec3("<<upDirection.x<<", "<<upDirection.y<<", "<<upDirection.z<<");\n";
			ofs << "eyePos = klVec3("<<eyePos.x<<", "<<eyePos.y<<", "<<eyePos.z<<");\n";
			ofs << "centerPos = klVec3("<<centerPos.x<<", "<<centerPos.y<<", "<<centerPos.z<<");\n";
		}
		
		printf("-----DisplayCurrentCameraState-----\n");
		printf("upDirection(%f, %f, %f) \n", upDirection.x, upDirection.y, upDirection.z);
		printf("eyePos(%f, %f, %f) \n", eyePos.x, eyePos.y, eyePos.z);
		printf("centerPos(%f, %f, %f) \n", centerPos.x, centerPos.y, centerPos.z);
		printf("-----------------------------------\n");
	}
	void ChangeCameraState()
	{
		int cam = camState;
		cam++;
		if( cam == cam_Num ) cam = 0;
		camState = CameraState(cam);

		if( camState == cam_Front )
		{
			upDirection = klVec3(0, 1, 0);
			eyePos = klVec3(1.27569, 4.30451, 9.31323e-010);
			centerPos = klVec3(9.31323e-010,-0.0925062, 0.647543);
		}
		else if( camState == cam_Right )
		{
			upDirection = klVec3(0, 1, 0);
			eyePos = klVec3(1.84676, 1.30207, -3.69364);
			centerPos = klVec3( 0.690867, 0.0488946, 0.0347897);
		}
		else if( camState == cam_Back )
		{
			upDirection = klVec3(0, 1, 0);
			eyePos = klVec3(3.7179,-1.22031, -0.00801188);
			centerPos = klVec3(0.24351, 0.31975, 0.00629665);
		}
	}

	void ResetCameraPos()
	{
		upDirection = klVec3(0,1,0);
		//upDirection = klVec3(0,1,0);

		centerPos = klVec3(0,0,0);
		eyePos = klVec3(3, 2, 5);
	}


	void MoveCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(getLookAtMat().m);
		
		/* Ž‹“_‚ÌˆÚ“® */
		//gluLookAt(eyePos.x, eyePos.y, eyePos.z,
		//		  centerPos.x, centerPos.y, centerPos.z,
		//		  upDirection.x, upDirection.y, upDirection.z);	
	}

	void RotateCamera(float degree_x, float degree_y)
	{
		//convert matrix(world coordinate to center coordinate)
		klMat44 world2center;
		world2center.LoadIdentity();
		world2center.Translate(centerPos);

		//convert to center coordinate
		
		klVec3 new_eyePos = world2center * eyePos;
		
		//x axis rotate
		new_eyePos.Rotate(degree_x, 1, 0, 0);

		//y axis rotate
		new_eyePos.Rotate(degree_y, 0, 1, 0);

		//convert to world coordinate(multiply inverse matrix)
		new_eyePos = !world2center * new_eyePos;

		eyePos = new_eyePos;
	}

	void TranslateCamera(float y, float z)
	{
		klMat44 world2eye = getLookAtMat();

		klMat44 cameraTrans;
		cameraTrans.LoadIdentity();
		cameraTrans.Translate(y, z, 0);

		
		eyePos =  !world2eye * cameraTrans * world2eye * eyePos;
		centerPos =  !world2eye * cameraTrans * world2eye * centerPos;

		//1. convert to eye coordinate
		//2. translate camera(xy)
		//3. convert to world coordinate (multiply inverse matrix)
	}

	void ZoomCamera(float zoom)
	{
		klMat44 world2eye = getLookAtMat();
		
		klMat44 eyeTransMat;
		eyeTransMat.LoadIdentity();
		

		//1. convert to eye coordinate
		//eyeTransMat *= world2eye;
		
		//2. translate eye (z direction)
		//eyeTransMat.Translate(0,0,zoom);

		//3. convert to wolrd coordinate
		//eyeTransMat *= !world2eye;

		//apply to eye position
		//eyePos = eyeTransMat * eyePos;
		
		eyeTransMat.Translate(0, 0, zoom);
		eyePos = !world2eye * eyeTransMat * world2eye * eyePos;
	}

	//from
	//http://www.memorize-being.net/2008/08/16/calc-camera-matrix-at-opengl.htm
	klMat44 getLookAtMat()
	{
		klVec3 x = eyePos - centerPos;
		x.Normalize();
		
		klVec3 y = upDirection % x;
		y.Normalize();
		
		klVec3 z = x % y;
		z.Normalize();
		
		//‚Ù‚ñ‚Æ‚Íc‚É‚È‚ç‚Ô
		double mat[16] = 
		{
			y.v[0], z.v[0], x.v[0], 0,
			y.v[1], z.v[1], x.v[1], 0,
			y.v[2], z.v[2], x.v[2], 0,
			-(eyePos*y), -(eyePos*z), -(eyePos*x), 1
		};

		klMat44 result;
		
		for(int i = 0; i<16; i++)
		{
			result.m[i] = mat[i];
		}

		return result;
	}

	void setUpDirection( klVec3 dir)
	{
		upDirection = dir;
	}

	void setCenterPos( klVec3 pos)
	{
		centerPos = pos;
	}

	void setEyePos( klVec3 pos)
	{
		eyePos = pos;
	}

	//double* getLookAtMat( bool for_distinction )
	//{
	//	klVec3 z = eyePos - centerPos;
	//	z.Normalize();
	//	
	//	klVec3 x = upDirection % z;
	//	x.Normalize();
	//	
	//	klVec3 y = z % x;
	//	y.Normalize();
	//	
	//	//‚Ù‚ñ‚Æ‚Íc‚É‚È‚ç‚Ô
	//	double mat[16] = 
	//	{
	//		x.v[0], y.v[0], z.v[0], 0,
	//		x.v[1], y.v[1], z.v[1], 0,
	//		x.v[2], y.v[2], z.v[2], 0,
	//		-(eyePos*x), -(eyePos*y), -(eyePos*z), 1
	//	};

	//	return mat;
	//}

private :
}; // class GLUTCamera

} // namespace GLUT

#endif //GLUTCamera.hpp
