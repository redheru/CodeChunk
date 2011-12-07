//-----------------------------------------------------------------------------
//                             GLUTCamera.h
//
// GLUTでカメラを管理するクラス
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/13 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_CAMERA_HEADER__
#define GLUT_CAMERA_HEADER__

////STL, C++
//#include <vector>
//#include <map>
//#include <string>
//#include <algorithm>
//#include <fstream>
//#include <iostream>
//
////C
//#include <time.h>
//#include <math.h>
//
////GLUT
//#include <GL/glew.h>
//#include <GL/glut.h>
//
////Math
//#include <KlMath/klMath.h>

using namespace std;

// 名前空間: NGLUT
// GLUTで描画するためのクラス群
namespace NGLUT
{
// 説明:
// GLUTでカメラを管理するクラス
class GLUTCamera
{
// 説明:
// 列挙型
public:
	typedef enum{
		cam_Front,
		cam_Right,
		cam_Back,
		cam_Num
	}CameraState;

// 説明:
// 変数
public:
	Vector3f m_upDirection;
	Vector3f m_eyePos;
	Vector3f m_centerPos;

// Eigenライブラリの要素を動的に生成するときに必要
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW


// 説明:
// コンストラクタ，デストラクタ
public:
	// デフォルトコンストラクタ
	GLUTCamera()
	{
		ResetCameraPos();
		camState = CameraState(0);
	}
	CameraState camState;

// 説明:
// 関数
public:
	void ChangeCameraState()
	{
		int cam = camState;
		cam++;
		if( cam == cam_Num ) cam = 0;
		camState = CameraState(cam);

		if( camState == cam_Front )
		{
			m_upDirection = Vector3f(0, 1, 0);
			m_eyePos = Vector3f(1.27569, 4.30451, 9.31323e-010);
			m_centerPos = Vector3f(9.31323e-010,-0.0925062, 0.647543);
		}
		else if( camState == cam_Right )
		{
			m_upDirection = Vector3f(0, 1, 0);
			m_eyePos = Vector3f(1.84676, 1.30207, -3.69364);
			m_centerPos = Vector3f( 0.690867, 0.0488946, 0.0347897);
		}
		else if( camState == cam_Back )
		{
			m_upDirection = Vector3f(0, 1, 0);
			m_eyePos = Vector3f(3.7179,-1.22031, -0.00801188);
			m_centerPos = Vector3f(0.24351, 0.31975, 0.00629665);
		}
	}

	void ResetCameraPos()
	{
		m_upDirection = Vector3f(0,1,0);
		//upDirection = Vector3f(0,1,0);

		m_centerPos = Vector3f(0,0,0);
		m_eyePos = Vector3f(2, 6, 2);
	}


	void MoveCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(getLookAtMat().data());
		
		/* 視点の移動 */
		//gluLookAt(eyePos.x, eyePos.y, eyePos.z,
		//		  centerPos.x, centerPos.y, centerPos.z,
		//		  upDirection.x, upDirection.y, upDirection.z);	
	}

	void RotateCamera(float degree_x, float degree_y)
	{
		//convert matrix(world coordinate to center coordinate)
		Matrix4f world2center = Matrix4f::Identity();
		NEigenMath::Matrix4fTranslation(&world2center, m_centerPos.x(), m_centerPos.y(), m_centerPos.z());
		//world2center.LoadIdentity();
		//world2center.Translate(centerPos);

		//convert to center coordinate
		//Vector4f new_eyePos = world2center * eyePos;
		Vector3f new_eyePos = Vector3f::Zero();
		NEigenMath::Vector3fTransformCoord(&new_eyePos, &m_eyePos, &world2center);

		//x axis rotate
		Matrix4f tmp = Matrix4f::Identity();
		NEigenMath::Matrix4fRotationX(&tmp, NEigenMath::ToRadian(degree_x));
		NEigenMath::Vector3fTransformCoord(&new_eyePos, &new_eyePos, &tmp);
		//new_eyePos.Rotate(degree_x, 1, 0, 0);

		//y axis rotate
		NEigenMath::Matrix4fRotationY(&tmp, NEigenMath::ToRadian(degree_y));
		NEigenMath::Vector3fTransformCoord(&new_eyePos, &new_eyePos, &tmp);
		//new_eyePos.Rotate(degree_y, 0, 1, 0);

		//convert to world coordinate(multiply inverse matrix)
		Matrix4f invWorld2Center = world2center.inverse();
		NEigenMath::Vector3fTransformCoord(&new_eyePos, &new_eyePos, &invWorld2Center);
		//new_eyePos = !world2center * new_eyePos;

		m_eyePos = new_eyePos;
	}

	void TranslateCamera(float y, float z)
	{
		Matrix4f world2eye = getLookAtMat();

		Matrix4f cameraTrans = Matrix4f::Identity();
		NEigenMath::Matrix4fTranslation(&cameraTrans, y, z, 0.f);

		Vector4f eyePos;
		eyePos << m_eyePos.x(), m_eyePos.y(), m_eyePos.z(), 1.0f;
		Vector4f centerPos;
		centerPos << m_centerPos.x(), m_centerPos.y(), m_centerPos.z(), 1.0f;

		eyePos =  world2eye.inverse() * cameraTrans * world2eye * eyePos;
		centerPos = world2eye.inverse() * cameraTrans * world2eye  * centerPos;

		m_eyePos = eyePos.head(3);
		m_centerPos = centerPos.head(3);

		//1. convert to eye coordinate
		//2. translate camera(xy)
		//3. convert to world coordinate (multiply inverse matrix)
	}

	void ZoomCamera(float zoom)
	{
		Matrix4f world2eye = getLookAtMat();
		Matrix4f eyeTransMat = Matrix4f::Identity();		

		//1. convert to eye coordinate
		//eyeTransMat *= world2eye;
		
		//2. translate eye (z direction)
		//eyeTransMat.Translate(0,0,zoom);

		//3. convert to wolrd coordinate
		//eyeTransMat *= !world2eye;

		//apply to eye position
		//eyePos = eyeTransMat * eyePos;
		
		NEigenMath::Matrix4fTranslation(&eyeTransMat, 0.f, 0.f, zoom);

		Vector4f eyePos;
		eyePos << m_eyePos.x(), m_eyePos.y(), m_eyePos.z(), 1.0f;

		eyePos = world2eye.inverse() * eyeTransMat * world2eye * eyePos;

		m_eyePos = eyePos.head(3);
	}

	//from
	//http://www.memorize-being.net/2008/08/16/calc-camera-matrix-at-opengl.htm
	Matrix4f getLookAtMat()
	{
		Vector3f x = m_eyePos - m_centerPos;
		x = x.normalized();
		
		Vector3f y = m_upDirection.cross(x);
		y = y.normalized();
		
		Vector3f z = x.cross(y);
		z = z.normalized();

		Matrix4f mat = Matrix4f::Identity();
		mat << y.x(), y.y(), y.z(), -(m_eyePos.dot(y)),
		       z.x(), z.y(), z.z(), -(m_eyePos.dot(z)),
		       x.x(), x.y(), x.z(), -(m_eyePos.dot(x)),
		       0.f,     0.f,   0.f,                1.f;

		////ほんとは縦にならぶ
		//double mat[16] = 
		//{
		//	y.v[0], z.v[0], x.v[0], 0,
		//	y.v[1], z.v[1], x.v[1], 0,
		//	y.v[2], z.v[2], x.v[2], 0,
		//	-(eyePos*y), -(eyePos*z), -(eyePos*x), 1
		//};

		//klMat44 result;
		//
		//for(int i = 0; i<16; i++)
		//{
		//	result.m[i] = mat[i];
		//}
		return mat;
	}

	void setUpDirection( Vector3f dir)
	{
		m_upDirection = dir;
	}

	void setCenterPos( Vector3f pos)
	{
		m_centerPos = pos;
	}

	void setEyePos( Vector3f pos)
	{
		m_eyePos = pos;
	}

private :
}; // class GLUTCamera

} // namespace GLUT

#endif // GLUT_CAMERA_HEADER__
