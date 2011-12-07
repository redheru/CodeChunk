#include "GLUTBase.hpp"
#include "GLUTView.hpp"
#include "GLUTShadow.h"

#include "GLUTWidget.h"

#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace GLUT;

//GLUTWidget::GLUTWidget(QWidget *parent)
//	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
GLUTWidget::GLUTWidget()
{

	//�Z���T�f�[�^�o�͗p�̏����ݒ�

	mocap_ = new DrawMocapData();
	view_ = new GLUTView();
	base_ = new GLUTBase();
	shadow_ = new GLUTShadow();
    shadow_->Load(getThis(), true, 50, 2.0, 10.0 );
    shadow_->Initialize();
	//timeLine_ = new QTimeLine();

	//�^�C�����C��
	//connect(timeLine_, SIGNAL(frameChanged(int)), this, SLOT(updateScene()));

}

GLUTWidget::~GLUTWidget(){
	delete mocap_;
	delete view_;
	delete base_;
	//delete timeLine_;
	//delete player_;
	//delete puppetMocap_;
	//delete puppetSkl_;

	//delete mocapDb_;
	//delete model_;
}


//GL�̏�����
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

	view_->ApplyClearColor();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	view_->InitiallizeView(width_, height_);

	init();//GL�̏���������
}

//���̑��̏�����
void GLUTWidget::init()
{
	////�n�ʂ̕`��
	base_->flags.set( GLUTBase::f_GroundAxes, 0);
	base_->ground.rows = 16;
	base_->ground.scale = 0.5;
	base_->ground.colors[0] = klVec4(0.8, 0.8, 0.8, 1.0);
	base_->ground.colors[1] = klVec4(1.0, 1.0, 1.0, 1.0);
	view_->clearColor = klVec4(1.0, 1.0, 1.0, 1);

	//���Ԃ̐ݒ�
	curFrame_ = -1;
	startFrame_ = -1;
	isLoaded_ = false;
	isSingleLoop_ = true;
	state_ = UNKNOWN;
	
}

//Load Motion (Bvh file)
//Motion��ǂ�
void GLUTWidget::loadBvhFile( string bvhFileName)
{
	
	mocap_->Initialize();

	if( !mocap_->LoadMotion(bvhFileName))
	{
		return;//Error
	}

	//�^�C�����C���̐ݒ�
	curFrame_ = 0;
	startFrame_ = 0;
	isLoaded_ = true;
	isSingleLoop_ = true;
	state_ = STOP;
}

//GL��̃V�[���\�z
void GLUTWidget::displayGL()
{
    if( shadow_->getIsEnagled() )
	{// �e�t���ŕ\��
        //paintGL();
		shadow_->drawScene();  //�����ł�paintGL���Ă΂�Ă���.
	}
	else
	{// �e�Ȃ��ŕ\��
    	view_->ApplyCurrentStatus( false, true );        
        paintGL();
	}
	glutSwapBuffers();	
}

//GL��̃V�[���`��(�`�敔��.GLUTShadow���󂯎���Ă�֐�)
void GLUTWidget::paintGL()
{
	base_->Scene();
	if(mocap_->getIsLoaded())
	{
		mocap_->DrawFrame(curFrame_);
	}
}

//GL�̍X�V�����̒��g
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

//GL�̃T�C�Y���ύX���ꂽ���̏���
void GLUTWidget::resizeGL(int w, int h)
{
	//process resize keep good aspect ratio for 3D scene
	view_->Reshape(w, h);
	width_ = w;
	height_ = h;

	updateGL();
}

//�}�E�X�����������̏���
void GLUTWidget::mousePressEvent(int button, int state, int x, int y)
{
	view_->MouseFunc(button, state, x, y);
	//updateGL();
}

//�}�E�X�𓮂��������̏���
void GLUTWidget::mouseMoveEvent(int x, int y)
{
	view_->MouseMove(x, y);
	//updateGL();
}

//�L�[���͂̏���
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

void GLUTWidget::specialKeyPressEvent(int key, int x, int y)
{

}


//GL���X�V����(Load�����f�[�^�̍Đ��p)
void GLUTWidget::updateFrame(int frame)
{
	//timeLine_->setCurrentTime(frame * timeLine_->updateInterval());	
	//updateGL();

	if(frame >= mocap_->getFrameNum())
		return;

	curFrame_ = frame;
	updateGL();
}

//���Ԃ��X�V����
void GLUTWidget::updateFrame()
{
	if(!state_ == PLAY) //�Đ��ȊO�̏�ԂȂ玞�ԍX�V�Ȃ�
		return;

	double time;
	clock_t tmp = clock();
	time = (double)( tmp - startTime_ ) / CLOCKS_PER_SEC;
	//time *= (double)playSpeed; //�X�s�[�h�����@�\

	curFrame_ = startFrame_ + (int)fabs(time/mocap_->getFrameTime());

	if( curFrame_ >= mocap_->getFrameNum())
	{//���[�V�����̍Ō�܂œ��B������
		if(isSingleLoop_)
		{
			stop();//�ŏ��ɖ߂�
			play();//�Đ�
		}
	}
}

//�Đ����~���Ċ����߂�
void GLUTWidget::stop()
{
	state_ = STOP;
	curFrame_ = 0;
	startFrame_ = 0;

	cout << "Stop at " << curFrame_ << endl;

	//timeLine_->stop();
	//timeLine_->setCurrentTime(0);
	////TODO �X�V�𑣂��V�O�i�����K�v
}


//���݂̃t���[���ʒu����Đ��܂��͈ꎞ��~����
void GLUTWidget::play()
{
	if(state_ == PLAY)
	{//�ꎞ��~
		state_ = PAUSE;
		startFrame_ = curFrame_;
		//timeLine_->stop();

		cout << "Pause at " << curFrame_ << endl;		
	}
	else if(state_ == PAUSE || state_ == STOP)
	{//�Đ�
		state_ = PLAY;
		startFrame_ = curFrame_;
		startTime_ = clock();

		cout << "Play start from " << curFrame_ << endl;
	}
	//if(timeLine_->currentFrame() != 0){
	//	timeLine_->resume();
	//}else{
	//	timeLine_->start();
	//}
}