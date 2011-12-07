#ifndef GLUTWIDGET_H
#define GLUTWIDGET_H

#include "DrawMocapData.h"

#include "GLUTBase.hpp"
#include "GLUTView.hpp"

#include <KlMath/klMath.h>

//#include <GL/glew.h>
//#include <GL/glut.h>
//#include <QGLWidget>
//#include <QtOpenGL>
//#include <QTimeLine>

#include <time.h>
#include <iostream>

using namespace std;
using namespace KlMath;

namespace GLUT
{

enum State
{
	PLAY,
	PAUSE,
	STOP,
	UNKNOWN
};


class GLUTShadow; //�O���錾(�N���X�̑��ݎQ�Ƃ̂���)
class GLUTWidget //: public QGLWidget
{
	//Q_OBJECT

public:
	//GLUTWidget(QWidget *parent);
	GLUTWidget();
	~GLUTWidget();

private:

	//GL��
	GLUTBase* base_;
	GLUTView* view_;
	GLUTShadow* shadow_;

	DrawMocapData *mocap_;//Mocap
	//QTimeLine *timeLine_;//�^�C�����C��

	int curFrame_;
	int startFrame_;
	clock_t startTime_;

	int width_, height_;

	bool isLoaded_;
	bool isSingleLoop_;
	State state_;

// > GL�̊֐�
public:
	void initializeGL();
	void displayGL();
    void paintGL();
	void updateGL();
	void resizeGL(int w, int h);
	void mousePressEvent(int button, int state, int x, int y);
	void mouseMoveEvent(int x, int y);
	void keyPressEvent(unsigned char key, int x, int y);
	void specialKeyPressEvent(int key, int x, int y);
	void init();//������

// >�X���b�g
//public slots:
	void flipGroundTile(){base_->flags.flip( GLUTBase::f_GroundTile);updateGL();}
	void flipGroundAxes(){base_->flags.flip( GLUTBase::f_GroundAxes );updateGL();}
	void flipLocalAxes(){mocap_->setIsDrawLocalAxes(!(mocap_->getIsDrawLocalAxes()));updateGL();}

	void updateFrame();//GL�̍X�V
	void updateFrame(int frame);//�t���[���w��ɂ��GL�̍X�V

// > �V�O�i��
//signals:
	void frameChanged(int frame);//�X�V�̃V�O�i���𑗂�

public:
	void loadBvhFile(string bvhFileName);//Motion�̃��[�h

	void stop();//���[�V�����Đ��̒�~
	void play();//���[�V�����Đ��̊J�n

// >�A�N�Z�b�T
	GLUTWidget* getThis(){return this;}
	DrawMocapData* getMocap(){ return mocap_;}
	GLUTView* getGLUTView(){return view_;}
	GLUTBase* getGLUTBase(){return base_;}
	GLUTShadow* getGLUTShadow(){return shadow_;}
	int getCurrentFrame(){return ((isLoaded_)? curFrame_ : -1);}//int getCurrentFrame(){return timeLine_->currentFrame();}
	float getCurrentTime(){return ((isLoaded_)? ((float)curFrame_ / (float)mocap_->getFps()) : -1);}//int getCurrentTime(){return timeLine_->currentTime();}
	int getEndFrame(){return ((isLoaded_)? mocap_->getFrameNum() : -1);}
	float getUpdateInterval(){return ((isLoaded_)? mocap_->getFrameTime() : -1.0);}
	State getState(){return state_;}

// >�~���[�e�[�^
	void setCurrentTime(int time){ (isLoaded_)? curFrame_ = time * mocap_->getFps() : curFrame_ = -1;}
	void setCurrentFrame(int frame){ (isLoaded_)? curFrame_ = frame : curFrame_ = -1;}
	void setState(State state){state_ = state;}
	//void setEndTime(){timeLine_->setCurrentTime((timeLine_->endFrame()-1) * timeLine_->updateInterval());}

};

}//namespace GLUT

#endif // GLWIDGET_H