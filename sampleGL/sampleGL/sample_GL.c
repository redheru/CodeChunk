/*---------------------------------------------------------------------------
           MQO�`����3D���f����ǂݍ����OpenGL��ŕ\������T���v��
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

#include "GLMetaseq.h"	// ���f�����[�_


// �O���[�o���ϐ�
int g_mouseX = 0;	// �}�E�XX���W
int g_mouseY = 0;	// �}�E�XY���W
int g_rotX = 0;		// X������̉�]
int	g_rotY = 0;		// Y������̉�]

MQO_MODEL g_mqoModel;	// MQO���f��


// �v���g�^�C�v�錾
void mySetLight(void);
void Draw(void);
void Reshape (int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);


// �����̐ݒ���s���֐�
void mySetLight(void)
{
	GLfloat light_diffuse[]  = { 0.9, 0.9, 0.9, 1.0 };	// �g�U���ˌ�
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	// ���ʔ��ˌ�
	GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 0.1 };	// ����
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };	// �ʒu�Ǝ��

	// �����̐ݒ�
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_diffuse );	 // �g�U���ˌ��̐ݒ�
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular ); // ���ʔ��ˌ��̐ݒ�
	glLightfv( GL_LIGHT0, GL_AMBIENT,  light_ambient );	 // �����̐ݒ�
	glLightfv( GL_LIGHT0, GL_POSITION, light_position ); // �ʒu�Ǝ�ނ̐ݒ�

	glShadeModel( GL_SMOOTH );	// �V�F�[�f�B���O�̎�ނ̐ݒ�
	glEnable( GL_LIGHT0 );		// �����̗L����
}


// �`��֐�
void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// ������
	glClearColor(0.f, 0.f, 0.2f, 1.0);					// �w�i�F
	glMatrixMode(GL_MODELVIEW);

	glEnable( GL_DEPTH_TEST );		// �B�ʏ����̓K�p
	mySetLight();					// �����̐ݒ�
	glEnable( GL_LIGHTING );		// ����ON

	glPushMatrix();
		glTranslatef(0.0, -100.0, -400.0);	// ���s�ړ�
		glRotatef( g_rotX, 1, 0, 0 );		// X����]
		glRotatef( g_rotY, 0, 1, 0 );		// Y����]
		mqoCallModel(g_mqoModel);			// MQO���f���̃R�[��
	glPopMatrix();

	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );

	glutSwapBuffers();
}


// �E�B���h�E�ό`���ɌĂ΂��֐�
void Reshape (int w, int h)
{
	double	znear = 10;
	double	fovy = 50;
	double	zfar = 10000;

	// �r���[�|�[�g�ݒ�
	glViewport(0, 0, w, h);

	// �ˉe�ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)w/h, znear, zfar);
}


// �L�[�C�x���g�֐�
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
		case 0x1b:
			exit(0);	// �i���̂��ƏI�������֐����Ă΂��j
		default:
			break;
	}
}


// �}�E�X�C�x���g�֐�
void Mouse(int button, int state, int x, int y)
{
	if ( state == GLUT_DOWN ) {
		g_mouseX = x;
		g_mouseY = y;				
	}
}


// �}�E�X�h���b�O�̏����֐�
void Motion(int x, int y)
{
	int xd, yd;

	// �}�E�X�ړ��ʂ̎Z�o
	xd = x - g_mouseX;
	yd = y - g_mouseY;

	// ��]�ʂ̐ݒ�
	g_rotX += yd;
	g_rotY += xd;

	// �}�E�X���W�̍X�V
	g_mouseX = x;
	g_mouseY = y;
}


// �I�������֐�
void Quit(void)
{
	mqoDeleteModel( g_mqoModel );	// ���f���̍폜
	mqoCleanup();					// GLMetaseq�̏I������
}


// main�֐�
int main(int argc, char *argv[])
{
	// ������
	glutInit(&argc,argv);										// OpenGL������
	glutInitWindowPosition(100, 50);							// �E�B���h�E�̕\���ʒu
	glutInitWindowSize(640, 480);								// �E�B���h�E�̃T�C�Y
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// �f�B�X�v���C�ݒ�
	glutCreateWindow("MQO Loader for OpenGL");					// �E�B���h�E�̐���

	// ���f����\�������鏀��
	mqoInit();											// GLMetaseq�̏�����
	g_mqoModel = mqoCreateModel("miku_xx_03_18_210.mqo",0.5);		// ���f���̃��[�h

	// �I�������֐��̐ݒ�
	atexit(Quit);

	// �R�[���o�b�N�֐��̐ݒ�
	glutDisplayFunc(Draw);			// �`�揈���֐��̐ݒ�
	glutIdleFunc(Draw);				// �A�C�h�����̏����֐��̐ݒ�
	glutReshapeFunc(Reshape);		// �E�B���h�E�ό`���̏������s���֐��̐ݒ�
	glutKeyboardFunc(Keyboard);		// �L�[���͎��̏����֐��̐ݒ�
	glutMouseFunc(Mouse);			// �}�E�X���͎��̏����֐��̐ݒ�
	glutMotionFunc(Motion);			// �}�E�X�h���b�O���̏����֐��̐ݒ�

	// �C�x���g�҂��̖������[�v�ɓ���
	glutMainLoop();

	return 0;
}
