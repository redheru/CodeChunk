//-----------------------------------------------------------------------------
//                             GLUTShadow.cpp
//
// GLUTで影を描画するウィジェットクラス
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/14 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#include "GLUTWidget.h"
#include "GLUTShadow.h"

#include "glsl.h"

#include <iostream>
using namespace std;
using namespace NGLUT;

//-----------------------------------------------------------------------------
// 説明: コンストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
GLUTShadow::GLUTShadow(int textureWidth, int textureHeight)
{
	Clear();
	Initialize(textureWidth, textureHeight);
}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::Initialize(int textureWidth, int textureHeight)
{
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;
	
	/* テクスチャの割り当て */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight, 0,
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

	/* 比較の結果をアルファ値として得る */
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

	/* アルファテストの比較関数（しきい値） */
	glAlphaFunc(GL_GEQUAL, 0.5f);

	/* テクスチャ座標に視点座標系における物体の座標値を用いる */
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	/* 生成したテクスチャ座標をそのまま (S, T, R, Q) に使う */
	static const GLdouble genfunc[][4] = {
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};
	glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
	glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
	glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
	glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

	/* 初期設定 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// 初期化済みにする
	m_isInitialized = true;

}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::Clear()
{
	m_isInitialized = false;

	m_textureWidth = 0;
	m_textureHeight = 0;
}

//-----------------------------------------------------------------------------
// 説明: デプスマップの作成(光源位置からのシーンの描画前)
// 引数: 
//       shadowLightPos [in] 光源位置         
//       angle [in] 視角度         
//       textureWidth [in] デプスマップ横幅
//       textureHeight [in] デプスマップ縦幅
//       front [in] 前面の位置
//       back [in] 後面の位置
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back)
{
	Vector4f light_pos = *shadowLightPos;

	/*
	** 第１ステップ：デプステクスチャの作成
	*/

	/* デプスバッファをクリアする */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* 現在のビューポートを保存しておく */
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	/* ビューポートをテクスチャのサイズに設定する */
	glViewport(0, 0, m_textureWidth, m_textureHeight);

	/* 現在の透視変換行列を保存しておく */
	glGetFloatv(GL_PROJECTION_MATRIX, m_projection);

	/* 透視変換行列を単位行列に設定する */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle, static_cast<GLfloat>(m_textureWidth) / static_cast<GLfloat>(m_textureHeight), front, back );
	gluLookAt( light_pos.x(), light_pos.y(), light_pos.z(),    0, 0, 0,    0, 1, 0);

	/* 設定したモデルビュー変換行列を保存しておく */
	glGetFloatv(GL_MODELVIEW_MATRIX, m_modelview);

	/* デプスバッファの内容だけを取得するのでフレームバッファには書き込まない */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* したがって陰影付けも不要なのでライティングをオフにする */
	glDisable(GL_LIGHTING);

	/* デプスバッファには背面のポリゴンの奥行きを記録するようにする */
	glCullFace(GL_FRONT);

	/* ポリゴンオフセットの設定 モアレ(影の縞々)パターンが出てくるのを回避するため*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
}

//-----------------------------------------------------------------------------
// 説明: デプスマップの作成(光源位置からのシーン描画後)
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::createDepthMapEnd()
{
    /* ポリゴンオフセットの設定解除 */
	glDisable(GL_POLYGON_OFFSET_FILL);

	/* デプスバッファの内容をテクスチャメモリに転送する (FBOを使ってる場合はコメントアウト。ここの処理が重い)*/
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_textureWidth, m_textureHeight);

	/* 通常の描画の設定に戻す */
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出す前に呼ぶ)
// 引数: 
//       lightName [in] 光源の名前
//       lightDif [in] 光源のディフューズ
//       lightSpc [in] 光源のスペキュラ
//       lookAtMat [in] カメラ姿勢への変換行列
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((*lookAtMat).data());
	//m_glutWidget->getGLUTView()->ApplyViewPoint();
	//GLfloat model[16];

	/* 光源の位置を設定する */
	//glutWidget_->getGLUTView()->ApplyLightsPos();

	/* 光源の明るさを影の部分での明るさに設定 */
	glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
	glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );
}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出した後に呼ぶ)
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderShadowEnd()
{

}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出す前に呼ぶ)
// 引数: 
//       lightName [in] 光源の名前
//       lightDif [in] 光源のディフューズ
//       lightSpc [in] 光源のスペキュラ
//       lookAtMat [in] カメラ姿勢への変換行列
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderSunnyPlaceBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);

	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	glMultMatrixf(m_modelview);

	/* 現在のモデルビュー変換の逆変換をかけておく */
	Matrix4f mat = (*lookAtMat);
	Matrix4f inv_mat = mat;
	//glGetFloatv(GL_MODELVIEW_MATRIX, mat.data());
	inv_mat = mat.inverse();
	glMultMatrixf( inv_mat.data() );

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);

	/* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	/* アルファテストを有効にして影の部分だけを描画する */
	glEnable(GL_ALPHA_TEST);

	/* 日向の部分がもとの図形に重ねて描かれるように奥行きの比較関数を変更する */
	glDepthFunc(GL_LEQUAL);

	/* 光源の明るさを日向の部分での明るさに設定 */
    glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
    glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );

}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出した後に呼ぶ)
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderSunnyPlaceEnd()
{
	/* 奥行きの比較関数を元に戻す */
	glDepthFunc(GL_LESS);

	/* アルファテストを無効にする */
	glDisable(GL_ALPHA_TEST);

	/* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

}


///////////////////////////////////////////////////////////////////////////////
////////////////////////ここから別クラス///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 説明: コンストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
GLUTShadow_GLSL_VBO::GLUTShadow_GLSL_VBO(int textureWidth, int textureHeight)
{
	Clear();
	Initialize(textureWidth, textureHeight);
}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::Initialize(int textureWidth, int textureHeight)
{
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

    /* シェーダプログラムのコンパイル／リンク結果を得る変数 */
    GLint compiled, linked;

    /* テクスチャオブジェクトを生成して結合する */
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

	/* テクスチャの割り当て */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight, 0,
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

	//if( m_isUseAlpha )
	//{
	//	/* 比較の結果をアルファ値として得る */
	//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

	//	/* アルファテストの比較関数（しきい値） */
	//	glAlphaFunc(GL_GEQUAL, 0.5f);
	//}
	//else 
	//{
		/* 比較の結果を輝度値として得る */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//}

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
    glGenFramebuffersEXT(1, &m_fb);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fb);

    /* フレームバッファオブジェクトにデプスバッファ用のテクスチャを結合する */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_TEXTURE_2D, m_tex, 0);

    /* フレームバッファオブジェクトのチェック */
	//if(!checkFBO())
	//	return;

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
	m_vertShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	  
	/* シェーダのソースプログラムの読み込み */
    if (readShaderSource(m_vertShader, "shadow.vert")) exit(1);
    if (readShaderSource(m_fragShader, "shadow.frag")) exit(1);

	/* バーテックスシェーダのソースプログラムのコンパイル */
	glCompileShader(m_vertShader);
	glGetShaderiv(m_vertShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(m_vertShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in vertex shader.\n");
		exit(1);
	}
	  
	/* フラグメントシェーダのソースプログラムのコンパイル */
	glCompileShader(m_fragShader);
	glGetShaderiv(m_fragShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(m_fragShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in fragment shader.\n");
		exit(1);
	}
	  
	/* プログラムオブジェクトの作成 */
	m_gl2Program = glCreateProgram();
	  
	/* シェーダオブジェクトのシェーダプログラムへの登録 */
	glAttachShader(m_gl2Program, m_vertShader);
	glAttachShader(m_gl2Program, m_fragShader);
	  
	/* シェーダオブジェクトの削除 */
	glDeleteShader(m_vertShader);
	glDeleteShader(m_fragShader);
	  
	/* シェーダプログラムのリンク */
	glLinkProgram(m_gl2Program);
	glGetProgramiv(m_gl2Program, GL_LINK_STATUS, &linked);
	printProgramInfoLog(m_gl2Program);
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error.\n");
		exit(1);
	}
	  
	/* シェーダプログラムの適用 */
	glUseProgram(m_gl2Program);

	/* テクスチャユニット０を指定する */
	glUniform1i(glGetUniformLocation(m_gl2Program, "texture"), 0);

	// 初期化済みにする
	m_isInitialized = true;
}

//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::Clear()
{
	//m_isLoaded = false;
	//m_isInitialized = false;
 //   m_isEnabled = false;
 //   m_isUseAlpha = false;

 //   m_angle = 0.f;
	//m_nearDistance = 0.f;
	//m_farDistance = 0.f;
	//m_glutWidget = NULL;
	
	m_textureWidth = 0;       // シャドウマップのサイズ
    m_textureHeight = 0;      // シャドウマップのサイズ

    m_vertShader = 0;
	m_fragShader = 0;
	m_gl2Program = 0;
}

//-----------------------------------------------------------------------------
// 説明: デプスマップの作成(光源位置からのシーンの描画前)
// 引数: 
//       shadowLightPos [in] 光源位置         
//       angle [in] 視角度         
//       textureWidth [in] デプスマップ横幅
//       textureHeight [in] デプスマップ縦幅
//       front [in] 前面の位置
//       back [in] 後面の位置
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back)
{
	Vector4f light_pos = *shadowLightPos;

	/*
	** 第１ステップ：デプステクスチャの作成
	*/

    /* フレームバッファオブジェクトへのレンダリング開始 */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fb);

	/* デプスバッファをクリアする */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* 現在のビューポートを保存しておく */
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	/* ビューポートをテクスチャのサイズに設定する */
	glViewport(0, 0, m_textureWidth, m_textureHeight);

	/* 現在の透視変換行列を保存しておく */
	glGetFloatv(GL_PROJECTION_MATRIX, m_projection);

	/* 透視変換行列を単位行列に設定する */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle, static_cast<GLfloat>(m_textureWidth) / static_cast<GLfloat>(m_textureHeight), front, back );
	gluLookAt( light_pos.x(), light_pos.y(), light_pos.z(),    0, 0, 0,    0, 1, 0);

	/* 設定したモデルビュー変換行列を保存しておく */
	glGetFloatv(GL_MODELVIEW_MATRIX, m_modelview);

	/* デプスバッファの内容だけを取得するのでフレームバッファには書き込まない */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* したがって陰影付けも不要なのでライティングをオフにする */
	glDisable(GL_LIGHTING);

	/* デプスバッファには背面のポリゴンの奥行きを記録するようにする */
	glCullFace(GL_FRONT);

	/* ポリゴンオフセットの設定 モアレ(影の縞々)パターンが出てくるのを回避するため*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
}

//-----------------------------------------------------------------------------
// 説明: デプスマップの作成(光源位置からのシーン描画後)
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::createDepthMapEnd(void)
{
    /* ポリゴンオフセットの設定解除 */
	glDisable(GL_POLYGON_OFFSET_FILL);

    /* フレームバッファオブジェクトへのレンダリング終了 */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* デプスバッファの内容をテクスチャメモリに転送する (FBOを使ってるのでコメントアウト。ここの処理が重い)*/
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth_, texHeight_);

	/* 通常の描画の設定に戻す */
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出す前に呼ぶ)
// 引数: 
//       lightName [in] 光源の名前
//       lightDif [in] 光源のディフューズ
//       lightSpc [in] 光源のスペキュラ
//       lookAtMat [in] カメラ姿勢への変換行列
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((*lookAtMat).data());
	//m_glutWidget->getGLUTView()->ApplyViewPoint();
	//GLfloat model[16];
	Matrix4f mat = Matrix4f::Identity();
	Matrix4f inv_mat = mat;
	glGetFloatv(GL_MODELVIEW_MATRIX, mat.data());
	//inv_mat.data() = model;
	inv_mat = mat.inverse();
	//inv_mat = !inv_mat;

	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);

	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	glMultMatrixf(m_modelview);

	/* 現在のモデルビュー変換の逆変換をかけておく */
	glMultMatrixf( inv_mat.data() );

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);

    /* テクスチャオブジェクトを結合する */
    glBindTexture(GL_TEXTURE_2D, m_tex);

	/* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	//glEnable(GL_TEXTURE_GEN_R);
	//glEnable(GL_TEXTURE_GEN_Q);

	/* 光源の明るさを日向の部分での明るさに設定 */
    glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
    glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );
	//m_glutWidget->getGLUTView()->ApplyLightFv(false);

}

//-----------------------------------------------------------------------------
// 説明: シャドウのレンダリング(ポリゴンなどの描画関数を呼び出した後に呼ぶ)
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::renderShadowEnd()
{
	/* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	//glDisable(GL_TEXTURE_GEN_R);
	//glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

	/* テクスチャオブジェクトの結合を解除する */
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
// 説明: フレームバッファオブジェクトの作成チェック
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
bool GLUTShadow_GLSL_VBO::checkFBO()
{
	// check FBO status
    GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER );
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
            std::cout << "Framebuffer complete." << std::endl;
            return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
            return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
            return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
            return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
            return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
            return false;

    default:
            std::cout << "[ERROR] Unknow error." << std::endl;
            return false;
    }
}