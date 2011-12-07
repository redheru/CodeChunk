//-----------------------------------------------------------------------------
//                             DrawFigure.h
//
// MotionData��GL�ŕ`�悷�邽�߂̃N���X
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/05 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef DRAW_FIGURE_CLASS_HEADER__
#define DRAW_FIGURE_CLASS_HEADER__

//MotionData
#include "Figure.h"
#include "MotionData.h"

//OpenGL
#include <GL/glew.h>
#include <GL/glut.h>

#include <stack>

#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

// ����:
// GL�ł̕`��@�\��t���������[�V�����f�[�^�N���X
class CDrawFigure
{
// ����:
// variables
private:
    // �o�b�t�@�I�u�W�F�N�g�̖��O��3�p�ӂ���
    GLuint m_buffers[3];

	// �e�o�b�t�@�I�u�W�F�N�g
	GLfloat* m_vertices;
    GLfloat* m_normals;
	GLuint* m_indices;

	CFigure	*m_figure;      // ���i����ێ�
	CMotionData *m_motion;  // �p������ێ�

	Vector3f m_pos;          // �����ʒu
	float m_scale;           // ���i�̃X�P�[��
	int m_sect;              // �|���S���̕�����
	Vector3f m_color;          // �{�[���̐F
	float m_specular;        // �{�[���̃X�y�L����
	float m_amb;             // �{�[���̃A���r�G���g
	float m_diffuse;         // �{�[���̃f�B�t���[�Y
	float m_alpha;           // �A���t�@�l
    bool m_isDrawCylinderModel; // �{�[���̌`��̑I��(�V�����_�[�^�ɂ��邩�ǂ���)
	bool m_isDrawLocalAxes;    // ���[�J�����W��`�ʂ�I��
	int m_indicesArraySize;  // ���_�f�[�^�̃T�C�Y��ێ�

// Eigen���C�u�����̗v�f�𓮓I�ɐ�������Ƃ��ɕK�v
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// ����:
// �R���X�g���N�^�C�f�X�g���N�^
public:
	// �f�t�H���g�R���X�g���N�^
	CDrawFigure();
	// �������q�t�R���X�g���N�^
	CDrawFigure(std::string fileName);
	// �R�s�[�R���X�g���N�^
	CDrawFigure(const CDrawFigure &src);
	// �f�X�g���N�^
	~CDrawFigure(){}

// ����:
// ������
public:
	// �������֐�
	void Initialize();
	// �t�@�C���ǂݍ���
	void LoadBVH(std::string bvhFileName);
private:
	// VBO�̏������֐�
	void initCylinderVBO();
    void initCubeVBO();
    void initSphereVBO();

public:
	void DrawSkeleton();
	void DrawSkeleton(unsigned int frame);
private:
	void DrawSkeletonSub(const CJoint* joint, unsigned int childNum, std::stack<int> matStack);
	void DrawMesh(const CJoint* joint);
    // pos1����pos2�ւ�Body��`�ʂ���B���a��radius�ŕ�������sect
	void DrawMeshSub(Vector3f pos1, Vector3f pos2, int sect, float radius);
	void DrawMeshEllipsoid(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, int sect);
	void DrawMeshVBO(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, float length, float radius);
	void DrawMeshVBOSub();
	void setMaterial(const Vector3f color, float alpha = 1.f, float specular = 0.f, float diffuse = 0.7f, float ambient = 0.5f);


// ����:
// �A�N�Z�b�T
public:
	Vector3f getPos() { return m_pos; }
	bool getIsDrawLocalAxes(){return m_isDrawLocalAxes;}
	int getFps(){ return static_cast<int>(1.f / m_motion->GetFrameTime());}
	float getFrameTime(){ return m_motion->GetFrameTime();}
	int getFrameNum(){ return m_motion->NumFrames();}
	bool getIsActive(){return m_motion->IsActive();}
	bool getIsLoaded(){return m_motion != NULL && m_figure != NULL;}


// ����:
// �~���[�e�[�^
	void setAlpha(float a) { m_alpha = a; }
	void setScale(float s) { m_scale = s; }
	void setColor(float R, float G,  float B)
	{
		m_color.x() = R;m_color.y() = G;m_color.z() = B;
	}
	void setColor(const Vector3f color)
	{
		m_color.x() = color.x();m_color.y() = color.y();m_color.z() = color.z();
	}

	void setCharacterSpecular(float s){m_specular = s;}
	void setSmoothness(int sect){m_sect = sect;}
	void setPos(float x, float y, float z)
	{
		m_pos.x() = x;m_pos.y() = y;m_pos.z() = z;
	}
	void setPram(float scale, int rendering_method, int sect)
	{
		m_scale = scale;
		m_sect = sect;
	}
	void setIsDrawLocalAxes(bool isDrawLocalAxes) {m_isDrawLocalAxes = isDrawLocalAxes; }

};

#endif //DRAW_FIGURE_CLASS_HEADER__