//-----------------------------------------------------------------------------
//                             DrawFigure.cpp
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

#include "DrawFigure.h"
#include "MotionFile.h"

//-----------------------------------------------------------------------------
// ����: �R���X�g���N�^
// ����: 
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
CDrawFigure::CDrawFigure()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// ����: �R���X�g���N�^
// ����: 
//       fileName [in] �t�@�C����
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
CDrawFigure::CDrawFigure(std::string bvhFileName)
{
	Initialize();
	this->LoadBVH( bvhFileName);
}


//-----------------------------------------------------------------------------
// ����: �������֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
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
// ����: BVH�t�@�C���̓ǂݍ���
// ����: 
//       bvhFileName [in] �t�@�C����
// �Ԃ�l:
// ���̑�: 
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
// ����: �{�[����GL�ł̕`��֐�
// ����: 
//      pJoint [in] �`�悷��{�[��
// �Ԃ�l:
// ���̑�: 
//       m_figure��CFigure::setPose()�Ŏp�������炩���߃Z�b�g����Ă���
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMesh(const CJoint* joint)
{
	// ���[�J�����W�n��`�悷�邩�ǂ���
	//if( m_isDrawLocalAxes )
	DrawLocalAxes();

	// �{�[���̊J�n�ʒu
	Vector3f boneStart = Vector3f(0,0,0);

	// �{�[���̏I���ʒu����{�[���̊J�n�ʒu�Ɍ����ă��b�V���������_�����O
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
// ����: ���i��GL��ŕ`�ʂ���
// ����: 
//      pJoint [in] �`��Ώۂ̊֐�
// �Ԃ�l:
// ���̑�: 
//       m_figure��CFigure::setPose()�Ŏp�������炩���߃Z�b�g����Ă���
//-----------------------------------------------------------------------------
void CDrawFigure::DrawSkeletonSub(const CJoint* joint, unsigned int childNum, std::stack<int> matStack)
{
	if(joint == NULL)
		return;

	//// �s�� "M" ��
	//// TposMScaleTrootRrootT0R0...Ti-2Ri-2

	// �s����X�^�b�N�ɒǉ����� push M on the Matrix stack,
	// �eJoint�̃J�E���g�𑝂₷
	glPushMatrix();
	matStack.push(childNum);
	{
		//���b�V���̕`��
		DrawMesh(joint);

		// "Ti-1" �̈ړ��s����s�� "M" �ɉE���炩����
		Vector3f offset = joint->GetOffset();
		glTranslatef(offset.x(), offset.y(), offset.z());

		// �s�� "M" ��
		// TposMscaleTrootRrootR0T0...Ti-1

		//Ri-1�̉�]�s����s��ɉE���炩����
		Matrix4f tmp;
		tmp = joint->GetRotationMatrix();
		glMultMatrixf(tmp.data());

		// �s�� "M" ��
		// TposMscaleTrootRrootR0T0...Ti-1Ri-1
		for(unsigned int j = 0; j < joint->NumChildren(); j++)
		{
			DrawSkeletonSub(joint->GetChild(j), j, matStack);
		}
	}
	// �s����X�^�b�N������o�� ,
	// �eJoint�̃J�E���g�����炷
	glPopMatrix();
	matStack.pop();

}

//-----------------------------------------------------------------------------
// ����: ���炩���߃Z�b�g�����p����GL��ŕ`�ʂ���
// ����: 
// �Ԃ�l:
// ���̑�: 
//       m_figure��CFigure::setPose()�Ŏp�������炩���߃Z�b�g����Ă���
//-----------------------------------------------------------------------------
void CDrawFigure::DrawSkeleton()
{
	if( m_figure == NULL)
		return;

	const CJoint *root;

	// �X�^�b�N�̌v�Z
	std::stack<int> matStack;

	// �ʒu�̏C�� ( �s�� "M" �� �ʒu "TPos" ���E���炩���� )
	glTranslatef( m_pos.x(), m_pos.y(), m_pos.z());

	// �X�P�[�����O( �s�� "M" �ɃX�P�[�� "Mscale" ���E���炩���� )
	glScalef(m_scale, m_scale, m_scale);

	glPushMatrix();
	{
		root = m_figure->GetRoot();
		glPushMatrix();
		{
			// ���[�g�ʒu�̏C��( �s�� "M" �� ���[�g�ʒu "Troot" ���E���炩���� )		
			Vector3f rootPos = root->GetPosition();
			glTranslatef( rootPos.x(), rootPos.y(), rootPos.z() );
			// ���[�g�ʒu�̏C�� ( �s�� "M" �� ���[�g��]�s�� "Rroot" ���E���炩���� )
			Matrix4f tmp = root->GetRotationMatrix();
			glMultMatrixf(tmp.data());
			// ���[�g�̐e�i���݂��Ȃ�)�̔ԍ����X�^�b�N�ɐς�)
			matStack.push(-1);
			// �{�[����`�悷��
			for(unsigned int i = 0; i < root->NumChildren(); i++)
			{
				DrawSkeletonSub(root->GetChild(i), i, matStack);
			}
			// ���[�g�̐e���|�b�v����
			matStack.pop();
		}
		glPopMatrix();
		// �X�^�b�N���� TrootRroot ���|�b�v����
	}
	glPopMatrix();
	// �X�^�b�N���� TposMscale ���|�b�v����
	
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
// ����: �w�肵���t���[���̎p����GL��ŕ`�ʂ���
// ����: 
//       frame [in] �`�悷��p���̃t���[��
// �Ԃ�l:
// ���̑�: 
//       m_figure��CFigure::setPose()�Ŏp�������炩���߃Z�b�g����Ă���
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
// ����: �`�悷��ۂ̃}�e���A����ݒ肷��
// ����: 
//       color [in] �ݒ肷��F
//       specular_rate [in] �e�֐߃m�[�h�ւ̃|�C���^
// �Ԃ�l:

// ���̑�: 
//       m_figure��CFigure::setPose()�Ŏp�������炩���߃Z�b�g����Ă���
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
// ����: ���b�V����GL�ŕ`�悷��⏕�֐�
// ����: 
//       pos1 [in] �{�[���̊J�n�n�_
//       pos2 [in] �{�[���̏I���n�_
//       sect [in] �|���S���̕�����
//       radius [in] �{�[���̑���(���a)
// �Ԃ�l:
// ���̑�: 
//       
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshSub(Vector3f pos1, Vector3f pos2, int sect, float radius) 
{
	Vector3f p,v1,v2,v3;
	p = (pos1 + pos2) /2;
	v1 = (pos1 - pos2) /2;
  	float length = (pos1 - pos2).norm();

	//auxiliary vector(�⏕)
	Vector3f temp(1,1,1);

    if(m_isDrawCylinderModel)
	{
		v2 = v1.cross(temp);
		v2 = v2.normalized();
		v2 = v2 * 0.02f;
		v3 = v1.cross(v2);
		v3 = v3.normalized();
		v3 = v3 * 0.02f;

    	DrawMeshVBO(pos2, v1, v2, v3, length, (float)radius); //pos2�̈ʒu�����_�ɂ���
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
// ����: ���b�V����ȉ~���ŕ`�悷��֐�
// ����: 
//       p [in] �ȉ~���̒��S�ʒu
//       v1 [in] X�������̐L�k��
//       v2 [in] Y�������̐L�k��
//       v3 [in] Z�������̐L�k��
//       sect [in] �|���S���̕�����
// �Ԃ�l:
// ���̑�: 
//       �c�߂���ԏ�ɋ���`�悷�邱�Ƃőȉ~���ɕς���
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

    // glu�ŋ���`��
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
// ����: ���b�V����ȉ~���ŕ`�悷��֐�
// ����: 
//       p [in] �ȉ~���̒��S�ʒu
//       v1 [in] X�������̐L�k��
//       v2 [in] Y�������̐L�k��
//       v3 [in] Z�������̐L�k��
//       sect [in] �|���S���̕�����
// �Ԃ�l:
// ���̑�: 
//       �c�߂���ԏ�ɋ���`�悷�邱�Ƃőȉ~���ɕς���
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshVBO(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, float length, float radius)
{
	// �}�`�̒��S�ʒu�Ɉړ����邽�߂̍s����쐬
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
// ����: ���b�V����VBO��p���ĕ`�悷��⏕�֐�
// ����: 
// �Ԃ�l:
// ���̑�: 
//       ���b�V���̌`�� initCylinderVBO()�AinitSphereVBO()�AinitCubeVBO() �ŗ\�ߒ�`���Ă���
//-----------------------------------------------------------------------------
void CDrawFigure::DrawMeshVBOSub()
{
    /* ���_�f�[�^�̏ꏊ���w�肷�� */
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
	//���_�ɗ^����f�[�^��:3, �f�[�^�^:float, �f�[�^�Ԋu:0, �f�[�^�i�[�ꏊ:0(BindBuffer�ɂ����m_buffers[0]�̃|�C���^�����Ƀo�C���h����Ă���̂ŁA�I�t�Z�b�g��n�������ɂȂ�)
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* �@���f�[�^�̏ꏊ���w�肷�� */
	//�f�[�^�^:float, �f�[�^�Ԋu:0, �f�[�^�i�[�ꏊ:0(BindBuffer�ɂ����m_buffers[1]�̃|�C���^�����Ƀo�C���h����Ă���̂ŁA�I�t�Z�b�g��n�������ɂȂ�)
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
    glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* �e�N�X�`�����W�̏ꏊ���w�肷�� */
    //glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
    //glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
  
    /* ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�� */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
    //glEnable(GL_TEXTURE_2D);

    /* ���_�f�[�^�C�@���f�[�^�C�e�N�X�`�����W�̔z���L���ɂ��� */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//�}�`�^�C�v:Triangle_FAN, �`�悷��v�f��(���_��):���Ɖ���, ���_�C���f�b�N�X�̌^:unsigned int, �f�[�^�i�[�ꏊ:0(BindBuffer�ɂ����m_buffers[1]�̃|�C���^�����Ƀo�C���h����Ă���̂ŁA�I�t�Z�b�g��n�������ɂȂ�)
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indicesArraySize, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	//glDisable(GL_TEXTURE_2D);

	//�@�o�C���h�������̂����ǂ�
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* ���_�f�[�^�C�@���f�[�^�C�e�N�X�`�����W�̔z��𖳌��ɂ��� */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//-----------------------------------------------------------------------------
// ����: �V�����_�^�̃��b�V����VBO�Œ�`����
// ����: 
// �Ԃ�l:
// ���̑�: 
//       DrawFigure::Initialize()�ŌĂ΂��
//-----------------------------------------------------------------------------
void CDrawFigure::initCylinderVBO()
{
    unsigned int verticesNum = m_sect + m_sect + m_sect * 2; //��� + ���� + ����
	m_indicesArraySize = ((m_sect-2) * 3) * 2 + (m_sect * 2) * 3; // ���(�O�p�`)�A����(�O�p�`)�A����(�O�p�`*2�Ŏl�p�`)

	m_vertices = new GLfloat[ verticesNum * 3 ];
	m_normals = new GLfloat[ verticesNum * 3 ];
	m_indices= new GLuint[ m_indicesArraySize ];


	float step = (M_PI * 2) / (float)m_sect;
	float radius = 1.f;
	float height = 2.f; 

    //
    //  ���_�A�@���A�F�̐ݒ�
	//
    unsigned int vertIndex = 0;

	//���
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

    //�@����
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

    // ����
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
	// ���_�C���f�b�N�X�̐ݒ�
	//
    vertIndex = 0;

	// ���
    for( int i=0; i < m_sect - 2; i++)
	{
		m_indices[vertIndex] = 0;
		m_indices[vertIndex+1] = i+1;
        m_indices[vertIndex+2] = i+2;

        vertIndex += 3;
	}

	// ����
    for( int i = m_sect; i < m_sect * 2 - 2; i++)
	{
        m_indices[vertIndex] = m_sect;
		m_indices[vertIndex+1] = i+1;
        m_indices[vertIndex+2] = i+2;

		vertIndex +=3;
	}

	// ����
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

    //�Ō�̖�2�͍ŏ��̒��_���g��
    m_indices[vertIndex] = m_sect * 4 - 2;
	m_indices[vertIndex+1] = m_sect * 4 -1;
	m_indices[vertIndex+2] = m_sect * 2;

    m_indices[vertIndex+3] = m_sect * 2;
	m_indices[vertIndex+4] = m_sect * 4 - 1;
	m_indices[vertIndex+5] = m_sect * 2 + 1;

   /* �o�b�t�@�I�u�W�F�N�g�̖��O��3��� */
   glGenBuffers(3, m_buffers);
  
   /* �P�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�f�[�^�z���]������ */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* �Q�ڂ̃o�b�t�@�I�u�W�F�N�g�ɖ@���f�[�^�z���]������ */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesNum * 3 * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* �R�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�̃C���f�b�N�X��]������ */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}

//-----------------------------------------------------------------------------
// ����: ���^�̃��b�V����VBO�Œ�`����
// ����: 
// �Ԃ�l:
// ���̑�: 
//       DrawFigure::Initialize()�ŌĂ΂��
//-----------------------------------------------------------------------------
void CDrawFigure::initSphereVBO()
{
	float mRadius = 1.f;
    float zOffset = 1.f;

	int gridU = m_sect;
	int gridV = m_sect;
	int gridU1 = gridU + 1;
	int gridV1 = gridV + 1;
	float incU = 360 / gridU;           // U�����ɉ��������邩�H(�~���`�ɕ����������)
	float incV = 2 * mRadius / gridV;   // V�����ɉ��������邩�H(�����~�ɕ����������)
	int cnt = 0;                        // ���݂̔z�������

				
	//���_�z��Ɩ@���z����i�[����
    unsigned int verticesArraySize = (2 + (gridV1-2) * gridU) * 3;
	m_vertices = new GLfloat[verticesArraySize]; // {�[�̒��_�{(Y�������ւ�*��`�ւ̕�����)} * (X,Y,Z)
	m_normals = new GLfloat[verticesArraySize]; // {�[�̒��_�{(Y�������ւ�*��`�ւ̕�����)} * (X,Y,Z)


	m_vertices[cnt++] = 0;                               //Y���ł̒[�̒��_���W
	m_vertices[cnt++] = -mRadius; 
	m_vertices[cnt++] = 0 + zOffset;
    Vector3f normal = Vector3f(0, -mRadius, 0);
    normal = normal.normalized();
    m_normals[cnt-3] = normal.x();
    m_normals[cnt-2] = normal.y();
    m_normals[cnt-1] = normal.z();

	float d = mRadius;
	float y, t, r;
	for( int iv = 1; iv < gridV1 - 1; ++ iv ) {            // �����~�ɕ�������
		y = iv * incV - d;                                     // Y����ł̒l(V�����̕����l)
		r = sqrt(d * d - y * y);                          // ���݂�Y�l��ł̉~�̔��a
		for( int iu = 0; iu < gridU; ++ iu ) {             // �~���`�ɕ�������
			t = iu * incU * M_PI / 180;                     // ��`�̊p�x(Radian)
			m_vertices[cnt++] = (GLfloat)(r * cos(t));      // ���_���W���i�[(XZ���ʂŃv���b�g�B
			m_vertices[cnt++] = (GLfloat)y;                      // Z=0�̂Ƃ��A�܂�X����̈ʒu����J�n
			m_vertices[cnt++] = (GLfloat)(r * sin(t)) + zOffset;      // ���v���ɕ`��(X������Z�������ւ̌���) 

			normal = Vector3f(m_vertices[cnt-3], m_vertices[cnt-2], m_vertices[cnt-1]);
			normal = normal.normalized();
			m_normals[cnt-3] = normal.x();
			m_normals[cnt-2] = normal.y();
			m_normals[cnt-1] = normal.z();
		}
	}
	m_vertices[cnt++] = 0;                               //Y���ł̒[�̒��_���W
	m_vertices[cnt++] = mRadius;
	m_vertices[cnt++] = 0 + zOffset;			

	normal = Vector3f(m_vertices[cnt-3], m_vertices[cnt-2], m_vertices[cnt-1]);
	normal = normal.normalized();
    m_normals[cnt-3] = normal.x();
	m_normals[cnt-2] = normal.y();
	m_normals[cnt-1] = normal.z();

	//�C���f�b�N�X�z����Z�b�g(�O�p�`�v���~�e�B�u�̒��_�����_�����O���Ԃ��i�[)
    m_indicesArraySize = ( ( gridV - 2) * (gridU - 1) * 2 + (gridU * 2) ) * 3;
	m_indices= new GLuint[m_indicesArraySize];
	cnt = 0;
	for( int iu = 0; iu < gridU; ++ iu ) {                               // �[�����i�ڂ̃v���~�e�B�u�̌v�Z
		m_indices[cnt++] = 0;				                                 // �O�p�`��1�ڂ̒��_�͒[�̓_����n�܂� 
		m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1);                   // 0 -> 2 -> 1 �̏���(�\�ʂɑ΂��Ď��v���)
		m_indices[cnt++] = (GLuint)(iu + 1);                                 // �]������߂Ă���͍̂Ō�̃v���~�e�B�u�̌v�Z���ɍŏ��̒��_���g������
	}
	for( int iv = 1; iv < gridV1 - 2; ++ iv ) {                          // 2�i�ڈȍ~�A�ŏI�i1��O�܂�(�����O�̌`)
		for( int iu = 0; iu < gridU - 1; ++ iu ) {                           
			int m = (iv - 1) * gridU;                                    // m�͒i���ɂ��I�t�Z�b�g
			//Triangle A                                                 // TriangleA�͏������i����2���_�����O�p�`
			m_indices[cnt++] = (GLuint)(iu + 1 + m);                         // ���v������
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
			m_indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			
			//Triangle B
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + gridU + m);   //TriangleB�͑傫���i����2���_�����O�p�`
			m_indices[cnt++] = (GLuint)(iu + 1 + gridU + m);
			m_indices[cnt++] = (GLuint)((iu + 1) % gridU + 1 + m);
		}
	}
	int n = m_indicesArraySize - 1;      // ���_�̍Ō�̃C���f�b�N�X�ԍ�
	for( int iu = n - gridU; iu < n; ++ iu ) {                          // �ŏI�i�̃v���~�e�B�u�̌v�Z  
		m_indices[cnt++] = (GLuint)iu;                                    // �E�E�E�@-> [(n-1) -> (n-2) -> (n)] �̏��� (�\�ʂɑ΂��Ď��v���)
		m_indices[cnt++] = (GLuint)(iu % gridU + n - gridU );				// �]������߂Ă���͈̂�����čŏ��̒��_���g���Ƃ��̂���
		m_indices[cnt++] = (GLuint)n;                                     // �[�̒��_
	}

   /* �o�b�t�@�I�u�W�F�N�g�̖��O��3��� */
   glGenBuffers(3, m_buffers);
  
   /* �P�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�f�[�^�z���]������ */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* �Q�ڂ̃o�b�t�@�I�u�W�F�N�g�ɖ@���f�[�^�z���]������ */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* �R�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�̃C���f�b�N�X��]������ */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}


//-----------------------------------------------------------------------------
// ����: �����̌^�̃��b�V����VBO�Œ�`����
// ����: 
// �Ԃ�l:
// ���̑�: 
//       DrawFigure::Initialize()�ŌĂ΂��
//-----------------------------------------------------------------------------
void CDrawFigure::initCubeVBO()
{
	int cnt = 0;                        // ���݂̔z�������

	//���_�z��Ɩ@���z����i�[����
    unsigned int verticesArraySize = (4 * 6) * 3;
	m_vertices = new GLfloat[verticesArraySize]; // {�[�̒��_�{(Y�������ւ�*��`�ւ̕�����)} * (X,Y,Z)
	m_normals = new GLfloat[verticesArraySize]; // {�[�̒��_�{(Y�������ւ�*��`�ւ̕�����)} * (X,Y,Z)
    
	// �O��
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

    // �w��
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

	// ����
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

	// �E��
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

	// ���
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

	// ����
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

	//�C���f�b�N�X�z����Z�b�g(�O�p�`�v���~�e�B�u�̒��_�����_�����O���Ԃ��i�[)
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

   /* �o�b�t�@�I�u�W�F�N�g�̖��O��3��� */
   glGenBuffers(3, m_buffers);
  
   /* �P�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�f�[�^�z���]������ */
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat) , m_vertices, GL_DYNAMIC_DRAW);
  
  /* �Q�ڂ̃o�b�t�@�I�u�W�F�N�g�ɖ@���f�[�^�z���]������ */
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesArraySize * sizeof(GLfloat), m_normals, GL_DYNAMIC_DRAW);
  
  /* �R�ڂ̃o�b�t�@�I�u�W�F�N�g�ɒ��_�̃C���f�b�N�X��]������ */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesArraySize * sizeof(GLuint), m_indices, GL_DYNAMIC_DRAW);

}


/***********************************************************************************************/
/************************************    �⏕�֐�    *******************************************/
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