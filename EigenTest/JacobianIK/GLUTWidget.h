//-----------------------------------------------------------------------------
//                             GLUTWidget.h
//
// GLUTで描画するウィジェットクラス
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

// 名前空間: NGLUT
// GLUTで描画するためのクラス群
namespace NGLUT
{

// 説明:
// 列挙型
enum State
{
	PLAY,
	PAUSE,
	STOP,
	UNKNOWN
};

// 説明:
// GLUTで描画するウィジェットクラス
class GLUTWidget //: public QGLWidget
{
	//Q_OBJECT

public:
	//GLUTWidget(QWidget *parent);
	GLUTWidget();
	~GLUTWidget();

private:

	//GL環境
	GLUTBase* m_base;
	GLUTView* m_view;

	CDrawFigure *m_figure;//Mocap
	//QTimeLine *timeLine_;//タイムライン

	int m_curFrame;
	int m_startFrame;
	clock_t m_startTime;

	int m_width, m_height;

	bool m_isLoaded;
	bool m_isSingleLoop;
	State m_state;

// > GLの関数
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
	void init();//初期化

// >スロット
//public slots:
	void flipGroundTile(){m_base->flags.flip( GLUTBase::f_GroundTile);updateGL();}
	void flipGroundAxes(){m_base->flags.flip( GLUTBase::f_GroundAxes );updateGL();}
	void flipLocalAxes(){m_figure->setIsDrawLocalAxes(!(m_figure->getIsDrawLocalAxes()));updateGL();}

	void updateFrame();//GLの更新
	void updateFrame(int frame);//フレーム指定によるGLの更新

// > シグナル
//signals:
	void frameChanged(int frame);//更新のシグナルを送る

public:
	void loadBvhFile(string bvhFileName);//Motionのロード

	void stop();//モーション再生の停止
	void play();//モーション再生の開始

// >アクセッサ
	GLUTWidget* getThis(){return this;}
	CDrawFigure* getFigure(){ return m_figure;}
	GLUTView* getGLUTView(){return m_view;}
	GLUTBase* getGLUTBase(){return m_base;}
	int getCurrentFrame(){return ((m_isLoaded)? m_curFrame : -1);}//int getCurrentFrame(){return timeLine_->currentFrame();}
	float getCurrentTime(){return ((m_isLoaded)? ((float)m_curFrame / (float)m_figure->getFps()) : -1);}//int getCurrentTime(){return timeLine_->currentTime();}
	int getEndFrame(){return ((m_isLoaded)? m_figure->getFrameNum() : -1);}
	float getUpdateInterval(){return ((m_isLoaded)? m_figure->getFrameTime() : -1.0);}
	State getState(){return m_state;}

// >ミューテータ
	void setCurrentTime(int time){ (m_isLoaded)? m_curFrame = time * m_figure->getFps() : m_curFrame = -1;}
	void setCurrentFrame(int frame){ (m_isLoaded)? m_curFrame = frame : m_curFrame = -1;}
	void setState(State state){m_state = state;}
	//void setEndTime(){timeLine_->setCurrentTime((timeLine_->endFrame()-1) * timeLine_->updateInterval());}
};

}//namespace GLUT

#endif // GLWIDGET_H