#ifndef DRAWMOCAPDATA_H
#define DRAWMOCAPDATA_H

//MocapData
#include "MocapData.h"
#include "Skeleton.h"
#include "Pose.h"

//Math
#include "../KlMath/klMath.h"

//OpenGL
#include <GL/glut.h>
#include <GL/glu.h>

using namespace KlMath;

class DrawMocapData : public MocapData
{

//コンストラクタ，デストラクタ
public:
	DrawMocapData();
	DrawMocapData(string file_name);

	void Initialize();

	
////////////////////////
// member
////////////////////////
public:
	bool isDrawLocalAxes;
	klVec3 pos;

private:
	double scale_;
	int sect_;
	klVec3 color_;
	//klVec4 specular;
	double specular_;
	double amb_;
	double diffuse_;
	int renderingMethod_;
	//GLenum mode_;//rendering mode
	//int selectedBoneIndex_;
	bool isDrawLocalLine_;
	bool isDrawRootToEffecterLine_;
	double alpha_;
	
//	bool isSensor;
//
//public:
//	SensorData sensor;
	

////////////////////////
// method 
////////////////////////
public:

	////SensorData* getSensorData(){ return &this->sensor; }
	//void DrawLineToBone(int frame, klVec3 from_point, string bone_name)
	//{
	//	//compute node position
	//	Pose& pose = this->getPose(frame);
	//	klVec3 node = pose.getNodePosition(pose.getBoneIndex(bone_name));
	//	//render line
	//	QGLColor::setMaterial(QGLColor::light_blue, 0, 0, 1.0);
	//	QGLUtil::DrawLine(from_point, node, 5.0);
	//}	
	//void DrawTwoPointLine(int frame, string bone_name1, string bone_name2)
	//{
	//	//compute node position
	//	Pose& pose = this->getPose(frame);
	//	klVec3 node1 = pose.getNodePosition(pose.getBoneIndex(bone_name1));
	//	klVec3 node2 = pose.getNodePosition(pose.getBoneIndex(bone_name2));

	//	//render line
	//	QGLColor::setMaterial(QGLColor::red, 0, 0, 1.0);
	//	QGLUtil::DrawLine(node1, node2, 5.0);
	//}



	//void DrawLocalLine(int frame, string to_bone_name, string from_bone_name)
	//{
	//	//compute local node position Pi(j)
	//	Pose& pose = this->getPose(frame);
	//	klVec3 node_temp = pose.getNodePositionFromBone(pose.getBoneIndex(to_bone_name), pose.getBoneIndex(from_bone_name));
	//	klVec4 node = klVec4(node_temp.x, node_temp.y, node_temp.z, 1.0);
	//	klVec4 zero = klVec4(0,0,0,1);

	//	//multiply BoneCoordinate-to-WorldCoordinate convert matrix
	//	klMat44 mat;
	//	mat.LoadIdentity();

	//	mat = pose.ComputeLocaltoWolrdConvertMatrix( pose.getBoneIndex( from_bone_name ) );
	//	node = mat * node;
	//	zero = mat * zero;
	//	
	//	//rendering line
	//	QGLColor::setMaterial(QGLColor::light_blue, 0, 0, 1.0);
	//	QGLUtil::DrawLine(zero, node, 3.0 * this->scale);
	//}

	//void DrawLocalOffsetLine(int frame, string from_bone_name)
	//{
	//	int from_bone_num = this->getBoneIndex(from_bone_name);
	//	//compute local node position Pi(j)

	//	Pose& pose = this->getPose(frame);
	//	Bone* bone = pose.getBone(from_bone_num);
	//	//bone = pose.getBone(bone.parent);
	//	Vector3d node_temp = bone->direction;
	//	
	//	klVec4 off = klVec4(node_temp.x, node_temp.y, node_temp.z, 1.0);
	//	klVec4 zero = klVec4(0,0,0,1);

	//	int from_bone_parent = bone->parent;
	//	klMat44 mat = pose.ComputeLocaltoWolrdConvertMatrix(from_bone_parent);
	//	off = mat * off;	
	//	zero = mat * zero;
	//	
	//	//rendering line
	//	QGLColor::setMaterial(QGLColor::yerrow_green, 0, 0, 1.0);
	//	QGLUtil::DrawLine(zero, off, 6.0);
	//}

	//void DrawNeedLocalAxes(string bone_name, double length = 0.2, double width = 3.0)
	//{
	//	//draw coordinate axes if not bone is phantom 
	//	//if(
	//	//	bone_name != "lfemur_phantom"&&
	//	//	bone_name != "rfemur_phantom"&&
	//	//	bone_name != "lowerback_phantom"&&
	//	//	bone_name != "LowerNeck_phantom"&&
	//	//	bone_name != "lclavicle_phantom"&&
	//	//	bone_name != "rclavicle_phantom"
	//	//)
	//	if( true )
	//	{
	//		QGLUtil::DrawAxes(length, width);
	//		QGLColor::setMaterial(this->color, this->specular, this->diffuse, this->amb);
	//	}
	//}

	void DrawFrame( int frame, bool apply_root_translation = true );

	//void DrawFrame( int frame, bool apply_root_translation = true )
	//{
	//	Pose& pose = poses[frame];
	//	vector<int> parent_num_stack;
	//	
	//	glPushMatrix();
	//	{
	//		//pos correction( multiply Tpos to M from right )
	//		glTranslate( this->pos );

	//		//scaling( multiply Mscale to M from right )
	//		glScaled(this->scale, this->scale, this->scale);
	//
	//		//Quatf correct_q =  rotation( Converter::toVector3f(klVec3(1,0,0)), Converter::toVector3f(klVec3(0,0,-1)) );
	//		//Quatf correct_q =  rotation( Converter::toVector3f(klVec3(0,0,-1)), Converter::toVector3f(klVec3(1,0,0)) );
	//		//glRotate( correct_q );


	//		//draw auxiliary lines for observation
	//		DrawAuxiliaryLines_(frame, this->skeleton->getSkeletonType() );

	//		//root corection( multiply Troot to M from right )
	//		
	//		if( apply_root_translation ) glTranslate( pose.getRootPosition() );//Troot
	//		else glTranslate( klVec3(0,0,1) );//Troot

	//		//root corection( multiply Rroot to M from right )
	//		glRotate( pose.getRootAngle() );//Rroot

	//		//push root's parent number on the stack 
	//		parent_num_stack.push_back(-1);

	//		//follow bones
	//		for(int bone_num = 0; bone_num < pose.getBoneNum(); bone_num++)
	//		{
	//			Bone& bone = this->skeleton->bones[bone_num];

	//			//pop stack
	//			while(parent_num_stack.back() != bone.parent)
	//			{
	//				parent_num_stack.pop_back();
	//				glPopMatrix();
	//			}
	//			//// Now, matrix M                          ////
	//			////  = TposMscaleTrootRrootR0T0...Ri-2Ti-2 ////

	//			//push M on the Matrix stack,
	//			//and push parent num on the parent num stack 
	//			glPushMatrix();
	//			parent_num_stack.push_back(bone_num);
	//		
	//			//draw axes
	//			if(this->isDrawLocalAxes)
	//			{
	//				DrawNeedLocalAxes(bone.name, 0.15, 3.0);
	//			}
	//			
	//			//bone start point
	//			klVec3 bone_start = klVec3(0,0,0);
	//			
	//			//multiply Ri-1 to M from right
	//			glRotate( pose.getBoneAngle(bone_num) );

	//			//render ellipsoid from bone end point to bone start point
	//			klVec3 bone_end = klVec3(bone.direction.x, bone.direction.y, bone.direction.z);
	//			bone_end *= bone.length;


	//			//klVec4 temp_color( color.r, color.g, color.b, alpha);
	//			//Color::setMaterial(temp_color, this->specular, this->diffuse, this->amb);
	//			double temp_alpha = this->alpha;
	//			//if( ! (bone.name == "rfemur_phantom" || bone.name == "rfemur" || bone.name == "rtibia" || bone.name == "rfoot"|| bone.name == "rtoesw"))
	//			if( ! (bone.name == "rhand" || bone.name == "rradius" || bone.name == "rhumerus" || bone.name == "rclavicle"))
	//			//if(false)
	//			{
	//				//if( this->alpha < 0.99 ) temp_alpha = 0.0;
	//			}

	//			QGLColor::setMaterial(this->color, this->specular, this->diffuse, this->amb, temp_alpha);
	//			//Color::setMaterial(this->color, this->specular, this->diffuse, this->amb, 1.0);
	//			QGLUtil::DrawEllipsoidalBody( bone_start, bone_end, this->sect, bone.radius);


	//			
	//			//multiply Ti-1 to M from right
	//			glTranslated(bone_end.x, bone_end.y, bone_end.z);

	//			//// Now, matrix M                          ////
	//			////  = TposMscaleTrootRrootR0T0...Ri-1Ti-1 ////
	//		}
	//		
	//		//clean Matrix stack
	//		while (!parent_num_stack.empty())
	//		{
	//			glPopMatrix();
	//			parent_num_stack.pop_back();
	//		}			
	//	}
	//	//pop TposMscaleTrootRroot
	//	glPopMatrix();
	//}

	// >setter
	void setAlpha(double alpha){alpha_ = alpha;}
	void setIsDrawRootToEffecterLine(bool is_draw) { isDrawRootToEffecterLine_ = is_draw; }
	void setScale(double s) { scale_ = s; }
	//void setSelectedBoneIndex(int boneIndex) { selectedBoneIndex_ = boneIndex; }
	//void setGLRenderingMode(GLenum glmode) { mode_ = glmode; }
	void setColor(double R, double G,  double B)
	{
		color_.x = R;color_.y = G;color_.z = B;
	}
	void setColor(klVec4 color)
	{
		color_.r = color.r;color_.g = color.g;color_.b = color.b;
	}

	void setCharacterSpecular(double s){specular_ = s;}
	void setRenderingMethod(int method){renderingMethod_ = method;}
	void setSmoothness(int divide_num){sect_ = divide_num;}
	void setPos(double x, double y, double z)
	{
		pos.x = x;pos.y = y;pos.z = z;
	}

	void setPram(double scale, int rendering_method, int smoothness)
	{
		this->scale_ = scale;
		this->renderingMethod_ = rendering_method;
		this->sect_ = smoothness;
	}
	void setIsDrawLocalAxes(bool is_draw_local_axes) {isDrawLocalAxes = is_draw_local_axes; }
	void setIsDrawLocalLine(bool is_draw_local_line) { isDrawLocalLine_ = is_draw_local_line; }
	void setScaleSize(int size)
	{
		this->getSkeleton()->ScaleSkeleton(size);
		for(int i=0; i < this->getFrameNum(); i++)
		{			
			this->setRootPosition( this->getRootPosition(i)*size, i);
		}
	}

	//void setIsSensor( bool b ) { this->isSensor = b; }
	
	// >getter
	//int getSelectedBoneIndex() { return selectedBoneIndex_; }
	//GLenum getGLRenderingMode() { return mode_; }
	klVec3 getPos() { return pos; }
	//bool getIsSensor() { return this->isSensor; }

private:

	void glRotate(klQuat q);
	void setMaterial(klVec4 color, double specular_rate = 0.0, double diffuse_rate = 0.7, double amb_rate = 0.5);

	void DrawEllipsoidalBody(klVec3 pos1, klVec3 pos2, int sect, double radius);

	void DrawEllipsoid(klVec3 p, klVec3 v1, klVec3 v2, klVec3 v3, int sect);

	//rendering by Jim's way
	//void DrawCylinderEtc(const Bone& bone)
	//{			
	//		static GLUquadric *quad = NULL;
	//		if (quad == NULL) {
	//			quad = gluNewQuadric();
	//		}

	//		Vector3d t = bone.direction;
	//		Vector3d t2;
	//		if (t.x == t.y && t.y == t.z) {
	//			t2 = normalize(cross_product(t, make_vector(t.y,t.z,-t.x)));
	//		} else {
	//			t2 = normalize(cross_product(t, make_vector(t.y,t.z,t.x)));
	//		}
	//		Vector3d t3 = cross_product(t2, t);
	//		
	//		glPushMatrix();

	//		double mat[16] = {
	//			t2.x, t2.y, t2.z, 0,
	//			t3.x, t3.y, t3.z, 0,
	//			t.x, t.y, t.z, 0,
	//			0,0,0,1
	//		};
	//		glMultMatrixd(mat);
	//		
	//		gluCylinder(quad, bone.radius, bone.radius, bone.length, 8, 1);
	//		glRotated(180,1,0,0);
	//		gluDisk(quad, 0, bone.radius, 8, 1);
	//		glRotated(180,1,0,0);
	//		glTranslated(0, 0, bone.length);
	//		gluDisk(quad, 0, bone.radius, 8, 1);
	//		glTranslated(-bone.radius, 0, -bone.length);
	//		
	//		glPopMatrix();
	//}

	//void DrawAuxiliaryLines_(int frame, Skeleton::SkeletonType skl_type)
	//{	
	//	if(this->isDrawLocalLine)
	//	{
	//		if( Skeleton::Human == skl_type )
	//		{
	//			DrawLocalLine(frame, "rhand", "rclavicle");
	//			DrawLocalLine(frame, "lhand", "lclavicle");
	//			DrawLocalLine(frame, "rtoes", "rfemur_phantom");
	//			DrawLocalLine(frame, "ltoes", "lfemur_phantom");
	//		}
	//		else if( Skeleton::Puppet == skl_type ) 
	//		{
	//			DrawLocalLine(frame, "rhumerus", "rclavicle_phantom");
	//			DrawLocalLine(frame, "lhumerus", "lclavicle_phantom");
	//			DrawLocalLine(frame, "rfemur", "rfemur_phantom");
	//			DrawLocalLine(frame, "lfemur", "lfemur_phantom");
	//		}
	//	}

	//	if(this->isDrawRootToEffecterLine)
	//	{
	//		if( Skeleton::Human == skl_type )
	//		{
	//			DrawLocalLine(frame, "rhand", "pelvis");
	//			DrawLocalLine(frame, "lhand", "pelvis");
	//			DrawLocalLine(frame, "rtoes", "pelvis");
	//			DrawLocalLine(frame, "ltoes", "pelvis");
	//		}
	//		else if( Skeleton::Puppet == skl_type )
	//		{
	//			DrawLocalLine(frame, "rhumerus", "pelvis");
	//			DrawLocalLine(frame, "lhumerus", "pelvis");
	//			DrawLocalLine(frame, "rfemur", "pelvis");
	//			DrawLocalLine(frame, "lfemur", "pelvis");
	//		}
	//	}

	//}	

};//Class DrawMocapData


#endif