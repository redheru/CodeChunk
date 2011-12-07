#include "DrawMocapData.h"

DrawMocapData::DrawMocapData()
{
	Initialize();
}

DrawMocapData::DrawMocapData(string file_name)
{//BvhêÍóp
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
		glRotate( pose.getRootAngle() );//Rroot

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

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spc);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
}

void DrawMocapData::DrawEllipsoidalBody(klVec3 pos1, klVec3 pos2, int sect, double radius) 
{
	klVec3 p,v1,v2,v3;
	p = (pos1 + pos2) /2;
	v1 = (pos1 - pos2) /2;
	
	//auxiliary vector(ï‚èï)
	klVec3 temp(1,1,1);

	v2 = v1 % temp;
	v2.Normalize();
	v2 = v2 * radius;
	v3 = v1 % v2;
	v3.Normalize();
	v3 = v3 * radius;

	DrawEllipsoid(p,v1,v2,v3,sect);
}

void DrawMocapData::DrawEllipsoid(klVec3 p, klVec3 v1, klVec3 v2, klVec3 v3, int sect)
{ 
	GLfloat mat[16]; 
	mat[0] = v1.x;mat[1] = v1.y; mat[2] = v1.z;mat[3] = 0;
	mat[4] = v2.x; mat[5] = v2.y; mat[6] = v2.z;
	mat[7] = 0; mat[8] = v3.x; mat[9] = v3.y;
	mat[10] = v3.z; mat[11] = 0; mat[12] = p.x;
	mat[13] = p.y; mat[14] = p.z; mat[15] = 1;
	//glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glMultMatrixf(mat);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutSolidSphere(1, sect, sect);
	glDisable(GL_BLEND);

	glDisable(GL_NORMALIZE);
	glPopMatrix();
}