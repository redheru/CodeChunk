#include "GLUTWidget.h"
#include "GLUTShadow.h"

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

	// reshape function�̏C��
	glutWidget_->getGLUTView()->widthMin = texWidth_; 
	glutWidget_->getGLUTView()->heightMin = texHeight_; 

	// �e�t���̃����_�����O
	createShadowMap();

}

void GLUTShadow::Initialize()
{
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
	isInitialized_ = true;
}

void GLUTShadow::Init()
{
	isLoaded_ = false;
	isInitialized_ = false;
    isEnabled_ = false;

	//this->scene_P = NULL;
	glutWidget_ = NULL;
	texWidth_ = 1024;
    texHeight_ = 1024;
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
	//(*scene_P)();
    glutWidget_->paintGL(); 


    /* �|���S���I�t�Z�b�g�̐ݒ���� */
	glDisable(GL_POLYGON_OFFSET_FILL);

	/* �f�v�X�o�b�t�@�̓��e���e�N�X�`���������ɓ]������ */
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texWidth_, texHeight_);

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
	///glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	//glutWidget_->getGLUTView()->ApplyLightsPos();

	if( isUseAlpha_ )
	{
		/* �����̖��邳���e�̕����ł̖��邳�ɐݒ� */
		glutWidget_->getGLUTView()->ApplyLightFv(true);
		///glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdim);
		///glLightfv(GL_LIGHT0, GL_SPECULAR, lightblk);

		/* �V�[����`�悷�� */
		//(*scene_P)();
		//glutWidget_->paintGL();
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
	///glMultTransposeMatrixd(trackballRotation());
	///glTranslated(0.0, 0.0, 10.0);
	glMultMatrixd( inv_mat.m );

	/* ���f���r���[�ϊ��s��ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	if( isUseAlpha_ )
	{		
		/* �A���t�@�e�X�g��L���ɂ��ĉe�̕���������`�悷�� */
		glEnable(GL_ALPHA_TEST);

		/* �����̕��������Ƃ̐}�`�ɏd�˂ĕ`�����悤�ɉ��s���̔�r�֐���ύX���� */
		glDepthFunc(GL_LEQUAL);
	}

	/* �����̖��邳������̕����ł̖��邳�ɐݒ� */
	glutWidget_->getGLUTView()->ApplyLightFv(false);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);

	/* �V�[����`�悷�� */
	glutWidget_->paintGL();
	//(*scene_P)();
	
	if( isUseAlpha_ )
	{
		/* ���s���̔�r�֐������ɖ߂� */
		glDepthFunc(GL_LESS);

		/* �A���t�@�e�X�g�𖳌��ɂ��� */
		glDisable(GL_ALPHA_TEST);
	}

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎��������𖳌��ɂ��� */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);
}
