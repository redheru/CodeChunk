/*---------------------------------------------------------------------------
           MQO形式の3Dモデルを読み込んでOpenGL上で表示するサンプル
---------------------------------------------------------------------------*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

#include "GLMetaseq.h"	// モデルローダ


// グローバル変数
int g_mouseX = 0;	// マウスX座標
int g_mouseY = 0;	// マウスY座標
int g_rotX = 0;		// X軸周りの回転
int	g_rotY = 0;		// Y軸周りの回転

MQO_MODEL g_mqoModel;	// MQOモデル


// プロトタイプ宣言
void mySetLight(void);
void Draw(void);
void Reshape (int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);


// 光源の設定を行う関数
void mySetLight(void)
{
	GLfloat light_diffuse[]  = { 0.9, 0.9, 0.9, 1.0 };	// 拡散反射光
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	// 鏡面反射光
	GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 0.1 };	// 環境光
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };	// 位置と種類

	// 光源の設定
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_diffuse );	 // 拡散反射光の設定
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular ); // 鏡面反射光の設定
	glLightfv( GL_LIGHT0, GL_AMBIENT,  light_ambient );	 // 環境光の設定
	glLightfv( GL_LIGHT0, GL_POSITION, light_position ); // 位置と種類の設定

	glShadeModel( GL_SMOOTH );	// シェーディングの種類の設定
	glEnable( GL_LIGHT0 );		// 光源の有効化
}


// 描画関数
void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 初期化
	glClearColor(0.f, 0.f, 0.2f, 1.0);					// 背景色
	glMatrixMode(GL_MODELVIEW);

	glEnable( GL_DEPTH_TEST );		// 隠面処理の適用
	mySetLight();					// 光源の設定
	glEnable( GL_LIGHTING );		// 光源ON

	glPushMatrix();
		glTranslatef(0.0, -100.0, -400.0);	// 平行移動
		glRotatef( g_rotX, 1, 0, 0 );		// X軸回転
		glRotatef( g_rotY, 0, 1, 0 );		// Y軸回転
		mqoCallModel(g_mqoModel);			// MQOモデルのコール
	glPopMatrix();

	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );

	glutSwapBuffers();
}


// ウィンドウ変形時に呼ばれる関数
void Reshape (int w, int h)
{
	double	znear = 10;
	double	fovy = 50;
	double	zfar = 10000;

	// ビューポート設定
	glViewport(0, 0, w, h);

	// 射影設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)w/h, znear, zfar);
}


// キーイベント関数
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
		case 0x1b:
			exit(0);	// （このあと終了処理関数が呼ばれる）
		default:
			break;
	}
}


// マウスイベント関数
void Mouse(int button, int state, int x, int y)
{
	if ( state == GLUT_DOWN ) {
		g_mouseX = x;
		g_mouseY = y;				
	}
}


// マウスドラッグの処理関数
void Motion(int x, int y)
{
	int xd, yd;

	// マウス移動量の算出
	xd = x - g_mouseX;
	yd = y - g_mouseY;

	// 回転量の設定
	g_rotX += yd;
	g_rotY += xd;

	// マウス座標の更新
	g_mouseX = x;
	g_mouseY = y;
}


// 終了処理関数
void Quit(void)
{
	mqoDeleteModel( g_mqoModel );	// モデルの削除
	mqoCleanup();					// GLMetaseqの終了処理
}


// main関数
int main(int argc, char *argv[])
{
	// 初期化
	glutInit(&argc,argv);										// OpenGL初期化
	glutInitWindowPosition(100, 50);							// ウィンドウの表示位置
	glutInitWindowSize(640, 480);								// ウィンドウのサイズ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// ディスプレイ設定
	glutCreateWindow("MQO Loader for OpenGL");					// ウィンドウの生成

	// モデルを表示させる準備
	mqoInit();											// GLMetaseqの初期化
	g_mqoModel = mqoCreateModel("miku_xx_03_18_210.mqo",0.5);		// モデルのロード

	// 終了処理関数の設定
	atexit(Quit);

	// コールバック関数の設定
	glutDisplayFunc(Draw);			// 描画処理関数の設定
	glutIdleFunc(Draw);				// アイドル時の処理関数の設定
	glutReshapeFunc(Reshape);		// ウィンドウ変形時の処理を行う関数の設定
	glutKeyboardFunc(Keyboard);		// キー入力時の処理関数の設定
	glutMouseFunc(Mouse);			// マウス入力時の処理関数の設定
	glutMotionFunc(Motion);			// マウスドラッグ時の処理関数の設定

	// イベント待ちの無限ループに入る
	glutMainLoop();

	return 0;
}
