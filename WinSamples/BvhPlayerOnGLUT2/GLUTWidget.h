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


class GLUTShadow; //前方宣言(クラスの相互参照のため)
class GLUTWidget //: public QGLWidget
{
	//Q_OBJECT

public:
	//GLUTWidget(QWidget *parent);
	GLUTWidget();
	~GLUTWidget();

private:

	//GL環境
	GLUTBase* base_;
	GLUTView* view_;
	GLUTShadow* shadow_;

	DrawMocapData *mocap_;//Mocap
	//QTimeLine *timeLine_;//タイムライン

	int curFrame_;
	int startFrame_;
	clock_t startTime_;

	int width_, height_;

	bool isLoaded_;
	bool isSingleLoop_;
	State state_;

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
	void flipGroundTile(){base_->flags.flip( GLUTBase::f_GroundTile);updateGL();}
	void flipGroundAxes(){base_->flags.flip( GLUTBase::f_GroundAxes );updateGL();}
	void flipLocalAxes(){mocap_->setIsDrawLocalAxes(!(mocap_->getIsDrawLocalAxes()));updateGL();}

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
	DrawMocapData* getMocap(){ return mocap_;}
	GLUTView* getGLUTView(){return view_;}
	GLUTBase* getGLUTBase(){return base_;}
	GLUTShadow* getGLUTShadow(){return shadow_;}
	int getCurrentFrame(){return ((isLoaded_)? curFrame_ : -1);}//int getCurrentFrame(){return timeLine_->currentFrame();}
	float getCurrentTime(){return ((isLoaded_)? ((float)curFrame_ / (float)mocap_->getFps()) : -1);}//int getCurrentTime(){return timeLine_->currentTime();}
	int getEndFrame(){return ((isLoaded_)? mocap_->getFrameNum() : -1);}
	float getUpdateInterval(){return ((isLoaded_)? mocap_->getFrameTime() : -1.0);}
	State getState(){return state_;}

// >ミューテータ
	void setCurrentTime(int time){ (isLoaded_)? curFrame_ = time * mocap_->getFps() : curFrame_ = -1;}
	void setCurrentFrame(int frame){ (isLoaded_)? curFrame_ = frame : curFrame_ = -1;}
	void setState(State state){state_ = state;}
	//void setEndTime(){timeLine_->setCurrentTime((timeLine_->endFrame()-1) * timeLine_->updateInterval());}

};

}//namespace GLUT

#endif // GLWIDGET_H