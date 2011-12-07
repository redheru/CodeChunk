//-----------------------------------------------------------------------------
//                             GLUTShadow_GLSL_VBO.h
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

#ifndef GLUT_SHADOW_HEADER__
#define GLUT_SHADOW_HEADER__

// 名前空間: NGLUT
// GLUTで描画するためのクラス群
namespace NGLUT
{

// 前方宣言(相互参照だから)
class GLUTWidget;

// 説明:
// GLUTで影を描画するクラス
class GLUTShadow
{

// 説明:
// 変数
private:
    bool m_isInitialized;

	GLint m_viewport[4];            /* ビューポートの保存用　　　　 */
	GLfloat m_modelview[16];        /* モデルビュー変換行列の保存用 */
	GLfloat m_projection[16];       /* 透視変換行列の保存用　　　　 */

	int m_textureWidth;      // デプスマップのテクスチャサイズ
	int m_textureHeight;      // デプスマップのテクスチャサイズ

// 説明:
// コンストラクタ，デストラクタ
public:
	// デフォルトコンストラクタ
    GLUTShadow(int textureWidth, int textureHeight);

// 説明:
// 関数
public:
	void Initialize(int textureWidth, int textureHeight);

	void createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back);
	void createDepthMapEnd();
	void renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat);
	void renderShadowEnd();
	void renderSunnyPlaceBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat);
	void renderSunnyPlaceEnd();

	//from
	//http://www.memorize-being.net/2008/08/16/calc-camera-matrix-at-opengl.htm
	Matrix4f getLookAtMat(Vector3f eyePos, Vector3f centerPos, Vector3f upDirection)
	{
		Vector3f x = eyePos - centerPos;
		x = x.normalized();
		
		Vector3f y = upDirection.cross(x);
		y = y.normalized();
		
		Vector3f z = x.cross(y);
		z = z.normalized();

		Matrix4f mat = Matrix4f::Identity();
		mat << y.x(), y.y(), y.z(), -(eyePos.dot(y)),
		       z.x(), z.y(), z.z(), -(eyePos.dot(z)),
		       x.x(), x.y(), x.z(), -(eyePos.dot(x)),
		       0.f,     0.f,   0.f,      1.f;
		return mat;
	}

// 説明:
// アクセッサ
public:
	bool getIsInitialized(){return m_isInitialized;}

// 説明:
// ミューテータ
public:

// 説明:
// 関数
private:
	void Clear();

}; // end class Shadow


// 説明:
// GLUTで影を描画するクラス(フレームバッファオブジェクト(VBO)とGLSLシェーダを利用)
class GLUTShadow_GLSL_VBO
{

// 説明:
// 変数
private:
    bool m_isInitialized;

    GLuint m_vertShader;          // バーテックスシェーダオブジェクト
    GLuint m_fragShader;          // フラグメントシェーダオブジェクト
    GLuint m_gl2Program;

    GLuint m_tex;                 // テクスチャオブジェクト
    GLuint m_fb;                  // フレームバッファオブジェクト

	GLint m_viewport[4];            /* ビューポートの保存用　　　　 */
	GLfloat m_modelview[16];        /* モデルビュー変換行列の保存用 */
	GLfloat m_projection[16];       /* 透視変換行列の保存用　　　　 */

	int m_textureWidth;      // デプスマップのテクスチャサイズ
	int m_textureHeight;      // デプスマップのテクスチャサイズ

// 説明:
// コンストラクタ，デストラクタ
public:
	// デフォルトコンストラクタ
    GLUTShadow_GLSL_VBO(int textureWidth, int textureHeight);

// 説明:
// 関数
public:
	void Initialize(int textureWidth, int textureHeight);

	void createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back);
	void createDepthMapEnd();
	void renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat);
	void renderShadowEnd();

	bool checkFBO();

	//from
	//http://www.memorize-being.net/2008/08/16/calc-camera-matrix-at-opengl.htm
	Matrix4f getLookAtMat(Vector3f eyePos, Vector3f centerPos, Vector3f upDirection)
	{
		Vector3f x = eyePos - centerPos;
		x = x.normalized();
		
		Vector3f y = upDirection.cross(x);
		y = y.normalized();
		
		Vector3f z = x.cross(y);
		z = z.normalized();

		Matrix4f mat = Matrix4f::Identity();
		mat << y.x(), y.y(), y.z(), -(eyePos.dot(y)),
		       z.x(), z.y(), z.z(), -(eyePos.dot(z)),
		       x.x(), x.y(), x.z(), -(eyePos.dot(x)),
		       0.f,     0.f,   0.f,      1.f;
		return mat;
	}

// 説明:
// アクセッサ
public:
	bool getIsInitialized(){return m_isInitialized;}

// 説明:
// ミューテータ
public:

// 説明:
// 関数
private:
	void Clear();

}; // end class Shadow

} // end namespace NGLUT

#endif // GLUT_SHADOW_HEADER__