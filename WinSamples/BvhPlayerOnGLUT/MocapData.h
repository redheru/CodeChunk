
#ifndef MOCAPDATA_H
#define MOCAPDATA_H

#include <assert.h>

//STL
#include <vector>
#include <string>
#include <map>
#include <math.h>

//C++
#include <iostream>
#include <fstream>

//MocapData
#include "Skeleton.h"
#include "Pose.h"

//Math
#include <KlMath/klMath.h>

using namespace std;
using namespace KlMath;

class MocapData
{

//コンストラクタ,デストラクタ
public:
	MocapData();
	MocapData(string fileName);
	~MocapData();

////////////////////////////////////////
// member
////////////////////////////////////////
public:
	vector<Pose> poses;

protected:
	//Skeleton* skeleton_;

	bool loaded_;

	int fps_;
	int frameNum_;
	double frameTime_;
	string name_;

//public:
//	vector<int> keyframes;

////////////////////////////////////////
// public method
////////////////////////////////////////
public:
	// motion全体の向きの補正(座標軸の向きの補正などに使う)
	void correctMotionDirection( klMat44 correct_mat );

	void initialize();

	// mocapdataをbvh変換する(引数のbvhに入れる)
	//void convertToBvh(Bvh &bvh)
	//{
	//	//// 0. 準備・基本的な情報の変換
	//	bvh.file_name = "new_bvh_file.bvh";
	//	bvh.motion_name = "new_motion";
	//	bvh.num_frame = this->frameNum;
	//	bvh.interval = this->frameTime;

	//	//// 1. hierarchyとmotionのデータを作る
	//	//// ( joints, joint_index, channels )

	//	// 1.1 bonesのデータからjoints, channelsを作る
	//	makeJoint( bvh.joints, 0, NULL, bvh.channels );
	//	bvh.num_channel = bvh.channels.size();

	//	// 1.2 joint_indexを作る
	//	for(int i = 0; i < bvh.joints.size(); i++)
	//	{
	//		Joint* joint = bvh.joints[i];
	//		bvh.joint_index.insert( map<string, Joint*>::value_type( bvh.joints[i]->name, bvh.joints[i] ) );
	//	}
	//	
	//	//// 2. motionデータを作る
	//	bvh.motion = new double[ bvh.num_frame * bvh.num_channel ];
	//	for(int frame = 0; frame < this->frameNum; frame++)
	//	{
	//		int motion_index = bvh.num_channel * frame;
	//		Pose& pose = this->poses[frame];

	//		// rootの情報
	//		//// rootの位置
	//		for(int i = 0; i < 3; i++) 
	//			bvh.motion[motion_index++] = pose.root_position[i] * 100;//cm->meter
	//		
	//		//// rootの向き
	//		klVec3 root_zxy = Converter::toEulerZxy(pose.root_orientation);
	//		bvh.motion[motion_index++] = root_zxy.z;
	//		bvh.motion[motion_index++] = root_zxy.x;
	//		bvh.motion[motion_index++] = root_zxy.y;

	//		// それ以外(rootのboneは無視するので、bone=1から)
	//		for(int bone = 1; bone < pose.bone_orientations.size(); bone++)
	//		{
	//			// end site かどうか判定
	//			int child_index = -1;
	//			for(int i = 0; i < skeleton->bones.size(); i++)
	//			{
	//				// 子供発見
	//				if( skeleton->bones[i].parent == bone )
	//				{
	//					child_index = i;
	//					break;
	//				}
	//			}
	//			// end siteなのでmotionにはデータを入れない
	//			if( child_index == -1 ) continue;

	//			// jointの向きをmotionに入れる
	//			klVec3 zxy = Converter::toEulerZxy(pose.bone_orientations[child_index]);
	//			bvh.motion[motion_index++] = zxy.z;
	//			bvh.motion[motion_index++] = zxy.x;
	//			bvh.motion[motion_index++] = zxy.y;
	//		}
	//	}

	//	//// 3. おしまい
	//	bvh.is_load_success = true;
	//}

	//void loadFromBvh( Bvh &bvh )
	//{
	//	//// 1. 基本的な情報のコピー
	//	this->name = bvh.file_name;
	//	this->frameNum = bvh.num_frame;
	//	this->frameTime = bvh.interval;
	//	this->fps = ceil(1.0/bvh.interval);
	//	this->poses.clear();
	//	this->skeleton = new Skeleton();

	//	//// 2. 一旦ポーズデータを作る
	//	for( int frame = 0; frame < this->frameNum; frame++ )
	//	{
	//		Pose new_pose;
	//		new_pose.skeleton = this->skeleton;
	//		Joint* root_joint_P = bvh.joints[0];
	//		klVec3 root_pos;
	//		
	//		// root位置の読み込み
	//		if( bvh.ComputePosFromJoint( *root_joint_P, frame, root_pos ) == false )
	//		{
	//			// root jointに位置情報が含まれなかった時のエラー処理
	//		}
	//		root_pos = root_pos;
	//		new_pose.root_position = Converter::toVector3f( root_pos ) / 100.0;

	//		// root向きの読み込み
	//		klMat44 root_rot_mat = bvh.ComputeRotMatFromJoint( *root_joint_P, frame );
	//		root_rot_mat = root_rot_mat;
	//		new_pose.root_orientation = Converter::toQuatf( root_rot_mat);
	//		this->poses.push_back( new_pose );
	//	}

	//	//// 3. BvhのHierarchy情報からSkeletonクラス作成(+posesのboneの向きの読み込み)
	//	skeleton->Initialize();
	//	addBoneFromBvhJoint( bvh, *(bvh.joints.begin()), -1, NULL, false );// rootのjoint指定したら後は再帰的に子をたどってやってくれる
	//	skeleton->ScaleSkeleton( 0.01 );

	//	for(int i = 0; i < skeleton->bones.size(); i++)
	//	{
	//		Bone& bone = skeleton->bones[i];
	//		skeleton->boneName.insert( map<int,string>::value_type( i, bone.name ) );
	//		skeleton->boneIndex.insert( map<string, int>::value_type( bone.name, i ) );
	//	}

	//	//CMU mocapのデータはEndEffecterとRetargeting用の関節名を決定する
	//	if(skeleton->findBoneName("Spine1")) 
	//	{
	//		skeleton->setSpecialIndexNames2nd( 0, //root
	//							"RightArm", "LeftArm", //shoulder
	//							"RightForeArm", "LeftForeArm", //elbow
	//		                    "RightHand", "LeftHand", // hand (end-effecter)
	//							"RightUpLeg", "LeftUpLeg", // hip
	//							"RightLeg", "LeftLeg", //knee
	//							"RightFoot", "LeftFoot", //foot(end-effecter)
	//							"Neck1", "Spine1");// head(end-effecter), chest

	//		skeleton->setRetargetingIndexNames(
	//			"RightForeArm", "LeftForeArm", // shoulder
	//			"RightHand", "LeftHand", // elbow
	//			"RightLeg", "LeftLeg", // hip
	//			"RightFoot", "LeftFoot"); // knee

	//		skeleton->setSkeletonType(Skeleton::CMU);
	//	}

	//	//// 4.向きの修正(X軸周りに90度回す)
	//	//klMat44 correct_mat;
	//	//correct_mat.LoadIdentity();
	//	//correct_mat.Rotate( 90, 1, 0, 0 );
	//	//correctMotionDirection( correct_mat );

 //		loaded_ = true;
	//}
	//void writeAsBvh( string file_name )
	//{
	//	Bvh bvh;
	//	convertToBvh(bvh);
	//	bvh.output(file_name);
	//}

	
	//void ComputeKeyframes(double height_thresh = 0.04, int frame_width_thresh = 5 )
	//{
	//	// for test 
	//	// output intensity graph
	//		
	//	int local_min_frame = 0;
	//	int local_max_frame = 0;
	//	double local_min = 0;
	//	double local_max = 0;
	//	int width_count = 0;


	//	
	//	//→極小値を探すフェーズ→極大値を探すフェーズ→
	//	//と移り変わっていく
	//	bool min_search_mode = true;
	//	vector<double> temp_intensities;

	//	for(int frame = 0; frame < this->frameNum; frame++)
	//	{
	//		double temp_intensity = getIntensity(frame, 3, 2.0);
	//		#ifndef OUTPUT_CSV
	//		temp_intensities.push_back(temp_intensity);
	//		#endif
	//		
	//		//極大値の更新
	//		if( temp_intensity > local_max)
	//		{
	//			local_max = temp_intensity;
	//			local_max_frame = frame;
	//		}
	//		
	//		//極小値の更新
	//		if( temp_intensity < local_min)
	//		{
	//			local_min = temp_intensity;
	//			local_min_frame = frame;
	//		}

	//		//極小値を確定するか判定
	//		if(min_search_mode)
	//		{				
	//			//山の高さがthreshold以上なら、
	//			//この前記憶した極小値をキーフレームとして認める
	//			if( frame - local_min_frame > frame_width_thresh)
	//			{
	//				if( local_max - local_min > height_thresh)
	//				{
	//					this->keyframes.push_back(local_min_frame);
	//					min_search_mode = false;
	//					local_min = 10000000;
	//				}
	//			}
	//		}
	//		//極大値を確定するか判定
	//		else
	//		{
	//			if( frame - local_max_frame > frame_width_thresh)
	//			{
	//				if( local_max - local_min > height_thresh)
	//				{
	//					min_search_mode = true;
	//					local_max = 0;
	//				}
	//			}
	//		}
	//	}

	//	#ifndef OUTPUT_CSV
	//	ofstream ost("intensity_graph3.csv");
	//	ost << "intensity, keyframe" << endl;
	//	
	//	

	//	int j = 0;
	//	for(int i = 0; i < (int)temp_intensities.size(); i++)
	//	{
	//		if(this->keyframes.size() > j)
	//		{
	//			if(this->keyframes[j] == i)
	//			{	
	//				ost << temp_intensities[i] << "," << "0.03" << endl;
	//				j++;
	//			}
	//			else
	//			{
	//				ost << temp_intensities[i] << "," << 0.0 << endl;
	//			}
	//		}
	//		else
	//		{
	//			ost << temp_intensities[i] << "," << 0.0 << endl;
	//		}
	//	}
	//	#endif
	//}




	//double getIntensity(int frame, int interval_frame, double root_rate = 2.0)
	//{
	//	assert( frame < (int)poses.size() );
	//	int pre_frame = frame - interval_frame;
	//	if( pre_frame < 0 ) return 0;

	//	Pose& pose = poses[frame];
	//	Pose& pre_pose = poses[pre_frame];
	//	
	//	double intensity = 0.0;
	//	Quatf cur_root = pose.getRootAngle();
	//	Quatf pre_root = pre_pose.getRootAngle();
	//	int bone_num = this->getBoneNum();

	//	intensity += root_rate * Rot::QuatDiff( klVec3(0.3, 0.5, 0.2).Normalize(), cur_root, pre_root);
	//	for(int i = 0; i < bone_num; i++)
	//	{
	//		klVec3 offset = pose.getBoneDirection(i);
	//		Quatf cur_bone_ang = pose.getBoneAngle(i);
	//		Quatf pre_bone_ang = pre_pose.getBoneAngle(i);
	//		intensity += Rot::QuatDiff( offset, cur_bone_ang, pre_bone_ang );
	//	}
	//	intensity /= root_rate + bone_num;
	//	return intensity;
	//}




	//trimming
	void Trimming(int start_frame, int end_frame);

	void EraseFirstFrame();	
	void EraseEndFrame();
	void ClearFrames();

	//
	// set data
	//

	//Add a new Pose at the end of "poses"
	void AddPose(Pose add_pose);
	
	//
	// read/write
	//

	//make vacant motion
	//then you should add Pose by AddPose
	bool LoadVacantMotion(Skeleton* skl, int fps);

	////read motion datap
	////from .v .amc (.bvh)
	//bool LoadMotion(string file_name, Skeleton* skl, int fps)
	//{
	//	if( !boost::filesystem::exists( boost::filesystem::path( file_name ) ) )
	//	{
	//		printf("[MocapData] File isn't exist!\n");
	//		return false;
	//	}

	//	//load only once
	//	if(loaded_ == true) return false;

	//	skeleton = skl;

	//	//load motion
	//	vector<double> data;
	//	LoadMotionToVector(file_name, data);
	//	
	//	this->fps = fps;
	//	frameNum =  (int)( data.size() / skl->frame_size );
	//	frameTime = 1.0 / (double)fps;
	//	//frameTime = (double)input_motion.length() / (double)frameNum;

	//	//build "poses"
	//	for(int f = 0; f < frameNum; f++)
	//	{
	//		Pose temp_pose;
	//		temp_pose.BuildPose(&(data[0]) + f * skl->frame_size, skl);
	//		poses.push_back(temp_pose);
	//	}

	//	//CorrectRootPositionToRelative();

	//	this->name = file_name;
	//	loaded_ = true;
	//	return true;
	//}

	//from .bvh
	bool LoadMotion(string fileName);

	//write motion data into V
	//bool WriteV(string filename, double fps = -1, int start_frame = -1, int end_frame = -1)
	//{
	//	if(-1 == fps) fps = this->fps;
	//	int frameRate = (int) ceil( (double)1 / frameTime);

	//	if( start_frame == -1 || end_frame == -1 )
	//	{
	//		WriteAnimationV2(filename, this->poses, frameRate, *(this->skeleton), this->skeleton->filename, fps);
	//		return true;
	//	}
	//	else 
	//	{
	//		assert(start_frame >= 0);
	//		assert(end_frame >= 0);
	//		assert(end_frame-start_frame > 0);

	//		Trimming(start_frame, end_frame);
	//		WriteAnimationV2(filename, this->poses, frameRate, *(this->skeleton), this->skeleton->filename, fps);
	//		return true;
	//	}
	//}

	//Eulerの順番はZXYに固定される
	bool WriteBvh( string file_name);

	void CorrectRootPosition(int frame);

////////////////////////////////////////
//private method
////////////////////////////////////////
private:
	// [loadFromBvhの補助メソッド]
	// 指定されたBvhのJointとその子Jointを
	// MotionDataのSkeletonのboneとして追加する
	//void addBoneFromBvhJoint( Bvh &target_bvh, Joint* read_joint, int parent_bone_index, Joint* parent_joint, bool copy_bone_flag = false )
	//{
	//	if( read_joint == NULL ) return;

	//	// 新しいBoneの作成
	//	Bone new_bone;
	//	klVec3 bone_vec = klVec3( read_joint->offset );
	//	
	//	// Boneの要素の代入
	//	new_bone.length = bone_vec.Length();
	//	if( new_bone.length < 0.000001 ) new_bone.direction = Converter::toVector3d( klVec3(0,0,1) );
	//	else new_bone.direction = Converter::toVector3d( bone_vec.Normalize() );
	//	new_bone.name = read_joint->name;
	//	new_bone.radius = new_bone.length/8; // 太さによって変えてもいいかもー
	//	new_bone.dof = 3;// とりあえず3にしとく
	//	new_bone.parent = parent_bone_index;
	//	//new_bone.is_copy = copy_bone_flag;
	//	
	//	// Boneの追加
	//	this->skeleton->bones.push_back( new_bone );
	//	int cur_bone_index = (int)this->skeleton->bones.size() -1;

	//	// posesにBoneの向きデータの代入
	//	addDataToPoses_forBvh( target_bvh, parent_joint );
	//			
	//	// 子Jointについて再帰的に追加
	//	bool many_children_flag = false;// 複数の子があるか
	//	if( read_joint->children.size() > 1 ) many_children_flag = true;
	//	for(int i = 0; i < read_joint->children.size(); i++)
	//	{
	//		addBoneFromBvhJoint( target_bvh, read_joint->children[i], cur_bone_index, read_joint, many_children_flag );
	//	}

	//	// EndSite(先っちょ)の例外処理
	//	// bvhがリンク(骨のつなぎ目)単位で扱っているのを骨単位で扱うので
	//	// 一番先のBoneは作る必要がある(ここではEndSiteは複数持たないものとして扱っている)
	//	if( read_joint->has_site == true )
	//	{
	//		Bone end_bone;
	//		end_bone.name = new_bone.name + "_End";
	//		klVec3 bone_vec = klVec3( read_joint->site );
	//		end_bone.length = bone_vec.Length();
	//		end_bone.direction = Converter::toVector3d( bone_vec.Normalize() );
	//		end_bone.radius = new_bone.length / 8;
	//		end_bone.dof = 3;
	//		end_bone.parent = cur_bone_index;

	//		//add
	//		skeleton->bones.push_back( end_bone );
	//		addDataToPoses_forBvh( target_bvh, read_joint );
	//	}
	//}

	// [loadFromBvhの補助メソッド]
	// posesにBoneの向きデータの代入
	//void addDataToPoses_forBvh( Bvh &bvh, Joint* parent_joint )
	//{
	//	for(int frame = 0; frame < this->frameNum; frame++)
	//	{
	//		Quatf q;

	//		// root jointの場合(向き0として扱う)
	//		if( parent_joint == NULL ) q.clear();
	//		
	//		// 親がroot Joint場合(向き0として扱う)
	//		else if( parent_joint->index == 0 ) q.clear();
	//		
	//		// 通常の場合(親のJointの向きが今のBoneの向きになる)
	//		else
	//		{
	//			klMat44 mat = bvh.ComputeRotMatFromJoint( *parent_joint, frame );
	//			q = Converter::toQuatf( mat );
	//		}
	//		poses[frame].bone_orientations.push_back(q);
	//	}
	//}


	// [convertToBvhの補助メソッド]
	// Bone[cur_bone_index]からJointとその子Joint全部を作って
	// さらにjoints, bvh_channelsに情報を追加する
	//Joint* makeJoint( vector<Joint*> &joints, int cur_bone_index, Joint* parent_joint, vector<Channel*> &bvh_channels )
	//{
	//	int channel_index = bvh_channels.size();
	//	Skeleton *skl = this->skeleton;
	//	Bone& cur_bone = skl->bones[cur_bone_index];
	//	int bone_num = skl->bones.size();
	//	
	//	// end siteかどうか判定
	//	bool is_end_site = true;
	//	for(int j = 0; j < bone_num; j++)
	//	{
	//		if( skl->bones[j].parent == cur_bone_index )
	//		{
	//			is_end_site = false;
	//			break;
	//		}
	//	}
	//	// end siteだったらjointは作らない
	//	if( is_end_site == true ) return NULL;


	//	// Jointを作っていく
	//	Joint* cur_joint;
	//	cur_joint = new Joint();
	//	joints.push_back( cur_joint );// ちゃんとjointsに追加しておく
	//	
	//	// 基本的な情報の追加
	//	cur_joint->index = joints.size()-1;
	//	cur_joint->parent = parent_joint;
	//	cur_joint->name = cur_bone.name;
	//	Vector3d offset = cur_bone.direction * cur_bone.length * 100;
	//	for(int i = 0; i < 3; i++) cur_joint->offset[i] = offset[i];

	//	// CHANNELの情報の追加
	//	//// 位置のchannel追加(ROOTの場合)
	//	Channel* channel;
	//	if( parent_joint == NULL )
	//	{	
	//		channel = new Channel();
	//		cur_joint->channels.push_back(channel);
	//		Bvh::makeChannel( channel, cur_joint, X_POSITION, channel_index++ );
	//		channel = new Channel();
	//		cur_joint->channels.push_back(channel);
	//		Bvh::makeChannel( channel, cur_joint, Y_POSITION, channel_index++ );
	//		channel = new Channel();
	//		cur_joint->channels.push_back(channel);
	//		Bvh::makeChannel( channel, cur_joint, Z_POSITION, channel_index++ );
	//	}
	//	//// 向きのchannelを追加(root,それ以外で共通)
	//	//// 順番はzxyにする(motionを入れる順番と統一されている)
	//	channel = new Channel();
	//	cur_joint->channels.push_back(channel);
	//	Bvh::makeChannel( channel, cur_joint, Z_ROTATION, channel_index++ );
	//	channel = new Channel();
	//	cur_joint->channels.push_back(channel);
	//	Bvh::makeChannel( channel, cur_joint, X_ROTATION, channel_index++ );
	//	channel = new Channel();
	//	cur_joint->channels.push_back(channel);
	//	Bvh::makeChannel( channel, cur_joint, Y_ROTATION, channel_index++ );

	//	//// Bvhクラスのchannelsに追加
	//	for(int i = 0; i < cur_joint->channels.size(); i++)
	//		bvh_channels.push_back( cur_joint->channels[i] );

	//	// 子Jointを作って追加(再帰的に行う)
	//	for(int j = 0; j < bone_num; j++)
	//	{
	//		Bone& child_bone = skl->bones[j];
	//		// 子供発見
	//		if( child_bone.parent == cur_bone_index)
	//		{
	//			// 再帰的に呼び出す
	//			Joint *child_joint = makeJoint( joints, j, cur_joint, bvh_channels ); 
	//			
	//			// 子BoneがEndSiteのとき
	//			if( child_joint == NULL )
	//			{
	//				// 現在のjointにend siteの情報を追加
	//				cur_joint->has_site = true;
	//				Vector3d site_offset = child_bone.direction * child_bone.length * 100;// m -> cm
	//				for(int k = 0; k < 3; k++) cur_joint->site[k] = site_offset[k];
	//				break;
	//			}
	//			else
	//			{
	//				cur_joint->has_site = false;
	//				cur_joint->children.push_back(child_joint);
	//			}
	//		}
	//	}

	//	// 終了	
	//	return cur_joint;
	//}




	//void LoadMotionToVector(string file_name, vector<double>& into)
	//{
	//	//get file extension
	//	string ex = file_name.substr( file_name.find("."), file_name.size()-file_name.find("."));

	//	//Load motion from v file
	//	if( ex == ".v" | ex == ".V" )
	//	{
	//		//implemented in "ReadVfile.cpp"
	//		if ( !ReadV(file_name, *skeleton, into) )
	//		{
	//			cerr << "Error reading motion from " << file_name << "." << endl;
	//		}
	//	}

	//	//Load motion from amc file
	//	else if ( ex == ".amc" | ex == ".AMC" )
	//	{
	//		//implemented in "ReadVfile.cpp"
	//		if ( !ReadAMC(file_name, *skeleton, into) )
	//		{
	//			cerr << "Error reading motion from " << file_name << "." << endl;
	//		}
	//	}

	//	//wrong fiile type
	//	else
	//	{
	//			cerr << "Wrong file type!" << endl;
	//			return;
	//	}

	//	//cout << "LoadMotionToVector : finish!" << endl;
	//		
	//}

	//void CorrectRootPositionToRelative()
	//{
	//	Vector3f base_pos = this->poses[0].getRootPosition();
	//	base_pos.z = 0.0;
	//	for(int i = 0; i < this->frameNum; i++)
	//	{
	//		this->poses[i].SetRootPosition( this->poses[i].getRootPosition() - base_pos );
	//	}
	//}


//
// >getter
// >setter
//

	Skeleton* ReadHierarchy(ifstream& file);
	bool ReadMotionInfo(ifstream& file);
	bool ReadMotionData(ifstream& file, Skeleton* skl);

	string getChannelNameString( klVec3 eulerOrder, bool isPosition);

	void WriteHierarchy(Skeleton* skl, Bone* bone, ofstream& ofs, int depth);

public:

// >getter
	Pose getPose(int frame){return this->poses[frame];}
	Skeleton* getSkeleton(int frame = 0) { return this->poses[frame].getSkeleton(); }
	int getFps(){ return fps_; }
	int getBoneNum(int frame = 0){return (int)getSkeleton(frame)->bones.size();}
	int getFrameNum(){ return frameNum_; }
	bool getIsLoaded(){return loaded_;}
	//set by AddPose()
	//void setFrameNum(int frame_num){ this->frameNum = frame_num; }
	klVec3 getBoneDirection(int bone_index, int frame = 0){ return getSkeleton(frame)->bones[bone_index]->direction; }
	string getName(){ return name_; }
	double getFrameTime(){ return frameTime_; }
	int getBoneIndex(string boneName, int frame = 0){return getSkeleton(frame)->getBoneIndex(boneName);}
	string getBoneName(int boneIndex, int frame = 0){return getSkeleton(frame)->getBoneName(boneIndex);}
	//fastest acces
	klQuat getBone_Quat(int frame, int boneIndex){return poses[frame].getBoneAngle(boneIndex);}
	//slower than getBone_Quaternion_by_index
	klQuat getBone_Quat(int frame, string boneName)
	{
		int index = getSkeleton(frame)->getBoneIndex(boneName);
		return getBone_Quat(frame, index);
	}
	klVec3 getNodePosition(int frame, string boneName)
	{
		int index = getSkeleton(frame)->getBoneIndex(boneName);
		return getNodePosition(frame, index);
	}
	//FK
	klVec3 getNodePosition(int frame, int bone_index){return poses[frame].getNodePosition(bone_index);}
	klVec3 getNodePositionFromBone(int frame, int bone_index, int from_bone_index){return poses[frame].getNodePositionFromBone( bone_index, from_bone_index );}

	klVec3 getRootPosition(int frame){return poses[frame].getRootPosition();}	
	klQuat getRootRotation(int frame){return poses[frame].getRootAngle();}

// > setter

	void setName(string name){ name_ = name; }
	void setFrameTime(float frame_time){ frameTime_ = frame_time; }

	//change bone orientation of already exit Pose
	bool SetBoneAngle(klQuat q, int frame, int bone_index)
	{
		if( frame >= (int)poses.size() ) return false;
		poses[frame].SetBoneAngle(q, bone_index);
		return true;
	}


	void setFps(int fps){ fps_ = fps; }

	void SetRootAngle(klQuat q, int frame){poses[frame].SetRootAngle(q);}
	void SetRootPos(int frame, float x, float y, float z){poses[frame].SetRootPosition(x, y, z);}
	bool SetBoneAngle(klQuat q, int frame, string bone_name){return SetBoneAngle(q, frame,  getSkeleton(frame)->getBoneIndex(bone_name));}


	//int getEffecterIndex( Enums::Effecter effecter_enum )
	//{
	//	if( effecter_enum == Enums::effecter_Rhand ) return skeleton->rhandEffecterIndex;
	//	else if( effecter_enum == Enums::effecter_Lhand ) return skeleton->lhandEffecterIndex;
	//	else if( effecter_enum == Enums::effecter_Rfoot ) return skeleton->rfootEffecterIndex;
	//	else if( effecter_enum == Enums::effecter_Lfoot ) return skeleton->lfootEffecterIndex;
	//	else if( effecter_enum == Enums::effecter_Head ) return skeleton->headEffecterIndex;
	//	else if( effecter_enum == Enums::effecter_Root ) return skeleton->rootIndex;
	//}

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
	
	void setRootPosition( klVec3 pos, int frame ){ this->poses[frame].SetRootPosition(pos); }
	void setRootRotation( klQuat q, int frame ){ this->poses[frame].SetRootAngle( q ); }

};
		//Quatf a = this->poses[500].getRootOrientation();
		//Vector3f b = this->poses[500].getRootPosition();
		//cout << "root orientation : " <<  a.x << " , " << a.y <<  " , " << a.z <<endl;
		//cout << "root position    : " <<  b.x << " , " << b.y <<  " , " << b.z <<endl;

		//for(int i = 0; i<20; i++)
		//{
		//	Quatf c = this->poses[500].getBoneOrientation(i);
		//	cout << "bone" << i << " orien      : " <<  c.x << " , " << c.y <<  " , " << c.z <<endl;
		//}

#endif