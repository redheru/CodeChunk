//-----------------------------------------------------------------------------
//                             GLUTWidget.cpp
//
// GLUT�ŕ`�悷��E�B�W�F�b�g�N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/13 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

//#include "GLUTBase.hpp"
//#include "GLUTView.hpp"
#include "GLUTWidget.h"
//#include "GLUTShadow.h"

#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace NGLUT;

//-----------------------------------------------------------------------------
// ����: �R���X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
GLUTWidget::GLUTWidget()
{
	m_figure = new CDrawFigure();
	m_view = new GLUTView();
	m_base = new GLUTBase();

	//m_shadow = new GLUTShadow(512, 512);
	//m_isEnableShadow = true;
	//timeLine_ = new QTimeLine();

	//�^�C�����C��
	//connect(timeLine_, SIGNAL(frameChanged(int)), this, SLOT(updateScene()));

}

//-----------------------------------------------------------------------------
// ����: �f�X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
GLUTWidget::~GLUTWidget(){
	delete m_figure;
	delete m_view;
	delete m_base;
	//delete m_shadow;
	//delete timeLine_;
	//delete player_;
	//delete puppetMocap_;
	//delete puppetSkl_;

	//delete mocapDb_;
	//delete model_;
}

//-----------------------------------------------------------------------------
// ����: GL�̏�����
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::initializeGL()
{
	//initialization of openGL

	glClearColor(0.85f, 0.85f, 0.85f, 0.f);

	glShadeModel( GL_SMOOTH ); //�F�̃O���f�B�G�[�V�����\��
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); //�A���`�G�C���A�X�̐ݒ�
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_MULTISAMPLE);

	m_view->ApplyClearColor();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	m_view->InitiallizeView(m_width, m_height);

	init();//GL�̏���������
}

//-----------------------------------------------------------------------------
// ����: ���̑��̏�����
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::init()
{
	////�n�ʂ̕`��
	m_base->flags.set( GLUTBase::f_GroundAxes, 1);
	m_base->ground.m_rows = 16;
	m_base->ground.m_scale = 0.5;
	m_base->ground.m_colors[0] = Vector3f(0.3, 0.3, 0.3);
	m_base->ground.m_colors[1] = Vector3f(0.7, 0.7, 0.7);
	m_view->m_clearColor = Vector4f(1.f, 1.f, 1.f, 1.f);

	//���Ԃ̐ݒ�
	m_curFrame = -1;
	m_startFrame = -1;
	m_isLoaded = false;
	m_isSingleLoop = true;
	m_state = UNKNOWN;
	
}

//-----------------------------------------------------------------------------
// ����: Motion��ǂ�
// ����: 
//       bvhFileName [in] Bvh�t�@�C����
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::loadBvhFile( string bvhFileName)
{
	
	m_figure->Initialize();
	m_figure->LoadBVH(bvhFileName);

	//�^�C�����C���̐ݒ�
	m_curFrame = 0;
	m_startFrame = 0;
	m_isLoaded = true;
	m_isSingleLoop = true;
	m_state = STOP;
}

//-----------------------------------------------------------------------------
// ����: GL��̃V�[���\�z
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::displayGL()
{
 //   if( m_isEnableShadow/*m_shadow->getIsEnagled()*/ )
	//{// �e�t���ŕ\��
	//	m_shadow->createDepthMapBegin( &(getGLUTView()->m_lights[0]->pos), 50, 0.1, 1000 ); 
	//	paintGL();
	//	m_shadow->createDepthMapEnd();
	//	m_shadow->renderShadowBegin(getGLUTView()->m_lights[0]->lightName, &(getGLUTView()->m_lights[0]->ambient_shadow), &(getGLUTView()->m_lights[0]->diffuse_shadow), &(getGLUTView()->m_lights[0]->specular_shadow), &(getGLUTView()->m_camera.getLookAtMat()));
	//	paintGL();
	//	m_shadow->renderShadowEnd();
	//	m_shadow->renderSunnyPlaceBegin(getGLUTView()->m_lights[0]->lightName, &(getGLUTView()->m_lights[0]->ambient), &(getGLUTView()->m_lights[0]->diffuse), &(getGLUTView()->m_lights[0]->specular), &(getGLUTView()->m_camera.getLookAtMat()));
	//	paintGL();
	//	m_shadow->renderSunnyPlaceEnd();

	//	//m_shadow->drawScene();  //�����ł�paintGL���Ă΂�Ă���.
	//}
	//else
	{// �e�Ȃ��ŕ\��
    	m_view->ApplyCurrentStatus( false, true );        
        paintGL();

		Vector3f pos1 = Vector3f::Zero();
		Vector3f pos2 = CFigureUtil::GetJointPosition(m_figure->getFigure(), "lradius");
		GLUTUtil::DrawLine(pos1, pos2);
		cout << pos2.x() << ", " << pos2.y() << ", " << pos2.z() << endl;
	}
	glutSwapBuffers();	
}

//-----------------------------------------------------------------------------
// ����: GL��̃V�[���`��(�`�敔�����󂯎���Ă�֐�)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::paintGL()
{
	m_base->Scene();
	if(m_figure->getIsLoaded())
	{
		m_figure->DrawSkeleton(m_curFrame);
	}
}

//-----------------------------------------------------------------------------
// ����: GL�̍X�V�����̒��g
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::updateGL()
{
	//
	//���Ԃ̍X�V
	//
	updateFrame();

	//
	//GL�`��̍X�V
	//
	displayGL();

}

//-----------------------------------------------------------------------------
// ����: GL�̃T�C�Y���ύX���ꂽ���̏���
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::resizeGL(int w, int h)
{
	//process resize keep good aspect ratio for 3D scene
	m_view->Reshape(w, h);
	m_width = w;
	m_height = h;

	updateGL();
}

//-----------------------------------------------------------------------------
// ����: �}�E�X�����������̏���
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::mousePressEvent(int button, int state, int x, int y)
{
	m_view->MouseFunc(button, state, x, y);
	//updateGL();
}

//-----------------------------------------------------------------------------
// ����: �}�E�X�𓮂��������̏���
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::mouseMoveEvent(int x, int y)
{
	m_view->MouseMove(x, y);
	//updateGL();
}

//-----------------------------------------------------------------------------
// ����: �L�[���͂̏���
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::keyPressEvent(unsigned char key, int x, int y)
{
	//player_->Keyboard( event );

	//���[�V�����̍Đ�
	if(key == 's')
		stop();
	else if(key == ' ')
		play();

	//���̕ύX
	if(key == 't')
		flipGroundTile();
	else if(key == 'a')
		flipGroundAxes();
	else if(key == 'A')
		flipLocalAxes();

}

//-----------------------------------------------------------------------------
// ����: ���ʃL�[���͂̏���
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::specialKeyPressEvent(int key, int x, int y)
{

}


//-----------------------------------------------------------------------------
// ����: GL���X�V����(Load�����f�[�^�̍Đ��p)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::updateFrame(int frame)
{
	//timeLine_->setCurrentTime(frame * timeLine_->updateInterval());	
	//updateGL();

	if(frame >= m_figure->getFrameNum())
		return;

	m_curFrame = frame;
	updateGL();
}

//-----------------------------------------------------------------------------
// ����: ���Ԃ��X�V����
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::updateFrame()
{
	if(!m_state == PLAY) //�Đ��ȊO�̏�ԂȂ玞�ԍX�V�Ȃ�
		return;

	double time;
	clock_t tmp = clock();
	time = (double)( tmp - m_startTime ) / CLOCKS_PER_SEC;
	//time *= (double)playSpeed; //�X�s�[�h�����@�\

	m_curFrame = m_startFrame + (int)fabs(time/m_figure->getFrameTime());

	if( m_curFrame >= m_figure->getFrameNum())
	{//���[�V�����̍Ō�܂œ��B������
		if(m_isSingleLoop)
		{
			stop();//�ŏ��ɖ߂�
			play();//�Đ�
		}
	}
}

//-----------------------------------------------------------------------------
// ����: �Đ����~���Ċ����߂�
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::stop()
{
	m_state = STOP;
	m_curFrame = 0;
	m_startFrame = 0;

	cout << "Stop at " << m_curFrame << endl;

	//timeLine_->stop();
	//timeLine_->setCurrentTime(0);
	////TODO �X�V�𑣂��V�O�i�����K�v
}


//-----------------------------------------------------------------------------
// ����: ���݂̃t���[���ʒu����Đ��܂��͈ꎞ��~����
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTWidget::play()
{
	if(m_state == PLAY)
	{//�ꎞ��~
		m_state = PAUSE;
		m_startFrame = m_curFrame;
		//timeLine_->stop();

		cout << "Pause at " << m_curFrame << endl;		
	}
	else if(m_state == PAUSE || m_state == STOP)
	{//�Đ�
		m_state = PLAY;
		m_startFrame = m_curFrame;
		m_startTime = clock();

		cout << "Play start from " << m_curFrame << endl;
	}
	//if(timeLine_->currentFrame() != 0){
	//	timeLine_->resume();
	//}else{
	//	timeLine_->start();
	//}
}