//-----------------------------------------------------------------------------------------------------
// File : Main.cpp
//
// GLUT Empty Project
//
// Date : Jan. 03, 2008
// Version : 2.0
// Author : Pocol
// Memo :
//-----------------------------------------------------------------------------------------------------

// Disable Warning C4996
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif
#ifndef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#endif

//
// include
//
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
using namespace std;

//
// Global Variable
//
int WindowPositionX = 100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "Empty Project";
char Version[256];
char Vender[256];
char Renderer[256];

//
// Forward declarations
//
void Initialize();
void Shutdown();
void Display();
void Idle();
void Reshape(int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void PassiveMotion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);
void Render2D();
void Render3D();
void glutRenderText(void* bitmapfont, char* text);
void glutToggleFullScreen();



//----------------------------------------------------------------------------------------------------
// Name : main()
// Desc : �A�v���P�[�V�����̃G���g���[�|�C���g
//----------------------------------------------------------------------------------------------------
int main( int argc, char **argv )
{
	//�@GLUT�R�[���o�b�N�֐��Ȃǂ̐ݒ�
	glutInit(&argc, argv);
	glutInitWindowPosition(WindowPositionX, WindowPositionY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(WindowTitle);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	
	//�@OpenGL������
	Initialize();	

	//�@���C�����[�v
	glutMainLoop();

	//�@��Еt��
	Shutdown();

	return 0;
}


//----------------------------------------------------------------------------------------------------
// Name : Initialize()
// Desc : ����������
//----------------------------------------------------------------------------------------------------
void Initialize()
{
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *vender = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);

	sprintf(Version, "OpenGL %s", version);
	sprintf(Vender, "%s", vender);
	sprintf(Renderer, "%s", renderer);

	//�@�o�b�N�o�b�t�@���N���A����F�̎w��
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//�@�[�x�e�X�gON
	glEnable(GL_DEPTH_TEST);

	//�@�E�B���h�E��\��
	glutShowWindow();
}

//---------------------------------------------------------------------------------------------------
// Name : Idle()
// Desc : �A�C�h�����O���̏���
//---------------------------------------------------------------------------------------------------
void Idle()
{
	//�@�ɂȎ��ɍĕ`��
	glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------------
// Name : Reshape()
// Desc : �T�C�Y�ύX
//---------------------------------------------------------------------------------------------------
void Reshape(int x, int y)
{
	//�@�E�B���h�E�T�C�Y��ۑ�
	WindowWidth = x;
	WindowHeight = y;

	//�@�T�C�Y�`�F�b�N
	if ( WindowWidth < 1 ) WindowWidth = 1;
	if ( WindowHeight < 1 ) WindowHeight = 1;

	//�@�r���[�|�[�g�̐ݒ�
	glViewport(0, 0, WindowWidth, WindowHeight);

	//�@�ˉe�s��̐ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
}

//--------------------------------------------------------------------------------------------------
// Name : Render2D()
// Desc : 2�����V�[���̕`��
//--------------------------------------------------------------------------------------------------
void Render2D()
{
	//�@3D�@���@2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//�@�����̕`��
	glColor4f(1.0, 1.0, 1.0, 1.0); 
	glRasterPos2i(15, 15);
	glutRenderText(GLUT_BITMAP_HELVETICA_12 , Version);
	glRasterPos2i(15, 30);
	glutRenderText(GLUT_BITMAP_HELVETICA_12 , Vender);
	glRasterPos2i(15, 45);
	glutRenderText(GLUT_BITMAP_HELVETICA_12 , Renderer);

	//�@2D�@���@3D
	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------------------------------------------------------------------
// Name : Render3D()
// Desc : 3�����V�[���̕`��
//--------------------------------------------------------------------------------------------------
void Render3D()
{
	//glutSolidTeapot(1.f);
}

//---------------------------------------------------------------------------------------------------
// Name : Display()
// Desc : �E�B���h�E�ւ̕`��
//---------------------------------------------------------------------------------------------------
void Display()
{
	//�@�o�b�N�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�@���f���r���[�s��̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//�@���_�̐ݒ�
	gluLookAt(0.0, 0.0, -5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);	

	//
	glPushMatrix();
	
	//�@3D�V�[���̕`��
	Render3D();	

	//�@2D�V�[���̕`��
	Render2D();

	//
	glPopMatrix();

	//�@�_�u���o�b�t�@
	glutSwapBuffers();
}

//---------------------------------------------------------------------------------------------------
// Name : Mouse()
// Desc : �}�E�X����
//---------------------------------------------------------------------------------------------------
void Mouse(int button, int state, int x, int y)
{
	switch ( button )
	{
	case GLUT_LEFT_BUTTON:
		switch ( state )
		{
		case GLUT_DOWN:
			break;

		case GLUT_UP:
			break;
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		switch ( state )
		{
		case GLUT_DOWN:
			break;

		case GLUT_UP:
			break;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		switch ( state )
		{
		case GLUT_DOWN:
			break;

		case GLUT_UP:
			break;
		}
		break;

	default:
		break;
	}
	

}

//--------------------------------------------------------------------------------------------------
// Name : Motion()
// Desc : �}�E�X�h���b�O��
//-------------------------------------------------------------------------------------------------
void Motion(int x, int y)
{
}

//--------------------------------------------------------------------------------------------------
// Name : PassiveMotion()
// Desc : �}�E�X�ړ���
//--------------------------------------------------------------------------------------------------
void PassiveMotion(int x, int y)
{
}

//--------------------------------------------------------------------------------------------------
// Name : Keyboard()
// Desc : �L�[�{�[�h����
//--------------------------------------------------------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	switch ( key )
	{
	case '\033':
		exit(0);
		break;

	default:
		break;
	}

	//>GLUT
	//GlutWidget
}

//--------------------------------------------------------------------------------------------------
// Name : Special()
// Desc : ����L�[����
//--------------------------------------------------------------------------------------------------
void Special(int key, int x, int y)
{
	switch ( key )
	{
	case GLUT_KEY_F1:
		glutToggleFullScreen();
		break;

	case GLUT_KEY_F2:
		break;

	case GLUT_KEY_F3:
		break;

	case GLUT_KEY_F4:
		break;

	case GLUT_KEY_F5:
		break;

	case GLUT_KEY_F6:
		break;

	case GLUT_KEY_F7:
		break;

	case GLUT_KEY_F8:
		break;

	case GLUT_KEY_F9:
		break;

	case GLUT_KEY_F10:
		break;

	case GLUT_KEY_F11:
		break;

	case GLUT_KEY_F12:
		break;

	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;

	case GLUT_KEY_PAGE_UP:
		break;

	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_HOME:
		break;

	case GLUT_KEY_END:
		break;

	case GLUT_KEY_INSERT:
		break;
	}
}

//--------------------------------------------------------------------------------------------------
// Name : Shutdown()
// Desc : ��Еt��
//--------------------------------------------------------------------------------------------------
void Shutdown()
{
}

//--------------------------------------------------------------------------------------------------
// Name : glutRenderText()
// Desc : ASCII������̕`��
//--------------------------------------------------------------------------------------------------
void glutRenderText(void* bitmapfont, char*text)
{
	for ( int i=0; i<(int)strlen(text); i++ )
		glutBitmapCharacter(bitmapfont, (int)text[i]);
}

//--------------------------------------------------------------------------------------------------
// Name : glutToggleFullScreen()
// Desc : �t���X�N���[���ƃE�B���h�E���[�h�̐؂�ւ�
//--------------------------------------------------------------------------------------------------
void glutToggleFullScreen()
{
	static bool FullScreenMode = false;
	static int beforeWidth = WindowWidth;
	static int beforeHeight = WindowHeight;
	FullScreenMode ? FullScreenMode = false : FullScreenMode = true;
	if ( FullScreenMode )
	{
		beforeWidth = WindowWidth;
		beforeHeight = WindowHeight;
		glutFullScreen();
	}
	else
	{
		glutPositionWindow(WindowPositionX, WindowPositionY);
		glutReshapeWindow(beforeWidth, beforeHeight);
		glutShowWindow();
	}
}