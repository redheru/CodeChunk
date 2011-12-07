#ifndef POSE_H
#define POSE_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <iostream>


#include <KlMath/klMath.h>

//#include <ReadSensorData/Rsd.h>

#include "Skeleton.h"


using namespace std;
//using namespace Sensor;

using namespace KlMath;

//class MocapData;
class Pose 
{

	//friend class MocapData;

/////////////////////////////////////
// public member
/////////////////////////////////////
public:
	Skeleton *skeleton;

/////////////////////////////////////
// private member
/////////////////////////////////////
private:
	klVec3 rootPosition_;
	klQuat rootOrientation_;
	vector< klQuat > boneOrientations_;
	
	bool isCorrected_;
	
private:
	//static Vector3f baseRootPos;
	//static Quatf  baseRootRot;

/////////////////////////////////////
// method
/////////////////////////////////////
public:
	//int getRshoulderIndex() { return skeleton->rshoulderIndex; }
	//int getLshoulderIndex() { return skeleton->lshoulderIndex; }
	//	
	//int getRhandEffecterIndex() { return skeleton->rhandEffecterIndex; }
	//int getLhandEffecterIndex() { return skeleton->lhandEffecterIndex; }

	//int getRhipIndex() { return skeleton->rhipIndex; }
	//int getLhipIndex() { return skeleton->lhipIndex; }

	//int getRfootEffecterIndex() { return skeleton->rfootEffecterIndex; }
	//int getLfootEffecterIndex() { return skeleton->lfootEffecterIndex; }
	//int getRootIndex() { return skeleton->rootIndex; }
	//
	//int getChestIndex() { return skeleton->chestIndex; }
	//int getHeadEffecterIndex() { return skeleton->headEffecterIndex; }
	
	//int getBoneNum() { return this->skeleton->bones.size(); }


	//コンストラクタ
	Pose(){ Clear();}
	//デストラクタ
	~Pose(){Clear();}

	//clear members and resize bone orientation array
	void Clear(unsigned int bones = 0);

	//make a new vacant pose
	//then you should set bone orientation by SetBoneOrientation()
	void LoadVacantPose(Skeleton* skl);

	klMat44 ComputeLocaltoWolrdConvertMatrix(int bone_index);

	//build pose by directional vector and skeleton
	// we use it when load from motion capture file, particularly
	void BuildPose(double const *frame_data, Skeleton* skl);
	//void BuildPose(ReadSensorData::DATA data, Skeleton* skl);

	void CorrectRoot();

	//テスト用関数
	bool testPose(string fileName, Skeleton* skl);


// >getter
	Skeleton* getSkeleton(){return this->skeleton;}
	Bone* getBone(int bone_index) { return this->skeleton->bones[bone_index]; }
	klVec3 getRootPosition() { return rootPosition_; }
	klQuat getRootAngle(){ return rootOrientation_; }
	klQuat getBoneAngle(int boneIndex){  return boneOrientations_[boneIndex]; }
	klQuat getBoneAngle(string bone_name){ return getBoneAngle( skeleton->getBoneIndex(bone_name) ); }
	int getBoneNum() { return boneOrientations_.size(); }
	int getBoneIndex(string name) { return this->skeleton->getBoneIndex(name); }
	string getBoneName(int index) { return this->skeleton->getBoneName(index); }
	
	klVec3 getBoneOffset( int bone_index ){ return this->skeleton->bones[bone_index]->direction * this->skeleton->bones[bone_index]->length;}
	klVec3 getBoneDirection( int bone_index ){ return this->skeleton->bones[bone_index]->direction;}
	klVec3 getBoneOffset( string bone_name ) { return getBoneOffset( getBoneIndex(bone_name) ); }
	int getBoneParentNum( int bone_index ){ return this->skeleton->bones[bone_index]->parent; }
	klVec3 getNodePosition(int bone_index);
	klVec3 getNodePositionFromRoot( int bone_index, int root_index = 0, bool with_root_rotation = true);
	klVec3 getNodePositionFromBone(int dist_bone_index,	int base_bone_index);

// >setter

	//set root orientation
	void SetRootAngle(klQuat q){rootOrientation_ = q;}

	//set root position 
	void SetRootPosition(klVec3 root_pos){rootPosition_ = root_pos;}
	void SetRootPosition(double x, double y, double z){this->rootPosition_.x = x;this->rootPosition_.y = y;this->rootPosition_.z = z;}

	//set bone orientation data
	void SetBoneAngle(klQuat q, int bone_index);

	//slower set
	void SetBoneAngle(klQuat q, string bone_name){SetBoneAngle(q, skeleton->getBoneIndex(bone_name) );}

	void SetIsCorrected(bool flag){isCorrected_ = flag;}

	//compute pi(world)
		//klMat44 mat;
		//mat.LoadIdentity();

		//Bone* bone = &this->skeleton->bones[base_bone_index];
		////int stop_index = this->skeleton->bones[from_bone_index].parent;
		////int stop_index = from_bone_index;
		//
		//while( bone->parent != stop_index )
		//{
		//	//multiply translate matrix from left
		//	klVec3 t = klVec3(bone->direction.x, bone->direction.y, bone->direction.z);
		//	t *= bone->length;
		//	
		//	
		//	//Translate in klMat44 multiply translate mat from right
		//	klMat44 t_mat;
		//	t_mat.LoadIdentity();
		//	t_mat.Translate(t);
		//	mat = t_mat * mat;
		//				
		//	//multiply rotate matrix from left
		//	mat = Converter::Quat_to_klMat( this->getBoneAngle(bone_index)) * mat;
		//	
		//	//goto parent bone
		//	bone_index = bone->parent;
		//	bone = &this->skeleton->bones[bone_index];
		//}
		//
		////multiply matrix to zero vector
		//klVec4 p_i = klVec4(0, 0, 0, 1);
		//p_i = mat * p_i;
		//
		//klVec3 res = klVec3(p_i.x, p_i.y, p_i.z);
		//return res;









		//klMat44 mat;
		//klVec4 p_i = klVec4(0, 0, 0, 1);
		//Bone* b = &this->skeleton->bones[bone_index];

		//int stop_index = this->skeleton->bones[from_bone_index].parent;
		////stop_index =
		//while( b->parent != stop_index )
		//{
		//	//translate
		//	mat.LoadIdentity();
		//	klVec3 t = klVec3(b->direction.x, b->direction.y, b->direction.z);
		//	t *= b->length;
		//	mat.Translate(t);
		//	p_i = mat * p_i;
		//	
		//	//rotate
		//	mat = Converter::Quat_to_klMat( this->getBoneAngle(bone_index));
		//	p_i = mat * p_i;
		//	
		//	//goto parent bone
		//	bone_index = b->parent;
		//	b = &this->skeleton->bones[bone_index];
		//}
		//
		//klVec3 res = klVec3(p_i.x, p_i.y, p_i.z);
		//return res;
	//}

	//access by name
	//klVec3 getEndEffecterVec_from_Bone(string bone_name, )
	//{
	//	int bone_index = this->getBoneIndex(bone_name);
	//	return getEndEffecterPosition(bone_name)
	//}

	////get end effecter's position in a bone's coordinate
	//klVec3 getEndEffecterPosition(int bone_index)
	//{
	//	
	//}
	
	//void ConvertToLocalAngle()
	//{
	//	if(! (this->boneAngle_is_Local) )
	//	{
	//		cout << "already local coordinate!" << endl;
	//	}
	//	this->boneAngle_is_Local = true;
	//}


	//correct root position and rotation for target VSK skeleton
private:
	//for get pose
	klVec3 get_dof_trans(vector<klVec3> positionEulerOrder, double const *info, int start_pos);
	//Vector3d get_dof_trans(string const &dof, double const *info, int start_pos) {
	//	Vector3d trans;
	//	trans.x = trans.y = trans.z = 0;
	//	info += start_pos;
	//	for (unsigned int i = 0; i != dof.size(); ++i) {
	//		double d = *info;
	//		switch (dof[i]) {
	//			case 'x':
	//			case 'y':
	//			case 'z':
	//				break;
	//			case 'a':
	//				info += 2;
	//				break;
 //				case 'X':
	//				trans.x = d;
	//				break;
 //				case 'Y':
	//				trans.y = d;
	//				break;
 //				case 'Z':
	//				trans.z = d;
	//				break;
 //				case 'l':
	//				cerr << "Currently, we're lazy about length." << endl;
	//				break;
 //				default:
	//				cerr << "Enountered dof '" << dof[i] << "' we don't know about in '" << dof << "'." << endl;
	//				assert(0);
 //			}
	//		++info;
 //		}
	//	return trans;
	//}

	//for get pose
	klQuat ordered_rotation(vector<klVec3>eulerOrder, klVec3 const &rot);
	//Quatd ordered_rotation(string const &order, Vector3d const &rot) 
	//{
	//	Quatd ret;
	//	ret.clear();
	//	for (unsigned int i = 0; i < order.size(); ++i) {
	//		switch (order[i]) {
	//			case 'x':
	//				ret = multiply(rotation(rot.x * M_PI / 180.0, make_vector(1.0,0.0,0.0)), ret);
	//				break;
	//			case 'y':
	//				ret = multiply(rotation(rot.y * M_PI / 180.0, make_vector(0.0,1.0,0.0)), ret);
	//				break;
	//			case 'z':
	//				ret = multiply(rotation(rot.z * M_PI / 180.0, make_vector(0.0,0.0,1.0)), ret);
	//				break;
	//			case 'X':
	//			case 'Y':
	//			case 'Z':
	//			case 'l':
	//			break;
	//			default:
	//				cerr << "Unexpected offset_order character '" << order[i] << "' in '" << order << "'." << endl;
	//				assert(0);
	//		}
	//	}
	//	return normalize(ret);
	//}

	//Pose &operator=(Pose const &other);
	//void to_angles(Angles &into) const;

	//for get pose
	klQuat get_dof_rot(vector<klVec3> eulerOrder, double const *info, int start_pos);
	//Quatd get_dof_rot(string const &dof, double const *info, int start_pos) {
	//	Quatd ret;
	//	ret.clear();
	//	info += start_pos;
	//	for (unsigned int i = 0; i != dof.size(); ++i) {
	//		double d = *info;
	//		switch (dof[i]) {
	//			case 'x':
	//				ret = multiply(rotation(d * M_PI / 180.0, make_vector(1.0,0.0,0.0)), ret);
	//				break;
	//			case 'y':
	//				ret = multiply(rotation(d * M_PI / 180.0, make_vector(0.0,1.0,0.0)), ret);
	//				break;
	//			case 'z':
	//				ret = multiply(rotation(d * M_PI / 180.0, make_vector(0.0,0.0,1.0)), ret);
	//				break;
	//			case 'a':
	//				{
	//				Vector3d axis = *(Vector3d*)info;
	//				info += 2;
	//				ret = multiply(rotation(length(axis), normalize(axis)), ret);
	//				break;
	//				}

 //				case 'X':
 //				case 'Y':
 //				case 'Z':
	//				break;
 //				case 'l':
	//				cerr << "Currently, we're lazy about length." << endl;
	//				break;
 //				default:
	//				cerr << "Enountered dof '" << dof[i] << "' we don't know about in '" << dof << "'." << endl;
	//				assert(0);
 //			}
	//		++info;
 //		}
	//	return normalize(ret);
	//}

};


//class WorldBones {
//public:
//	vector< Vector3f > bases;
//	vector< Vector3f > tips;
//	vector< Quatf > orientations;
//};
//
//void get_world_bones(Pose &pose, WorldBones &out);
//
////float world_distance(WorldBones &a, WorldBones &b) {
////	assert(a.tips.size() == b.tips.size());
////	float dis = 0.0f;
////	for (unsigned int i = 0; i < a.tips.size(); ++i) {
////		dis += length(a.tips[i] - b.tips[i]);
////	}
////	return dis;
////}


#endif POSE_HPP


//Angles is a joint-angles precursor to Pose
//(though, to be fair, the first three things in angles are
// actually root position, but let's not stress too much.)
/*
class Angles {
public:
	void clear(unsigned int DOF = 0, Library::Skeleton const *skel = NULL);
	void to_pose(Pose &into) const;
	vector< double > angles;
	Library::Skeleton const *skeleton;
};
*/












	//klVec3 getNodePosition(int bone_index)
	//{

	//	klMat44 mat;
	//	klVec4 p_i = klVec4(0, 0, 0, 1);
	//	Bone* b = &this->skeleton->bones[bone_index];

	//	while( b->parent != -1 )
	//	{
	//		//translate
	//		mat.LoadIdentity();
	//		klVec3 t = klVec3(b->direction.x, b->direction.y, b->direction.z);
	//		t *= b->length;
	//		mat.Translate(t);
	//		p_i = mat * p_i;
	//		
	//		//rotate
	//		mat = Converter::Quat_to_klMat( this->getBoneAngle(bone_index));
	//		p_i = mat * p_i;
	//		
	//		//goto parent bone
	//		bone_index = b->parent;
	//		b = &this->skeleton->bones[bone_index];
	//	}
	//	
	//	//root rotate
	//	mat = Converter::Quat_to_klMat( this->getRootAngle() );
	//	p_i = mat * p_i;

	//	//root translate
	//	Vector3f t_root = this->getRootPosition();
	//	mat.LoadIdentity();
	//	mat.Translate(t_root.x, t_root.y, t_root.z);
	//	p_i = mat * p_i;
	//	
	//	klVec3 res = klVec3(p_i.x, p_i.y, p_i.z);
	//	return res;
	//}









	////compute pi(world)
	//klVec3 getNodePosition(int bone_index)
	//{
	//	Bone* bone = &this->skeleton->bones[bone_index];

	//	glPushMatrix();
	//	glLoadIdentity();
	//	while( bone->parent != -1 )
	//	{
	//		//multiply translate matrix from left
	//		glTranslated(bone->direction.x * bone->length, bone->direction.y * bone->length, bone->direction.z * bone->length);
	//								
	//		//multiply rotate matrix from left
	//		glRotate( this->getBoneAngle(bone_index) );
	//		
	//		//goto parent bone
	//		bone_index = bone->parent;
	//		bone = &this->skeleton->bones[bone_index];
	//	}
	//	
	//	//multiply Rroot from left
	//	glRotate( this->getRootAngle() );

	//	//multiply Troot from left
	//	glTranslate( this->getRootPosition() );
	//	
	//	klMat44 mat;
	//	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	//	glPopMatrix();

	//	//multiply matrix to zero vector
	//	klVec4 p_i = klVec4(0, 0, 0, 1);
	//	p_i = mat * p_i;
	//	
	//	klVec3 res = klVec3(p_i.x, p_i.y, p_i.z);
	//	return res;
	//}