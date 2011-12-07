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
	// ���[�h�ς݂�
	if( !isLoaded_ )
	{
		printf( "[Shadow]Light position and scene function are not loaded!\n" );
		printf( "[Shadow]Please call Load() before using this function!\n" );
		return;
	}

	// ���������Ă邩
	if( !isInitialized_ )
	{
		printf( "[Shadow]Please call Initialize() before using this function!\n" );
		return;
	}

	// ���C�g���Z�b�g����Ă��邩
	if( glutWidget_->getGLUTView()->lights.size() < 1 )
	{
		printf( "[Shadow]Please add a LightGL to ViewGL before using this function!\n" );
		return;
	}

    // FBO���g���Ă�̂Ńe�N�X�`���T�C�Y�����킹��K�v�͂Ȃ�
	// reshape function�̏C��
	//glutWidget_->getGLUTView()->widthMin = texWidth_; 
	//glutWidget_->getGLUTView()->heightMin = texHeight_; 

	// �e�t���̃����_�����O
	createShadowMap();

}

void GLUTShadow::Initialize()
{
    /* �V�F�[�_�v���O�����̃R���p�C���^�����N���ʂ𓾂�ϐ� */
    GLint compiled, linked;

    /* �e�N�X�`���I�u�W�F�N�g�𐶐����Č������� */
    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);

	/* �e�N�X�`���̊��蓖�� */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth_, texHeight_, 0,
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

	if( isUseAlpha_ )
	{
		/* ��r�̌��ʂ��A���t�@�l�Ƃ��ē��� */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

		/* �A���t�@�e�X�g�̔�r�֐��i�������l�j */
		glAlphaFunc(GL_GEQUAL, 0.5f);
	}
	else 
	{
		/* ��r�̌��ʂ��P�x�l�Ƃ��ē��� */
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	}

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
    glGenFramebuffersEXT(1, &fb_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�Ƀf�v�X�o�b�t�@�p�̃e�N�X�`������������ */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_TEXTURE_2D, tex_, 0);

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
	vertShader_ = glCreateShader(GL_VERTEX_SHADER);
	fragShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	  
	/* �V�F�[�_�̃\�[�X�v���O�����̓ǂݍ��� */
    if (readShaderSource(vertShader_, "shadow.vert")) exit(1);
    if (readShaderSource(fragShader_, "shadow.frag")) exit(1);

	/* �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
	glCompileShader(vertShader_);
	glGetShaderiv(vertShader_, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(vertShader_);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in vertex shader.\n");
		exit(1);
	}
	  
	/* �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
	glCompileShader(fragShader_);
	glGetShaderiv(fragShader_, GL_COMPILE_STATUS, &compiled);
	printShaderInfoLog(fragShader_);
	if (compiled == GL_FALSE) {
		fprintf(stderr, "Compile error in fragment shader.\n");
		exit(1);
	}
	  
	/* �v���O�����I�u�W�F�N�g�̍쐬 */
	gl2Program_ = glCreateProgram();
	  
	/* �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^ */
	glAttachShader(gl2Program_, vertShader_);
	glAttachShader(gl2Program_, fragShader_);
	  
	/* �V�F�[�_�I�u�W�F�N�g�̍폜 */
	glDeleteShader(vertShader_);
	glDeleteShader(fragShader_);
	  
	/* �V�F�[�_�v���O�����̃����N */
	glLinkProgram(gl2Program_);
	glGetProgramiv(gl2Program_, GL_LINK_STATUS, &linked);
	printProgramInfoLog(gl2Program_);
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error.\n");
		exit(1);
	}
	  
	/* �V�F�[�_�v���O�����̓K�p */
	glUseProgram(gl2Program_);

	/* �e�N�X�`�����j�b�g�O���w�肷�� */
	glUniform1i(glGetUniformLocation(gl2Program_, "texture"), 0);

	// �������ς݂ɂ���
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
	texWidth_ = 1024;       // �V���h�E�}�b�v�̃T�C�Y
    texHeight_ = 1024;      // �V���h�E�}�b�v�̃T�C�Y

    vertShader_ = 0;
	fragShader_ = 0;
	gl2Program_ = 0;
}


void GLUTShadow::createShadowMap(void)
{
	GLint viewport[4];       /* �r���[�|�[�g�̕ۑ��p�@�@�@�@ */
	GLdouble modelview[16];  /* ���f���r���[�ϊ��s��̕ۑ��p */
	GLdouble projection[16]; /* �����ϊ��s��̕ۑ��p�@�@�@�@ */
	klVec4 light_pos = glutWidget_->getGLUTView()->lights[0].pos;

	/*
	** ��P�X�e�b�v�F�f�v�X�e�N�X�`���̍쐬
	*/

    /* �t���[���o�b�t�@�I�u�W�F�N�g�ւ̃����_�����O�J�n */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_);

	/* �f�v�X�o�b�t�@���N���A���� */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* ���݂̃r���[�|�[�g��ۑ����Ă��� */
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� */
	glViewport(0, 0, texWidth_, texHeight_);

	/* ���݂̓����ϊ��s���ۑ����Ă��� */
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	/* �����ϊ��s���P�ʍs��ɐݒ肷�� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* �����ʒu�����_�Ƃ��V�[��������Ɏ��܂�悤���f���r���[�ϊ��s���ݒ肷�� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective( angle_, (GLfloat)texWidth_ / (GLfloat)texHeight_, nearDistance_, farDistance_ );
	gluLookAt( light_pos.x, light_pos.y, light_pos.z,    0, 0, 0,    0, 1, 0);

	/* �ݒ肵�����f���r���[�ϊ��s���ۑ����Ă��� */
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	/* �f�v�X�o�b�t�@�̓��e�������擾����̂Ńt���[���o�b�t�@�ɂ͏������܂Ȃ� */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* ���������ĉA�e�t�����s�v�Ȃ̂Ń��C�e�B���O���I�t�ɂ��� */
	glDisable(GL_LIGHTING);

	/* �f�v�X�o�b�t�@�ɂ͔w�ʂ̃|���S���̉��s�����L�^����悤�ɂ��� */
	glCullFace(GL_FRONT);

	/* �|���S���I�t�Z�b�g�̐ݒ� ���A��(�e�̎ȁX)�p�^�[�����o�Ă���̂�������邽��*/
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1, 4.0);
   
	/* �V�[����`�悷�� */
    glutWidget_->paintGL(); 

    /* �|���S���I�t�Z�b�g�̐ݒ���� */
	glDisable(GL_POLYGON_OFFSET_FILL);

    /* �t���[���o�b�t�@�I�u�W�F�N�g�ւ̃����_�����O�I�� */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* �f�v�X�o�b�t�@�̓��e���e�N�X�`���������ɓ]������ (FBO���g���Ă�̂ŃR�����g�A�E�g�B�����̏������d��)*/
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth_, texHeight_);

	/* �ʏ�̕`��̐ݒ�ɖ߂� */
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projection);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	/*
	** ��Q�X�e�b�v�F�S�̂̕`��
	*/

	/* �t���[���o�b�t�@�ƃf�v�X�o�b�t�@���N���A���� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutWidget_->getGLUTView()->ApplyViewPoint();
	GLdouble model[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	klMat44 inv_mat( model );
	inv_mat = !inv_mat;
	
	///* ���_�̈ʒu��ݒ肷��i���̂̕������Ɉړ�����j*/
	///glTranslated(0.0, 0.0, -10.0);

	///* �g���b�N�{�[�����̉�]��^���� */
	///glMultMatrixd(trackballRotation());


	/* �����̈ʒu��ݒ肷�� */
	glutWidget_->getGLUTView()->ApplyLightsPos();

	if( isUseAlpha_ )
	{
		/* �����̖��邳���e�̕����ł̖��邳�ɐݒ� */
		glutWidget_->getGLUTView()->ApplyLightFv(true);

		/* �V�[����`�悷�� */
		glutWidget_->paintGL();
	}

	/*
	** ��R�X�e�b�v�F�����̕�����`�悷��
	*/

	/* �e�N�X�`���ϊ��s���ݒ肷�� */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* �e�N�X�`�����W�� [-1,1] �͈̔͂� [0,1] �͈̔͂Ɏ��߂� */
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);

	/* �e�N�X�`���̃��f���r���[�ϊ��s��Ɠ����ϊ��s��̐ς������� */
	glMultMatrixd(modelview);

	/* ���݂̃��f���r���[�ϊ��̋t�ϊ��������Ă��� */
	glMultMatrixd( inv_mat.m );

	/* ���f���r���[�ϊ��s��ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

    /* �e�N�X�`���I�u�W�F�N�g���������� */
    glBindTexture(GL_TEXTURE_2D, tex_);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	//glEnable(GL_TEXTURE_GEN_R);
	//glEnable(GL_TEXTURE_GEN_Q);

 	if( isUseAlpha_ )
	{		
		/* �A���t�@�e�X�g��L���ɂ��ĉe�̕���������`�悷�� */
		glEnable(GL_ALPHA_TEST);

		/* �����̕��������Ƃ̐}�`�ɏd�˂ĕ`�����悤�ɉ��s���̔�r�֐���ύX���� */
		glDepthFunc(GL_LEQUAL);
	}

	/* �����̖��邳������̕����ł̖��邳�ɐݒ� */
	glutWidget_->getGLUTView()->ApplyLightFv(false);

	/* �V�[����`�悷�� */
	glutWidget_->paintGL();
	
	if( isUseAlpha_ )
	{
		/* ���s���̔�r�֐������ɖ߂� */
		glDepthFunc(GL_LESS);

		/* �A���t�@�e�X�g�𖳌��ɂ��� */
		glDisable(GL_ALPHA_TEST);
	}

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎��������𖳌��ɂ��� */
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	//glDisable(GL_TEXTURE_GEN_R);
	//glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

    /* �e�N�X�`���I�u�W�F�N�g�̌������������� */
    glBindTexture(GL_TEXTURE_2D, 0);

}
