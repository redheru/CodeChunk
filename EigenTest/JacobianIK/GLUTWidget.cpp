//-----------------------------------------------------------------------------
//                             GLUTWidget.cpp
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
// 説明: コンストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
GLUTWidget::GLUTWidget()
{
	m_figure = new CDrawFigure();
	m_view = new GLUTView();
	m_base = new GLUTBase();

	//m_shadow = new GLUTShadow(512, 512);
	//m_isEnableShadow = true;
	//timeLine_ = new QTimeLine();

	//タイムライン
	//connect(timeLine_, SIGNAL(frameChanged(int)), this, SLOT(updateScene()));

}

//-----------------------------------------------------------------------------
// 説明: デストラクタ
// 引数: 
// 返り値:
// その他: 
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
// 説明: GLの初期化
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
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

	m_view->ApplyClearColor();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	m_view->InitiallizeView(m_width, m_height);

	init();//GLの初期化処理
}

//-----------------------------------------------------------------------------
// 説明: その他の初期化
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::init()
{
	////地面の描写
	m_base->flags.set( GLUTBase::f_GroundAxes, 1);
	m_base->ground.m_rows = 16;
	m_base->ground.m_scale = 0.5;
	m_base->ground.m_colors[0] = Vector3f(0.3, 0.3, 0.3);
	m_base->ground.m_colors[1] = Vector3f(0.7, 0.7, 0.7);
	m_view->m_clearColor = Vector4f(1.f, 1.f, 1.f, 1.f);

	//時間の設定
	m_curFrame = -1;
	m_startFrame = -1;
	m_isLoaded = false;
	m_isSingleLoop = true;
	m_state = UNKNOWN;
	
}

//-----------------------------------------------------------------------------
// 説明: Motionを読む
// 引数: 
//       bvhFileName [in] Bvhファイル名
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::loadBvhFile( string bvhFileName)
{
	
	m_figure->Initialize();
	m_figure->LoadBVH(bvhFileName);

	//タイムラインの設定
	m_curFrame = 0;
	m_startFrame = 0;
	m_isLoaded = true;
	m_isSingleLoop = true;
	m_state = STOP;
}

//-----------------------------------------------------------------------------
// 説明: GL上のシーン構築
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::displayGL()
{
 //   if( m_isEnableShadow/*m_shadow->getIsEnagled()*/ )
	//{// 影付きで表示
	//	m_shadow->createDepthMapBegin( &(getGLUTView()->m_lights[0]->pos), 50, 0.1, 1000 ); 
	//	paintGL();
	//	m_shadow->createDepthMapEnd();
	//	m_shadow->renderShadowBegin(getGLUTView()->m_lights[0]->lightName, &(getGLUTView()->m_lights[0]->ambient_shadow), &(getGLUTView()->m_lights[0]->diffuse_shadow), &(getGLUTView()->m_lights[0]->specular_shadow), &(getGLUTView()->m_camera.getLookAtMat()));
	//	paintGL();
	//	m_shadow->renderShadowEnd();
	//	m_shadow->renderSunnyPlaceBegin(getGLUTView()->m_lights[0]->lightName, &(getGLUTView()->m_lights[0]->ambient), &(getGLUTView()->m_lights[0]->diffuse), &(getGLUTView()->m_lights[0]->specular), &(getGLUTView()->m_camera.getLookAtMat()));
	//	paintGL();
	//	m_shadow->renderSunnyPlaceEnd();

	//	//m_shadow->drawScene();  //内部ではpaintGLが呼ばれている.
	//}
	//else
	{// 影なしで表示
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
// 説明: GL上のシーン描画(描画部分が受け取ってる関数)
// 引数: 
// 返り値:
// その他: 
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
// 説明: GLの更新処理の中身
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// 説明: GLのサイズが変更された時の処理
// 引数: 
// 返り値:
// その他: 
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
// 説明: マウスを押した時の処理
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::mousePressEvent(int button, int state, int x, int y)
{
	m_view->MouseFunc(button, state, x, y);
	//updateGL();
}

//-----------------------------------------------------------------------------
// 説明: マウスを動かした時の処理
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::mouseMoveEvent(int x, int y)
{
	m_view->MouseMove(x, y);
	//updateGL();
}

//-----------------------------------------------------------------------------
// 説明: キー入力の処理
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// 説明: 特別キー入力の処理
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::specialKeyPressEvent(int key, int x, int y)
{

}


//-----------------------------------------------------------------------------
// 説明: GLを更新する(Loadしたデータの再生用)
// 引数: 
// 返り値:
// その他: 
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
// 説明: 時間を更新する
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::updateFrame()
{
	if(!m_state == PLAY) //再生以外の状態なら時間更新なし
		return;

	double time;
	clock_t tmp = clock();
	time = (double)( tmp - m_startTime ) / CLOCKS_PER_SEC;
	//time *= (double)playSpeed; //スピード調整機能

	m_curFrame = m_startFrame + (int)fabs(time/m_figure->getFrameTime());

	if( m_curFrame >= m_figure->getFrameNum())
	{//モーションの最後まで到達した時
		if(m_isSingleLoop)
		{
			stop();//最初に戻す
			play();//再生
		}
	}
}

//-----------------------------------------------------------------------------
// 説明: 再生を停止して巻き戻す
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::stop()
{
	m_state = STOP;
	m_curFrame = 0;
	m_startFrame = 0;

	cout << "Stop at " << m_curFrame << endl;

	//timeLine_->stop();
	//timeLine_->setCurrentTime(0);
	////TODO 更新を促すシグナルが必要
}


//-----------------------------------------------------------------------------
// 説明: 現在のフレーム位置から再生または一時停止する
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTWidget::play()
{
	if(m_state == PLAY)
	{//一時停止
		m_state = PAUSE;
		m_startFrame = m_curFrame;
		//timeLine_->stop();

		cout << "Pause at " << m_curFrame << endl;		
	}
	else if(m_state == PAUSE || m_state == STOP)
	{//再生
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