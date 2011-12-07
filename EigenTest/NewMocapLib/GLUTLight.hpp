//-----------------------------------------------------------------------------
//                             GLUTLight.h
//
// GLUT�ł̃��C�g�̊Ǘ��N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/13 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------


#ifndef GLUT_LIGHT_HEADER_
#define GLUT_LIGHT_HEADER_

////C
//#include <math.h>
//#include <time.h>
//
////STL, C++
//#include <vector>
//#include <map>
//#include <string>
//#include <algorithm>
//#include <fstream>
//#include <iostream>
//
////GLUT
//#include <GL/glew.h>
//#include <GL/glut.h>
//
////Math
//#include <KlMath/klMatrix.h>
//#include <KlMath/klVector.h>
//
////GLUT
//#include "GLUTUtil.hpp"

using namespace std;

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{

// ����:
// GLUT�Ń��C�g���Ǘ�����N���X
class GLUTLight
{
// ����:
// �ϐ�
public:
	Vector4f specular;
	Vector4f diffuse;
	Vector4f ambient;
	Vector4f diffuse_shadow;
	Vector4f specular_shadow;
	Vector4f ambient_shadow;

	Vector4f pos;// ���s�����ɂ��邽�߂ɁApos.w = 0�ɂ���I
	GLenum lightName;
	
// Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	GLUTLight(){ Init(); }

	GLUTLight(GLenum light_name)
	{
		Init();
		lightName = light_name;
        //SetLightAsSpotLight();
	}

// ����:
// �������֐�
public:
	void Init()
	{
		pos = Vector4f(1, 10, 2, 0.f);

		specular = Vector4f( 0.50, 0.50, 0.50, 1.0 );
		diffuse = Vector4f( 1.0,  1.0, 1.0, 1.0 );
		ambient = Vector4f( 0.65, 0.65, 0.65, 1.0 );

		specular_shadow = Vector4f( 0.0,  0.0, 0.0, 1.0 );
		diffuse_shadow = Vector4f( 0.3, 0.3, 0.3, 1.0 );
		ambient_shadow = Vector4f( 0.65, 0.65, 0.65, 1.0 );

		//specular = Vector4f( 1.0, 1.0, 1.0, 1.0 );
		//diffuse = Vector4f( 1.0,  1.0, 1.0, 1.0 );
		//ambient = Vector4f( 0.0, 0.0, 0.0, 0.0 );

		//specular_shadow = Vector4f( 0.5,  0.5, 0.5, 1.0 );
		//diffuse_shadow = Vector4f( 0.5, 0.5, 0.5, 1.0 );
		//ambient_shadow = Vector4f( 0.5, 0.5, 0.5, 1.0 );
		
	}

	void SetLightStatus( bool in_shadow = false )
	{
		SetLightPos();
		SetLightfv( in_shadow );
	}

	void SetLightfv( bool in_shadow = false  )
	{	
		//glLightfv(lightName, GL_AMBIENT, ambient );
		if( in_shadow )// �e�̒��̏ꍇ
		{	
			glLightfv(lightName, GL_AMBIENT, ambient_shadow.data() );
		    glLightfv(lightName, GL_DIFFUSE, diffuse_shadow.data() );
		    glLightfv(lightName, GL_SPECULAR, specular_shadow.data() );
		}
		else// �����̏ꍇ
		{
			glLightfv(lightName, GL_AMBIENT, ambient.data() );
		    glLightfv(lightName, GL_DIFFUSE, diffuse.data() );
		    glLightfv(lightName, GL_SPECULAR, specular.data() );
		}

		//if( is_draw_light_pos )
		//{
		//	NinoGL::Util::DrawSphere( klVec3(pos.x,pos.y,pos.z), 1.0, 50 );
		//}
	}

	void SetLightPos()
	{
		glLightfv(lightName, GL_POSITION, pos.data() );

	}

    void SetLightAsSpotLight()
	{
		//pos = Vector4f(3.0, 7.0, 3.0, 1.0); //w�ɂ�0�ȊO������ƃX�|�b�g���C�g�ɂȂ�
		GLfloat lightDir[] = {0.f, -1.0f, 0.f, 1.f};
		glLightfv( lightName, GL_SPOT_DIRECTION, lightDir);
		glLightf( lightName, GL_SPOT_CUTOFF, 30.f);
		glLightf( lightName, GL_SPOT_EXPONENT, 20.0f );
	}

}; // class GLUTLight

}// namespace NGLUT

#endif // GLUT_LIGHT_HEADER_