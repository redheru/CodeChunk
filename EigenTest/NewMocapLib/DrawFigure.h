//-----------------------------------------------------------------------------
//                             DrawFigure.h
//
// MotionDataをGLで描画するためのクラス
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

// 説明:
// GLでの描画機能を付加したモーションデータクラス
class CDrawFigure
{
// 説明:
// variables
private:
    // バッファオブジェクトの名前を3つ用意する
    GLuint m_buffers[3];

	// 各バッファオブジェクト
	GLfloat* m_vertices;
    GLfloat* m_normals;
	GLuint* m_indices;

	CFigure	*m_figure;      // 骨格情報を保持
	CMotionData *m_motion;  // 姿勢情報を保持

	Vector3f m_pos;          // 初期位置
	float m_scale;           // 骨格のスケール
	int m_sect;              // ポリゴンの分割数
	Vector3f m_color;          // ボーンの色
	float m_specular;        // ボーンのスペキュラ
	float m_amb;             // ボーンのアンビエント
	float m_diffuse;         // ボーンのディフューズ
	float m_alpha;           // アルファ値
    bool m_isDrawCylinderModel; // ボーンの形状の選択(シリンダー型にするかどうか)
	bool m_isDrawLocalAxes;    // ローカル座標を描写を選択
	int m_indicesArraySize;  // 頂点データのサイズを保持

// Eigenライブラリの要素を動的に生成するときに必要
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// 説明:
// コンストラクタ，デストラクタ
public:
	// デフォルトコンストラクタ
	CDrawFigure();
	// 初期化子付コンストラクタ
	CDrawFigure(std::string fileName);
	// コピーコンストラクタ
	CDrawFigure(const CDrawFigure &src);
	// デストラクタ
	~CDrawFigure(){}

// 説明:
// 初期化
public:
	// 初期化関数
	void Initialize();
	// ファイル読み込み
	void LoadBVH(std::string bvhFileName);
private:
	// VBOの初期化関数
	void initCylinderVBO();
    void initCubeVBO();
    void initSphereVBO();

public:
	void DrawSkeleton();
	void DrawSkeleton(unsigned int frame);
private:
	void DrawSkeletonSub(const CJoint* joint, unsigned int childNum, std::stack<int> matStack);
	void DrawMesh(const CJoint* joint);
    // pos1からpos2へのBodyを描写する。半径がradiusで分割数がsect
	void DrawMeshSub(Vector3f pos1, Vector3f pos2, int sect, float radius);
	void DrawMeshEllipsoid(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, int sect);
	void DrawMeshVBO(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, float length, float radius);
	void DrawMeshVBOSub();
	void setMaterial(const Vector3f color, float alpha = 1.f, float specular = 0.f, float diffuse = 0.7f, float ambient = 0.5f);


// 説明:
// アクセッサ
public:
	Vector3f getPos() { return m_pos; }
	bool getIsDrawLocalAxes(){return m_isDrawLocalAxes;}
	int getFps(){ return static_cast<int>(1.f / m_motion->GetFrameTime());}
	float getFrameTime(){ return m_motion->GetFrameTime();}
	int getFrameNum(){ return m_motion->NumFrames();}
	bool getIsActive(){return m_motion->IsActive();}
	bool getIsLoaded(){return m_motion != NULL && m_figure != NULL;}


// 説明:
// ミューテータ
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