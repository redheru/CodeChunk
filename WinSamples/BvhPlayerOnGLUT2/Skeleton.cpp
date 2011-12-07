#include "Skeleton.h"

#include <assert.h>
#include <functional>
#include <algorithm>

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using std::cout;
using std::endl;

void Bone::Initialize()
{
	name = "";
	length = 0.0;
	parent = 0;
	radius = 0.0;
	frameOffset = 0;
	direction = klVec3(0,0,0);

	eulerOrder.resize(3);
	eulerOrder[0].x = 0; eulerOrder[0].y = 0; eulerOrder[0].z = 0;
	eulerOrder[1].x = 0; eulerOrder[1].y = 0; eulerOrder[1].z = 0;
	eulerOrder[2].x = 0; eulerOrder[2].y = 0; eulerOrder[2].z = 0;

}


void Skeleton::Initialize()
{
	dataSize_ = 0;
	fileName_ = "";
	boneName_.clear();
	boneIndex_.clear();

	bones.clear();
	//this->positionEulerOrder.resize(3);
	//this->positionEulerOrder[0].x = 0; this->positionEulerOrder[0].y = 0; this->positionEulerOrder[0].z = 0;
	//this->positionEulerOrder[1].x = 0; this->positionEulerOrder[1].y = 0; this->positionEulerOrder[1].z = 0;
	//this->positionEulerOrder[2].x = 0; this->positionEulerOrder[2].y = 0; this->positionEulerOrder[2].z = 0;
}

void Skeleton::Load(string file_name)
{
	//get file extension
	string ex = file_name.substr( file_name.find("."), file_name.size()-file_name.find("."));
	
	//Load skeleton from bvh file
	if( ex == ".bvh" | ex == ".BVH" )
	{
		//implemented in "ReadBVH.cpp"
		if ( !ReadBVH(file_name ) )
		{
			cerr << "Error reading skeleton from " << file_name << "." << endl;
		}
	}
	//if( ex == ".vsk" | ex == ".VSK" )
	//{
	//	//implemented in "ReadVSK.cpp"
	//	if ( !ReadVSK(file_name, *this ) )
	//	{
	//		cerr << "Error reading skeleton from " << file_name << "." << endl;
	//	}
	//}
	////Load skeleton from asf file
	//else if( ex == ".asf" | ex == ".ASF" )
	//{
	//	//implemented in "ReadASF.cpp"
	//	if ( !ReadASF(file_name, *this) )
	//	{
	//		cerr << "Error reading skeleton from " << file_name << "." << endl;
	//	}
	//}
	//wrong fiile type
	else
	{
		cerr << "Wrong file type!" << endl;
		return;
	}

	//build bone dictionaries
	for(int i = 0; i < (int)this->bones.size(); i++)
	{			
		boneName_.insert(map<int,string>::value_type
			(i,  this->bones[i]->name));
		boneIndex_.insert(map<string,int>::value_type
			(this->bones[i]->name, i));
	}
}

//TODO : 自作Stringクラスを使ってもっとすっきりさせる
bool Skeleton::ReadBVH(string fileName){
	// parse the .bvh and check it has a KinematicModel

	//#define  BUFFER_LENGTH  1024*4
	ifstream  file;
	//char      line[BUFFER_LENGTH];
	//char *    token;
	//char      separater[] = " :,\t";
	//vector< Bone *>   bone_stack;
	//Bone* bone = NULL;
	//Bone* new_bone = NULL;
	//bool      is_site = false;
	//double    x, y ,z;
	//int       i;

	// ファイルのオープン
	//skl.setFileName(fileName);

	file.open(fileName.c_str(), ios::in );
	if(file.is_open() == 0) return false;// ファイルが開けなかったら終了

	if(BuildSkeleton(file) == false) return false;

	fileName_ = fileName;//ファイル名を登録

	////データ配列のオフセットを格納する
	//int frameOffset = 0;

	//// 階層情報の読み込み
	//while ( ! file.eof() )
	//{
	//	// ファイルの最後まできてしまったら異常終了
	//	if ( file.eof() )  return false;

	//	// １行読み込み、先頭の単語を取得
	//	file.getline( line, BUFFER_LENGTH );
	//	token = strtok( line, separater );

	//	// 空行の場合は次の行へ
	//	if ( token == NULL )  continue;

	//	// 関節ブロックの開始
	//	if ( strcmp( token, "{" ) == 0 )
	//	{
	//		// 現在の関節をスタックに積む
	//		bone_stack.push_back( bone );
	//		bone = new_bone;
	//		continue;
	//	}
	//	// 関節ブロックの終了
	//	if ( strcmp( token, "}" ) == 0 )
	//	{
	//		// 現在の関節をスタックから取り出す
	//		bone = bone_stack.back();
	//		bone_stack.pop_back();
	//		is_site = false;
	//		continue;
	//	}

	//	// 関節情報の開始
	//	if ( ( strcmp( token, "ROOT" ) == 0 ) ||
	//		( strcmp( token, "JOINT" ) == 0 ) )
	//	{
	//		// 新しいBoneの作成
	//		new_bone = new Bone();

	//		//Boneの名前の読み込み
	//		token = strtok( NULL, "" );
	//		while ( *token == ' ' )  token ++;
	//		new_bone->name = token;

	//		//オフセットの格納
	//		new_bone->frameOffset = frameOffset;

	//		// インデックスへ追加
	//		int index = getBoneNum();
	//		bones.push_back(new_bone);
	//		insertBoneName( index, new_bone->name );
	//		insertBoneIndex( new_bone->name, index );

	//		//親Boneと接続
	//		if(bone == NULL)
	//			new_bone->parent = -1;
	//		else
	//		{
	//			new_bone->parent = getBoneIndex(bone->name);
	//			bone->children.push_back( getBoneIndex(new_bone->name));
	//		}
	//		
	//		//// 関節データの作成
	//		//new_joint = new Joint();
	//		//new_joint->index = joints.size();
	//		//new_joint->parent = joint;
	//		//new_joint->has_site = false;
	//		//new_joint->offset[0] = 0.0;  new_joint->offset[1] = 0.0;  new_joint->offset[2] = 0.0;
	//		//new_joint->site[0] = 0.0;  new_joint->site[1] = 0.0;  new_joint->site[2] = 0.0;
	//		//joints.push_back( new_joint );
	//		//if ( joint )
	//		//	joint->children.push_back( new_joint );

	//		//// 関節名の読み込み
	//		//token = strtok( NULL, "" );
	//		//while ( *token == ' ' )  token ++;
	//		//new_joint->name = token;

	//		//// インデックスへ追加
	//		//joint_index[ new_joint->name ] = new_joint;
	//		continue;
	//	}

	//	// 末端情報の開始
	//	if ( ( strcmp( token, "End" ) == 0 ) )
	//	{
	//		new_bone = bone;
	//		//new_joint = joint;
	//		is_site = true;
	//		continue;
	//	}

	//	// 関節のオフセット or 末端位置の情報
	//	if ( strcmp( token, "OFFSET" ) == 0 )
	//	{
	//		// 座標値を読み込み
	//		token = strtok( NULL, separater );
	//		x = token ? atof( token ) : 0.0;
	//		token = strtok( NULL, separater );
	//		y = token ? atof( token ) : 0.0;
	//		token = strtok( NULL, separater );
	//		z = token ? atof( token ) : 0.0;

	//		// 関節のオフセットに座標値を設定
	//		if ( is_site )
	//		{
	//			// 新しいBone(EndSite)の作成
	//			Bone* end_bone = new Bone();

	//			//Boneの名前の読み込み
	//			end_bone->name = getBoneName(getBoneNum()-1) + "_End";

	//			//オフセットの格納
	//			end_bone->frameOffset = frameOffset;

	//			// インデックスへ追加
	//			int index = getBoneNum();
	//			bones.push_back(end_bone);
	//			insertBoneName( index, end_bone->name );
	//			insertBoneIndex( end_bone->name, index );

	//			//親Boneと接続
	//			end_bone->parent = getBoneIndex(bone->name);
	//			bone->children.push_back( getBoneIndex(end_bone->name));

	//			klVec3 bone_vec = klVec3( x, y, z );
	//			end_bone->length = bone_vec.Length();
	//			end_bone->direction = bone_vec.Normalize();
	//			end_bone->radius = new_bone->length / 8;

	//			//joint->has_site = true;
	//			//joint->site[0] = x;
	//			//joint->site[1] = y;
	//			//joint->site[2] = z;
	//		}
	//		else
	//		// 末端位置に座標値を設定
	//		{
	//			klVec3 bone_vec = klVec3(x, y, z);
	//			bone->length = bone_vec.Length();
	//			if( bone->length < 0.000001) bone->direction = klVec3(0, 0, 1);
	//			else bone->direction = bone_vec.Normalize();
	//			bone->radius = bone->length/8;
	//		}
	//		continue;
	//	}

	//	// 関節のチャンネル情報
	//	if ( strcmp( token, "CHANNELS" ) == 0 )
	//	{
	//		// チャンネル数を読み込み
	//		token = strtok( NULL, separater );
	//		int channel_num = token ? atoi(token) : 0;
	//		//joint->channels.resize( token ? atoi( token ) : 0 );

	//		// チャンネル情報を読み込み
	//		for ( i=0; i<channel_num; i++ )
	//		{
	//			//// チャンネルの作成
	//			//Channel *  channel = new Channel();
	//			//channel->joint = joint;
	//			//channel->index = channels.size();
	//			//channels.push_back( channel );
	//			//joint->channels[ i ] = channel;

	//			// チャンネルの種類の判定
	//			token = strtok( NULL, separater );

	//			if ( strcmp( token, "Xrotation" ) == 0 )
	//				bone->eulerOrder.push_back(klVec3(1.0, 0.0, 0.0));
	//			else if ( strcmp( token, "Yrotation" ) == 0 )
	//				bone->eulerOrder.push_back(klVec3(0.0, 1.0, 0.0));
	//			else if ( strcmp( token, "Zrotation" ) == 0 )
	//				bone->eulerOrder.push_back(klVec3(0.0, 0.0, 1.0));
	//			else if ( strcmp( token, "Xposition" ) == 0 )
	//				positionEulerOrder.push_back(klVec3(1.0, 0.0, 0.0));
	//			else if ( strcmp( token, "Yposition" ) == 0 )
	//				positionEulerOrder.push_back(klVec3(0.0, 1.0, 0.0));
	//			else if ( strcmp( token, "Zposition" ) == 0 )
	//				positionEulerOrder.push_back(klVec3(0.0, 0.0, 1.0));

	//			//if ( strcmp( token, "Xrotation" ) == 0 )
	//			//	channel->type = X_ROTATION;
	//			//else if ( strcmp( token, "Yrotation" ) == 0 )
	//			//	channel->type = Y_ROTATION;
	//			//else if ( strcmp( token, "Zrotation" ) == 0 )
	//			//	channel->type = Z_ROTATION;
	//			//else if ( strcmp( token, "Xposition" ) == 0 )
	//			//	channel->type = X_POSITION;
	//			//else if ( strcmp( token, "Yposition" ) == 0 )
	//			//	channel->type = Y_POSITION;
	//			//else if ( strcmp( token, "Zposition" ) == 0 )
	//			//	channel->type = Z_POSITION;
	//		}

	//		//オフセットの更新
	//		frameOffset += channel_num;

	//	}
	//	// Motionデータのセクションへ移る
	//	if ( strcmp( token, "MOTION" ) == 0 )
	//		break;
	//}

	//// スケルトンのタイプを設定
	//skeletonType_ = Human;

	// ファイルのクローズ
	file.close();

	return true;
}


bool Skeleton::BuildSkeleton(ifstream& file)
{
	#define  BUFFER_LENGTH  1024*4
	char      line[BUFFER_LENGTH];
	char *    token;
	char      separater[] = " :,\t";
	vector< Bone *>   bone_stack;
	Bone* bone = NULL;
	Bone* new_bone = NULL;
	bool      is_site = false;
	double    x, y ,z;
	int       i;

	//データ配列のオフセットを格納する
	int frameOffset = 0;

	// 階層情報の読み込み
	while ( ! file.eof() )
	{
		// ファイルの最後まできてしまったら異常終了
		if ( file.eof() )  return false;

		// １行読み込み、先頭の単語を取得
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );

		// 空行の場合は次の行へ
		if ( token == NULL )  continue;

		// 関節ブロックの開始
		if ( strcmp( token, "{" ) == 0 )
		{
			// 現在の関節をスタックに積む
			bone_stack.push_back( bone );
			bone = new_bone;
			continue;
		}
		// 関節ブロックの終了
		if ( strcmp( token, "}" ) == 0 )
		{
			// 現在の関節をスタックから取り出す
			bone = bone_stack.back();
			bone_stack.pop_back();
			is_site = false;
			continue;
		}

		// 関節情報の開始
		if ( ( strcmp( token, "ROOT" ) == 0 ) ||
			( strcmp( token, "JOINT" ) == 0 ) )
		{
			// 新しいBoneの作成
			new_bone = new Bone();

			//Boneの名前の読み込み
			token = strtok( NULL, "" );
			while ( *token == ' ' )  token ++;
			new_bone->name = token;

			//オフセットの格納
			new_bone->frameOffset = frameOffset;

			// インデックスへ追加
			int index = getBoneNum();
			bones.push_back(new_bone);
			insertBoneName( index, new_bone->name );
			insertBoneIndex( new_bone->name, index );

			//親Boneと接続
			if(bone == NULL)
				new_bone->parent = -1;
			else
			{
				new_bone->parent = getBoneIndex(bone->name);
				bone->children.push_back( getBoneIndex(new_bone->name));
			}
			
			//// 関節データの作成
			//new_joint = new Joint();
			//new_joint->index = joints.size();
			//new_joint->parent = joint;
			//new_joint->has_site = false;
			//new_joint->offset[0] = 0.0;  new_joint->offset[1] = 0.0;  new_joint->offset[2] = 0.0;
			//new_joint->site[0] = 0.0;  new_joint->site[1] = 0.0;  new_joint->site[2] = 0.0;
			//joints.push_back( new_joint );
			//if ( joint )
			//	joint->children.push_back( new_joint );

			//// 関節名の読み込み
			//token = strtok( NULL, "" );
			//while ( *token == ' ' )  token ++;
			//new_joint->name = token;

			//// インデックスへ追加
			//joint_index[ new_joint->name ] = new_joint;
			continue;
		}

		// 末端情報の開始
		if ( ( strcmp( token, "End" ) == 0 ) )
		{
			new_bone = bone;
			//new_joint = joint;
			is_site = true;
			continue;
		}

		// 関節のオフセット or 末端位置の情報
		if ( strcmp( token, "OFFSET" ) == 0 )
		{
			// 座標値を読み込み
			token = strtok( NULL, separater );
			x = token ? atof( token ) : 0.0;
			token = strtok( NULL, separater );
			y = token ? atof( token ) : 0.0;
			token = strtok( NULL, separater );
			z = token ? atof( token ) : 0.0;

			// 関節のオフセットに座標値を設定
			if ( is_site )
			{
				// 新しいBone(EndSite)の作成
				Bone* end_bone = new Bone();

				//Boneの名前の読み込み
				end_bone->name = getBoneName(getBoneNum()-1) + "_End";

				//オフセットの格納
				end_bone->frameOffset = frameOffset;

				// インデックスへ追加
				int index = getBoneNum();
				bones.push_back(end_bone);
				insertBoneName( index, end_bone->name );
				insertBoneIndex( end_bone->name, index );

				//親Boneと接続
				end_bone->parent = getBoneIndex(bone->name);
				bone->children.push_back( getBoneIndex(end_bone->name));

				klVec3 bone_vec = klVec3( x, y, z );
				end_bone->length = bone_vec.Length();
				end_bone->direction = bone_vec.Normalize();
				end_bone->radius = new_bone->length / 8;

				//joint->has_site = true;
				//joint->site[0] = x;
				//joint->site[1] = y;
				//joint->site[2] = z;
			}
			else
			// 末端位置に座標値を設定
			{
				klVec3 bone_vec = klVec3(x, y, z);
				bone->length = bone_vec.Length();
				if( bone->length < 0.000001) bone->direction = klVec3(0, 0, 1);
				else bone->direction = bone_vec.Normalize();
				bone->radius = bone->length/8;
			}
			continue;
		}

		// 関節のチャンネル情報
		if ( strcmp( token, "CHANNELS" ) == 0 )
		{
			// チャンネル数を読み込み
			token = strtok( NULL, separater );
			int channel_num = token ? atoi(token) : 0;
			//joint->channels.resize( token ? atoi( token ) : 0 );

			// チャンネル情報を読み込み
			for ( i=0; i<channel_num; i++ )
			{
				//// チャンネルの作成
				//Channel *  channel = new Channel();
				//channel->joint = joint;
				//channel->index = channels.size();
				//channels.push_back( channel );
				//joint->channels[ i ] = channel;

				// チャンネルの種類の判定
				token = strtok( NULL, separater );

				if ( strcmp( token, "Xrotation" ) == 0 )
					bone->eulerOrder.push_back(klVec3(1.0, 0.0, 0.0));
				else if ( strcmp( token, "Yrotation" ) == 0 )
					bone->eulerOrder.push_back(klVec3(0.0, 1.0, 0.0));
				else if ( strcmp( token, "Zrotation" ) == 0 )
					bone->eulerOrder.push_back(klVec3(0.0, 0.0, 1.0));
				else if ( strcmp( token, "Xposition" ) == 0 )
					positionEulerOrder.push_back(klVec3(1.0, 0.0, 0.0));
				else if ( strcmp( token, "Yposition" ) == 0 )
					positionEulerOrder.push_back(klVec3(0.0, 1.0, 0.0));
				else if ( strcmp( token, "Zposition" ) == 0 )
					positionEulerOrder.push_back(klVec3(0.0, 0.0, 1.0));

				//if ( strcmp( token, "Xrotation" ) == 0 )
				//	channel->type = X_ROTATION;
				//else if ( strcmp( token, "Yrotation" ) == 0 )
				//	channel->type = Y_ROTATION;
				//else if ( strcmp( token, "Zrotation" ) == 0 )
				//	channel->type = Z_ROTATION;
				//else if ( strcmp( token, "Xposition" ) == 0 )
				//	channel->type = X_POSITION;
				//else if ( strcmp( token, "Yposition" ) == 0 )
				//	channel->type = Y_POSITION;
				//else if ( strcmp( token, "Zposition" ) == 0 )
				//	channel->type = Z_POSITION;
			}

			//オフセットの更新
			frameOffset += channel_num;

		}
		// Motionデータのセクションへ移る
		if ( strcmp( token, "MOTION" ) == 0 )
			break;
	}

	// スケルトンのタイプを設定
	dataSize_ = frameOffset;
	skeletonType_ = Human;
	return true;
}



void Skeleton::LoadPuppetSkeleton()
{
	this->Initialize();
	this->skeletonType_ = this->Puppet;

	double shoulder_length = 0.07;
	double hip_length = 0.03;

	double arm_length = 0.05;
	double radius_length = 0.085;
	double leg_length = 0.065;
	double knee_length = 0.065;
	
	double upper_back_length = 0.06;
	double lower_back_length = 0.06;
	double neck_length = 0.09;
	
	double r = 0.006;
	double shoulder_radius = r;
	double hip_radius = r;
	double arm_radius = r;
	double leg_radius = r;
	double knee_radius = r;
	double lower_back_radius = r;
	double upper_back_radius = r;
	double neck_radius = r;
	
	//klVec3 up( 1, 0, 0 );
	//klVec3 down = -up;
	//klVec3 left( 0, -1, 0 );
	//klVec3 right = -left;
	//klVec3 front( 0, 0, 1 );

	klVec3 up( 0, 1, 0 );
	klVec3 down = -up;
	klVec3 left( 1, 0, 0 );
	klVec3 right = -left;
	klVec3 front( 0, 0, 1 );


	//pelvis
	this->bones.resize(18);

	this->bones[0]  = MakeNewBone( "center", -1, down, 0, 0);
	this->bones[1]  = MakeNewBone( "lowerback_phantom", 0, down, lower_back_length, lower_back_radius );
	this->bones[2]  = MakeNewBone( "pelvis", 1, down, 0, 0 );
	
	//legs
	this->bones[3]  = MakeNewBone( "lfemur_phantom", 2, left, hip_length, hip_radius );
	this->bones[4]  = MakeNewBone( "lfemur", 3, down, leg_length, leg_radius );
	this->bones[5]  = MakeNewBone( "ltibia", 4, down, knee_length, knee_radius);
	this->bones[6]  = MakeNewBone( "rfemur_phantom", 2, right, hip_length, hip_radius*1.5 );
	this->bones[7]  = MakeNewBone( "rfemur", 6, down, leg_length, leg_radius*2.0 );
	this->bones[8]  = MakeNewBone( "rtibia", 7, down, knee_length, knee_radius*2.0 );

	this->bones[9]  = MakeNewBone( "upperback_phantom", 0, up, upper_back_length, upper_back_radius );
	this->bones[10]  = MakeNewBone( "chest", 9, down, 0, 0 );
	
	//arms
	this->bones[11]  = MakeNewBone( "lclavicle_phantom", 10, left, shoulder_length, shoulder_radius );
	this->bones[12] = MakeNewBone( "lhumerus", 11, left, arm_length, arm_radius );
	this->bones[13] = MakeNewBone( "lradius", 12, left, radius_length, arm_radius );
	this->bones[14] = MakeNewBone( "rclavicle_phantom", 10, right, shoulder_length, shoulder_radius*1.5 );
	this->bones[15] = MakeNewBone( "rhumerus", 14, right, arm_length, arm_radius*2.0 );
	this->bones[16] = MakeNewBone( "rradius", 15, right, radius_length, arm_radius*2.0 );

	//neck
	this->bones[17] = MakeNewBone( "neck_phantom", 10, up, neck_length, neck_radius );


	//build bone dictionaries
	for(int i = 0; i < (int)this->bones.size(); i++)
	{			
		boneName_.insert(map<int,string>::value_type
			(i,  this->bones[i]->name));
		boneIndex_.insert(map<string,int>::value_type
			(this->bones[i]->name, i));
	}

	////関節位置の対応付け
	//setSpecialIndexNames2nd( 0, //root
	//						"rclavicle_phantom", "lclavicle_phantom", //shoulder
	//						"rhumerus", "lhumerus", //elbow
	//	                    "rradius", "lradius", // hand (end-effecter)
	//						"rfemur_phantom", "lfemur_phantom", // hip
	//						"rfemur", "lfemur", //knee
	//						"rtibia", "ltibia", //foot(end-effecter)
	//						"neck_phantom", "upperback_phantom");// head(end-effecter), chest

	////Retargeting時に用いる関節角度の対応付け
	//setRetargetingIndexNames(
	//	"rhumerus", "lhumerus", // shoulder
	//	"rradius", "lradius", // elbow
	//	"rfemur", "lfemur", // hip
	//	"rtibia", "ltibia"); // knee
}


Bone* Skeleton::MakeNewBone(string name, int parent, klVec3 direction, double length, double radius)
{
	return MakeNewBone( name, parent, direction.x, direction.y, direction.z, length, radius );
}


Bone* Skeleton::MakeNewBone(string name, int parent, double direction_x, double direction_y, double direction_z, double length, double radius)
{
	Bone* bone = new Bone();
	bone->Initialize();

	bone->name = name;
	bone->parent = parent;
	bone->direction.x = direction_x;
	bone->direction.y = direction_y;
	bone->direction.z = direction_z;
	bone->radius = radius;
	bone->length = length;
	//bone.density = 1000;

	return bone;
}

void Skeleton::ScaleSkeleton(double scale)
{
	for(int i = 0; i < (int)this->bones.size(); i++)
	{
		this->bones[i]->length *= scale;
		this->bones[i]->radius *= scale;
	}
}

void Skeleton::ScaleSkeletonWidth(double scale)
{
	for(int i = 0; i < (int)this->bones.size(); i++)
	{
		if( abs(this->bones[i]->direction.y) > 0.5 )
		{
			this->bones[i]->length *= scale;
		}
	}
}

string Skeleton::getBoneName(int bone_index)
{ 
	if(boneName_.find(bone_index) != boneName_.end())
	{
		return string( boneName_[bone_index] );
	}
	return string("");
}

int Skeleton::getBoneIndex(string bone_name)
{ 
	if(boneIndex_.find(bone_name) != boneIndex_.end())
	{
		return boneIndex_[bone_name];
	}
	return -1;
}



//void put_dof_rot(string const &dof, Quatd const &rot, double *info, int start_pos);


//Quatd ordered_rotation(string const &order, Vector3d const &rotation);

//ostream &operator<<(std::ostream &os, const Bone &b) {
//	os << "Bone: " << b.name << endl;
//	os << " parent " << b.parent << endl;
//	os << " dof " << b.dof << endl;
//	os << " direction " << b.direction << endl;
//	os << " frame_offset " << b.frame_offset << endl;
//	os << " axis_offset " << b.axis_offset << endl;
//	os << " offset_order " << b.offset_order << endl;
//	os << " global_to_local " << b.global_to_local << endl;
//	os << "(radius,density,length) (" << b.radius << ", " << b.density << ", " << b.length << ")" << endl;
//	return os;
//}


//class NameCompare {
//public:
//	NameCompare(Skeleton const *_skel) : skel(_skel) {
//	}
//	bool operator()(int a, int b) const {
//		assert(skel);
//		string name_a = "";
//		string name_b = "";
//		if ((unsigned)a < skel->bones.size()) name_a = skel->bones[a].name;
//		if ((unsigned)b < skel->bones.size()) name_b = skel->bones[b].name;
//		return name_a < name_b;
//	}
//	Skeleton const *skel;
//};



//void unroll_sk(vector< vector< int > > const &list, vector< int > &target, int row = 0) {
//	assert(row >= 0);
//	target.push_back(row);
//	if (row < (signed)list.size()) {
//		for (unsigned int i = 0; i < list[row].size(); ++i) {
//			unroll_sk(list, target, list[row][i]+1);
//		}
//	}
//}


//bool Skeleton::check_parse() {
//	if (in_bone) {
//		cerr << "Bone without closing tag." << endl;
//		return false;
//	}
//	// might as well also set bones up for painless traversal.
//	vector< vector< int > > children;
//	for (unsigned int b = 0; b < bones.size(); ++b) {
//		int list = bones[b].parent + 1; //to make root zero.
//		if (list < 0) {
//			cerr << "Bone without a valid parent." << endl;
//			return false;
//		}
//		while (children.size() <= (unsigned)list) {
//			children.push_back(vector< int >());
//		}
//		assert(list >= 0 && list < (signed)children.size());
//		children[list].push_back(b);
//	}
//
//	//make sure we have the lexicographically least child ordering.
//	for (unsigned int c = 0; c < children.size(); ++c) {
//		sort(children[c].begin(), children[c].end(), NameCompare(this));
//	}
//
//	vector< int > new_inds;
//	unroll_sk(children, new_inds);
//
//	vector< int > old_to_new_inds;
//	old_to_new_inds.resize(new_inds.size(), -2);
//	assert(new_inds.size() == bones.size() + 1);
//
//	old_to_new_inds[0] = -1;
//
//	frame_size = 6; //i.e. just root info.
//	vector< Bone > new_bones;
//	for (unsigned int i = 1; i < new_inds.size(); ++i) {
//		int b = new_inds[i] - 1; //bone for new first index.
//		assert(b >= 0 && b < (signed)bones.size());
//		old_to_new_inds[b+1] = i - 1;
//		new_bones.push_back(bones[b]);
//		new_bones.back().frame_offset = frame_size;
//		frame_size += new_bones.back().dof.size();
//
//		int op = new_bones.back().parent + 1;
//		assert(op >= 0 && op < (signed)old_to_new_inds.size());
//		int p = old_to_new_inds[op]; //look up parent.
//		assert(p >= -1 && p < (signed)bones.size());
//		new_bones.back().parent = p;
//	}
//
//	//bones now ordered for easy traversal.
//	bones = new_bones;
//
//	for (unsigned int b = 0; b < bones.size(); ++b) {
//		bones[b].global_to_local = ordered_rotation(bones[b].offset_order, bones[b].axis_offset);
//	}
//
//	return true;
//}



//Quatd ordered_rotation(string const &order, Vector3d const &rot) {
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



//axis & probe should be orthonormal.
//double get_rotation(Vector3d axis, Vector3d probe, Quatd const &rot)
//{
//	Vector3d perp = cross_product(axis, probe);
//	Vector3d rotated = rotate(probe, rot);
//	rotated = normalize(rotated - axis * (axis * rotated));
//	return atan2(rotated * perp, rotated * probe);
//}


//int Skeleton::get_bone_by_name(string name) const {
//	for (unsigned int b = 0; b < bones.size(); ++b) {
//		if (name == bones[b].name) {
//			return b;
//		}
//	}
//	return -1;
//}

//string Skeleton::get_dof_description(unsigned int dof) const {
//	string ret = "unknown";
//	if (dof < order.size()) {
//		ret = "root";
//		ret += order[dof];
//	} else {
//		for (unsigned int b = 0; b < bones.size(); ++b) {
//			if ((signed)dof >= bones[b].frame_offset
//			 && (unsigned)(dof - bones[b].frame_offset) < bones[b].dof.size()) {
//				ret = bones[b].name + bones[b].dof[dof - bones[b].frame_offset];
//			}
//		}
//	}
//	return ret;
//}
