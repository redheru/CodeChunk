//-----------------------------------------------------------------------------
//                             DrawFigure.cpp
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

#include "DrawFigure.h"
#include "MotionFile.h"

//-----------------------------------------------------------------------------
// 説明: コンストラクタ
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CDrawFigure::CDrawFigure()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// 説明: コンストラクタ
// 引数: 
//       fileName [in] ファイル名
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
CDrawFigure::CDrawFigure(std::string bvhFileName)
{
	Initialize();
	this->LoadBVH( bvhFileName);
}


//-----------------------------------------------------------------------------
// 説明: 初期化関数
// 引数: 
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CDrawFigure::Initialize()
{
	//initialize(); //implemented in MocapData

	m_figure = NULL;
	m_motion = NULL;

	m_pos = Vector3f(0.f, 0.f, 0.f);
	m_scale = 1.0;
	m_sect = 20;

	m_color = Vector3f(0.85, 0.66, 0.15);
	m_specular = 0;
	m_amb = 0.5;
	m_diffuse = 1.0;
	m_alpha = 1.0;

	m_isDrawLocalAxes = false;

	m_indicesArraySize = 0;
	m_isDrawCylinderModel = true;
	if(m_isDrawCylinderModel){
	    initCubeVBO();
	    //initCylinderVBO();
		//initSphereVBO();
	}

	m_pos = Vector3f(0,0,0);
}

//-----------------------------------------------------------------------------
// 説明: BVHファイルの読み込み
// 引数: 
//       bvhFileName [in] ファイル名
// 返り値:
// その他: 
//-----------------------------------------------------------------------------
void CDrawFigure::LoadBVH(std::string bvhFileName)
{
	if( m_motion != NULL)
		delete m_motion;
	m_motion = new CMotionData;
	if( m_figure != NULL)
		delete m_figure;
	m_figure = new CFigure;

	if(NMotionFile::LoadBvhFile(bvhFileName.c_str(), m_figure, m_motion))
	{
		return;
	}

	delete m_motion;
	m_motion = NULL;
	delete m_figure;
	m_figure = NULL;
}

//-----------------------------------------------------------------------------
// 説明: ボーンのGLでの描画関数
// 引数: 
//      pJoint [in] 描画するボーン
// 返り値:
// その他: 
//       m_figureはCFigure::setPose()で姿勢をあらかじめセットされている
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMesh(const CJoint* joint)
{
	// ローカル座標系を描画するかどうか
	//if( m_isDrawLocalAxes )
	DrawLocalAxes();

	// ボーンの開始位置
	Vector3f boneStart = Vector3f(0,0,0);

	// ボーンの終了位置からボーンの開始位置に向けてメッシュをレンダリング
	Vector3f boneEnd = joint->GetOffset();
	//Vector3f bone_end = Vector3f(bone.direction.x, bone.direction.y, bone.direction.z);
	//bone_end *= bone.length;

	////Vector4f temp_color( color.r, color.g, color.b, alpha);
	////Color::setMaterial(temp_color, this->specular, this->diffuse, this->amb);
	float temp_alpha = m_alpha;

	setMaterial( m_color, temp_alpha, m_specular, m_diffuse, m_amb);
	//QGLColor::setMaterial(this->color, this->specular, this->diffuse, this->amb, temp_alpha);
	//Color::setMaterial(this->color, this->specular, this->diffuse, this->amb, 1.0);
	//QGLUtil::DrawEllipsoidalBody( bone_start, bone_end, this->sect, bone.radius);
	DrawMeshSub( boneStart, boneEnd, m_sect, 0.f);

}

//-----------------------------------------------------------------------------
// 説明: 骨格をGL上で描写する
// 引数: 
//      pJoint [in] 描画対象の関節
// 返り値:
// その他: 
//       m_figureはCFigure::setPose()で姿勢をあらかじめセットされている
//-----------------------------------------------------------------------------
void CDrawFigure::DrawSkeletonSub(const CJoint* joint, unsigned int childNum, std::stack<int> matStack)
{
	if(joint == NULL)
		return;

	//// 行列 "M" は
	//// TposMScaleTrootRrootT0R0...Ti-2Ri-2

	// 行列をスタックに追加する push M on the Matrix stack,
	// 親Jointのカウントを増やす
	glPushMatrix();
	matStack.push(childNum);
	{
		//メッシュの描画
		DrawMesh(joint);

		// "Ti-1" の移動行列を行列 "M" に右からかける
		Vector3f offset = joint->GetOffset();
		glTranslatef(offset.x(), offset.y(), offset.z());

		// 行列 "M" は
		// TposMscaleTrootRrootR0T0...Ti-1

		//Ri-1の回転行列を行列に右からかける
		Matrix4f tmp;
		tmp = joint->GetRotationMatrix();
		glMultMatrixf(tmp.data());

		// 行列 "M" は
		// TposMscaleTrootRrootR0T0...Ti-1Ri-1
		for(unsigned int j = 0; j < joint->NumChildren(); j++)
		{
			DrawSkeletonSub(joint->GetChild(j), j, matStack);
		}
	}
	// 行列をスタックから取り出す ,
	// 親Jointのカウントを減らす
	glPopMatrix();
	matStack.pop();

}

//-----------------------------------------------------------------------------
// 説明: あらかじめセットした姿勢をGL上で描写する
// 引数: 
// 返り値:
// その他: 
//       m_figureはCFigure::setPose()で姿勢をあらかじめセットされている
//-----------------------------------------------------------------------------
void CDrawFigure::DrawSkeleton()
{
	if( m_figure == NULL)
		return;

	const CJoint *root;

	// スタックの計算
	std::stack<int> matStack;

	// 位置の修正 ( 行列 "M" に 位置 "TPos" を右からかける )
	glTranslatef( m_pos.x(), m_pos.y(), m_pos.z());

	// スケーリング( 行列 "M" にスケール "Mscale" を右からかける )
	glScalef(m_scale, m_scale, m_scale);

	glPushMatrix();
	{
		root = m_figure->GetRoot();
		glPushMatrix();
		{
			// ルート位置の修正( 行列 "M" に ルート位置 "Troot" を右からかける )		
			Vector3f rootPos = root->GetPosition();
			glTranslatef( rootPos.x(), rootPos.y(), rootPos.z() );
			// ルート位置の修正 ( 行列 "M" に ルート回転行列 "Rroot" を右からかける )
			Matrix4f tmp = root->GetRotationMatrix();
			glMultMatrixf(tmp.data());
			// ルートの親（存在しない)の番号をスタックに積む)
			matStack.push(-1);
			// ボーンを描画する
			for(unsigned int i = 0; i < root->NumChildren(); i++)
			{
				DrawSkeletonSub(root->GetChild(i), i, matStack);
			}
			// ルートの親をポップする
			matStack.pop();
		}
		glPopMatrix();
		// スタックから TrootRroot をポップする
	}
	glPopMatrix();
	// スタックから TposMscale をポップする
	
	return;

	//if( m_figure == NULL)
	//	return;

	//const CJoint *root;
	//
	////Pose& pose = poses[frame];
	////Skeleton* skl = pose.getSkeleton();
	//vector<int> parent_num_stack;
	//
	//glPushMatrix();
	//{
	//	//pos correction( multiply Tpos to M from right )
	//	glTranslatef( m_pos.x(), m_pos.y(), m_pos.z());

	//	//scaling( multiply Mscale to M from right )
	//	glScalef(m_scale, m_scale, m_scale);

	//	root = m_figure->GetRoot();

	//	//root corection( multiply Troot to M from right )		
	//	//if( apply_root_translation ) glTranslated( pose.getRootPosition().x, pose.getRootPosition().y, pose.getRootPosition().z );//Troot
	//	//else glTranslated( 0 ,0 ,1 );//Troot

	//	//root corection( multiply Rroot to M from right )
	//	glMultMatrixf(root->GetTransformMatrix().data());
	//	//glRotate( pose.getRootAngle() );//Rroot

	//	//push root's parent number on the stack 
	//	parent_num_stack.push_back(-1);

	//	//follow bones
	//	for(unsigned int boneNum = 0; boneNum < root->NumChildren(); boneNum++)
	//	{
	//		Bone& bone = *skl->bones[bone_num];

	//		//pop stack
	//		while(parent_num_stack.back() != bone.parent)
	//		{
	//			parent_num_stack.pop_back();
	//			glPopMatrix();
	//		}
	//		//// Now, matrix M                          ////
	//		////  = TposMscaleTrootRrootR0T0...Ri-2Ti-2 ////

	//		//push M on the Matrix stack,
	//		//and push parent num on the parent num stack 
	//		glPushMatrix();
	//		parent_num_stack.push_back(bone_num);
	//	
	//		//draw axes
	//		//if(this->isDrawLocalAxes)
	//		//{
	//		//	DrawNeedLocalAxes(bone.name, 0.15, 3.0);
	//		//}
	//		
	//		//bone start point
	//		Vector3f bone_start = Vector3f(0,0,0);
	//		
	//		//multiply Ri-1 to M from right
	//		glRotate( pose.getBoneAngle(bone_num) );

	//		//render ellipsoid from bone end point to bone start point
	//		Vector3f bone_end = Vector3f(bone.direction.x, bone.direction.y, bone.direction.z);
	//		bone_end *= bone.length;


	//		////Vector4f temp_color( color.r, color.g, color.b, alpha);
	//		////Color::setMaterial(temp_color, this->specular, this->diffuse, this->amb);
	//		double temp_alpha = this->alpha_;
	//		////if( ! (bone.name == "rfemur_phantom" || bone.name == "rfemur" || bone.name == "rtibia" || bone.name == "rfoot"|| bone.name == "rtoesw"))
	//		//if( ! (bone.name == "rhand" || bone.name == "rradius" || bone.name == "rhumerus" || bone.name == "rclavicle"))
	//		////if(false)
	//		//{
	//		//	//if( this->alpha < 0.99 ) temp_alpha = 0.0;
	//		//}

	//		setMaterial( Vector4f( color_, temp_alpha), specular_, diffuse_, amb_);
	//		//QGLColor::setMaterial(this->color, this->specular, this->diffuse, this->amb, temp_alpha);
	//		//Color::setMaterial(this->color, this->specular, this->diffuse, this->amb, 1.0);
	//		//QGLUtil::DrawEllipsoidalBody( bone_start, bone_end, this->sect, bone.radius);
	//		DrawEllipsoidalBody( bone_start, bone_end, this->m_sect, bone.radius);
	//		
	//		//multiply Ti-1 to M from right
	//		glTranslated(bone_end.x, bone_end.y, bone_end.z);

	//		//// Now, matrix M                          ////
	//		////  = TposMscaleTrootRrootR0T0...Ri-1Ti-1 ////
	//	}
	//	
	//	//clean Matrix stack
	//	//while (!parent_num_stack.empty())
	//	while (parent_num_stack.size() >= 1)
	//	{
	//		glPopMatrix();
	//		parent_num_stack.pop_back();
	//	}			
	//}
	////pop TposMscaleTrootRroot
	//glPopMatrix();
}

//-----------------------------------------------------------------------------
// 説明: 指定したフレームの姿勢をGL上で描写する
// 引数: 
//       frame [in] 描画する姿勢のフレーム
// 返り値:
// その他: 
//       m_figureはCFigure::setPose()で姿勢をあらかじめセットされている
//-----------------------------------------------------------------------------
void CDrawFigure::DrawSkeleton(unsigned int frame)
{
	if( m_figure->SetPose( m_motion, frame) )
	{
		DrawSkeleton();
	}
}


//void DrawFigure::glRotate(klQuat q) {
//	double mat[16];
//	q.ToMatrix(mat);
//	glMultMatrixd(mat);
//}

//-----------------------------------------------------------------------------
// 説明: 描画する際のマテリアルを設定する
// 引数: 
//       color [in] 設定する色
//       specular_rate [in] 親関節ノードへのポインタ
// 返り値:

// その他: 
//       m_figureはCFigure::setPose()で姿勢をあらかじめセットされている
//-----------------------------------------------------------------------------
void CDrawFigure::setMaterial(const Vector3f color, float alpha, float specular, float diffuse, float ambient)
{
	Vector4f m_color(color.x(), color.y(), color.z(), alpha);

	Vector4f spc = m_color * specular;
	spc.w() = m_color.w();

	Vector4f dif = m_color * diffuse;
	dif.w() = m_color.w();

	Vector4f amb = m_color * ambient;
	amb.w() = m_color.w();

	glColor4fv(color.data());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spc.data());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif.data());
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb.data());
}

//-----------------------------------------------------------------------------
// 説明: メッシュをGLで描画する補助関数
// 引数: 
//       pos1 [in] ボーンの開始地点
//       pos2 [in] ボーンの終了地点
//       sect [in] ポリゴンの分割数
//       radius [in] ボーンの太さ(半径)
// 返り値:
// その他: 
//       
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshSub(Vector3f pos1, Vector3f pos2, int sect, float radius) 
{
	Vector3f p,v1,v2,v3;
	p = (pos1 + pos2) /2;
	v1 = (pos1 - pos2) /2;
  	float length = (pos1 - pos2).norm();

	//auxiliary vector(補助)
	Vector3f temp(1,1,1);

    if(m_isDrawCylinderModel)
	{
		v2 = v1.cross(temp);
		v2 = v2.normalized();
		v2 = v2 * 0.02f;
		v3 = v1.cross(v2);
		v3 = v3.normalized();
		v3 = v3 * 0.02f;

    	DrawMeshVBO(pos2, v1, v2, v3, length, (float)radius); //pos2の位置を原点にする
	}
	else
	{
		v2 = v1.cross(temp);
		v2 = v2.normalized();
		v2 = v2 * radius;
		v3 = v1.cross(v2);
		v3 = v3.normalized();
		v3 = v3 * radius;

		DrawMeshEllipsoid(p, v1, v2, v3, sect);
	}
}

//-----------------------------------------------------------------------------
// 説明: メッシュを楕円球で描画する関数
// 引数: 
//       p [in] 楕円球の中心位置
//       v1 [in] X軸方向の伸縮率
//       v2 [in] Y軸方向の伸縮率
//       v3 [in] Z軸方向の伸縮率
//       sect [in] ポリゴンの分割数
// 返り値:
// その他: 
//       歪めた空間上に球を描画することで楕円球に変える
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshEllipsoid(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, int sect)
{ 
	GLfloat mat[16]; 
	mat[0] = v1.x(); mat[1] = v1.y(); mat[2] = v1.z();  mat[3] = 0;
	mat[4] = v2.x(); mat[5] = v2.y(); mat[6] = v2.z();  mat[7] = 0;
	mat[8] = v3.x(); mat[9] = v3.y(); mat[10] = v3.z(); mat[11] = 0;
	mat[12] = p.x(); mat[13] = p.y(); mat[14] = p.z();  mat[15] = 1;
	//glMatrixMode(GL_MODELVIEW);

 	glPushMatrix();
	glMultMatrixf(mat);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // gluで球を描画
    GLUquadricObj *sphere;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluSphere(sphere, 1.0, sect, sect);
    gluDeleteQuadric(sphere);

	glDisable(GL_BLEND);

	glDisable(GL_NORMALIZE);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 説明: メッシュを楕円球で描画する関数
// 引数: 
//       p [in] 楕円球の中心位置
//       v1 [in] X軸方向の伸縮率
//       v2 [in] Y軸方向の伸縮率
//       v3 [in] Z軸方向の伸縮率
//       sect [in] ポリゴンの分割数
// 返り値:
// その他: 
//       歪めた空間上に球を描画することで楕円球に変える
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshVBO(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, float length, float radius)
{
	// 図形の中心位置に移動するための行列を作成
	GLfloat mat[16]; 
	mat[8] = v1.x(); mat[9] = v1.y(); mat[10] = v1.z();  mat[11] = 0;
	mat[4] = v2.x(); mat[5] = v2.y(); mat[6] = v2.z();  mat[7] = 0;
	mat[0] = v3.x(); mat[1] = v3.y(); mat[2] = v3.z(); mat[3] = 0;
	mat[12] = p.x(); mat[13] = p.y(); mat[14] = p.z();  mat[15] = 1;

 	glPushMatrix();
	glMultMatrixf(mat);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DrawMeshVBOSub();

	glDisable(GL_BLEND);

	glDisable(GL_NORMALIZE);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 説明: メッシュをVBOを用いて描画する補助関数
// 引数: 
// 返り値:
// その他: 
//       メッシュの形は initCylinderVBO()、initSphereVBO()、initCubeVBO() で予め定義しておく
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshVBOSub()
{
    /* 頂点データの場所を指定する */
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
	//頂点に与えるデータ数:3, データ型:float, データ間隔:0, データ格納場所:0(BindBufferによってm_buffers[0]のポインタが既にバインドされているので、オフセットを渡すだけになる)
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* 法線データの場所を指定する */
	//データ型:float, データ間隔:0, データ格納場所:0(BindBufferによってm_buffers[1]のポインタが既にバインドされているので、オフセットを渡すだけになる)
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
    glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* テクスチャ座標の場所を指定する */
    //glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
    //glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* 頂点のインデックスの場所を指定して図形を描く */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
    //glEnable(GL_TEXTURE_2D);

    /* 頂点データ，法線データ，テクスチャ座標の配列を有効にする */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//図形タイプ:Triangle_FAN, 描画する要素数(頂点数):上底と下底, 頂点インデックスの型:unsigned int, データ格納場所:0(BindBufferによってm_buffers[1]のポインタが既にバインドされているので、オフセットを渡すだけになる)
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indicesArraySize, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	//glDisable(GL_TEXTURE_2D);

	//　バインドしたものをもどす
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* 頂点データ，法線データ，テクスチャ座標の配列を無効にする */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//-----------------------------------------------------------------------------
// 説明: シリンダ型のメッシュをVBOで定義する
// 引数: 
// 返り値:
// その他: 
//       DrawFigure::Initialize()で呼ばれる
//-----------------------------------------------------------------------------
void CDrawFigure::initCylinderVBO()
{
    unsigned int verticesNum = m_sect + m_sect + m_sect * 2; //上底 + 下底 + 側面
	m_indicesArraySize = ((m_sect-2) * 3) * 2 + (m_sect * 2) * 3; // 上底(三角形)、下底(三角形)、側面(三角形*2で四角形)

	m_vertices = new GLfloat[ verticesNum * 3 ];
	m_normals = new GLfloat[ verticesNum * 3 ];
	m_indices= new GLuint[ m_indicesArraySize ];


	float step = (M_PI * 2) / (float)m_sect;
	float radius = 1.f;
	float height = 2.f; 

    //
    //  頂点、法線、色の設定
	//
    unsigned int vertIndex = 0;

	//上底
	for(unsigned int i=0; i < (unsigned int)m_sect; i++)
	{
        float t = step * (float)i;

		m_normals[vertIndex * 3] = 0.f;
		m_normals[vertIndex * 3 + 1] = 0.f;
		m_normals[vertIndex * 3 + 2] = 1.f;

		m_vertices[vertIndex * 3] = radius * cos(t);
		m_vertices[vertIndex * 3 + 1] = radius * sin(t);
		m_vertices[vertIndex * 3 + 2] = height;

	    vertIndex++;
	}

    //　下底
	for(int i = m_sect; --i >= 0;)
	{
	    float t = step * (float)i;

		m_normals[vertIndex * 3] = 0.f;
		m_normals[vertIndex * 3 + 1] = 0.f;
		m_normals[vertIndex * 3 + 2] = -1.f;

		m_vertices[vertIndex * 3] = radius * cos(t);
		m_vertices[vertIndex * 3 + 1] = radius * sin(t);
		m_vertices[vertIndex * 3 + 2] = 0.f;

	    vertIndex++;
	}

    // 側面
	for(unsigned int i=0; i < (unsigned int)m_sect; i++)
	{
        float t = step * (float)i;
        float y = sin(t);
        float x = cos(t);

		m_normals[vertIndex * 3] = x;
		m_normals[vertIndex * 3 + 1] = y;
		m_normals[vertIndex * 3 + 2] = 0.f;

		m_vertices[vertIndex * 3] = radius * x;
		m_vertices[vertIndex * 3 + 1] = radius * y;
		m_vertices[vertIndex * 3 + 2] = height;

		m_normals[(vertIndex+1) * 3] = x;
		m_normals[(vertIndex+1) * 3 + 1] = y;
		m_normals[(vertIndex+1) * 3 + 2] = 0.f;

		m_vertices[(vertIndex+1) * 3] = radius * x;
		m_vertices[(vertIndex+1) * 3 + 1] = radius * y;
		m_vertices[(vertIndex+1) * 3 + 2] = 0.0;

	    vertIndex += 2;
	}

    //
	// 頂点インデックスの設定
	//
    vertIndex = 0;

	// 上底
    for( int i=0; i < m_sect - 2; i++)
	{
		m_indices[vertIndex] = 0;
		m_indices[vertIndex+1] = i+1;
        m_indices[vertIndex+2] = i+2;

        vertIndex += 3;
	}

	// 下底
    for( int i = m_sect; i < m_sect * 2 - 2; i++)
	{
        m_indices[vertIndex] = m_sect;
		m_indices[vertIndex+1] = i+1;
        m_indices[vertIndex+2] = i+2;

		vertIndex +=3;
	}

	// 側面
	for( int i = m_sect * 2; i < m_sect * 4 - 2; i+=2)
	{
        m_indices[vertIndex] = i;
		m_indices[vertIndex+1] = i + 1;
		m_indices[vertIndex+2] = i + 2;

		m_indices[vertIndex+3] = i + 2;
		m_indices[vertIndex+4] = i + 1;
		m_indices[vertIndex+5] = i + 3;

		vertIndex += 6;
	}

    //最後の面2つは最初の頂点を使う
    m_indices[vertIndex] = m_sect * 4 - 2;
	m_indices[vertIndex+1] = m_sect * 4 -1;
	m_indices[vertIndex+2] = m_sect * 2;

    m_indices[vertIndex+3] = m_sect * 2;
	m_indices[vertIndex+4] = m_sect * 4 - 1;
	m_indices[vertIndex+5] = m_sect * 2 + 1;

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, m_buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}

//-----------------------------------------------------------------------------
// 説明: 球型のメッシュをVBOで定義する
// 引数: 
// 返り値:
// その他: 
//       DrawFigure::Initialize()で呼ばれる
//-----------------------------------------------------------------------------
void CDrawFigure::initSphereVBO()
{
	float mRadius = 1.f;
    float zOffset = 1.f;

	int gridU = m_sect;
	int gridV = m_sect;
	int gridU1 = gridU + 1;
	int gridV1 = gridV + 1;
	float incU = 360 / gridU;           // U方向に何分割するか？(円を扇形に分解する方向)
	float incV = 2 * mRadius / gridV;   // V方向に何分割するか？(球を円に分割する方向)
	int cnt = 0;                        // 現在の配列を示す

				
	//頂点配列と法線配列を格納する
    unsigned int verticesArraySize = (2 + (gridV1-2) * gridU) * 3;
	m_vertices = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
	m_normals = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)


	m_vertices[cnt++] = 0;                               //Y軸での端の頂点座標
	m_vertices[cnt++] = -mRadius; 
	m_vertices[cnt++] = 0 + zOffset;
    Vector3f normal = Vector3f(0, -mRadius, 0);
    normal = normal.normalized();
    m_normals[cnt-3] = normal.x();
    m_normals[cnt-2] = normal.y();
    m_normals[cnt-1] = normal.z();

	float d = mRadius;
	float y, t, r;
	for( int iv = 1; iv < gridV1 - 1; ++ iv ) {            // 球を円に分解する
		y = iv * incV - d;                                     // Y軸上での値(V方向の分割値)
		r = sqrt(d * d - y * y);                          // 現在のY値上での円の半径
		for( int iu = 0; iu < gridU; ++ iu ) {             // 円を扇形に分解する
			t = iu * incU * M_PI / 180;                     // 扇形の角度(Radian)
			m_vertices[cnt++] = (GLfloat)(r * cos(t));      // 頂点座標を格納(XZ平面でプロット。
			m_vertices[cnt++] = (GLfloat)y;                      // Z=0のとき、つまりX軸上の位置から開始
			m_vertices[cnt++] = (GLfloat)(r * sin(t)) + zOffset;      // 時計回りに描写(X軸からZ軸方向への向き) 

			normal = Vector3f(m_vertices[cnt-3], m_vertices[cnt-2], m_vertices[cnt-1]);
			normal = normal.normalized();
			m_normals[cnt-3] = normal.x();
			m_normals[cnt-2] = normal.y();
			m_normals[cnt-1] = normal.z();
		}
	}
	m_vertices[cnt++] = 0;                               //Y軸での端の頂点座標
	m_vertices[cnt++] = mRadius;
	m_vertices[cnt++] = 0 + zOffset;			

	normal = Vector3f(m_vertices[cnt-3], m_vertices[cnt-2], m_vertices[cnt-1]);
	normal = normal.normalized();
    m_normals[cnt-3] = normal.x();
	m_normals[cnt-2] = normal.y();
	m_normals[cnt-1] = normal.z();

	//インデックス配列をセット(三角形プリミティブの頂点レンダリング順番を格納)
    m_indicesArraySize = ( ( gridV - 2) * (gridU - 1) * 2 + (gridU * 2) ) * 3;
	m_indices= new GLuint[m_indicesArraySize];
	cnt = 0;
	for( int iu = 0; iu < gridU; ++ iu ) {                               // 端から一段目のプリミティブの計算
		m_indices[cnt++] = 0;				                                 // 三角形の1つ目の頂点は端の点から始まる 
		m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1);                   // 0 -> 2 -> 1 の順番(表面に対して時計回り)
		m_indices[cnt++] = (GLuint)(iu + 1);                                 // 余りを求めているのは最後のプリミティブの計算時に最初の頂点を使うため
	}
	for( int iv = 1; iv < gridV1 - 2; ++ iv ) {                          // 2段目以降、最終段1つ手前まで(リングの形)
		for( int iu = 0; iu < gridU - 1; ++ iu ) {                           
			int m = (iv - 1) * gridU;                                    // mは段数によるオフセット
			//Triangle A                                                 // TriangleAは小さい段数に2頂点を持つ三角形
			m_indices[cnt++] = (GLuint)(iu + 1 + m);                         // 時計回り方向
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
			m_indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			
			//Triangle B
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + gridU + m);   //TriangleBは大きい段数に2頂点を持つ三角形
			m_indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
		}
	}
	int n = m_indicesArraySize - 1;      // 頂点の最後のインデックス番号
	for( int iu = n - gridU; iu < n; ++ iu ) {                          // 最終段のプリミティブの計算  
		m_indices[cnt++] = (GLuint)iu;                                    // ・・・　-> [(n-1) -> (n-2) -> (n)] の順番 (表面に対して時計回り)
		m_indices[cnt++] = (GLuint)(iu % gridU + n - gridU );				// 余りを求めているのは一周して最初の頂点を使うときのため
		m_indices[cnt++] = (GLuint)n;                                     // 端の頂点
	}

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, m_buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}


//-----------------------------------------------------------------------------
// 説明: 直方体型のメッシュをVBOで定義する
// 引数: 
// 返り値:
// その他: 
//       DrawFigure::Initialize()で呼ばれる
//-----------------------------------------------------------------------------
void CDrawFigure::initCubeVBO()
{
	int cnt = 0;                        // 現在の配列を示す

	//頂点配列と法線配列を格納する
    unsigned int verticesArraySize = (4 * 6) * 3;
	m_vertices = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
	m_normals = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
    
	// 前面
	Vector3f normal = Vector3f(1.f, 0.f, 0.f);
    Vector3f vertex = Vector3f(1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

    // 背面
	normal = Vector3f(-1.f, 0.f, 0.f);
	vertex = Vector3f(-1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	// 左面
	normal = Vector3f(0.f, -1.f, 0.f);
    vertex = Vector3f(-1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	// 右面
	normal = Vector3f(0.f, 1.f, 0.f);
	vertex = Vector3f(-1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	// 上面
	normal = Vector3f(0.f, 0.f, 1.f);
	vertex = Vector3f(1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, -1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 2.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	// 下面
	normal = Vector3f(0.f, 0.f, 1.f);
	vertex = Vector3f(1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, 1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	vertex = Vector3f(-1.f, -1.f, 0.f); 
	m_vertices[cnt++] = vertex.x();  m_vertices[cnt++] = vertex.y();  m_vertices[cnt++] = vertex.z();
	m_normals[cnt-3] = normal.x();    m_normals[cnt-2] = normal.y();    m_normals[cnt-1] = normal.z();

	//インデックス配列をセット(三角形プリミティブの頂点レンダリング順番を格納)
    m_indicesArraySize = 3 * 2 * 6;
	m_indices= new GLuint[m_indicesArraySize];
	cnt = 0;
	for( int i = 0; i < 6; i++) {                               
			m_indices[cnt++] = i * 4;				                
			m_indices[cnt++] = i * 4 + 1;				            
			m_indices[cnt++] = i * 4 + 3;				            

			m_indices[cnt++] = i * 4 + 1;				            
			m_indices[cnt++] = i * 4 + 2;				            
			m_indices[cnt++] = i * 4 + 3;				            
  	}

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, m_buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}


/***********************************************************************************************/
/************************************    補助関数    *******************************************/
/***********************************************************************************************/

void CDrawFigure::DrawLocalAxes(float length, float width)
{
	glLineWidth(width);
	float distance = length * 0.08;

	setMaterial(Vector3f(0.55f, 0.f, 0.f),1.f, 0.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex3d(distance, distance, distance);
	glVertex3d(distance + length, distance, distance);
	glEnd();
		
	setMaterial(Vector3f(0.f, 0.39f, 0.f),1.f, 0.f, 1.f, 1.f);
	glBegin(GL_LINES);		
	glVertex3d(distance, distance, distance);
	glVertex3d(distance, distance + length, distance);
	glEnd();
		
	setMaterial(Vector3f(0.1f, 0.1f, 0.44f),1.f, 0.f, 1.f, 1.f);
	glBegin(GL_LINES);		
	glVertex3d(distance, distance, distance);
	glVertex3d(distance, distance, distance + length);
	glEnd();	
}