#include "GLUTWidget.h"
#include "GLUTShadow.h"

#include "glsl.h"

using namespace GLUT;


GLUTShadow::GLUTShadow()
{
	Init();
}

void GLUTShadow::Load( GLUTWidget* glutWidget, bool isUseAlpha, float angle, float nearDistance, float farDistance )
{

	glutWidget_ = glutWidget;
	//this->scene_P = _scene;
	isLoaded_ = true;
	isUseAlpha_ = isUseAlpha;
	//this->view_P = &_view;

	angle_ = angle;
	nearDistance_ = nearDistance;
	farDistance_ = farDistance;
}

void GLUTShadow::drawScene()
{
	// ロード済みか
	if( !isLoaded_ )
	{
		printf( "[Shadow]Light position and scene function are not loaded!\n" );
		printf( "[Shadow]Please call Load() before using this function!\n" );
		return;
	}

	// 初期化してるか
	if( !isInitialized_ )
	{
		printf( "[Shadow]Please call Initialize() before using this function!\n" );
		return;
	}

	// ライトがセットされているか
	if( glutWidget_->getGLUTView()->lights.size() < 1 )
	{
		printf( "[Shadow]Please add a LightGL to ViewGL before using this function!\n" );
		return;
	}

    // FBOを使ってるのでテクスチャサイズを合わせる必要はない
	// reshape functionの修正
	//glutWidget_->getGLUTView()->widthMin = texWidth_; 
	//glutWidget_->getGLUTView()->heightMin = texHeight_; 

	// 影付きのレンダリング
	createShadowMap();

}

void GLUTShadow::Initialize()
{
    /* シェーダプログラムのコンパイル／リンク結果を得る変数 */
    GLint compiled, linked;

    /* テクスチャオブジェクトを生成して結合する */
    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);

	/* テクスチャの割り当て */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth_, texHeight_, 0,
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* テクスチャを拡大・縮小する方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* テクスチャの繰り返し方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	/* もしＲの値がテクスチャの値以下なら真（つまり日向） */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	if( isUseAlpha_ )
	{
		/* 比較の結果をアルファ値として得る */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

		/* アルファテストの比較関数（しきい値） */
		glAlphaFunc(GL_GEQUAL, 0.5f);
	}
	else 
	{
		/* 比較の結果を輝度値として得る */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	}

	///* テクスチャ座標に視点座標系における物体の座標値を用いる */
	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	///* 生成したテクスチャ座標をそのまま (S, T, R, Q) に使う */
	//static const GLdouble genfunc[][4] = {
	//	{ 1.0, 0.0, 0.0, 0.0 },
	//	{ 0.0, 1.0, 0.0, 0.0 },
	//	{ 0.0, 0.0, 1.0, 0.0 },
	//	{ 0.0, 0.0, 0.0, 1.0 },
	//};
	//glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
	//glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
	//glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
	//glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

    /* テクスチャオブジェクトの結合を解除する */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* フレームバッファオブジェクトを生成して結合する */
    glGenFramebuffersEXT(1, &fb_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_);

    /* フレームバッファオブジェクトにデプスバッファ用のテクスチャを結合する */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_TEXTURE_2D, tex_, 0);

    /* カラーバッファが無いので読み書きしない */
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //CHECK_FRAMEBUFFER_STATUS();

    /* フレームバッファオブジェクトの結合を解除する */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* 初期設定 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* GLSL の初期化 */
	//if (glslInit()) exit(1);
	  
	/* シェーダオブジェクトの作成 */
	vertShader_ = glCreateShader(GL_VERTEX_SHADER);
	fragShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	  
	/* シェーダのソースプログラムの読み込み */
    if (readShaderSource(vertShader_, "shadow.vert")) exit(1);
    if (readShaderSource(fragShader_, "shadow.frag")) exit(1);

	/* バーテックスシェーダのソースプログラムのコンパイル */
	glCompileShader(vertShader_);
	glGetShaderiv(vertShader_, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(vertShader_);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in vertex shader.\n");
		exit(1);
	}
	  
	/* フラグメントシェーダのソースプログラムのコンパイル */
	glCompileShader(fragShader_);
	glGetShaderiv(fragShader_, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(fragShader_);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in fragment shader.\n");
		exit(1);
	}
	  
	/* プログラムオブジェクトの作成 */
	gl2Program_ = glCreateProgram();
	  
	/* シェーダオブジェクトのシェーダプログラムへの登録 */
	glAttachShader(gl2Program_, vertShader_);
	glAttachShader(gl2Program_, fragShader_);
	  
	/* シェーダオブジェクトの削除 */
	glDeleteShader(vertShader_);
	glDeleteShader(fragShader_);
	  
	/* シェーダプログラムのリンク */
	glLinkProgram(gl2Program_);
	glGetProgramiv(gl2Program_, GL_LINK_STATUS, &linked);
	printProgramInfoLog(gl2Program_);
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error.\n");
		exit(1);
	}
	  
	/* シェーダプログラムの適用 */
	glUseProgram(gl2Program_);

	/* テクスチャユニット０を指定する */
	glUniform1i(glGetUniformLocation(gl2Program_, "texture"), 0);

	// 初期化済みにする
	isInitialized_ = true;
}

void GLUTShadow::Init()
{
	isLoaded_ = false;
	isInitialized_ = false;
    isEnabled_ = false;
    isUseAlpha_ = false;

    angle_ = 0.f;
	nearDistance_ = 0.f;
	farDistance_ = 0.f;
	glutWidget_ = NULL;
	texWidth_ = 1024;       // シャドウマップのサイズ
    texHeight_ = 1024;      // シャドウマップのサイズ

    vertShader_ = 0;
	fragShader_ = 0;
	gl2Program_ = 0;
}


void GLUTShadow::createShadowMap(void)
{
	GLint viewport[4];       /* ビューポートの保存用　　　　 */
	GLdouble modelview[16];  /* モデルビュー変換行列の保存用 */
	GLdouble projection[16]; /* 透視変換行列の保存用　　　　 */
	klVec4 light_pos = glutWidget_->getGLUTView()->lights[0].pos;

	/*
	** 第１ステップ：デプステクスチャの作成
	*/

    /* フレームバッファオブジェクトへのレンダリング開始 */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_);

	/* デプスバッファをクリアする */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* 現在のビューポートを保存しておく */
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* ビューポートをテクスチャのサイズに設定する */
	glViewport(0, 0, texWidth_, texHeight_);

	/* 現在の透視変換行列を保存しておく */
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	/* 透視変換行列を単位行列に設定する */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle_, (GLfloat)texWidth_ / (GLfloat)texHeight_, nearDistance_, farDistance_ );
	gluLookAt( light_pos.x, light_pos.y, light_pos.z,    0, 0, 0,    0, 1, 0);

	/* 設定したモデルビュー変換行列を保存しておく */
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	/* デプスバッファの内容だけを取得するのでフレームバッファには書き込まない */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* したがって陰影付けも不要なのでライティングをオフにする */
	glDisable(GL_LIGHTING);

	/* デプスバッファには背面のポリゴンの奥行きを記録するようにする */
	glCullFace(GL_FRONT);

	/* ポリゴンオフセットの設定 モアレ(影の縞々)パターンが出てくるのを回避するため*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
   
	/* シーンを描画する */
    glutWidget_->paintGL(); 

    /* ポリゴンオフセットの設定解除 */
	glDisable(GL_POLYGON_OFFSET_FILL);

    /* フレームバッファオブジェクトへのレンダリング終了 */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* デプスバッファの内容をテクスチャメモリに転送する (FBOを使ってるのでコメントアウト。ここの処理が重い)*/
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth_, texHeight_);

	/* 通常の描画の設定に戻す */
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	/*
	** 第２ステップ：全体の描画
	*/

	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutWidget_->getGLUTView()->ApplyViewPoint();
	GLdouble model[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	klMat44 inv_mat( model );
	inv_mat = !inv_mat;
	
	///* 視点の位置を設定する（物体の方を奥に移動する）*/
	///glTranslated(0.0, 0.0, -10.0);

	///* トラックボール式の回転を与える */
	///glMultMatrixd(trackballRotation());


	/* 光源の位置を設定する */
	glutWidget_->getGLUTView()->ApplyLightsPos();

	if( isUseAlpha_ )
	{
		/* 光源の明るさを影の部分での明るさに設定 */
		glutWidget_->getGLUTView()->ApplyLightFv(true);

		/* シーンを描画する */
		glutWidget_->paintGL();
	}

	/*
	** 第３ステップ：日向の部分を描画する
	*/

	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);

	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	glMultMatrixd(modelview);

	/* 現在のモデルビュー変換の逆変換をかけておく */
	glMultMatrixd( inv_mat.m );

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);

    /* テクスチャオブジェクトを結合する */
    glBindTexture(GL_TEXTURE_2D, tex_);

	/* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	//glEnable(GL_TEXTURE_GEN_R);
	//glEnable(GL_TEXTURE_GEN_Q);

 	if( isUseAlpha_ )
	{		
		/* アルファテストを有効にして影の部分だけを描画する */
		glEnable(GL_ALPHA_TEST);

		/* 日向の部分がもとの図形に重ねて描かれるように奥行きの比較関数を変更する */
		glDepthFunc(GL_LEQUAL);
	}

	/* 光源の明るさを日向の部分での明るさに設定 */
	glutWidget_->getGLUTView()->ApplyLightFv(false);

	/* シーンを描画する */
	glutWidget_->paintGL();
	
	if( isUseAlpha_ )
	{
		/* 奥行きの比較関数を元に戻す */
		glDepthFunc(GL_LESS);

		/* アルファテストを無効にする */
		glDisable(GL_ALPHA_TEST);
	}

	/* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	//glDisable(GL_TEXTURE_GEN_R);
	//glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

    /* テクスチャオブジェクトの結合を解除する */
    glBindTexture(GL_TEXTURE_2D, 0);

}
