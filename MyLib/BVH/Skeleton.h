#ifndef SKELETON_H
#define SKELETON_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <map>
#include "math.h"

//Math
#include "../KlMath/klMath.h"
using namespace KlMath;

using std::string;
using std::vector;
using std::cerr;
using std::endl;
using std::map;

//collection of joint angles, offset matrices, and so on.

class Bone {

public:
	void Initialize();
	//{
	//	axis_offset.x = 0; axis_offset.y = 0; axis_offset.z = 0;
	//	offset_order = "xyz";
	//	global_to_local.x = 0; global_to_local.y = 0; global_to_local.z = 0;
	//	density = 1000;
	//	torque_limits.resize(4);
	//	torque_limits[0].x = 0; torque_limits[0].y = 0;
	//	torque_limits[1].x = 0; torque_limits[1].y = 0;
	//	torque_limits[2].x = 0; torque_limits[2].y = 0;
	//	torque_limits[3].x = 0; torque_limits[3].y = 0;
	//	dof = "aXYZ";
	//	color.x = 0; color.y = 0; color.z = 0;
	//	frame_offset = 0;
	//	euler_axes.resize(3);
	//	euler_axes[0].x = 0; euler_axes[0].y = 0; euler_axes[0].z = 0;
	//	euler_axes[1].x = 0; euler_axes[1].y = 0; euler_axes[1].z = 0;
	//	euler_axes[2].x = 0; euler_axes[2].y = 0; euler_axes[2].z = 0;
	//}

	string name;//Boneの名前
	int parent;//親のBoneのインデックス
	vector<int> children;
	klVec3 direction; //Boneの向き
	//Vector3d axis_offset; //in degrees
	//string offset_order; //オフセットのOrderを持つ

	//Quatd global_to_local; //Local座標系での表記に変換させる

	double radius; //Boneの太さ
	//double density; //Boneの密度??
	double length; //Boneの長さ

	//vector< Vector2d > torque_limits; //回転させられる範囲

	//string dof; //list of degrees of freedom -> 'xyz' -> rotation around 'x' 'y' 'z'. 'a' -> angle-axis rotation [3 values]

	//Vector3d color; //Boneの色

	//データ配列で何番目かを格納する
	int frameOffset; //where in lists of per-frame data this fellow's data rests.

	// filled, but not yet taken advantage of
	vector< klVec3 > eulerOrder; // for v-file. Actual axes of joint rotation
	//Eulerによる回転表現

	//inline void pre_parse() {
	//	name = "";
	//	parent = 0;
	//	direction.x = direction.y = direction.z = 0;
	//	axis_offset.x = axis_offset.y = axis_offset.z = 0;
	//	offset_order = "xyz";
	//	radius = density = length = -1;
	//	dof = "";
	//	torque_limits.clear();
	//	color.r = rand() / double(RAND_MAX);
	//	color.g = rand() / double(RAND_MAX);
	//	color.b = rand() / double(RAND_MAX);
	//}

	//inline bool post_parse() {
	//	if (name == "") {
	//		cerr << "Bone lacks name." << endl;
	//		return false;
	//	}
	//	if (length < 0) {
	//		cerr << "Bone lacks length. Picking 1." << endl;
	//		length = 1;
	//	}
	//	if (radius < 0) {
	//		//cerr << "Bone lacks radius. Picking length / 10." << endl;
	//		radius = length / 10.0;
	//	}
	//	if (density < 0) {
	//		//cerr << "Bone lacks density. Picking 1000." << endl;
	//		density = 1000;
	//	}
	//	if (torque_limits.size() > dof.size()) {
	//		cerr << "There are extra torque limits on " << name << " that will be ignored." << endl;
	//	}
	//	if (torque_limits.size() < dof.size()) {
	//		//cerr << "Not all dof of " << name << " have limits. Padding with 100s." << endl;
	//		while (torque_limits.size() < dof.size()) {
	//			torque_limits.push_back(make_vector(-100.0, 100.0));
	//		}
	//	}
	//	return true;
	//}

};

class Skeleton
{

	//friend class Reader;

public:
	typedef enum
	{
		Human,
		Puppet,
	}SkeletonType;

// >public member
public:
	// little letters are rotations, big letters are translations.
	// (think 'xXyzYZ' -- rotate x, trans x, rot y, rot z, trans Y, trans Z)
	//string order; //order of transformation for root bone

	//klVec3 position; //position for root bone
	vector< klVec3 > positionEulerOrder; // for v-file. Actual axes of joint rotation
	//string offset_order; //order for axis offset rotation values
	//Vector3d axis_offset; //rotation for root bone (degrees)

	//double mass;
	//double length;
	//double timestep;//should not use

	//int frame_size; //how may dof per frame.
	//string filename; //what file this was loaded from.
	vector< Bone *> bones;
	

// private member
private:
	map<int, string> boneName_;
	map<string, int> boneIndex_;
	SkeletonType skeletonType_;
	string fileName_;
	int dataSize_; //位置と回転データの数

// >public method
public:
	Skeleton(){ Initialize();}

	void Load(string file_name);
	void LoadPuppetSkeleton();
	bool ReadBVH(string fileName);
	bool BuildSkeleton(ifstream& file);

	Bone* MakeNewBone(string name, int parent, double direction_x, double direction_y, double direction_z, double length, double radius);
	Bone* MakeNewBone(string name, int parent, klVec3 direction, double length, double radius);

	void ScaleSkeleton(double scale);	
	void ScaleSkeletonWidth(double scale);

// >private method
private:
	//int Skeleton::get_bone_by_name(string name) const;
	void Initialize();

	//void LoadPuppetSkeleton()
	//{
	//	this->Initialize();
	//	this->skeletonType = this->Puppet;

	//	double shoulder_length = 0.065;
	//	double hip_length = 0.037;

	//	double arm_length = 0.118;
	//	double leg_length = 0.09;
	//	
	//	double upper_back_length = 0.03;
	//	double lower_back_length = 0.03;
	//	double neck_length = 0.08;
	//	
	//	double r = 0.006;
	//	double shoulder_radius = r;
	//	double hip_radius = r;
	//	double arm_radius = r;
	//	double leg_radius = r;
	//	double lower_back_radius = r;
	//	double upper_back_radius = r;
	//	double neck_radius = r;
	//	
	//	klVec3 up( 1, 0, 0 );
	//	klVec3 down = -up;
	//	klVec3 left( 0, -1, 0 );
	//	klVec3 right = -left;
	//	klVec3 front( 0, 0, 1 );
	//
	//	//pelvis
	//	this->bones.resize(14);

	//	this->bones[0]  = MakeNewBone( "center", -1, down, 0, 0);
	//	this->bones[1]  = MakeNewBone( "lowerback_phantom", 0, down, lower_back_length, lower_back_radius );
	//	this->bones[2]  = MakeNewBone( "pelvis", 1, down, 0, 0 );
	//	
	//	//legs
	//	this->bones[3]  = MakeNewBone( "lfemur_phantom", 2, left, hip_length, hip_radius );
	//	this->bones[4]  = MakeNewBone( "lfemur", 3, down, leg_length, leg_radius );
	//	this->bones[5]  = MakeNewBone( "rfemur_phantom", 2, right, hip_length, hip_radius*1.5 );
	//	this->bones[6]  = MakeNewBone( "rfemur", 5, down, leg_length, leg_radius*2.0 );

	//	this->bones[7]  = MakeNewBone( "upperback_phantom", 0, up, upper_back_length, upper_back_radius );
	//	this->bones[8]  = MakeNewBone( "chest", 7, down, 0, 0 );
	//	
	//	//arms
	//	this->bones[9]  = MakeNewBone( "lclavicle_phantom", 8, left, shoulder_length, shoulder_radius );
	//	this->bones[10] = MakeNewBone( "lhumerus", 9, down, arm_length, arm_radius );
	//	this->bones[11] = MakeNewBone( "rclavicle_phantom", 8, right, shoulder_length, shoulder_radius*1.5 );
	//	this->bones[12] = MakeNewBone( "rhumerus", 11, down, arm_length, arm_radius*2.0 );

	//	//neck
	//	this->bones[13] = MakeNewBone( "neck_phantom", 8, up, neck_length, neck_radius );

	//
	//	//build bone dictionaries
	//	for(int i = 0; i < (int)this->bones.size(); i++)
	//	{			
	//		this->boneName.insert(map<int,string>::value_type
	//			(i,  this->bones[i].name));
	//		this->boneIndex.insert(map<string,int>::value_type
	//			(this->bones[i].name, i));
	//	}

	//	setSpecialIndexNames2nd( 0,
	//							"rclavicle_phantom", "lclavicle_phantom",
	//							"rhumerus", "lhumerus",
	//		                    "rraius", "lradius",
	//							"rfemur_phantom", "lfemur_phantom",
	//							"rtibia", "ltibia", 
	//							"rfemur", "lfemur", "neck_phantom", "upperback_phantom");
	//	
	//}

	//bool check_parse();
	//inline void init_parse() {
	//	in_bone = false;
	//	bones.clear();
	//	mass = 1.0;
	//	length = 1.0;
	//	ang_is_deg = true;
	//	offset_order = "xyz";
	//	order = "XYZxyz";
	//	timestep = 1.0 / 120.0;
	//	rot_is_glob = false;
	//	//z_is_up = false;
	//}
	
	//void setSpecialIndexNames(int root_index, string rshoulder, string lshoulder, string rhand_effecter, string lhand_effecter, string rhip, string lhip, string rfoot_effecter, string lfoot_effecter)
	//{
	//	this->rootIndex = root_index;
	//	
	//	this->rshoulderIndex = this->getBoneIndex(rshoulder);
	//	this->lshoulderIndex = this->getBoneIndex(lshoulder);

	//	this->rhandEffecterIndex = this->getBoneIndex(rhand_effecter);
	//	this->lhandEffecterIndex = this->getBoneIndex(lhand_effecter);

	//	this->rhipIndex = this->getBoneIndex(rhip);
	//	this->lhipIndex = this->getBoneIndex(lhip);

	//	this->rfootEffecterIndex = this->getBoneIndex(rfoot_effecter);
	//	this->lfootEffecterIndex = this->getBoneIndex(lfoot_effecter);

	//	printf( "----setSpecialIndexNames results-----\n");
	//	printf( "rootIndex = %d\n", rootIndex );
	//	
	//	printf( "rshoulderIndex = %d\n", rshoulderIndex );
	//	printf( "lshoulderIndex = %d\n", lshoulderIndex );
	//	
	//	printf( "rhandEffecterIndex = %d\n", rhandEffecterIndex );
	//	printf( "lhandEffecterIndex = %d\n", lhandEffecterIndex );
	//	
	//	printf( "rhipIndex = %d\n", rhipIndex );
	//	printf( "lhipIndex = %d\n", lhipIndex );
	//	
	//	printf( "rfootEffecterIndex = %d\n", rfootEffecterIndex );
	//	printf( "lfootEffecterIndex = %d\n", lfootEffecterIndex );
	//}

	//void setSpecialIndexNames2nd(int root_index, string rshoulder, string lshoulder, 
	//	string relbow, string lelbow, string rhand_effecter, string lhand_effecter, 
	//	string rhip, string lhip, string rknee, string lknee, string rfoot_effecter, string lfoot_effecter, 
	//	string head_effecter, string chest)
	//{
	//	this->rootIndex = root_index;
	//	
	//	this->rshoulderIndex = this->getBoneIndex(rshoulder);
	//	this->lshoulderIndex = this->getBoneIndex(lshoulder);

	//	this->relbowIndex = this->getBoneIndex(relbow);
	//	this->lelbowIndex = this->getBoneIndex(lelbow);

	//	this->rhandEffecterIndex = this->getBoneIndex(rhand_effecter);
	//	this->lhandEffecterIndex = this->getBoneIndex(lhand_effecter);

	//	this->rhipIndex = this->getBoneIndex(rhip);
	//	this->lhipIndex = this->getBoneIndex(lhip);

	//	this->rkneeIndex = this->getBoneIndex(rknee);
	//	this->lkneeIndex = this->getBoneIndex(lknee);

	//	this->rfootEffecterIndex = this->getBoneIndex(rfoot_effecter);
	//	this->lfootEffecterIndex = this->getBoneIndex(lfoot_effecter);

	//	this->headEffecterIndex = this->getBoneIndex(head_effecter);
	//	this->chestIndex = this->getBoneIndex(chest);

	//	printf( "----setSpecialIndexNames results-----\n");
	//	printf( "rootIndex = %d\n", rootIndex );
	//	
	//	printf( "rshoulderIndex = %d\n", rshoulderIndex );
	//	printf( "lshoulderIndex = %d\n", lshoulderIndex );

	//	printf( "relbowIndex = %d\n", relbowIndex );
	//	printf( "lelbowIndex = %d\n", lelbowIndex );

	//	printf( "rhandEffecterIndex = %d\n", rhandEffecterIndex );
	//	printf( "lhandEffecterIndex = %d\n", lhandEffecterIndex );
	//	
	//	printf( "rhipIndex = %d\n", rhipIndex );
	//	printf( "lhipIndex = %d\n", lhipIndex );

	//	printf( "rkneeIndex = %d\n", rkneeIndex );
	//	printf( "lkneeIndex = %d\n", lkneeIndex );

	//	printf( "rfootEffecterIndex = %d\n", rfootEffecterIndex );
	//	printf( "lfootEffecterIndex = %d\n", lfootEffecterIndex );
	//}
	
	//void setRetargetingIndexNames(string shoR, string shoL, string elbowR, string elbowL,
	//	string hipR, string hipL, string kneeR, string kneeL)
	//{
	//	this->retargetingShoRIndex = this->getBoneIndex(shoR);
	//	this->retargetingShoLIndex = this->getBoneIndex(shoL);

	//	this->retargetingElbowRIndex = this->getBoneIndex(elbowR);
	//	this->retargetingElbowLIndex = this->getBoneIndex(elbowL);

	//	this->retargetingHipRIndex = this->getBoneIndex(hipR);
	//	this->retargetingHipLIndex = this->getBoneIndex(hipL);

	//	this->retargetingKneeRIndex = this->getBoneIndex(kneeR);
	//	this->retargetingKneeLIndex = this->getBoneIndex(kneeL);
	//}

	//void LoadPuppetSkeleton_old()
	//{
	//	this->Initialize();
	//	this->skeletonType = this->Puppet;

	//	double shoulder_length = 0.07;
	//	double hip_length = 0.04;
	//	double arm_length = 0.10;
	//	double leg_length = 0.09;
	//	double back_length = 0.08;
	//	double neck_length = 0.05;
	//	
	//	double r = 0.006;
	//	double shoulder_radius = r;
	//	double hip_radius = r;
	//	double arm_radius = r;
	//	double leg_radius = r;
	//	double back_radius = r;
	//	double neck_radius = r;

	//	
	//	klVec3 up( 1, 0, 0 );
	//	klVec3 down = -up;
	//	klVec3 left( 0, -1, 0 );
	//	klVec3 right = -left;
	//	klVec3 front( 0, 0, 1 );

	//	
	//	//pelvis
	//	this->bones.resize(12);
	//	this->bones[0] = MakeNewBone("pelvis", -1,
	//		                          //1, 0, 0, 0.0, 0.0);
	//								  front.x, front.y, front.z, 0.0, 0.0);
	//	//left leg
	//	this->bones[1] = MakeNewBone("lfemur_phantom", 0,
	//	                              left.x, left.y, left.z, hip_length, hip_radius);
	//	this->bones[2] = MakeNewBone("lfemur", 1,
	//		                          down.x, down.y, down.z, leg_length, leg_radius);
	//	//right leg
	//	this->bones[3] = MakeNewBone("rfemur_phantom", 0,
	//		                          right.x, right.y, right.z, hip_length, hip_radius*2.5);
	//	this->bones[4] = MakeNewBone("rfemur", 3,
	//		                          down.x, down.y, down.z, leg_length, leg_radius*2.5);
	//	//back
	//	this->bones[5] = MakeNewBone("back_phantom", 0,
	//		                          up.x, up.y, up.z, back_length, back_radius);
	//	this->bones[6] = MakeNewBone("chest", 5,
	//		                          down.x, down.y, down.z, 0.0, 0.0);
	//	//left arm
	//	this->bones[7] = MakeNewBone("lclavicle_phantom", 6,
	//		                          left.x, left.y, left.z, shoulder_length, shoulder_radius);
	//	this->bones[8] = MakeNewBone("lhumerus", 7,
	//		                          down.x, down.y, down.z, arm_length, arm_radius);
	//	//right arm
	//	this->bones[9] = MakeNewBone("rclavicle_phantom", 6,
	//		                          right.x, right.y, right.z, shoulder_length, shoulder_radius*2.5);
	//	this->bones[10] = MakeNewBone("rhumerus", 9,
	//		                          down.x, down.y, down.z, arm_length, arm_radius*2.5);
	//	//neck
	//	this->bones[11] = MakeNewBone("neck", 6,
	//		                          up.x, up.y, up.z, neck_length, neck_radius);

	//
	//	//build bone dictionaries
	//	for(int i = 0; i < (int)this->bones.size(); i++)
	//	{			
	//		this->boneName.insert(map<int,string>::value_type
	//			(i,  this->bones[i].name));
	//		this->boneIndex.insert(map<string,int>::value_type
	//			(this->bones[i].name, i));
	//	}

	//	setSpecialIndexNames2nd(0, "rclavicle_phantom", "lclavicle_phantom", 
	//		                    "rhumerus", "lhumerus","rhand", "lhand",
	//							"rfemur_phantom", "lfemur_phantom",
	//							"rfemur", "lfemur",
	//							"rfoot", "lfoot","neck", "back_phantom");
	//	
	//}

	//void setIndexesForWonBoxingData()
	//{
	//	this->phantomBoneIndexes.push_back(getBoneIndex("lfemur_phantom"));
	//	this->phantomBoneIndexes.push_back(getBoneIndex("rfemur_phantom"));
	//	this->phantomBoneIndexes.push_back(getBoneIndex("loweback_phantom"));
	//	this->phantomBoneIndexes.push_back(getBoneIndex("LoweNeck_phantom"));
	//	this->phantomBoneIndexes.push_back(getBoneIndex("lclavicle_phantom"));
	//	this->phantomBoneIndexes.push_back(getBoneIndex("rclavicle_phantom"));

	//	this->leftBoneIndexes.push_back(getBoneIndex("lclavicle"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("lhumerus"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("lradius"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("lhand"));
	//	
	//	this->leftBoneIndexes.push_back(getBoneIndex("lfemur"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("ltibia"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("lfoot"));
	//	this->leftBoneIndexes.push_back(getBoneIndex("ltoes"));

	//	this->rightBoneIndexes.push_back(getBoneIndex("rclavicle"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rhumerus"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rradius"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rhand"));

	//	this->rightBoneIndexes.push_back(getBoneIndex("rfemur"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rtibia"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rfoot"));
	//	this->rightBoneIndexes.push_back(getBoneIndex("rtoes"));

	//	this->mediumBoneIndexes.push_back(getBoneIndex("lowerback"));
	//	this->mediumBoneIndexes.push_back(getBoneIndex("upperback"));
	//	this->mediumBoneIndexes.push_back(getBoneIndex("LowerNeck"));
	//	this->mediumBoneIndexes.push_back(getBoneIndex("Neck"));

	//	this->upperBodyIndexes.push_back(getBoneIndex("lowerback"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("upperback"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("LowerNeck"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("Neck"));

	//	this->upperBodyIndexes.push_back(getBoneIndex("lclavicle"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("lhumerus"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("lradius"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("lhand"));

	//	this->upperBodyIndexes.push_back(getBoneIndex("rclavicle"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("rhumerus"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("rradius"));
	//	this->upperBodyIndexes.push_back(getBoneIndex("rhand"));

	//	this->lowerBodyIndexes.push_back(getBoneIndex("lfemur"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("ltibia"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("lfoot"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("ltoes"));

	//	this->lowerBodyIndexes.push_back(getBoneIndex("rfemur"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("rtibia"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("rfoot"));
	//	this->lowerBodyIndexes.push_back(getBoneIndex("rtoes"));

	//}

	//void LoadPuppetSkeleton_withCover()
	//{
	//	printf("[Skeleton]Load start.\n");
	//	this->Initialize();
	//	this->skeletonType = this->Puppet;

	//	double shoulder_length = 0.065;
	//	double hip_length = 0.045;

	//	double arm_length = 0.14;
	//	double leg_length = 0.185;
	//	
	//	double upper_back_length = 0.03;
	//	double lower_back_length = 0.03;
	//	double neck_length = 0.15;
	//	
	//	double r = 0.010;
	//	double shoulder_radius = r;
	//	double hip_radius = r;
	//	double arm_radius = r;
	//	double leg_radius = r;
	//	double lower_back_radius = r;
	//	double upper_back_radius = r;
	//	double neck_radius = r;
	//	
	//	klVec3 up( 1, 0, 0 );
	//	klVec3 down = -up;
	//	klVec3 left( 0, -1, 0 );
	//	klVec3 right = -left;
	//	klVec3 front( 0, 0, 1 );
	//
	//	//pelvis
	//	this->bones.resize(14);

	//	this->bones[0]  = MakeNewBone( "center", -1, front, 0, 0);
	//	this->bones[1]  = MakeNewBone( "lowerback_phantom", 0, down, lower_back_length, lower_back_radius );
	//	this->bones[2]  = MakeNewBone( "pelvis", 1, front, 0, 0 );
	//	
	//	//legs
	//	this->bones[3]  = MakeNewBone( "lfemur_phantom", 2, left, hip_length, hip_radius );
	//	this->bones[4]  = MakeNewBone( "lfemur", 3, down, leg_length, leg_radius );
	//	this->bones[5]  = MakeNewBone( "rfemur_phantom", 2, right, hip_length, hip_radius*1.5 );
	//	this->bones[6]  = MakeNewBone( "rfemur", 5, down, leg_length, leg_radius*2.0 );

	//	this->bones[7]  = MakeNewBone( "upperback_phantom", 0, up, upper_back_length, upper_back_radius );
	//	this->bones[8]  = MakeNewBone( "chest", 7, down, 0, 0 );
	//	
	//	//arms
	//	this->bones[9]  = MakeNewBone( "lclavicle_phantom", 8, left, shoulder_length, shoulder_radius );
	//	this->bones[10] = MakeNewBone( "lhumerus", 9, down, arm_length, arm_radius );
	//	this->bones[11] = MakeNewBone( "rclavicle_phantom", 8, right, shoulder_length, shoulder_radius*1.5 );
	//	this->bones[12] = MakeNewBone( "rhumerus", 11, down, arm_length, arm_radius*2.0 );

	//	//neck
	//	this->bones[13] = MakeNewBone( "neck_phantom", 8, up, neck_length, neck_radius );

	//
	//	//build bone dictionaries
	//	for(int i = 0; i < (int)this->bones.size(); i++)
	//	{			
	//		this->boneName.insert(map<int,string>::value_type
	//			(i,  this->bones[i].name));
	//		this->boneIndex.insert(map<string,int>::value_type
	//			(this->bones[i].name, i));
	//	}

	//	setSpecialIndexNames2nd( 0,
	//							"rclavicle_phantom", "lclavicle_phantom",
	//							"rhumerus", "lhumerus",
	//		                    "rhand", "lhand",
	//							"rfemur_phantom", "lfemur_phantom",
	//							"rfemur", "lfemur","rfootemur", "lfootemur", 
	//							"neck_phantom", "upperback_phantom");

	//	
	//	printf("[Skeleton]Load end.\n");
	//}

	//void LoadPuppetSkeletonWithElbow()
	//{
	//	this->Initialize();
	//	this->skeletonType = this->Puppet_AddElbow;

	//	double shoulder_length = 0.08;
	//	double hip_length = 0.0575;

	//	double arm_length = 0.0625;
	//	double radius_length = 0.0625;
	//	double leg_length = 0.11;
	//	
	//	double upper_back_length = 0.055;
	//	double lower_back_length = 0.045;
	//	double neck_length = 0.08;
	//	
	//	double r = 0.006;
	//	double shoulder_radius = r;
	//	double hip_radius = r;
	//	double arm_radius = r;
	//	double leg_radius = r;
	//	double lower_back_radius = r;
	//	double upper_back_radius = r;
	//	double neck_radius = r;
	//	
	//	klVec3 up( 1, 0, 0 );
	//	klVec3 down = -up;
	//	klVec3 left( 0, -1, 0 );
	//	klVec3 right = -left;
	//	klVec3 front( 0, 0, 1 );
	//	//klVec3 up( 0, 0, 1 );
	//	//klVec3 down = -up;
	//	//klVec3 left( 0, 1, 0 );
	//	//klVec3 right = -left;
	//	//klVec3 front( 1, 0, 0 );

	//
	//	//pelvis
	//	this->bones.resize(16);

	//	this->bones[0]  = MakeNewBone( "center", -1, down, 0, 0);
	//	this->bones[1]  = MakeNewBone( "lowerback_phantom", 0, down, lower_back_length, lower_back_radius );
	//	this->bones[2]  = MakeNewBone( "pelvis", 1, down, 0, 0 );
	//	
	//	//legs
	//	this->bones[3]  = MakeNewBone( "lfemur_phantom", 2, left, hip_length, hip_radius );
	//	this->bones[4]  = MakeNewBone( "lfemur", 3, down, leg_length, leg_radius );
	//	this->bones[5]  = MakeNewBone( "rfemur_phantom", 2, right, hip_length, hip_radius*1.5 );
	//	this->bones[6]  = MakeNewBone( "rfemur", 5, down, leg_length, leg_radius*2.0 );

	//	this->bones[7]  = MakeNewBone( "upperback_phantom", 0, up, upper_back_length, upper_back_radius );
	//	this->bones[8]  = MakeNewBone( "chest", 7, down, 0, 0 );
	//	
	//	//arms
	//	this->bones[9]  = MakeNewBone( "lclavicle_phantom", 8, left, shoulder_length, shoulder_radius );
	//	this->bones[10] = MakeNewBone( "lhumerus", 9, down, arm_length, arm_radius );
	//	this->bones[11] = MakeNewBone( "lradius", 10, down, radius_length, arm_radius );
	//	this->bones[12] = MakeNewBone( "rclavicle_phantom", 8, right, shoulder_length, shoulder_radius*1.5 );
	//	this->bones[13] = MakeNewBone( "rhumerus", 12, down, arm_length, arm_radius*2.0 );
	//	this->bones[14] = MakeNewBone( "rradius", 13, down, radius_length, arm_radius*2.0 );

	//	//neck
	//	this->bones[15] = MakeNewBone( "neck_phantom", 8, up, neck_length, neck_radius );

	//
	//	//build bone dictionaries
	//	for(int i = 0; i < (int)this->bones.size(); i++)
	//	{			
	//		this->boneName.insert(map<int,string>::value_type
	//			(i,  this->bones[i].name));
	//		this->boneIndex.insert(map<string,int>::value_type
	//			(this->bones[i].name, i));
	//	}

	//	setSpecialIndexNames2nd( 0,
	//							"rclavicle_phantom", "lclavicle_phantom",
	//							"rhumerus", "lhumerus",
	//		                    "rradius", "lradius",
	//							"rfemur_phantom", "lfemur_phantom",
	//							"rfemur", "lfemur","rfootemur", "lfootemur", 
	//							"neck_phantom", "upperback_phantom");
	//}


////////////////////////////////////////////////
// >member( private )
////////////////////////////////////////////////
//private:
//	int rshoulderIndex;
//	int lshoulderIndex;
//
//	int relbowIndex;
//	int lelbowIndex;
//
//	int rhandEffecterIndex;
//	int lhandEffecterIndex;
//	
//	int rhipIndex;
//	int lhipIndex;
//
//	int rkneeIndex;
//	int lkneeIndex;
//
//	int rfootEffecterIndex;
//	int lfootEffecterIndex;
//
//	int chestIndex;
//	int headEffecterIndex;
//
//	int rootIndex;
//
//	int retargetingShoRIndex;
//	int retargetingShoLIndex;
//	int retargetingElbowRIndex;
//	int retargetingElbowLIndex;
//	int retargetingHipRIndex;
//	int retargetingHipLIndex;
//	int retargetingKneeRIndex;
//	int retargetingKneeLIndex;
//
//public:
//	vector<int> phantomBoneIndexes;
//	vector<int> mediumBoneIndexes;
//	vector<int> rightBoneIndexes;
//	vector<int> leftBoneIndexes;
//
//	vector<int> upperBodyIndexes;
//	vector<int> lowerBodyIndexes;
//	
//	vector<int> rightArmIndexes;
//	vector<int> leftArmIndexes;
//	vector<int> rightLegIndexes;
//	vector<int> leftLegIndexes;
//	vector<int> neckIndexes;



// public member


	//bool in_bone;
	//bool ang_is_deg; //true -> degrees, false -> radians
	//bool rot_is_glob; //true -> global rotations (v-file), false -> local (amc)
	//bool z_is_up; //If set, will rotate z-up to y-up.

	//should be private

// >getter
public:
	string getBoneName(int bone_index);
	int getBoneIndex(string bone_name);
	SkeletonType getSkeletonType() { return skeletonType_; }
	string getFileName() {return fileName_; }
	int getBoneNum(){return bones.size();}

	void insertBoneName(int index, string boneName)
	{
		boneName_.insert( map<int,string>::value_type( index, boneName ));
	}
	void insertBoneIndex(string boneName, int index)
	{
		boneIndex_.insert( map<string, int>::value_type( boneName, index ));
	}
	int getDataSize(){return dataSize_ ;}


// >setter
public:
	void setSkeletonType( SkeletonType skl_type ) { skeletonType_ = skl_type; }
	void setFileName(string fileName) {fileName_ = fileName; }
	void setDataSize(int size){dataSize_ = size;}

	//int getRshoulderIndex() { return rshoulderIndex; }
	//int getLshoulderIndex() { return lshoulderIndex; }

	//int getRelbowIndex() { return relbowIndex; }
	//int getLelbowIndex() { return lelbowIndex; }
	//	
	//int getRhandEffecterIndex() { return rhandEffecterIndex; }
	//int getLhandEffecterIndex() { return lhandEffecterIndex; }

	//int getRhipIndex() { return rhipIndex; }
	//int getLhipIndex() { return lhipIndex; }

	//int getRkneeIndex() { return rkneeIndex;}
	//int getLkneeIndex() { return lkneeIndex;}

	//int getRfootEffecterIndex() { return rfootEffecterIndex; }
	//int getLfootEffecterIndex() { return lfootEffecterIndex; }
	//
	//int getRootIndex() { return rootIndex; }

	//int getHeadEffecterIndex() { return headEffecterIndex; }
	//int getChestIndex() { return chestIndex; }

	//int getRetargetingShoRIndex() { return retargetingShoRIndex;};
	//int getRetargetingShoLIndex() { return retargetingShoLIndex;};
	//int getRetargetingElbowRIndex() { return retargetingElbowRIndex;};
	//int getRetargetingElbowLIndex() { return retargetingElbowLIndex;};
	//int getRetargetingHipRIndex() { return retargetingHipRIndex;};
	//int getRetargetingHipLIndex() { return retargetingHipLIndex;};
	//int getRetargetingKneeRIndex() { return retargetingKneeRIndex;};
	//int getRetargetingKneeLIndex() { return retargetingKneeLIndex;};

	//string getRetargetingShoRName(){return boneName[getRetargetingShoRIndex()];}
	//string getRetargetingShoLName(){return boneName[getRetargetingShoLIndex()];}
	//string getRetargetingElbowRName(){return boneName[getRetargetingElbowRIndex()];}
	//string getRetargetingElbowLName(){return boneName[getRetargetingElbowLIndex()];}
	//string getRetargetingHipRName(){return boneName[getRetargetingHipRIndex()];}
	//string getRetargetingHipLName(){return boneName[getRetargetingHipLIndex()];}
	//string getRetargetingKneeRName(){return boneName[getRetargetingKneeRIndex()];}
	//string getRetargetingKneeLName(){return boneName[getRetargetingKneeLIndex()];}
	
};

//void unroll(vector< vector< int > > const &list, vector< int > &target, int row);


//implemented in ReadVSK.cpp
//bool ReadVSK(string filename, Skeleton& into);

//implemented in ReadASF.cpp
//bool ReadASF(string filename, Skeleton& into);
	
	//
	////how may dof per frame.
	//int getFrameSize();

	////returns a succinct string describing a dof
	//string get_dof_description(unsigned int dof) const;

	//string getOrder() { return order; }
	//Vector3d getPosition() { return position; }
	//string getOffsetOrder() { return offset_order; }
	//Vector3d getAxisOffset() { return axis_offset; }
	//double getMass() { return mass; }
	//double getLength() { return length; }
	//double getTimeStep() { return timestep; } //should not use
	//string getFileName() { return filename; }
	//
	


#endif //SKELETON_H


//#if 0
//
//	void LoadPuppetSkeleton()
//	{
//		this->Initialize();
//		this->skeletonType = this->Puppet;
//
//		double shoulder_length = 0.07;
//		double hip_length = 0.04;
//		double arm_length = 0.10;
//		double leg_length = 0.09;
//		double back_length = 0.08;
//		double neck_length = 0.05;
//		
//		double r = 0.006;
//		double shoulder_radius = r;
//		double hip_radius = r;
//		double arm_radius = r;
//		double leg_radius = r;
//		double back_radius = r;
//		double neck_radius = r;
//
//		
//		klVec3 up( 1, 0, 0 );
//		klVec3 down = -up;
//		klVec3 left( 0, -1, 0 );
//		klVec3 right = -left;
//		klVec3 front( 0, 0, 1 );
//
//		
//		//pelvis
//		this->bones.resize(12);
//		this->bones[0] = MakeNewBone("pelvis", -1,
//			                          //1, 0, 0, 0.0, 0.0);
//									  front.x, front.y, front.z, 0.0, 0.0);
//		//left leg
//		this->bones[1] = MakeNewBone("lfemur_phantom", 0,
//		                              left.x, left.y, left.z, hip_length, hip_radius);
//		this->bones[2] = MakeNewBone("lfemur", 1,
//			                          down.x, down.y, down.z, leg_length, leg_radius);
//		//right leg
//		this->bones[3] = MakeNewBone("rfemur_phantom", 0,
//			                          right.x, right.y, right.z, hip_length, hip_radius*2.5);
//		this->bones[4] = MakeNewBone("rfemur", 3,
//			                          down.x, down.y, down.z, leg_length, leg_radius*2.5);
//		//back
//		this->bones[5] = MakeNewBone("back_phantom", 0,
//			                          up.x, up.y, up.z, back_length, back_radius);
//		this->bones[6] = MakeNewBone("chest", 5,
//			                          down.x, down.y, down.z, 0.0, 0.0);
//		//left arm
//		this->bones[7] = MakeNewBone("lclavicle_phantom", 6,
//			                          left.x, left.y, left.z, shoulder_length, shoulder_radius);
//		this->bones[8] = MakeNewBone("lhumerus", 7,
//			                          down.x, down.y, down.z, arm_length, arm_radius);
//		//right arm
//		this->bones[9] = MakeNewBone("rclavicle_phantom", 6,
//			                          right.x, right.y, right.z, shoulder_length, shoulder_radius*2.5);
//		this->bones[10] = MakeNewBone("rhumerus", 9,
//			                          down.x, down.y, down.z, arm_length, arm_radius*2.5);
//		//neck
//		this->bones[11] = MakeNewBone("neck", 6,
//			                          up.x, up.y, up.z, neck_length, neck_radius);
//
//	
//		//build bone dictionaries
//		for(int i = 0; i < (int)this->bones.size(); i++)
//		{			
//			this->boneName.insert(map<int,string>::value_type
//				(i,  this->bones[i].name));
//			this->boneIndex.insert(map<string,int>::value_type
//				(this->bones[i].name, i));
//		}
//
//		setSpecialIndexNames2nd(0, "rclavicle_phantom", "lclavicle_phantom", 
//			                    "rhumerus", "lhumerus",
//								"rfemur_phantom", "lfemur_phantom",
//								"rfemur", "lfemur", "neck", "back_phantom");
//		
//	}
//#endif