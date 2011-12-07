//-----------------------------------------------------------------------------
//                             GLUTShadow.cpp
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

#include "GLUTWidget.h"
#include "GLUTShadow.h"

#include "glsl.h"

#include <iostream>
using namespace std;
using namespace NGLUT;

//-----------------------------------------------------------------------------
// ����: �R���X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
GLUTShadow::GLUTShadow(int textureWidth, int textureHeight)
{
	Clear();
	Initialize(textureWidth, textureHeight);
}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::Initialize(int textureWidth, int textureHeight)
{
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;
	
	/* �e�N�X�`���̊��蓖�� */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight, 0,
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* �e�N�X�`�����g��E�k��������@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* �e�N�X�`���̌J��Ԃ����@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* �������ރ|���S���̃e�N�X�`�����W�l�̂q�ƃe�N�X�`���Ƃ̔�r���s���悤�ɂ��� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	/* �����q�̒l���e�N�X�`���̒l�ȉ��Ȃ�^�i�܂�����j */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	/* ��r�̌��ʂ��A���t�@�l�Ƃ��ē��� */
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

	/* �A���t�@�e�X�g�̔�r�֐��i�������l�j */
	glAlphaFunc(GL_GEQUAL, 0.5f);

	/* �e�N�X�`�����W�Ɏ��_���W�n�ɂ����镨�̂̍��W�l��p���� */
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	/* ���������e�N�X�`�����W�����̂܂� (S, T, R, Q) �Ɏg�� */
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

	/* �����ݒ� */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// �������ς݂ɂ���
	m_isInitialized = true;

}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::Clear()
{
	m_isInitialized = false;

	m_textureWidth = 0;
	m_textureHeight = 0;
}

//-----------------------------------------------------------------------------
// ����: �f�v�X�}�b�v�̍쐬(�����ʒu����̃V�[���̕`��O)
// ����: 
//       shadowLightPos [in] �����ʒu         
//       angle [in] ���p�x         
//       textureWidth [in] �f�v�X�}�b�v����
//       textureHeight [in] �f�v�X�}�b�v�c��
//       front [in] �O�ʂ̈ʒu
//       back [in] ��ʂ̈ʒu
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back)
{
	Vector4f light_pos = *shadowLightPos;

	/*
	** ��P�X�e�b�v�F�f�v�X�e�N�X�`���̍쐬
	*/

	/* �f�v�X�o�b�t�@���N���A���� */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* ���݂̃r���[�|�[�g��ۑ����Ă��� */
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	/* �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� */
	glViewport(0, 0, m_textureWidth, m_textureHeight);

	/* ���݂̓����ϊ��s���ۑ����Ă��� */
	glGetFloatv(GL_PROJECTION_MATRIX, m_projection);

	/* �����ϊ��s���P�ʍs��ɐݒ肷�� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* �����ʒu�����_�Ƃ��V�[��������Ɏ��܂�悤���f���r���[�ϊ��s���ݒ肷�� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle, static_cast<GLfloat>(m_textureWidth) / static_cast<GLfloat>(m_textureHeight), front, back );
	gluLookAt( light_pos.x(), light_pos.y(), light_pos.z(),    0, 0, 0,    0, 1, 0);

	/* �ݒ肵�����f���r���[�ϊ��s���ۑ����Ă��� */
	glGetFloatv(GL_MODELVIEW_MATRIX, m_modelview);

	/* �f�v�X�o�b�t�@�̓��e�������擾����̂Ńt���[���o�b�t�@�ɂ͏������܂Ȃ� */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* ���������ĉA�e�t�����s�v�Ȃ̂Ń��C�e�B���O���I�t�ɂ��� */
	glDisable(GL_LIGHTING);

	/* �f�v�X�o�b�t�@�ɂ͔w�ʂ̃|���S���̉��s�����L�^����悤�ɂ��� */
	glCullFace(GL_FRONT);

	/* �|���S���I�t�Z�b�g�̐ݒ� ���A��(�e�̎ȁX)�p�^�[�����o�Ă���̂�������邽��*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
}

//-----------------------------------------------------------------------------
// ����: �f�v�X�}�b�v�̍쐬(�����ʒu����̃V�[���`���)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::createDepthMapEnd()
{
    /* �|���S���I�t�Z�b�g�̐ݒ���� */
	glDisable(GL_POLYGON_OFFSET_FILL);

	/* �f�v�X�o�b�t�@�̓��e���e�N�X�`���������ɓ]������ (FBO���g���Ă�ꍇ�̓R�����g�A�E�g�B�����̏������d��)*/
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_textureWidth, m_textureHeight);

	/* �ʏ�̕`��̐ݒ�ɖ߂� */
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo���O�ɌĂ�)
// ����: 
//       lightName [in] �����̖��O
//       lightDif [in] �����̃f�B�t���[�Y
//       lightSpc [in] �����̃X�y�L����
//       lookAtMat [in] �J�����p���ւ̕ϊ��s��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* �t���[���o�b�t�@�ƃf�v�X�o�b�t�@���N���A���� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((*lookAtMat).data());
	//m_glutWidget->getGLUTView()->ApplyViewPoint();
	//GLfloat model[16];

	/* �����̈ʒu��ݒ肷�� */
	//glutWidget_->getGLUTView()->ApplyLightsPos();

	/* �����̖��邳���e�̕����ł̖��邳�ɐݒ� */
	glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
	glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );
}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo������ɌĂ�)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderShadowEnd()
{

}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo���O�ɌĂ�)
// ����: 
//       lightName [in] �����̖��O
//       lightDif [in] �����̃f�B�t���[�Y
//       lightSpc [in] �����̃X�y�L����
//       lookAtMat [in] �J�����p���ւ̕ϊ��s��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderSunnyPlaceBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* �e�N�X�`���ϊ��s���ݒ肷�� */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* �e�N�X�`�����W�� [-1,1] �͈̔͂� [0,1] �͈̔͂Ɏ��߂� */
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);

	/* �e�N�X�`���̃��f���r���[�ϊ��s��Ɠ����ϊ��s��̐ς������� */
	glMultMatrixf(m_modelview);

	/* ���݂̃��f���r���[�ϊ��̋t�ϊ��������Ă��� */
	Matrix4f mat = (*lookAtMat);
	Matrix4f inv_mat = mat;
	//glGetFloatv(GL_MODELVIEW_MATRIX, mat.data());
	inv_mat = mat.inverse();
	glMultMatrixf( inv_mat.data() );

	/* ���f���r���[�ϊ��s��ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	/* �A���t�@�e�X�g��L���ɂ��ĉe�̕���������`�悷�� */
	glEnable(GL_ALPHA_TEST);

	/* �����̕��������Ƃ̐}�`�ɏd�˂ĕ`�����悤�ɉ��s���̔�r�֐���ύX���� */
	glDepthFunc(GL_LEQUAL);

	/* �����̖��邳������̕����ł̖��邳�ɐݒ� */
    glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
    glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );

}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo������ɌĂ�)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow::renderSunnyPlaceEnd()
{
	/* ���s���̔�r�֐������ɖ߂� */
	glDepthFunc(GL_LESS);

	/* �A���t�@�e�X�g�𖳌��ɂ��� */
	glDisable(GL_ALPHA_TEST);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎��������𖳌��ɂ��� */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

}


///////////////////////////////////////////////////////////////////////////////
////////////////////////��������ʃN���X///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ����: �R���X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
GLUTShadow_GLSL_VBO::GLUTShadow_GLSL_VBO(int textureWidth, int textureHeight)
{
	Clear();
	Initialize(textureWidth, textureHeight);
}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::Initialize(int textureWidth, int textureHeight)
{
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

    /* �V�F�[�_�v���O�����̃R���p�C���^�����N���ʂ𓾂�ϐ� */
    GLint compiled, linked;

    /* �e�N�X�`���I�u�W�F�N�g�𐶐����Č������� */
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

	/* �e�N�X�`���̊��蓖�� */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight, 0,
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* �e�N�X�`�����g��E�k��������@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* �e�N�X�`���̌J��Ԃ����@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* �������ރ|���S���̃e�N�X�`�����W�l�̂q�ƃe�N�X�`���Ƃ̔�r���s���悤�ɂ��� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	/* �����q�̒l���e�N�X�`���̒l�ȉ��Ȃ�^�i�܂�����j */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//if( m_isUseAlpha )
	//{
	//	/* ��r�̌��ʂ��A���t�@�l�Ƃ��ē��� */
	//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

	//	/* �A���t�@�e�X�g�̔�r�֐��i�������l�j */
	//	glAlphaFunc(GL_GEQUAL, 0.5f);
	//}
	//else 
	//{
		/* ��r�̌��ʂ��P�x�l�Ƃ��ē��� */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//}

	///* �e�N�X�`�����W�Ɏ��_���W�n�ɂ����镨�̂̍��W�l��p���� */
	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	///* ���������e�N�X�`�����W�����̂܂� (S, T, R, Q) �Ɏg�� */
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

    /* �e�N�X�`���I�u�W�F�N�g�̌������������� */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�𐶐����Č������� */
    glGenFramebuffersEXT(1, &m_fb);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fb);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�Ƀf�v�X�o�b�t�@�p�̃e�N�X�`������������ */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_TEXTURE_2D, m_tex, 0);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�̃`�F�b�N */
	//if(!checkFBO())
	//	return;

    /* �J���[�o�b�t�@�������̂œǂݏ������Ȃ� */
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //CHECK_FRAMEBUFFER_STATUS();

    /* �t���[���o�b�t�@�I�u�W�F�N�g�̌������������� */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* �����ݒ� */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* GLSL �̏����� */
	//if (glslInit()) exit(1);
	  
	/* �V�F�[�_�I�u�W�F�N�g�̍쐬 */
	m_vertShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	  
	/* �V�F�[�_�̃\�[�X�v���O�����̓ǂݍ��� */
    if (readShaderSource(m_vertShader, "shadow.vert")) exit(1);
    if (readShaderSource(m_fragShader, "shadow.frag")) exit(1);

	/* �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
	glCompileShader(m_vertShader);
	glGetShaderiv(m_vertShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(m_vertShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in vertex shader.\n");
		exit(1);
	}
	  
	/* �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
	glCompileShader(m_fragShader);
	glGetShaderiv(m_fragShader, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(m_fragShader);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in fragment shader.\n");
		exit(1);
	}
	  
	/* �v���O�����I�u�W�F�N�g�̍쐬 */
	m_gl2Program = glCreateProgram();
	  
	/* �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^ */
	glAttachShader(m_gl2Program, m_vertShader);
	glAttachShader(m_gl2Program, m_fragShader);
	  
	/* �V�F�[�_�I�u�W�F�N�g�̍폜 */
	glDeleteShader(m_vertShader);
	glDeleteShader(m_fragShader);
	  
	/* �V�F�[�_�v���O�����̃����N */
	glLinkProgram(m_gl2Program);
	glGetProgramiv(m_gl2Program, GL_LINK_STATUS, &linked);
	printProgramInfoLog(m_gl2Program);
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error.\n");
		exit(1);
	}
	  
	/* �V�F�[�_�v���O�����̓K�p */
	glUseProgram(m_gl2Program);

	/* �e�N�X�`�����j�b�g�O���w�肷�� */
	glUniform1i(glGetUniformLocation(m_gl2Program, "texture"), 0);

	// �������ς݂ɂ���
	m_isInitialized = true;
}

//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
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
	
	m_textureWidth = 0;       // �V���h�E�}�b�v�̃T�C�Y
    m_textureHeight = 0;      // �V���h�E�}�b�v�̃T�C�Y

    m_vertShader = 0;
	m_fragShader = 0;
	m_gl2Program = 0;
}

//-----------------------------------------------------------------------------
// ����: �f�v�X�}�b�v�̍쐬(�����ʒu����̃V�[���̕`��O)
// ����: 
//       shadowLightPos [in] �����ʒu         
//       angle [in] ���p�x         
//       textureWidth [in] �f�v�X�}�b�v����
//       textureHeight [in] �f�v�X�}�b�v�c��
//       front [in] �O�ʂ̈ʒu
//       back [in] ��ʂ̈ʒu
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::createDepthMapBegin(const Vector4f* shadowLightPos, float angle, float front, float back)
{
	Vector4f light_pos = *shadowLightPos;

	/*
	** ��P�X�e�b�v�F�f�v�X�e�N�X�`���̍쐬
	*/

    /* �t���[���o�b�t�@�I�u�W�F�N�g�ւ̃����_�����O�J�n */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fb);

	/* �f�v�X�o�b�t�@���N���A���� */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* ���݂̃r���[�|�[�g��ۑ����Ă��� */
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	/* �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� */
	glViewport(0, 0, m_textureWidth, m_textureHeight);

	/* ���݂̓����ϊ��s���ۑ����Ă��� */
	glGetFloatv(GL_PROJECTION_MATRIX, m_projection);

	/* �����ϊ��s���P�ʍs��ɐݒ肷�� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* �����ʒu�����_�Ƃ��V�[��������Ɏ��܂�悤���f���r���[�ϊ��s���ݒ肷�� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle, static_cast<GLfloat>(m_textureWidth) / static_cast<GLfloat>(m_textureHeight), front, back );
	gluLookAt( light_pos.x(), light_pos.y(), light_pos.z(),    0, 0, 0,    0, 1, 0);

	/* �ݒ肵�����f���r���[�ϊ��s���ۑ����Ă��� */
	glGetFloatv(GL_MODELVIEW_MATRIX, m_modelview);

	/* �f�v�X�o�b�t�@�̓��e�������擾����̂Ńt���[���o�b�t�@�ɂ͏������܂Ȃ� */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* ���������ĉA�e�t�����s�v�Ȃ̂Ń��C�e�B���O���I�t�ɂ��� */
	glDisable(GL_LIGHTING);

	/* �f�v�X�o�b�t�@�ɂ͔w�ʂ̃|���S���̉��s�����L�^����悤�ɂ��� */
	glCullFace(GL_FRONT);

	/* �|���S���I�t�Z�b�g�̐ݒ� ���A��(�e�̎ȁX)�p�^�[�����o�Ă���̂�������邽��*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
}

//-----------------------------------------------------------------------------
// ����: �f�v�X�}�b�v�̍쐬(�����ʒu����̃V�[���`���)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::createDepthMapEnd(void)
{
    /* �|���S���I�t�Z�b�g�̐ݒ���� */
	glDisable(GL_POLYGON_OFFSET_FILL);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�ւ̃����_�����O�I�� */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* �f�v�X�o�b�t�@�̓��e���e�N�X�`���������ɓ]������ (FBO���g���Ă�̂ŃR�����g�A�E�g�B�����̏������d��)*/
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth_, texHeight_);

	/* �ʏ�̕`��̐ݒ�ɖ߂� */
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo���O�ɌĂ�)
// ����: 
//       lightName [in] �����̖��O
//       lightDif [in] �����̃f�B�t���[�Y
//       lightSpc [in] �����̃X�y�L����
//       lookAtMat [in] �J�����p���ւ̕ϊ��s��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::renderShadowBegin(GLenum lightName, const Vector4f* lightAmb, const Vector4f* lightDif, const Vector4f* lightSpc, const Matrix4f* lookAtMat)
{
	/* �t���[���o�b�t�@�ƃf�v�X�o�b�t�@���N���A���� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ���f���r���[�ϊ��s��̐ݒ� */
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

	/* �e�N�X�`���ϊ��s���ݒ肷�� */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* �e�N�X�`�����W�� [-1,1] �͈̔͂� [0,1] �͈̔͂Ɏ��߂� */
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);

	/* �e�N�X�`���̃��f���r���[�ϊ��s��Ɠ����ϊ��s��̐ς������� */
	glMultMatrixf(m_modelview);

	/* ���݂̃��f���r���[�ϊ��̋t�ϊ��������Ă��� */
	glMultMatrixf( inv_mat.data() );

	/* ���f���r���[�ϊ��s��ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

    /* �e�N�X�`���I�u�W�F�N�g���������� */
    glBindTexture(GL_TEXTURE_2D, m_tex);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	//glEnable(GL_TEXTURE_GEN_R);
	//glEnable(GL_TEXTURE_GEN_Q);

	/* �����̖��邳������̕����ł̖��邳�ɐݒ� */
    glLightfv(lightName, GL_AMBIENT, (*lightAmb).data() );
	glLightfv(lightName, GL_DIFFUSE, (*lightDif).data() );
    glLightfv(lightName, GL_SPECULAR, (*lightSpc).data() );
	//m_glutWidget->getGLUTView()->ApplyLightFv(false);

}

//-----------------------------------------------------------------------------
// ����: �V���h�E�̃����_�����O(�|���S���Ȃǂ̕`��֐����Ăяo������ɌĂ�)
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void GLUTShadow_GLSL_VBO::renderShadowEnd()
{
	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎��������𖳌��ɂ��� */
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	//glDisable(GL_TEXTURE_GEN_R);
	//glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

	/* �e�N�X�`���I�u�W�F�N�g�̌������������� */
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
// ����: �t���[���o�b�t�@�I�u�W�F�N�g�̍쐬�`�F�b�N
// ����: 
// �Ԃ�l:
// ���̑�: 
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