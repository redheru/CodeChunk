//-----------------------------------------------------------------------------
//                             GLUTGround.h
//
// GLUT�Œn�ʂ�`�悷��N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/06 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_GROUND_HEADER__
#define GLUT_GROUND_HEADER__

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
////C
//#include <time.h>
//#include <math.h>
//
////Math
//#include <KlMath/klMatrix.h>
//#include <KlMath/klVector.h>

//GLUT
//#include "GLUTBase.hpp"
#include "GLUTUtil.hpp"
#include "GLUTColor.hpp"

using namespace std;

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{
// ����:
// GLUT�ŕ`�悷��n�ʂ̃N���X
class GLUTGround
{
// ����:
// �ϐ�
public:
	Vector3f m_colors[2];
	float m_specular;
	float m_amb;
	float m_diffuse;
	int m_rows;
	float m_scale;
	float m_yPos;
	float m_alpha;
	float m_axisWidth;

// Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
	GLUTGround()
	{
		m_alpha = 1.0;
		m_rows = 10;
		m_scale = 1.0;
		m_yPos = 0.0f;
		m_axisWidth = 5.0;

		m_amb = 1.0;
		m_diffuse = 0.1;
		m_specular = 0.0;
		m_colors[0] = Vector3f(66.0/255.0, 46.0/255.0, 46.0/255.0);
		m_colors[1] = Vector3f(254.0/255.0, 225.0/255.0, 150.0/255.0);
	}

// ����:
// �`��֐�
public:
	void Render( bool isDrawTile = true, bool isDrawAxes = false)
	{
		float length = m_scale * (float)m_rows * 2.0;
		glPushMatrix();
		glScalef( m_scale, m_scale, m_scale );
		glNormal3f(0.0, 1.0, 0.0);
		if(isDrawTile)
		{
			glBegin(GL_QUADS);
			for(int i = -m_rows; i < m_rows; i++)
			{
				for(int j = -m_rows; j < m_rows; j++)
				{
					GLUTColor::setMaterial(m_colors[(i+j) & 1], m_specular, m_diffuse, m_amb);

					glVertex3f((GLfloat)j, m_yPos, (GLfloat)(i + 1));
					glVertex3f((GLfloat)(j + 1), m_yPos, (GLfloat)(i + 1));
					glVertex3f((GLfloat)(j + 1), m_yPos, (GLfloat)i);
					glVertex3f((GLfloat)j, m_yPos, (GLfloat)i);
				}
			}
			glEnd();
		}
		glPopMatrix();

		float up = 0.02;
		if(isDrawAxes)
		{
			float axisLen = length / 2.0;
			GLUTColor::setMaterial(GLUTColor::red, 0, 0.8, 0.8);
			GLUTUtil::DrawLine(0, up, 0,   axisLen, 0, up,  m_axisWidth);
			
			GLUTColor::setMaterial(GLUTColor::green, 0, 0.8, 0.8);
			GLUTUtil::DrawLine(0, up, 0,   0, axisLen, up,  m_axisWidth);
			
			GLUTColor::setMaterial(GLUTColor::blue, 0, 0.8, 0.8);			
			GLUTUtil::DrawLine(0, up, 0,   0, 0, axisLen,    m_axisWidth);
		}
	}

}; // class GLUTGround

} // namespace GLUT

#endif // GLUT_GROUND_HEADER__