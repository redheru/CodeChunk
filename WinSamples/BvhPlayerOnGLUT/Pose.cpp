#include "Pose.h"

void Pose::Clear(unsigned int bones)
{
	rootPosition_ = klVec3(0.0,0.0,0.0);
	rootOrientation_.Clear();

	boneOrientations_.resize(bones);

	skeleton = NULL;

	isCorrected_ = false;

	for (unsigned int i = 0; i < bones; ++i) {
		boneOrientations_[i].Clear();
	}
}
void Pose::LoadVacantPose(Skeleton* skl)
{
	Clear(skl->bones.size());
	skeleton = skl;
}

klMat44 Pose::ComputeLocaltoWolrdConvertMatrix(int bone_index)
{
	klMat44 mat;
	mat.LoadIdentity();
	int b = bone_index;

	//
	//if -1
	//convert root coordinate to wolrd coordinate
	//

	if(bone_index != -1)
	{
		while ( true )
		{
			//compute Tb and Rb
			klMat44 Tb;
			Tb.LoadIdentity();
			Tb.Translate( this->getBoneOffset(b) );
			
			klMat44 Rb = klQuat::toklMat44( this->getBoneAngle(b) );
			
			//multiply Rb * Tb * mat
			mat = Tb * mat;
			mat = Rb * mat;
			
			//goto parent bone
			b = this->getBoneParentNum(b);
			if(b == -1)
			{
				break;
			}
		}
	}
	
	//compute Rroot and Troot
	klMat44 Rroot = klQuat::toklMat44( this->getRootAngle() );
	klVec3 t_root = this->getRootPosition();
	klMat44 Troot;
	Troot.LoadIdentity();
	Troot.Translate(t_root.x, t_root.y, t_root.z);
	
	//multiply Troot * Rroot * mat
	mat = Rroot * mat;
	mat = Troot * mat;
		
	return mat;
}


//BVH専用
void Pose::BuildPose(double const *frame_data, Skeleton* skl)
{
	//clear out the destination pose.
	Clear(skl->bones.size());

	//make sure we have the right amount of storage.
	assert(skl->bones.size() == boneOrientations_.size());

	//also, make sure the pose knows its skeleton.
	this->skeleton = skl;

	rootPosition_ = get_dof_trans(skl->positionEulerOrder, frame_data, 0);
	//rootPosition_ /= 100.0; 

	//rootOrientation_ = multiply(ordered_rotation( skl->offset_order, skl->axis_offset ), get_dof_rot(skl->order, frame_data, 0));
	rootOrientation_ = get_dof_rot(skl->bones[0]->eulerOrder, frame_data, skl->positionEulerOrder.size());

	//rootは回転0とみなす
	for (unsigned int b = 1; b < skl->bones.size(); ++b) {

		//親関節の回転が現在のBoneの向きとなる
		Bone* parent = skl->bones[skl->bones[b]->parent];
		if(parent->parent >= 0)
		{//親関節がルートでない場合
			klQuat rot = get_dof_rot(parent->eulerOrder, frame_data, parent->frameOffset);
			boneOrientations_[b] = rot;
		}

		//rot = multiply(conjugate(skl->bones[b].global_to_local), rot);
		//Quatd mul = get_dof_rot(skl->bones[b].dof, frame_data, skl->bones[b].frame_offset);
		//rot = multiply(mul, rot);
		//rot = multiply(skl->bones[b].global_to_local, rot);
		//rot = normalize(rot);
	}
	//for (unsigned int b = 0; b < skl->bones.size(); ++b) {
	//	Quatd rot;
	//	rot.clear();
	//	rot = multiply(conjugate(skl->bones[b].global_to_local), rot);
	//	Quatd mul = get_dof_rot(skl->bones[b].dof, frame_data, skl->bones[b].frame_offset);
	//	rot = multiply(mul, rot);
	//	rot = multiply(skl->bones[b].global_to_local, rot);
	//	rot = normalize(rot);

	//	this->bone_orientations[b] = rot;
	//}
	//if (skl->rot_is_glob) {
	//	for (unsigned int b = skl->bones.size()-1; b < skl->bones.size(); --b) { //neg wraps
	//		if (skl->bones[b].parent==-1) {
	//			Quatf f;
	//			f = get_dof_rot(skl->order, frame_data, 0);
	//			this->bone_orientations[b] = multiply(conjugate(f), this->bone_orientations[b]);
	//		} else {
	//			this->bone_orientations[b] = multiply(conjugate(this->bone_orientations[skl->bones[b].parent]), this->bone_orientations[b]);
	//		}
	//		this->bone_orientations[b] = normalize(this->bone_orientations[b]);
	//		assert(skl->bones[b].parent < int(b));
	//	}
	//}
	//if (skl->z_is_up) {
	//	Quatf rot;
	//	rot = rotation( -(float)M_PI * 0.5f, make_vector(1.0f, 0.0f, 0.0f) );
	//	this->root_position = rotate(this->root_position, rot);
	//	this->root_orientation = multiply(rot, this->root_orientation);
	//	
	//}
}

//センサデータ読み込み専用
//void Pose::BuildPose(Sensor::ReadSensorData::DATA data, Skeleton *skl)
//{
//	//Retargeting用Mocap
//	LoadVacantPose(skl);
//	SetRootAngle(data.root_rot);
//	SetBoneAngle(data.shoulderL, "lhumerus");
//	SetBoneAngle(data.shoulderR, "rhumerus");
//	SetBoneAngle(data.elbowL, "lradius");
//	SetBoneAngle(data.elbowR, "rradius");
//	
//	klQuat quat_hipL = klQuat((double)(17 * M_PI/180), klVec3(0,0,1)) * data.hipL;
//	//Quatf quat_hipL = multiply(rotation((float)( -17 * M_PI/180), Converter::toVector3f(klVec3(0,0,1))), data.hipL);
//	SetBoneAngle(quat_hipL, "lfemur");
//
//	klQuat quat_hipR = klQuat((double)( 17 * M_PI/180), klVec3(0,0,1)) * data.hipR;
//	//Quatf quat_hipR = multiply(rotation((float)( 17 * M_PI/180), Converter::toVector3f(klVec3(0,0,1))), data.hipR);
//	SetBoneAngle(quat_hipR, "rfemur");
//
//	SetBoneAngle(data.kneeL, "ltibia");
//	SetBoneAngle(data.kneeR, "rtibia");
//
//	klVec3 pos(-1.0, 1.0, 0.0);
//	SetRootPosition(pos);
//}
//
//
//
void Pose::CorrectRoot()
{
	if(isCorrected_)
	{
		return;
	}

	isCorrected_ = true;
	this->rootPosition_.y += 1.0;

	//Quatf correct_q, correct_q2;
	
	//correct_q = rotation( -M_PI/2.0, make_vector(1.0, 0.0, 0.0) );
	//correct_q2 = rotation( -M_PI/2.0, make_vector(0.0, 1.0, 0.0) );
	//correct_q = multiply(correct_q2, correct_q);
	//root_orientation = multiply(correct_, root_orientation);

	//root_position.y += 1.0;

	
	//Quatf rot;
	//rot = rotation( -(float)M_PI * 0.5f, make_vector(1.0f, 0.0f, 0.0f) );
	//this->root_position = rotate(this->root_position, rot);
	
	//this->root_orientation = multiply(rot, this->root_orientation);
}


klVec3 Pose::getNodePosition(int bone_index)
{
	klMat44 mat;
	mat.LoadIdentity();

	Bone bone = *this->skeleton->bones[bone_index];

	while( bone.parent != -1 )
	{
		//multiply translate matrix from left
		klVec3 t = bone.direction * bone.length;
		
		//Translate in klMat44 multiply translate mat from right
		klMat44 t_mat;
		t_mat.LoadIdentity();
		t_mat.Translate(t);
		mat = t_mat * mat;
					
		//multiply rotate matrix from left
		mat = klQuat::toklMat44( this->getBoneAngle(bone_index)) * mat;
		
		//goto parent bone
		bone_index = bone.parent;
		bone = *this->skeleton->bones[bone_index];
	}
	
	//multiply Rroot from left
	mat = klQuat::toklMat44( this->getRootAngle() ) * mat;

	//multiply Troot from left
	klVec3 t_root = this->getRootPosition();
	
	klMat44 Troot;
	Troot.LoadIdentity();
	Troot.Translate(t_root);
	mat = Troot * mat;

	//multiply matrix to zero vector
	klVec4 p_i = klVec4(0, 0, 0, 1);
	p_i = mat * p_i;
	
	klVec3 res = klVec3(p_i.x, p_i.y, p_i.z);
	return res;
}


klVec3 Pose::getNodePositionFromRoot( int bone_index, int root_index, bool with_root_rotation)
{
	klVec3 result;
	if( with_root_rotation )
	{
		result =  getNodePosition( bone_index );
		result -= getRootPosition();
	}
	else result = getNodePositionFromBone( bone_index, root_index);
	return result;
}

klVec3 Pose::getNodePositionFromBone(int dist_bone_index,	int base_bone_index)
{
	//compute node position
	klMat44 mat;
	mat.LoadIdentity();
	int b = dist_bone_index;
	
	while ( true )
	{
		//compute Tb and Rb
		klMat44 Tb;
		Tb.LoadIdentity();
		Tb.Translate( this->getBoneOffset(b) );
		
		klMat44 Rb = klQuat::toklMat44( this->getBoneAngle(b) );
		
		//multiply Rb * Tb * mat
		mat = Tb * mat;
		mat = Rb * mat;
		
		//goto parent bone
		b = this->getBoneParentNum(b);

		if( b == base_bone_index )
		{
			break;
		}
	}

	//multiply matrix
	klVec4 node = klVec4(0,0,0,1);
	node = mat * node;
	
	klVec3 res = klVec3(node.x, node.y, node.z);
	return res;
}


void Pose::SetBoneAngle(klQuat q, int bone_index)
{
	if((int)boneOrientations_.size() <= bone_index)
	{
		cerr << "Pose,SetBoneOrientation : Wrong bone_index" << endl;
		return;
	}
	boneOrientations_[bone_index] = q;
}

klVec3 Pose::get_dof_trans(vector<klVec3> positionEulerOrder, double const *info, int start_pos)
{
	klVec3 trans(0.0, 0.0, 0.0);
	info += start_pos;
	for (unsigned int i = 0; i != positionEulerOrder.size(); ++i) {
		double d = *info;
		if(positionEulerOrder[i].x >= 1.0){
			trans.x = d;
		}else if(positionEulerOrder[i].y >= 1.0){
			trans.y = d;
		}else if(positionEulerOrder[i].z >= 1.0 ){
			trans.z = d;
		}else{
			cerr << "Enountered dof '" << endl;//dof[i] << "' we don't know about in '" << dof << "'." << endl;
			assert(0);
		}
		++info;
	}
	return trans;
}

//degree, not theta
klQuat Pose::ordered_rotation(vector<klVec3>eulerOrder, klVec3 const &rot) 
{
	klMat44 mat;
	mat.LoadIdentity();

	for (unsigned int i = 0; i < eulerOrder.size(); ++i) {
		if(eulerOrder[i].x >= 1.0) {
			mat.Rotate(rot.x, eulerOrder[i]);
		}else if(eulerOrder[i].y >= 1.0){
			mat.Rotate(rot.y, eulerOrder[i]);
		}else if(eulerOrder[i].z >= 1.0){
			mat.Rotate(rot.z, eulerOrder[i]);			
		}else{
				cerr << "Unexpected offset_order character '" << endl;//order[i] << "' in '" << order << "'." << endl;
				assert(0);
		}
	}
	return klQuat::fromklMat(mat);
}

//degree, not theta
klQuat Pose::get_dof_rot(vector<klVec3> eulerOrder, double const *info, int start_pos) 
{
	klMat44 mat;
	mat.LoadIdentity();

	info += start_pos;
	for (unsigned int i = 0; i != eulerOrder.size(); ++i)
	{
		double d = *info;
		if(eulerOrder[i].x >= 1.0){
			mat.Rotate(d, eulerOrder[i]);
		}else if(eulerOrder[i].y >= 1.0){
			mat.Rotate(d, eulerOrder[i]);
		}else if(eulerOrder[i].z >= 1.0){
			mat.Rotate(d, eulerOrder[i]);
		}else{
			cerr << "Enountered dof '" << endl;//dof[i] << "' we don't know about in '" << dof << "'." << endl;
			assert(0);
		}
		++info;
	}

	return klQuat::fromklMat(mat);
}

bool Pose::testPose(string fileName, Skeleton*skl)
{
	#define  BUFFER_LENGTH  1024*4
	ifstream  file;
	char      line[BUFFER_LENGTH];
	char *    token;
	char      separater[] = " :,\t";
	bool      is_site = false;
	int       i, j;

	// ファイルのオープン
	//skl.setFileName(fileName);

	file.open(fileName.c_str(), ios::in );
	if(file.is_open() == 0) return false;// ファイルが開けなかったら終了

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

		// 関節のチャンネル情報
		if ( strcmp( token, "CHANNELS" ) == 0 )
		{
			// チャンネル数を読み込み
			token = strtok( NULL, separater );
			int channel_num = token ? atoi(token) : 0;
			//joint->channels.resize( token ? atoi( token ) : 0 );

			//オフセットの更新
			frameOffset += channel_num;
		}


		// Motionデータのセクションへ移る
		if ( strcmp( token, "MOTION" ) == 0 )
			break;
	}

	// モーション情報の読み込み
	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, separater );
	if ( strcmp( token, "Frames" ) != 0 )  return false;
	token = strtok( NULL, separater );
	if ( token == NULL )  return false;
	int num_frame = atoi( token );

	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );
	if ( strcmp( token, "Frame Time" ) != 0 )  return false;
	token = strtok( NULL, separater );
	if ( token == NULL )  return false;
	double interval = atof( token );

	double* motion = new double[ frameOffset ];

	// モーションデータの読み込み
	for ( i=0; i<num_frame; i++ )
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );
		for ( j=0; j<frameOffset; j++ )
		{
			if ( token == NULL )
				return false;
			motion[ j ] = atof( token );
			token = strtok( NULL, separater );
		}

		//Poseを作ってみる
		BuildPose(motion, skl);
	}

	// ファイルのクローズ
	file.close();

	return true;
}