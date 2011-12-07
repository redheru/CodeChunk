//-----------------------------------------------------------------------------
//                             GLUTShadow_GLSL_VBO.h
//
// GLUT�ŉe��`�悷��E�B�W�F�b�g�N���X
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

// ���O���: NGLUT
// GLUT�ŕ`�悷�邽�߂̃N���X�Q
namespace NGLUT
{

// �O���錾(���ݎQ�Ƃ�����)
class GLUTWidget;

// ����:
// GLUT�ŉe��`�悷��N���X
class GLUTShadow
{

// ����:
// �ϐ�
private:
    bool m_isInitialized;

	GLint m_viewport[4];            /* �r���[�|�[�g�̕ۑ��p�@�@�@�@ */
	GLfloat m_modelview[16];        /* ���f���r���[�ϊ��s��̕ۑ��p */
	GLfloat m_projection[16];       /* �����ϊ��s��̕ۑ��p�@�@�@�@ */

	int m_textureWidth;      // �f�v�X�}�b�v�̃e�N�X�`���T�C�Y
	int m_textureHeight;      // �f�v�X�}�b�v�̃e�N�X�`���T�C�Y

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
    GLUTShadow(int textureWidth, int textureHeight);

// ����:
// �֐�
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

// ����:
// �A�N�Z�b�T
public:
	bool getIsInitialized(){return m_isInitialized;}

// ����:
// �~���[�e�[�^
public:

// ����:
// �֐�
private:
	void Clear();

}; // end class Shadow


// ����:
// GLUT�ŉe��`�悷��N���X(�t���[���o�b�t�@�I�u�W�F�N�g(VBO)��GLSL�V�F�[�_�𗘗p)
class GLUTShadow_GLSL_VBO
{

// ����:
// �ϐ�
private:
    bool m_isInitialized;

    GLuint m_vertShader;          // �o�[�e�b�N�X�V�F�[�_�I�u�W�F�N�g
    GLuint m_fragShader;          // �t���O�����g�V�F�[�_�I�u�W�F�N�g
    GLuint m_gl2Program;

    GLuint m_tex;                 // �e�N�X�`���I�u�W�F�N�g
    GLuint m_fb;                  // �t���[���o�b�t�@�I�u�W�F�N�g

	GLint m_viewport[4];            /* �r���[�|�[�g�̕ۑ��p�@�@�@�@ */
	GLfloat m_modelview[16];        /* ���f���r���[�ϊ��s��̕ۑ��p */
	GLfloat m_projection[16];       /* �����ϊ��s��̕ۑ��p�@�@�@�@ */

	int m_textureWidth;      // �f�v�X�}�b�v�̃e�N�X�`���T�C�Y
	int m_textureHeight;      // �f�v�X�}�b�v�̃e�N�X�`���T�C�Y

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
    GLUTShadow_GLSL_VBO(int textureWidth, int textureHeight);

// ����:
// �֐�
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

// ����:
// �A�N�Z�b�T
public:
	bool getIsInitialized(){return m_isInitialized;}

// ����:
// �~���[�e�[�^
public:

// ����:
// �֐�
private:
	void Clear();

}; // end class Shadow

} // end namespace NGLUT

#endif // GLUT_SHADOW_HEADER__