#include "DrawMocapData.h"

#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

DrawMocapData::DrawMocapData()
{
	Initialize();
}

DrawMocapData::DrawMocapData(string file_name)
{//Bvh専用
	Initialize();
	this->LoadMotion( file_name);
}


void DrawMocapData::Initialize()
{
	initialize(); //implemented in MocapData

	//scale = 2.0;
	scale_ = 1.0;
	sect_ = 20;

	color_ = klVec3(0.85, 0.66, 0.15);
	//color = Color::getColor3( Color::puppet_pink );
	//color = Color::getColor3( Color::blue );
	specular_ = 0;
	amb_ = 0.5;
	diffuse_ = 1.0;
	renderingMethod_ = 1;
	alpha_ = 1.0;
	//alpha = 0.4;

	isDrawLocalAxes = false;
	//isDrawLocalAxes = true;
	isDrawLocalLine_ = false;
	isDrawRootToEffecterLine_ = false;

	indicesArraySize_ = 0;
	isDrawCylinderModel_ = true;
    //isDrawCylinderModel_ = false;
	if(isDrawCylinderModel_){
	    initCubeVBO();
	    //initCylinderVBO();
		//initSphereVBO();
	}

	//isSensor = false;
	pos = klVec3(0,0,0);
}

void DrawMocapData::DrawFrame( int frame, bool apply_root_translation)
{
	Pose& pose = poses[frame];
	Skeleton* skl = pose.getSkeleton();
	vector<int> parent_num_stack;
	
	glPushMatrix();
	{
		//pos correction( multiply Tpos to M from right )
		glTranslated( this->pos.x, this->pos.y, this->pos.z);

		//scaling( multiply Mscale to M from right )
		glScaled(this->scale_, this->scale_, this->scale_);

		//Quatf correct_q =  rotation( Converter::toVector3f(klVec3(1,0,0)), Converter::toVector3f(klVec3(0,0,-1)) );
		//Quatf correct_q =  rotation( Converter::toVector3f(klVec3(0,0,-1)), Converter::toVector3f(klVec3(1,0,0)) );
		//glRotate( correct_q );


		//draw auxiliary lines for observation
		//DrawAuxiliaryLines_(frame, this->skeleton->getSkeletonType() );

		//root corection( multiply Troot to M from right )
		
		if( apply_root_translation ) glTranslated( pose.getRootPosition().x, pose.getRootPosition().y, pose.getRootPosition().z );//Troot
		else glTranslated( 0 ,0 ,1 );//Troot

		//root corection( multiply Rroot to M from right )
		glRotate( pose.getRootAngle());

		//push root's parent number on the stack 
		parent_num_stack.push_back(-1);

		//follow bones
		for(int bone_num = 0; bone_num < pose.getBoneNum(); bone_num++)
		{
			Bone& bone = *skl->bones[bone_num];

			//pop stack
			while(parent_num_stack.back() != bone.parent)
			{
				parent_num_stack.pop_back();
				glPopMatrix();
			}
			//// Now, matrix M                          ////
			////  = TposMscaleTrootRrootR0T0...Ri-2Ti-2 ////

			//push M on the Matrix stack,
			//and push parent num on the parent num stack 
			glPushMatrix();
			parent_num_stack.push_back(bone_num);
		
			//draw axes
			//if(this->isDrawLocalAxes)
			//{
			//	DrawNeedLocalAxes(bone.name, 0.15, 3.0);
			//}
			
			//bone start point
			klVec3 bone_start = klVec3(0,0,0);
			
			//multiply Ri-1 to M from right
			glRotate( pose.getBoneAngle(bone_num) );

			//render ellipsoid from bone end point to bone start point
			klVec3 bone_end = klVec3(bone.direction.x, bone.direction.y, bone.direction.z);
			bone_end *= bone.length;


			////klVec4 temp_color( color.r, color.g, color.b, alpha);
			////Color::setMaterial(temp_color, this->specular, this->diffuse, this->amb);
			double temp_alpha = this->alpha_;
			////if( ! (bone.name == "rfemur_phantom" || bone.name == "rfemur" || bone.name == "rtibia" || bone.name == "rfoot"|| bone.name == "rtoesw"))
			//if( ! (bone.name == "rhand" || bone.name == "rradius" || bone.name == "rhumerus" || bone.name == "rclavicle"))
			////if(false)
			//{
			//	//if( this->alpha < 0.99 ) temp_alpha = 0.0;
			//}

			setMaterial( klVec4( color_, temp_alpha), specular_, diffuse_, amb_);
			//QGLColor::setMaterial(this->color, this->specular, this->diffuse, this->amb, temp_alpha);
			//Color::setMaterial(this->color, this->specular, this->diffuse, this->amb, 1.0);
			//QGLUtil::DrawEllipsoidalBody( bone_start, bone_end, this->sect, bone.radius);
			DrawEllipsoidalBody( bone_start, bone_end, this->sect_, bone.radius);
			
			//multiply Ti-1 to M from right
			glTranslated(bone_end.x, bone_end.y, bone_end.z);

			//// Now, matrix M                          ////
			////  = TposMscaleTrootRrootR0T0...Ri-1Ti-1 ////
		}
		
		//clean Matrix stack
		//while (!parent_num_stack.empty())
		while (parent_num_stack.size() >= 1)
		{
			glPopMatrix();
			parent_num_stack.pop_back();
		}			
	}
	//pop TposMscaleTrootRroot
	glPopMatrix();
}



void DrawMocapData::glRotate(klQuat q) {
	double mat[16];
	q.ToMatrix(mat);
	glMultMatrixd(mat);
}

void DrawMocapData::setMaterial(klVec4 color, double specular_rate, double diffuse_rate, double amb_rate)
{
	klVec4 spc = color * specular_rate;
	spc.a = color.a;

	klVec4 dif = color * diffuse_rate;
	dif.a = color.a;

	klVec4 amb = color * amb_rate;
	amb.a = color.a;

	glColor4fv(color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spc);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
}

void DrawMocapData::DrawEllipsoidalBody(klVec3 pos1, klVec3 pos2, int sect, double radius) 
{
	klVec3 p,v1,v2,v3;
	p = (pos1 + pos2) /2;
	v1 = (pos1 - pos2) /2;
  	float length = (pos1 - pos2).Length();	

	//auxiliary vector(補助)
	klVec3 temp(1,1,1);

    if(isDrawCylinderModel_)
	{
		v2 = v1 % temp;
		v2.Normalize();
		v2 = v2 * 0.02f;
		v3 = v1 % v2;
		v3.Normalize();
		v3 = v3 * 0.02f;

    	DrawBone(pos2, v1, v2, v3, length, (float)radius); //pos2の位置を原点にする
	}
	else
	{
		v2 = v1 % temp;
		v2.Normalize();
		v2 = v2 * radius;
		v3 = v1 % v2;
		v3.Normalize();
		v3 = v3 * radius;

		DrawEllipsoid(p, v1, v2, v3, sect);
	}

}

void DrawMocapData::DrawEllipsoid(klVec3 p, klVec3 v1, klVec3 v2, klVec3 v3, int sect)
{ 
	GLfloat mat[16]; 
	mat[0] = v1.x; mat[1] = v1.y; mat[2] = v1.z;  mat[3] = 0;
	mat[4] = v2.x; mat[5] = v2.y; mat[6] = v2.z;  mat[7] = 0;
	mat[8] = v3.x; mat[9] = v3.y; mat[10] = v3.z; mat[11] = 0;
	mat[12] = p.x; mat[13] = p.y; mat[14] = p.z;  mat[15] = 1;
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

	//glutSolidSphere(1, sect, sect);

	glDisable(GL_BLEND);

	glDisable(GL_NORMALIZE);
	glPopMatrix();
}

void DrawMocapData::DrawBone(klVec3 p, klVec3 v1, klVec3 v2, klVec3 v3, float length, float radius)
{
	// 図形の中心位置に移動するための行列を作成
	GLfloat mat[16]; 
	mat[8] = v1.x; mat[9] = v1.y; mat[10] = v1.z;  mat[11] = 0;
	mat[4] = v2.x; mat[5] = v2.y; mat[6] = v2.z;  mat[7] = 0;
	mat[0] = v3.x; mat[1] = v3.y; mat[2] = v3.z; mat[3] = 0;
	mat[12] = p.x; mat[13] = p.y; mat[14] = p.z;  mat[15] = 1;

 	glPushMatrix();
	glMultMatrixf(mat);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DrawVBO();

	glDisable(GL_BLEND);

	glDisable(GL_NORMALIZE);
	glPopMatrix();
}

//
void DrawMocapData::DrawVBO()
{
    /* 頂点データの場所を指定する */
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	//頂点に与えるデータ数:3, データ型:float, データ間隔:0, データ格納場所:0(BindBufferによってbuffers[0]のポインタが既にバインドされているので、オフセットを渡すだけになる)
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* 法線データの場所を指定する */
	//データ型:float, データ間隔:0, データ格納場所:0(BindBufferによってbuffers[1]のポインタが既にバインドされているので、オフセットを渡すだけになる)
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* テクスチャ座標の場所を指定する */
    //glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    //glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* 頂点のインデックスの場所を指定して図形を描く */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    //glEnable(GL_TEXTURE_2D);

    /* 頂点データ，法線データ，テクスチャ座標の配列を有効にする */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//図形タイプ:Triangle_FAN, 描画する要素数(頂点数):上底と下底, 頂点インデックスの型:unsigned int, データ格納場所:0(BindBufferによってbuffers[1]のポインタが既にバインドされているので、オフセットを渡すだけになる)
	glDrawElements(GL_TRIANGLES, (GLsizei)indicesArraySize_, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	//glDisable(GL_TEXTURE_2D);

	//　バインドしたものをもどす
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* 頂点データ，法線データ，テクスチャ座標の配列を無効にする */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void DrawMocapData::initCylinderVBO()
{
    unsigned int verticesNum = sect_ + sect_ + sect_ * 2; //上底 + 下底 + 側面
	indicesArraySize_ = ((sect_-2) * 3) * 2 + (sect_ * 2) * 3; // 上底(三角形)、下底(三角形)、側面(三角形*2で四角形)

	vertices = new GLfloat[ verticesNum * 3 ];
	normals = new GLfloat[ verticesNum * 3 ];
	indices = new GLuint[ indicesArraySize_ ];


	float step = (M_PI * 2) / (float)sect_;
	float radius = 1.f;
	float height = 2.f; 

    //
    //  頂点、法線、色の設定
	//
    unsigned int vertIndex = 0;

	//上底
	for(unsigned int i=0; i < (unsigned int)sect_; i++)
	{
        float t = step * (float)i;

		normals[vertIndex * 3] = 0.f;
		normals[vertIndex * 3 + 1] = 0.f;
		normals[vertIndex * 3 + 2] = 1.f;

		vertices[vertIndex * 3] = radius * cos(t);
		vertices[vertIndex * 3 + 1] = radius * sin(t);
		vertices[vertIndex * 3 + 2] = height;

	    vertIndex++;
	}

    //　下底
	for(int i = sect_; --i >= 0;)
	{
	    float t = step * (float)i;

		normals[vertIndex * 3] = 0.f;
		normals[vertIndex * 3 + 1] = 0.f;
		normals[vertIndex * 3 + 2] = -1.f;

		vertices[vertIndex * 3] = radius * cos(t);
		vertices[vertIndex * 3 + 1] = radius * sin(t);
		vertices[vertIndex * 3 + 2] = 0.f;

	    vertIndex++;
	}

    // 側面
	for(unsigned int i=0; i < (unsigned int)sect_; i++)
	{
        float t = step * (float)i;
        float y = sin(t);
        float x = cos(t);

		normals[vertIndex * 3] = x;
		normals[vertIndex * 3 + 1] = y;
		normals[vertIndex * 3 + 2] = 0.f;

		vertices[vertIndex * 3] = radius * x;
		vertices[vertIndex * 3 + 1] = radius * y;
		vertices[vertIndex * 3 + 2] = height;

		normals[(vertIndex+1) * 3] = x;
		normals[(vertIndex+1) * 3 + 1] = y;
		normals[(vertIndex+1) * 3 + 2] = 0.f;

		vertices[(vertIndex+1) * 3] = radius * x;
		vertices[(vertIndex+1) * 3 + 1] = radius * y;
		vertices[(vertIndex+1) * 3 + 2] = 0.0;

	    vertIndex += 2;
	}

    //
	// 頂点インデックスの設定
	//
    vertIndex = 0;

	// 上底
    for( int i=0; i < sect_ - 2; i++)
	{
		indices[vertIndex] = 0;
		indices[vertIndex+1] = i+1;
        indices[vertIndex+2] = i+2;

        vertIndex += 3;
	}

	// 下底
    for( int i = sect_; i < sect_ * 2 - 2; i++)
	{
        indices[vertIndex] = sect_;
		indices[vertIndex+1] = i+1;
        indices[vertIndex+2] = i+2;

		vertIndex +=3;
	}

	// 側面
	for( int i = sect_ * 2; i < sect_ * 4 - 2; i+=2)
	{
        indices[vertIndex] = i;
		indices[vertIndex+1] = i + 1;
		indices[vertIndex+2] = i + 2;

		indices[vertIndex+3] = i + 2;
		indices[vertIndex+4] = i + 1;
		indices[vertIndex+5] = i + 3;

		vertIndex += 6;
	}

    //最後の面2つは最初の頂点を使う
    indices[vertIndex] = sect_ * 4 - 2;
	indices[vertIndex+1] = sect_ * 4 -1;
	indices[vertIndex+2] = sect_ * 2;

    indices[vertIndex+3] = sect_ * 2;
	indices[vertIndex+4] = sect_ * 4 - 1;
	indices[vertIndex+5] = sect_ * 2 + 1;

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat) , vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat), normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesArraySize_ * sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

}


void DrawMocapData::initSphereVBO()
{
	float mRadius = 1.f;
    float zOffset = 1.f;

	int gridU = sect_;
	int gridV = sect_;
	int gridU1 = gridU + 1;
	int gridV1 = gridV + 1;
	float incU = 360 / gridU;           // U方向に何分割するか？(円を扇形に分解する方向)
	float incV = 2 * mRadius / gridV;   // V方向に何分割するか？(球を円に分割する方向)
	int cnt = 0;                        // 現在の配列を示す

				
	//頂点配列と法線配列を格納する
    unsigned int verticesArraySize = (2 + (gridV1-2) * gridU) * 3;
	vertices = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
	normals = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)


	vertices[cnt++] = 0;                               //Y軸での端の頂点座標
	vertices[cnt++] = -mRadius; 
	vertices[cnt++] = 0 + zOffset;
    klVec3 normal = klVec3(0, -mRadius, 0);
    normal.Normalize();
    normals[cnt-3] = normal.x;
    normals[cnt-2] = normal.y;
    normals[cnt-1] = normal.z;

	float d = mRadius;
	float y, t, r;
	for( int iv = 1; iv < gridV1 - 1; ++ iv ) {            // 球を円に分解する
		y = iv * incV - d;                                     // Y軸上での値(V方向の分割値)
		r = sqrt(d * d - y * y);                          // 現在のY値上での円の半径
		for( int iu = 0; iu < gridU; ++ iu ) {             // 円を扇形に分解する
			t = iu * incU * M_PI / 180;                     // 扇形の角度(Radian)
			vertices[cnt++] = (GLfloat)(r * cos(t));      // 頂点座標を格納(XZ平面でプロット。
			vertices[cnt++] = (GLfloat)y;                      // Z=0のとき、つまりX軸上の位置から開始
			vertices[cnt++] = (GLfloat)(r * sin(t)) + zOffset;      // 時計回りに描写(X軸からZ軸方向への向き) 

			normal = klVec3(vertices[cnt-3], vertices[cnt-2], vertices[cnt-1]);
			normal.Normalize();
			normals[cnt-3] = normal.x;
			normals[cnt-2] = normal.y;
			normals[cnt-1] = normal.z;
		}
	}
	vertices[cnt++] = 0;                               //Y軸での端の頂点座標
	vertices[cnt++] = mRadius;
	vertices[cnt++] = 0 + zOffset;			

	normal = klVec3(vertices[cnt-3], vertices[cnt-2], vertices[cnt-1]);
	normal.Normalize();
    normals[cnt-3] = normal.x;
	normals[cnt-2] = normal.y;
	normals[cnt-1] = normal.z;

	//インデックス配列をセット(三角形プリミティブの頂点レンダリング順番を格納)
    indicesArraySize_ = ( ( gridV - 2) * (gridU - 1) * 2 + (gridU * 2) ) * 3;
	indices = new GLuint[indicesArraySize_];
	cnt = 0;
	for( int iu = 0; iu < gridU; ++ iu ) {                               // 端から一段目のプリミティブの計算
		indices[cnt++] = 0;				                                 // 三角形の1つ目の頂点は端の点から始まる 
		indices[cnt++] = (GLuint)((iu + 1) % gridU + 1);                   // 0 -> 2 -> 1 の順番(表面に対して時計回り)
		indices[cnt++] = (GLuint)(iu + 1);                                 // 余りを求めているのは最後のプリミティブの計算時に最初の頂点を使うため
	}
	for( int iv = 1; iv < gridV1 - 2; ++ iv ) {                          // 2段目以降、最終段1つ手前まで(リングの形)
		for( int iu = 0; iu < gridU - 1; ++ iu ) {                           
			int m = (iv - 1) * gridU;                                    // mは段数によるオフセット
			//Triangle A                                                 // TriangleAは小さい段数に2頂点を持つ三角形
			indices[cnt++] = (GLuint)(iu + 1 + m);                         // 時計回り方向
			indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
			indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			
			//Triangle B
			indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + gridU + m);   //TriangleBは大きい段数に2頂点を持つ三角形
			indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
		}
	}
	int n = indicesArraySize_ - 1;      // 頂点の最後のインデックス番号
	for( int iu = n - gridU; iu < n; ++ iu ) {                          // 最終段のプリミティブの計算  
		indices[cnt++] = (GLuint)iu;                                    // ・・・　-> [(n-1) -> (n-2) -> (n)] の順番 (表面に対して時計回り)
		indices[cnt++] = (GLuint)(iu % gridU + n - gridU );				// 余りを求めているのは一周して最初の頂点を使うときのため
		indices[cnt++] = (GLuint)n;                                     // 端の頂点
	}

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesArraySize_ * sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

}

void DrawMocapData::initCubeVBO()
{
	int cnt = 0;                        // 現在の配列を示す

	//頂点配列と法線配列を格納する
    unsigned int verticesArraySize = (4 * 6) * 3;
	vertices = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
	normals = new GLfloat[verticesArraySize]; // {端の頂点＋(Y軸方向への*扇形への分割数)} * (X,Y,Z)
    
	// 前面
	klVec3 normal = klVec3(1.f, 0.f, 0.f);
    klVec3 vertex = klVec3(1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

    // 背面
	normal = klVec3(-1.f, 0.f, 0.f);
	vertex = klVec3(-1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	// 左面
	normal = klVec3(0.f, -1.f, 0.f);
    vertex = klVec3(-1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	// 右面
	normal = klVec3(0.f, 1.f, 0.f);
	vertex = klVec3(-1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	// 上面
	normal = klVec3(0.f, 0.f, 1.f);
	vertex = klVec3(1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, -1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 2.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	// 下面
	normal = klVec3(0.f, 0.f, 1.f);
	vertex = klVec3(1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, 1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	vertex = klVec3(-1.f, -1.f, 0.f); 
	vertices[cnt++] = vertex.x;  vertices[cnt++] = vertex.y;  vertices[cnt++] = vertex.z;
	normals[cnt-3] = normal.x;    normals[cnt-2] = normal.y;    normals[cnt-1] = normal.z;

	//インデックス配列をセット(三角形プリミティブの頂点レンダリング順番を格納)
    indicesArraySize_ = 3 * 2 * 6;
	indices = new GLuint[indicesArraySize_];
	cnt = 0;
	for( int i = 0; i < 6; i++) {                               
			indices[cnt++] = i * 4;				                
			indices[cnt++] = i * 4 + 1;				            
			indices[cnt++] = i * 4 + 3;				            

			indices[cnt++] = i * 4 + 1;				            
			indices[cnt++] = i * 4 + 2;				            
			indices[cnt++] = i * 4 + 3;				            
  	}

   /* バッファオブジェクトの名前を3つ作る */
   glGenBuffers(3, buffers);
  
   /* １つ目のバッファオブジェクトに頂点データ配列を転送する */
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , vertices, GL_DYNAMIC_DRAW);
  
  /* ２つ目のバッファオブジェクトに法線データ配列を転送する */
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), normals, GL_DYNAMIC_DRAW);
  
  /* ３つ目のバッファオブジェクトに頂点のインデックスを転送する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesArraySize_ * sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

}
