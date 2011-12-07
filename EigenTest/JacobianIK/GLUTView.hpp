//-----------------------------------------------------------------------------
//                             GLUTView.h
//
// GLUT�ł̌��������Ǘ�����N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/13 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_VIEW_HEADER_
#define GLUT_VIEW_HEADER_

////C
//#include <math.h>
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

//GLUT
#include "GLUTCamera.hpp"
#include "GLUTLight.hpp"

using namespace std;

// �O���錾
class GLUTLight;

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{
// ����:
// GLUT�Ō������̃N���X
class GLUTView
{

// ����:
// �ϐ�
public:
	Vector4f m_clearColor;
	int m_widthMin;
	int m_heightMin;
	GLUTCamera m_camera;
	vector<GLUTLight *> m_lights;

private:
	int m_width;
	int m_height;

	int m_curX, m_curY;
	//int z_pos;
	Matrix4f m_mat;

	int m_status[3];//for mouse control

// Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
	GLUTView()
	{
		m_width = 800;
		m_height = 600;
		m_widthMin = 0;
		m_heightMin = 0;

		m_clearColor = Vector4f(0.84, 0.81, 0.75, 1);

		m_mat = Matrix4f::Identity();
		//z_pos = 100;
	}
	// �f�X�g���N�^
	~GLUTView()
	{
		for(unsigned int i = 0; i < m_lights.size(); i++)
			delete m_lights[i];
	}


// ����:
// �A�N�Z�b�T
public:
	int getWidth(){ return m_width; }
	int getHeight(){ return m_height; }
	void setClearColor(float R, float G,  float B)
	{
		m_clearColor.x() = R;
		m_clearColor.y() = G;
		m_clearColor.z() = B;
	}


// ����:
// �������֐�
public:
	void InitiallizeView(int width = 800, int height = 600)
	{
		m_width = width;
		m_height = height;

		Reshape(m_width, m_height);
		//matModelView.SetIdentity();

		//mat.LoadIdentity();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixf(mat);
	}

// ����:
// �֐�
public:
	void AddLight(const Vector4f& pos)
	{
		int n = (int)m_lights.size();
		
		if( n > 7 || n < 0)
		{
			return;
		}

		GLUTLight* temp;
		if( n == 0)
		{
			temp = new GLUTLight( GL_LIGHT0 );
		}		
		else if( n == 1)
		{
			temp = new GLUTLight( GL_LIGHT1 );
		}
		else if( n == 2)
		{
			glEnable(GL_LIGHT2);
			temp = new GLUTLight( GL_LIGHT2 );
		}
		else if( n == 3)
		{
			glEnable(GL_LIGHT3);
			temp = new GLUTLight( GL_LIGHT3 );
		}
		else if( n == 4)
		{
			glEnable(GL_LIGHT4);
			temp = new GLUTLight( GL_LIGHT4 );
		}
		else if( n == 5)
		{
			glEnable(GL_LIGHT5);
			temp = new GLUTLight( GL_LIGHT5 );
		}
		else if( n == 6)
		{
			glEnable(GL_LIGHT6);
			temp = new GLUTLight( GL_LIGHT6 );
		}
		else if( n == 7)
		{
			glEnable(GL_LIGHT7);
			temp = new GLUTLight( GL_LIGHT7 );
		}
		
		temp->pos = pos;
		m_lights.push_back(temp);
	}


	void Reshape(int w, int h)
	{
		if (w < m_widthMin || h < m_heightMin) {
			if (w < m_widthMin) w = m_widthMin;
			if (h < m_heightMin) h = m_heightMin;
		}

		/* �E�B���h�E�S�̂��r���[�|�[�g�ɂ��� */
		glViewport(0, 0, w, h);

		/* �����ϊ��s��̎w�� */
		glMatrixMode(GL_PROJECTION);

		/* �����ϊ��s��̏����� */
		glLoadIdentity();
		gluPerspective(40.0, (double)w / (double)h, 1.0, 100.0);

		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluPerspective(40.0, GLdouble(w) / GLdouble(h), 0.01f, 3000);
		//glViewport(0, 0, w, h);

		m_width = w; m_height = h;

		//glMatrixMode(GL_MODELVIEW);
		//glLoadMatrixf(mat);	
	}

	void ApplyClearColor()
	{
		glClearColor(m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), m_clearColor.w());
	}

	void ApplyViewPoint()
	{
		m_camera.MoveCamera();
	}
	
	//set light position
	void ApplyLightsStatus( bool in_shadow = false )
	{
		for(int i = 0; i < (int)m_lights.size(); i++) m_lights[i]->SetLightStatus( in_shadow );
	}
	
	void ApplyLightsPos()
	{
		for(int i = 0; i < (int)m_lights.size(); i++) m_lights[i]->SetLightPos();
	}
	void ApplyLightFv(bool in_shadow = false)
	{
		for(int i = 0; i < (int)m_lights.size(); i++) m_lights[i]->SetLightfv( in_shadow );
	}
    void ApplyLightUseAsSpotLight()
	{
		for(int i = 0; i < (int)m_lights.size(); i++) m_lights[i]->SetLightAsSpotLight();        
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
		m_camera.ResetCameraPos();
	}
	
	void MouseFunc(int button, int state, int x, int y)
	{
		switch(button){
		case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN) {
				m_curX = x;
				m_curY = y;
				m_status[0] = 1;
			}
			if( state == GLUT_UP) {
				m_status[0] = 0;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN) {
				m_curX = x;
				m_curY = y;
				m_status[1] = 1;
			}
			if(state == GLUT_UP) {
				m_status[1] = 0;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) {
				m_curX = x;
				m_curY = y;
				m_status[2] = 1;
			}
			if(state == GLUT_UP) {
				m_status[2] = 0;
			}
			break;
		}
	}

	void MouseMove(int x, int y)
	{
		float wsize = (m_width > m_height ? m_height : m_width);

		if( m_status[0] == 1 )
		{// left mouse drag to translation
			wsize /= 2;
			float tx = x - m_curX;
			float ty = y - m_curY;
			if(tx*tx >= ty*ty)
			{
				m_camera.TranslateCamera(tx/wsize, 0 );
				
			}
			else
			{
				//camera.TranslateCamera(tx/wsize, 0 );
				m_camera.TranslateCamera(0, -ty/wsize);
			}
			//camera.TranslateCamera((float)(-(x-curx) /wsize), (float)( -(y-cury)/wsize) );
		}

		if(m_status[1] == 1)
		{// mid mouse drag to depth direction shift
			wsize /= 3;
			m_camera.ZoomCamera( (float)(x - m_curX)/wsize );
		}

		if(m_status[2] == 1)
		{// right button correspondts to rotation
			wsize /= 200.0;
			float ry = (float)(y - m_curY) / wsize;
			float rz = -(float)(x - m_curX) / wsize;
			
			if(fabs(ry) > fabs(rz))
			{
				m_camera.RotateCamera( ry, 0  );	
			}
			else
			{
				m_camera.RotateCamera( 0, rz  );
			}
		}
		m_curX = x;
		m_curY = y;
	}

}; // class GLUTView

} // namespace NGLUT

#endif GLUT_VIEW_HEADER_