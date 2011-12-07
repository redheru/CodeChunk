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

	//センサデータ出力用の初期設定

	mocap_ = new DrawMocapData();
	view_ = new GLUTView();
	base_ = new GLUTBase();
	shadow_ = new GLUTShadow();
    shadow_->Load(getThis(), true, 50, 2.0, 10.0 );
    shadow_->Initialize();
	//timeLine_ = new QTimeLine();

	//タイムライン
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


//GLの初期化
void GLUTWidget::initializeGL()
{
	//initialization of openGL

	glClearColor(0.85f, 0.85f, 0.85f, 0.f);

	glShadeModel( GL_SMOOTH ); //色のグラディエーション表示
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); //アンチエイリアスの設定
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

	init();//GLの初期化処理
}

//その他の初期化
void GLUTWidget::init()
{
	////地面の描写
	base_->flags.set( GLUTBase::f_GroundAxes, 0);
	base_->ground.rows = 16;
	base_->ground.scale = 0.5;
	base_->ground.colors[0] = klVec4(0.8, 0.8, 0.8, 1.0);
	base_->ground.colors[1] = klVec4(1.0, 1.0, 1.0, 1.0);
	view_->clearColor = klVec4(1.0, 1.0, 1.0, 1);

	//時間の設定
	curFrame_ = -1;
	startFrame_ = -1;
	isLoaded_ = false;
	isSingleLoop_ = true;
	state_ = UNKNOWN;
	
}

//Load Motion (Bvh file)
//Motionを読む
void GLUTWidget::loadBvhFile( string bvhFileName)
{
	
	mocap_->Initialize();

	if( !mocap_->LoadMotion(bvhFileName))
	{
		return;//Error
	}

	//タイムラインの設定
	curFrame_ = 0;
	startFrame_ = 0;
	isLoaded_ = true;
	isSingleLoop_ = true;
	state_ = STOP;
}

//GL上のシーン構築
void GLUTWidget::displayGL()
{
    if( shadow_->getIsEnagled() )
	{// 影付きで表示
        //paintGL();
		shadow_->drawScene();  //内部ではpaintGLが呼ばれている.
	}
	else
	{// 影なしで表示
    	view_->ApplyCurrentStatus( false, true );        
        paintGL();
	}
	glutSwapBuffers();	
}

//GL上のシーン描画(描画部分.GLUTShadowが受け取ってる関数)
void GLUTWidget::paintGL()
{
	base_->Scene();
	if(mocap_->getIsLoaded())
	{
		mocap_->DrawFrame(curFrame_);
	}
}

//GLの更新処理の中身
void GLUTWidget::updateGL()
{
	//
	//時間の更新
	//
	updateFrame();

	//
	//GL描画の更新
	//
	displayGL();

}

//GLのサイズが変更された時の処理
void GLUTWidget::resizeGL(int w, int h)
{
	//process resize keep good aspect ratio for 3D scene
	view_->Reshape(w, h);
	width_ = w;
	height_ = h;

	updateGL();
}

//マウスを押した時の処理
void GLUTWidget::mousePressEvent(int button, int state, int x, int y)
{
	view_->MouseFunc(button, state, x, y);
	//updateGL();
}

//マウスを動かした時の処理
void GLUTWidget::mouseMoveEvent(int x, int y)
{
	view_->MouseMove(x, y);
	//updateGL();
}

//キー入力の処理
void GLUTWidget::keyPressEvent(unsigned char key, int x, int y)
{
	//player_->Keyboard( event );

	//モーションの再生
	if(key == 's')
		stop();
	else if(key == ' ')
		play();

	//環境の変更
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


//GLを更新する(Loadしたデータの再生用)
void GLUTWidget::updateFrame(int frame)
{
	//timeLine_->setCurrentTime(frame * timeLine_->updateInterval());	
	//updateGL();

	if(frame >= mocap_->getFrameNum())
		return;

	curFrame_ = frame;
	updateGL();
}

//時間を更新する
void GLUTWidget::updateFrame()
{
	if(!state_ == PLAY) //再生以外の状態なら時間更新なし
		return;

	double time;
	clock_t tmp = clock();
	time = (double)( tmp - startTime_ ) / CLOCKS_PER_SEC;
	//time *= (double)playSpeed; //スピード調整機能

	curFrame_ = startFrame_ + (int)fabs(time/mocap_->getFrameTime());

	if( curFrame_ >= mocap_->getFrameNum())
	{//モーションの最後まで到達した時
		if(isSingleLoop_)
		{
			stop();//最初に戻す
			play();//再生
		}
	}
}

//再生を停止して巻き戻す
void GLUTWidget::stop()
{
	state_ = STOP;
	curFrame_ = 0;
	startFrame_ = 0;

	cout << "Stop at " << curFrame_ << endl;

	//timeLine_->stop();
	//timeLine_->setCurrentTime(0);
	////TODO 更新を促すシグナルが必要
}


//現在のフレーム位置から再生または一時停止する
void GLUTWidget::play()
{
	if(state_ == PLAY)
	{//一時停止
		state_ = PAUSE;
		startFrame_ = curFrame_;
		//timeLine_->stop();

		cout << "Pause at " << curFrame_ << endl;		
	}
	else if(state_ == PAUSE || state_ == STOP)
	{//再生
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