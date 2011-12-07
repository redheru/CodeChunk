#include "MocapData.h"

MocapData::MocapData()
{
	initialize();
}

MocapData::MocapData(string fileName)
{
	initialize();
	loaded_ = LoadMotion(fileName);
}

MocapData::~MocapData()
{
	delete skeleton_;
}

void MocapData::initialize()
{
	skeleton_ = new Skeleton();

	poses.clear();
	loaded_ = false;
	fps_ = 120;
	frameNum_ = 0;
	frameTime_ = 1.0;
}

// motion全体の向きの補正(座標軸の向きの補正などに使う)
void MocapData::correctMotionDirection( klMat44 correct_mat )
{
	// 使用例
	//	MocapData mocap;
	//  mocapにファイル読み込み
	// 	klMat44 correct_mat;
	//	correct_mat.LoadIdentity();
	//	correct_mat.Rotate( 90, 1,0,0 );// x軸について90度回転
	//	mocap.correctMotionDirection( correct_mat );// これで補正完了
	klQuat correct_q = klQuat::fromklMat( correct_mat );
	for(int i = 0; i < frameNum_; i++)
	{
		poses[i].SetRootAngle(correct_q * poses[i].getRootAngle());
		poses[i].SetRootPosition( correct_q.Rotate(poses[i].getRootPosition()));
	}
}

void MocapData::Trimming(int start_frame, int end_frame)
{
	if( start_frame < 0 || start_frame >= this->getFrameNum()
		|| end_frame < 0 || end_frame >= this->getFrameNum()
		|| start_frame > end_frame )
	{
		cout << "error in MocapData, Trimming : frame range." << endl;
		//Error("MakePartialSensorData", "frame range error.");
		return;
	}
	
	int before_frame_num = this->getFrameNum();

	//erase until start frame
	for(int i = 0; i < start_frame; i++)
	{
		this->EraseFirstFrame();
	}

	//erase until end frame
	for(int i = 0; i < before_frame_num - end_frame; i++)
	{
		this->EraseEndFrame();
	}
}

void MocapData::EraseFirstFrame()
{
	frameNum_--;
	this->poses.erase(poses.begin());
}

void MocapData::EraseEndFrame()
{
	frameNum_--;
	this->poses.pop_back();
}

void MocapData::ClearFrames()
{
	this->poses.clear();
	frameNum_ = 0;
}

//Add a new Pose at the end of "poses"
void MocapData::AddPose(Pose add_pose)
{
	poses.push_back(add_pose);
	frameNum_++;
}

//make vacant motion
//then you should add Pose by AddPose
bool MocapData::LoadVacantMotion(Skeleton* skl, int fps)
{
	//load only once
	if(loaded_ == true) return false;
	
	skeleton_ = skl;
	fps_ = fps;
	frameTime_ = 1.0/(double)fps;

	frameNum_ = 0;
	
	loaded_ = true;
	return true;
}

bool MocapData::ReadHierarchy(ifstream& file, Skeleton* skl)
{
	if(skl->BuildSkeleton(file) == false) return false;
	return true;
}

bool MocapData::ReadMotionInfo(ifstream& file)
{
	#define  BUFFER_LENGTH  1024*4
	char      line[ BUFFER_LENGTH ];
	char *    token;
	char      separater[] = " :,\t";

	// モーション情報の読み込み
	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, separater );
	if ( strcmp( token, "Frames" ) != 0 )  return false;
	token = strtok( NULL, separater );
	if ( token == NULL )  return false;
	frameNum_ = atoi( token );

	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );
	if ( strcmp( token, "Frame Time" ) != 0 )  return false;
	token = strtok( NULL, separater );
	if ( token == NULL )  return false;
	frameTime_ = atof( token );
	fps_ = (int)ceil(1/frameTime_);
	
	return true;
}

bool MocapData::ReadMotionData(ifstream& file)
{
	#define  BUFFER_LENGTH  1024*4
	char      line[ BUFFER_LENGTH ];
	char *    token;
	char      separater[] = " :,\t";

	int channelNum = skeleton_->getDataSize();//RootPos + BoneRot
	double* motion = new double[channelNum];

	// モーションデータの読み込み
	for (int i=0; i<frameNum_; i++ )
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );
		for (int j=0; j<channelNum; j++ )
		{
			if ( token == NULL )return false;

			motion[ j ] = atof( token );
			token = strtok( NULL, separater );
		}
		//ポーズの追加
		Pose newPose;
		newPose.BuildPose( motion, skeleton_);
		poses.push_back(newPose);
	}
	return true;
}

//from .bvh
bool MocapData::LoadMotion(string fileName)
{
	//if( !boost::filesystem::exists( boost::filesystem::path( fileName ) ) )
	//{
	//	printf("[MocapData] File isn't exist!\n");
	//	return false;
	//}

	//load only once
	if(loaded_ == true) return false;

	ifstream file;
	file.open(fileName.c_str(), ios::in );
	if(file.is_open() == 0) return false;// ファイルが開けなかったら終了

	//Hierarchy部を読み込み,Skeletonを定義
	if(ReadHierarchy(file, skeleton_) == false) return false;

	//フレーム数,FPSなどを読み込み
	if(ReadMotionInfo(file) == false) return false;

	//回転角度などを読み込み
	if(ReadMotionData(file) == false) return false;

	//ファイル名取得
	name_ = fileName;
	skeleton_->setFileName(fileName);

	//スケルトンのサイズを合わせる(BVHのみ？)
	skeleton_->ScaleSkeleton( 0.01 );

	////load motion
	//vector<double> data;
	//LoadMotionToVector(file_name, data);
	//
	//this->fps = fps;
	//frameNum =  (int)( data.size() / skl->frame_size );
	//frameTime = 1.0 / (double)fps;
	////frameTime = (double)input_motion.length() / (double)frameNum;

	////build "poses"
	//for(int f = 0; f < frameNum; f++)
	//{
	//	Pose temp_pose;
	//	temp_pose.BuildPose(&(data[0]) + f * skl->frame_size, skl);
	//	poses.push_back(temp_pose);
	//}

	////CorrectRootPositionToRelative();

	//this->name = file_name;
	loaded_ = true;
	return true;
}


string MocapData::getChannelNameString( klVec3 eulerOrder, bool isPosition = false)
{
	//X_ROTATION, Y_ROTATION, Z_ROTATION,
	//X_POSITION, Y_POSITION, Z_POSITION
	if(isPosition){
		if( eulerOrder.x >= 1.0) return "Xposition";
		else if( eulerOrder.y >= 1.0) return "Yposition";
		else if( eulerOrder.z >= 1.0) return "Zposition";
		else return "error";
	}else{
		if( eulerOrder.x >= 1.0) return "Xrotation";
		else if( eulerOrder.y >= 1.0) return "Yrotation";
		else if( eulerOrder.z >= 1.0) return "Zrotation";
		else return "error";
	}
	return "error";
}	

void MocapData::WriteHierarchy( Bone* bone, ofstream& ofs, int depth)
{
	string indent;
	for( int i = 0; i <= depth; i++ ) indent += "\t";

	ofs << indent;

	//// End Site
	//// {
	// end siteの出力
	if(bone->children.size() == 0)
	{
		ofs << "End Site\n";
		ofs << indent << "{\n";
		ofs << indent << "\t" << "OFFSET";
		// todo: 出力桁数の整形
		klVec3 offset = bone->direction * bone->length * 100; //m -> cm
		for( int i = 0; i < 3; i++ ) ofs << " " << offset[i];
		ofs << endl;
		ofs << indent << "}\n";
		return;
	}

	//// JOINT name
	//// {
	// rootの場合はJOINTではなくROOTにする
	if( bone->parent <  0 ) ofs << "ROOT ";
	else ofs << "JOINT ";
	ofs << bone->name << endl;
	ofs << indent << "{\n";

	// ここからインデントがさらに1段下がる
	{
		depth++;
		string indent2 = indent + "\t";
		
		//// OFFSET x y z
		klVec3 offset = bone->direction * bone->length * 100; //m -> cm
		ofs << indent2 << "OFFSET ";
		for( int i = 0; i < 3; i++ )ofs << offset[i] << " ";
		ofs << endl;

		//// CHANNELS n channel_enum1 channel_enum2...
		if( bone->parent < 0)
		{ //Rootの場合の処理(Position -> Rotationの順に出力する)
			int rootChannelNum = bone->eulerOrder.size() + skeleton_->positionEulerOrder.size();
			ofs << indent2 << "CHANNELS "
				<< rootChannelNum;
			for(unsigned int i = 0; i < skeleton_->positionEulerOrder.size(); i++) 
				ofs << " " << getChannelNameString( skeleton_->positionEulerOrder[i], true);
			for(unsigned int i = 0; i < bone->eulerOrder.size(); i++)
				ofs << " " << getChannelNameString(bone->eulerOrder[i]);
			ofs << endl;
		}else{
			ofs << indent2 << "CHANNELS " 
				<< bone->eulerOrder.size();
			for(unsigned int i = 0; i < bone->eulerOrder.size(); i++) 
				ofs << " " << getChannelNameString(bone->eulerOrder[i]);
			ofs << endl;
		}

		//// child jointの出力
		for(unsigned int i = 0; i < bone->children.size(); i++ )
		{
			WriteHierarchy( skeleton_->bones[bone->children[i]], ofs, depth );
		}

		//// end siteの出力
		//if( cur_joint->has_site == true )
		//{
		//	ofs << indent2 << "End Site\n";
		//	ofs << indent2 << "{\n";
		//	ofs << indent2 << "\t" << "OFFSET";
		//	// todo: 出力桁数の整形
		//	for( int i = 0; i < 3; i++ ) ofs << " " << cur_joint->site[i];
		//	ofs << endl;
		//	ofs << indent2 << "}\n";
		//}
	}
	ofs << indent << "}\n";
}

//Eulerの順番はZXYに固定される
bool MocapData::WriteBvh( string file_name )
{
	//ロケールの設定
	ofstream ofs;
	ofs.imbue( std::locale::classic() );

	//ファイルを開く
	ofs.open( file_name.c_str() );
	if (ofs.is_open() == 0) return false;//ファイルが開けなければ終了

	//// 1. hierarchy部の書き出し ////
	ofs << "HIERARCHY\n";
	//outputJoint( joints[0], ofs, 0 );
	WriteHierarchy( skeleton_->bones[0], ofs, 0);

	//// 2. motion部の書き出し ////

	ofs << "MOTION\n";
	ofs << "Frames:\t" << frameNum_ << endl;
	ofs << "Frame Time:\t" << frameTime_ << endl;

	// 各フレームのデータの書き出し
	for( int frame = 0; frame < frameNum_; frame++ )
	{
		Pose& pose = this->poses[frame];

		// rootの情報
		//// rootの位置
		klVec3 position = pose.getRootPosition() * 100;//cm->meter
		for(int i = 0; i < 3; i++)
		{
			if(skeleton_->positionEulerOrder[i].x >= 1.0) ofs << position.x << " ";
			else if(skeleton_->positionEulerOrder[i].y >= 1.0) ofs << position.y << " ";
			else if(skeleton_->positionEulerOrder[i].z >= 1.0) ofs << position.z << " ";
			else return false;
		}
		//// rootの向き
		klVec3 root_zxy = pose.getRootAngle().ToEulerZxy();
		ofs << root_zxy.z << " " << root_zxy.x << " " << root_zxy.y << " ";

		//関節の向き(RootのBoneは無視する)
		for( int i = 1; i < skeleton_->getBoneNum(); i++ )
		{
			if(skeleton_->bones[i]->children.size() > 0)
			{//EndSiteではない
				int index = skeleton_->bones[i]->children[0];
				klVec3 zxy = pose.getBoneAngle(index).ToEulerZxy();
				ofs << zxy.z << " " << zxy.x << " " << zxy.y << " ";
			}
		}
		ofs << endl;
	}

	//// 書き込み終了zd
	ofs.close();

	return true;

	////ConvertBVHのところ

	////// 0. 準備・基本的な情報の変換
	//bvh.file_name = "new_bvh_file.bvh";
	//bvh.motion_name = "new_motion";
	//bvh.num_frame = this->frameNum;
	//bvh.interval = this->frameTime;

	////// 1. hierarchyとmotionのデータを作る
	////// ( joints, joint_index, channels )

	//// 1.1 bonesのデータからjoints, channelsを作る
	//makeJoint( bvh.joints, 0, NULL, bvh.channels );
	//bvh.num_channel = bvh.channels.size();

	//// 1.2 joint_indexを作る
	//for(int i = 0; i < bvh.joints.size(); i++)
	//{
	//	Joint* joint = bvh.joints[i];
	//	bvh.joint_index.insert( map<string, Joint*>::value_type( bvh.joints[i]->name, bvh.joints[i] ) );
	//}
	//
	////// 2. motionデータを作る
	//bvh.motion = new double[ bvh.num_frame * bvh.num_channel ];
	//for(int frame = 0; frame < this->frameNum; frame++)
	//{
	//	int motion_index = bvh.num_channel * frame;
	//	Pose& pose = this->poses[frame];

	//	// rootの情報
	//	//// rootの位置
	//	for(int i = 0; i < 3; i++) 
	//		bvh.motion[motion_index++] = pose.root_position[i] * 100;//cm->meter
	//	
	//	//// rootの向き
	//	klVec3 root_zxy = Converter::toEulerZxy(pose.root_orientation);
	//	bvh.motion[motion_index++] = root_zxy.z;
	//	bvh.motion[motion_index++] = root_zxy.x;
	//	bvh.motion[motion_index++] = root_zxy.y;

	//	// それ以外(rootのboneは無視するので、bone=1から)
	//	for(int bone = 1; bone < pose.bone_orientations.size(); bone++)
	//	{
	//		// end site かどうか判定
	//		int child_index = -1;
	//		for(int i = 0; i < skeleton->bones.size(); i++)
	//		{
	//			// 子供発見
	//			if( skeleton->bones[i].parent == bone )
	//			{
	//				child_index = i;
	//				break;
	//			}
	//		}
	//		// end siteなのでmotionにはデータを入れない
	//		if( child_index == -1 ) continue;

	//		// jointの向きをmotionに入れる
	//		klVec3 zxy = Converter::toEulerZxy(pose.bone_orientations[child_index]);
	//		bvh.motion[motion_index++] = zxy.z;
	//		bvh.motion[motion_index++] = zxy.x;
	//		bvh.motion[motion_index++] = zxy.y;
	//	}
	//}

	////// 3. おしまい
	//bvh.is_load_success = true;

	//Bvh bvh;
	//convertToBvh(bvh);
	//bvh.output(file_name);
}



void MocapData::CorrectRootPosition(int frame)
{
	this->poses[frame].SetIsCorrected(false);
	this->poses[frame].CorrectRoot();
}
