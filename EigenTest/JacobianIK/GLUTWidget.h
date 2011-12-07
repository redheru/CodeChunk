//-----------------------------------------------------------------------------
//                             GLUTWidget.h
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

#ifndef GLUT_WIDGET_HEADER__
#define GLUT_WIDGET_HEADER__

#include "DrawFigure.h"
#include "FigureUtil.h"

#include "GLUTBase.hpp"
#include "GLUTView.hpp"

//#include <KlMath/klMath.h>

#include <GL/glew.h>
#include <GL/glut.h>
//#include <GL/glext.h>
//#include <QGLWidget>
//#include <QtOpenGL>
//#include <QTimeLine>

#include <time.h>
#include <iostream>

using namespace std;
//using namespace KlMath;

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{

// ����:
// �񋓌^
enum State
{
	PLAY,
	PAUSE,
	STOP,
	UNKNOWN
};

// ����:
// GLUT�ŕ`�悷��E�B�W�F�b�g�N���X
class GLUTWidget //: public QGLWidget
{
	//Q_OBJECT

public:
	//GLUTWidget(QWidget *parent);
	GLUTWidget();
	~GLUTWidget();

private:

	//GL��
	GLUTBase* m_base;
	GLUTView* m_view;

	CDrawFigure *m_figure;//Mocap
	//QTimeLine *timeLine_;//�^�C�����C��

	int m_curFrame;
	int m_startFrame;
	clock_t m_startTime;

	int m_width, m_height;

	bool m_isLoaded;
	bool m_isSingleLoop;
	State m_state;

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
	void flipGroundTile(){m_base->flags.flip( GLUTBase::f_GroundTile);updateGL();}
	void flipGroundAxes(){m_base->flags.flip( GLUTBase::f_GroundAxes );updateGL();}
	void flipLocalAxes(){m_figure->setIsDrawLocalAxes(!(m_figure->getIsDrawLocalAxes()));updateGL();}

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
	CDrawFigure* getFigure(){ return m_figure;}
	GLUTView* getGLUTView(){return m_view;}
	GLUTBase* getGLUTBase(){return m_base;}
	int getCurrentFrame(){return ((m_isLoaded)? m_curFrame : -1);}//int getCurrentFrame(){return timeLine_->currentFrame();}
	float getCurrentTime(){return ((m_isLoaded)? ((float)m_curFrame / (float)m_figure->getFps()) : -1);}//int getCurrentTime(){return timeLine_->currentTime();}
	int getEndFrame(){return ((m_isLoaded)? m_figure->getFrameNum() : -1);}
	float getUpdateInterval(){return ((m_isLoaded)? m_figure->getFrameTime() : -1.0);}
	State getState(){return m_state;}

// >�~���[�e�[�^
	void setCurrentTime(int time){ (m_isLoaded)? m_curFrame = time * m_figure->getFps() : m_curFrame = -1;}
	void setCurrentFrame(int frame){ (m_isLoaded)? m_curFrame = frame : m_curFrame = -1;}
	void setState(State state){m_state = state;}
	//void setEndTime(){timeLine_->setCurrentTime((timeLine_->endFrame()-1) * timeLine_->updateInterval());}
};

}//namespace GLUT

#endif // GLWIDGET_H