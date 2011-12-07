//-----------------------------------------------------------------------------
//                             MotionFile.h
//
// ���[�V�����f�[�^�ǂݏ����֐��Q�錾��
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/04 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

//#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
#include "MotionFile.h"

// ���O���: NMotionFileLocal
// ���[�V�����t�@�C����͗p���ʃT�u�֐��Q
namespace NMotionFileLocal
{
	// �񋓌^: channel_order
	// �ʒu�A��]�`�����l���I�[�_�[
	enum channel_order
	{
		ORDER_RX = 1, ORDER_RY = 2, ORDER_RZ = 3,
		ORDER_TX = 4, ORDER_TY = 5, ORDER_TZ = 6
	};

	// ������g�[�N���؂�o���ׂ̈̋�؂蕶��
	const char *s_delim = "\t \n";
	// �S�t�@�C�����e�̕�����̈�ւ̊i�[
	char* load_file(const char *fpath);
	// 3�����x�N�g���f�[�^�̓ǂݏo��
	inline bool parse_vector_data(Vector3f &v);
	// �E��n<-->����n ��]�s��ϊ�
	Matrix4f basis_transform(const Matrix4f &m);
	// �E��n<-->����n �x�N�g���ϊ�
	Vector3f basis_transform(const Vector3f &v);
};
using namespace NMotionFileLocal;


//-----------------------------------------------------------------------------
// ����: �g�[�N���擾�}�N��
// ����: 
//       token [out] �g�[�N���i�[�敶����|�C���^
//       eaction [in] �擾���s���̏���
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
#define GET_TOKEN(token, eaction) { if (token = strtok(NULL, s_delim), token == NULL) eaction; }
//-----------------------------------------------------------------------------
// ����: �g�[�N���擾 & �������r�}�N��
// ����: 
//       token [out] �g�[�N���i�[�敶����|�C���^
//       compare [in] ��r������
//       slen [in] �ő��r������
//       eaction [in] �擾���sor������s��v���̏���
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
#define GET_COMP_TOKEN(token, compare, slen, eaction) { if (token = strtok(NULL, s_delim), token == NULL || strncmp(token, compare, slen) != 0) eaction; }

//-----------------------------------------------------------------------------
// ����: �S�t�@�C�����e�̕�����̈�ւ̊i�[
// ����: 
//       fpath [in] �t�@�C���p�X
// �Ԃ�l:
//       �擾������擪�ւ̃|�C���^�i����̓��[�U�[�̐ӔC�j
// ���̑�: 
//-----------------------------------------------------------------------------
char* NMotionFileLocal::load_file(const char *fpath)
{
	FILE *fin = fopen(fpath, "r");
	if (fin == NULL)
		return NULL;
	fseek(fin, 0, SEEK_END);
	size_t fsize = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char *buf = new char[fsize];
	if (buf == NULL)
	{
		fclose(fin);
		return NULL;
	}

	fread(buf, sizeof(char), fsize, fin);
	fclose(fin);

	return buf;
}

//-----------------------------------------------------------------------------
// ����: �E��n<-->����n ��]�s��ϊ�
// ����: 
//       m [in] �ϊ�����]�s��
// �Ԃ�l:
//       �ϊ���̉�]�s��
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f NMotionFileLocal::basis_transform(const Matrix4f &m)
{
	static Matrix4f scale;
	scale << 1.0f, 0, 0, 0,  0, 1.0f, 0, 0,  0, 0, -1.0f, 0,  0, 0, 0, 1.0f;
	return scale * m * scale;
}

//-----------------------------------------------------------------------------
// ����: �E��n<-->����n �x�N�g���ϊ�
// ����: 
//       v [in] �ϊ����x�N�g��
// �Ԃ�l:
//       �ϊ���̃x�N�g��
// ���̑�: 
//-----------------------------------------------------------------------------
Vector3f NMotionFileLocal::basis_transform(const Vector3f &v)
{
	static Matrix4f scale;
	Vector3f vt;
	scale << 1.0f, 0, 0, 0,  0, 1.0f, 0, 0,  0, 0, -1.0f, 0, 0, 0, 0, 1.f;
	return *NEigenMath::Vector3fTransformCoord(&vt, &v, &scale);
	//return *D3DXVec3TransformCoord(&vt, &v, &scale);
}

//-----------------------------------------------------------------------------
// ����: 3�����x�N�g���f�[�^�̓ǂݏo��
// ����: 
//       v [out] 3D�x�N�g���f�[�^
// �Ԃ�l:
//       true �ǂݏo������
//       false �ǂݏo�����s
// ���̑�: 
//-----------------------------------------------------------------------------
inline bool NMotionFileLocal::parse_vector_data(Vector3f &v)
{
	char *token;
	size_t i;
	for (i = 0;i < 3;++i)
	{
		GET_TOKEN(token, break);
		v[i] = static_cast<float>(atof(token));
	}
	return i == 3 ? true : false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���O���: NBvhFile
// BVH�t�@�C����͗p�T�u�֐��Q
namespace NBvhFile
{
	CJoint* parse_hierarchy(std::vector<int> &order);
	CJoint* parse_node(CJoint *parent, std::vector<int> &order);
	CJoint* parse_endnode(CJoint *parent);
	bool parse_channel_order(size_t channels, int &order);

	bool parse_motion(CMotionData *mot, const std::vector<int> &orders);
	bool parse_root_coord(int order, Vector3f &v, Quaternionf &q);
	bool parse_joint_rotation(int order, Quaternionf &q);

	bool write_hierarchy(FILE *fout, const CFigure *fig);
	bool write_sub_hierarchy(FILE *fout, const CJoint *node, size_t depth);
	bool write_motion(FILE *fout, const CFigure *fig, const CMotionData *mot);

	void get_euler_zxy(float &rz, float &rx, float &ry, const Quaternionf &q);
};

//-----------------------------------------------------------------------------
// ����: �`�����l�����̈ʒu�C��]�I�[�_�[���ʎq�̉��
// ����: 
//       channels [in] �`�����l����
//       order [out] �ʒu�C��]�I�[�_�[���ʎq
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::parse_channel_order(size_t channels, int &order)
{
	if (channels < 1 || channels > 6)
		return false;

	order = 0;

	char *token;
	for (size_t i = 0;i < channels;++i)
	{
		GET_TOKEN(token, return false);

		int norder = 0;
		if (strncmp(token, "Xposition", 9) == 0)
			norder = ORDER_TX;
		else if (strncmp(token, "Yposition", 9) == 0)
			norder = ORDER_TY;
		else if (strncmp(token, "Zposition", 9) == 0)
			norder = ORDER_TZ;
		else if (strncmp(token, "Xrotation", 9) == 0)
			norder = ORDER_RX;
		else if (strncmp(token, "Yrotation", 9) == 0)
			norder = ORDER_RY;
		else if (strncmp(token, "Zrotation", 9) == 0)
			norder = ORDER_RZ;
		else
			return false;

		order = order * 10 + norder;
	}

	return true;
}

//-----------------------------------------------------------------------------
// ����: �G���h�T�C�g���
// ����: 
//       parent [in] �e�֐߃m�[�h�ւ̃|�C���^
// �Ԃ�l:
//       �������ꂽ�֐߃m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NBvhFile::parse_endnode(CJoint *parent)
{
	char *token, *jname;
	Vector3f offset(0, 0, 0);

	GET_TOKEN(jname, return NULL);
	GET_COMP_TOKEN(token, "{", 1, return NULL);

	CJoint *node = new CJoint(offset, parent);
	node->SetName(jname);
	while (1)
	{
		GET_COMP_TOKEN(token, "OFFSET", 6, break);
		if (!parse_vector_data(offset))
			break;
		//offset = basis_transform(offset);

		GET_COMP_TOKEN(token, "}", 1, break);

		node->SetOffset(offset);
		parent->AddChild(node);
		return node;
	}
	node->Destroy();
	return NULL;
}

//-----------------------------------------------------------------------------
// ����: �֐߃m�[�h���
// ����: 
//       parent [in] �e�֐߃m�[�h�ւ̃|�C���^
//       orders [in/out] ��]�I�[�_�[���ʎq��
// �Ԃ�l:
//       �������ꂽ�֐߃m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NBvhFile::parse_node(CJoint *parent, std::vector<int> &orders)
{
	char *token, *jname;
	Vector3f offset(0, 0, 0);
	int order = 0;

	GET_TOKEN(jname, return NULL);
	GET_COMP_TOKEN(token, "{", 1, return NULL);

	CJoint *node = new CJoint(offset, parent);
	node->SetName(jname);
	while (token = strtok(NULL, s_delim), token != NULL)
	{
		if (strncmp(token, "OFFSET", 6) == 0)
		{
			if (!parse_vector_data(offset))
				break;
			//offset = basis_transform(offset);

			GET_COMP_TOKEN(token, "CHANNELS", 8, break);
			GET_TOKEN(token, break);
			if (!parse_channel_order(static_cast<size_t>(atoi(token)), order))
				break;

			orders.push_back(order);
		}
		else if (strncmp(token, "JOINT", 5) == 0)
		{
			parse_node(node, orders);
		}
		else if (strncmp(token, "End", 3) == 0)
		{
			parse_endnode(node);
			orders.push_back(0);
		}
		else if (strncmp(token, "}", 1) == 0)
		{
			node->SetOffset(offset);
			if (parent != NULL)
				parent->AddChild(node);
			return node;
		}
		else
			break;
	}
	node->Destroy();

	return NULL;
}

//-----------------------------------------------------------------------------
// ����: �X�P���g���K�w�\�����̉��
// ����: 
//       orders [in/out] ��]�I�[�_�[���ʎq��
// �Ԃ�l:
//       ���[�g�m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NBvhFile::parse_hierarchy(std::vector<int> &orders)
{
	char *token;

	GET_COMP_TOKEN(token, "ROOT", 4, return NULL);
	return parse_node(NULL, orders);
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�����̉��
// ����: 
//       order [in] ��]�I�[�_�[���ʎq
//       q [out] ��]�N�H�[�^�j�I��
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::parse_joint_rotation(int order, Quaternionf &q)
{
	char *token;
	Matrix4f m,mt;
	float value;

	//Transform<float, 3, Affine> mt = Affine3f::Identity();
	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);
	for (int l = 100;l >= 1;l /= 10)
	{
		if (order >= l)
		{
			GET_TOKEN(token, return false);
			value = NEigenMath::ToRadian(static_cast<float>(atof(token)) );

			switch ((order / l)  % 10)
			{
			case ORDER_RX: NEigenMath::Matrix4fRotationX(&mt, value); break;
			case ORDER_RY: NEigenMath::Matrix4fRotationY(&mt, value); break;
			case ORDER_RZ: NEigenMath::Matrix4fRotationZ(&mt, value); break;
			default: return false;
			}
			m =  m * mt.matrix();
		}
	}
	//m = basis_transform(m);

	//AngleAxisf tmp;tmp = Matrix3f(m.block(0,0,3,3));
	//ROT(frm, jid) = Quaternionf(tmp);
	NEigenMath::QuaternionfRotationMatrix(&q, &m);

	return true;
}

//-----------------------------------------------------------------------------
// ����: ���[�g�ʒu�C��]�����̉��
// ����: 
//       order [in] ��]�I�[�_�[���ʎq
//       v [out] �ʒu�x�N�g��
//       q [out] ��]�N�H�[�^�j�I��
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::parse_root_coord(int order, Vector3f &v, Quaternionf &q)
{
	char *token;
	Matrix4f m, mt;
	float value;

	v.x() = v.y() = v.z() = 0;
	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);

	for (int l = 100000;l >= 1;l /= 10)
	{
		if (order >= l)
		{
			GET_TOKEN(token, return false);
			value = static_cast<float>(atof(token));

			mt = Matrix4f::Identity();
			//Matrix4fIdentity(&mt);
			switch ((order / l) % 10)
			{
			case ORDER_TX: v.x() = value; break;
			case ORDER_TY: v.y() = value; break;
			case ORDER_TZ: v.z() = value; break;
			case ORDER_RX: NEigenMath::Matrix4fRotationX(&mt, NEigenMath::ToRadian(value)); break;
			case ORDER_RY: NEigenMath::Matrix4fRotationY(&mt, NEigenMath::ToRadian(value)); break;
			case ORDER_RZ: NEigenMath::Matrix4fRotationZ(&mt, NEigenMath::ToRadian(value)); break;
			default: return false;
			}
			m = m * mt;
		}
	}
	//m = basis_transform(m);
	//v = basis_transform(v);
	NEigenMath::QuaternionfRotationMatrix(&q, &m);
	return true;
}

//-----------------------------------------------------------------------------
// ����: BVH���[�V�����f�[�^���̉��
// ����: 
//       mot [out] ���[�V�����f�[�^
//       orders [in] �ʒu�C��]�I�[�_�[���ʎq��
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::parse_motion(CMotionData *mot, const std::vector<int> &orders)
{
	char *token;
	GET_COMP_TOKEN(token, "Frames:", 7, return false);

	GET_TOKEN(token, return false);
	if (!mot->Initialize(static_cast<size_t>(atoi(token)), orders.size()))
		return false;

	GET_COMP_TOKEN(token, "Frame", 5, return false);
	GET_COMP_TOKEN(token, "Time:", 5, return false);
	GET_TOKEN(token, return false);
	mot->SetFrameTime(static_cast<float>(atof(token)));

	Vector3f v;
	Quaternionf q;
	for (size_t f = 0;f < mot->NumFrames();++f)
	{
		if (!parse_root_coord(orders[0], v, q))
			return false;
		mot->SetPosition(f, v);
		mot->SetRotation(f, 0, q);

		for (size_t j = 1;j < mot->NumJoints();++j)
		{
			if (orders[j] == 0)
				q = Quaternionf::Identity(); //QuaternionfIdentity(&q);
			else if (!parse_joint_rotation(orders[j], q))
				return false;
			mot->SetRotation(f, j, q);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// ����: BVH�t�@�C���̉��
// ����: 
//       fpath [in] �t�@�C���p�X
//       fig [out] �X�P���g���f�[�^
//       mot [out] ���[�V�����f�[�^
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NMotionFile::LoadBvhFile(const char *fpath, CFigure *fig, CMotionData *mot)
{
	char *buf, *token;
	std::vector<int> orders;
	CJoint *root;
	bool bSuccess = false;

	if (buf = NMotionFileLocal::load_file(fpath), buf == NULL)
		return false;

	token = strtok(buf, s_delim);
	while (1)
	{
		if (strncmp(token, "HIERARCHY", 9) != 0)
			break;
		if (root = NBvhFile::parse_hierarchy(orders), root == NULL)
			break;
		fig->Initialize(root);

		GET_TOKEN(token, break);
		if (strncmp(token, "MOTION", 6) != 0)
			break;

		bSuccess = NBvhFile::parse_motion(mot, orders);
		break;
	}
	delete[] buf;

	return bSuccess;
}

//-----------------------------------------------------------------------------
// ����: BVH�X�P���g���K�w���̃t�@�C���o��
// ����: 
//       fout [in] �o�͐�BVH�t�@�C���ւ̃|�C���^
//       fig [out] �X�P���g���f�[�^
// �Ԃ�l:
//       true �o�͐���
//       false �o�͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::write_hierarchy(FILE *fout, const CFigure *fig)
{
	const CJoint *root = fig->GetRoot();
	Vector3f offset = root->GetOffset();
	//Vector3f offset = basis_transform(root->GetOffset());

	fprintf(fout, "HIERARCHY\n");
	fprintf(fout, "ROOT %s\n", root->GetName());
	fprintf(fout, "{\n");
	fprintf(fout, "  OFFSET %f %f %f\n", offset.x(), offset.y(), offset.z());
	fprintf(fout, "  CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n");

	for (size_t i = 0;i < root->NumChildren();++i)
		write_sub_hierarchy(fout, root->GetChild(i), 1);
	fprintf(fout, "}\n");

	return true;
}

//-----------------------------------------------------------------------------
// ����: BVH�X�P���g���K�w���o�͊֐��̃T�u�֐�
// ����: 
//       fout [in] �o�͐�BVH�t�@�C���ւ̃|�C���^
//       node [in] �֐߃m�[�h�ւ̃|�C���^
//       depth [in] �ċA�[�x
// �Ԃ�l:
//       true �o�͐���
//       false �o�͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::write_sub_hierarchy(FILE *fout, const CJoint *node, size_t depth)
{
	//Vector3f offset = basis_transform(node->GetOffset());
	Vector3f offset = node->GetOffset();

	for (size_t i = 0;i < depth * 2;++i) fputc(' ', fout);
	if (node->NumChildren() > 0)
		fprintf(fout, "JOINT %s\n", node->GetName());
	else
		fprintf(fout, "End %s\n", node->GetName());
	for (size_t i = 0;i < depth * 2;++i) fputc(' ', fout);
	fprintf(fout, "{\n");

	for (size_t i = 0;i < depth * 2 + 2;++i) fputc(' ', fout);
	fprintf(fout, "OFFSET %f %f %f\n", offset.x(), offset.y(), offset.z());
	if (node->NumChildren() > 0)
	{
		for (size_t i = 0;i < depth * 2 + 2;++i) fputc(' ', fout);
		fprintf(fout, "CHANNELS 3 Zrotation Xrotation Yrotation\n");
	}

	for (size_t i = 0;i < node->NumChildren();++i)
		write_sub_hierarchy(fout, node->GetChild(i), depth + 1);

	for (size_t i = 0;i < depth * 2;++i) fputc(' ', fout);
	fprintf(fout, "}\n");

	return true;
}

//-----------------------------------------------------------------------------
// ����: ��]�N�H�[�^�j�I�����CZXY�I�[�_�[�̃I�C���[�p�ɕ���
// ����: 
//       rz [out] �I�C���[�p(Z)
//       rx [out] �I�C���[�p(X)
//       ry [out] �I�C���[�p(Y)
//       q [in] ��]�N�H�[�^�j�I��
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void NBvhFile::get_euler_zxy(float &rz, float &rx, float &ry, const Quaternionf &q)
{
	Matrix4f m;
	NEigenMath::Matrix4fRotationQuaternion(&m, &q);
	//m = basis_transform(m);

	ry = atan2f(m(0,2), m(2,2));
	rx = -asinf(m(1,2));
	rz = atan2f(m(1,0), m(1,1));

	if (fabsf(cosf(rx)) < 1.0e-6f)
	{
		if (m(0,2) < 0.0f)
			ry = NEigenMath::EigenPI + ry;
		else
			ry -= NEigenMath::EigenPI;
		if (m(1,0) < 0.0f)
			rz = NEigenMath::EigenPI + rz;
		else
			rz -= NEigenMath::EigenPI;
	}

	rx = -NEigenMath::ToDegree(rx);
	ry = -NEigenMath::ToDegree(ry);
	rz = -NEigenMath::ToDegree(rz);
}

//-----------------------------------------------------------------------------
// ����: BVH���[�V�������̃t�@�C���o��
// ����: 
//       fout [in] �o�͐�BVH�t�@�C���ւ̃|�C���^
//       fig [in] �X�P���g���f�[�^
//       mot [in] ���[�V�����f�[�^
// �Ԃ�l:
//       true �o�͐���
//       false �o�͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NBvhFile::write_motion(FILE *fout, const CFigure *fig, const CMotionData *mot)
{
	Vector3f v;
	Quaternionf q;
	Matrix4f m;
	float rx, ry, rz;

	get_euler_zxy(rz, rx, ry, q);

	fprintf(fout, "MOTION\n");
	fprintf(fout, "Frames:  %d\n", mot->NumFrames());
	fprintf(fout, "Frame Time: 0.03333\n");

	for (size_t f = 0;f < mot->NumFrames();++f)
	{
		//v = basis_transform(mot->GetPosition(f));
		v = mot->GetPosition(f);
		fprintf(fout, "%f %f %f", v.x(), v.y(), v.z());

		for (size_t j = 0;j < mot->NumJoints();++j)
		{
			if (fig->IsEffector(j))
				continue;
			q = mot->GetRotation(f, j);
			get_euler_zxy(rz, rx, ry, q);

			fprintf(fout, " %f %f %f", rz, rx, ry);
		}
		fputc('\n', fout);
	}
	return true;
}

//-----------------------------------------------------------------------------
// ����: BVH���[�V�����t�@�C���̃Z�[�u
// ����: 
//       fpath [in] �o�͐�BVH�t�@�C���ւ̃p�X
//       fig [in] �X�P���g���f�[�^
//       mot [in] ���[�V�����f�[�^
// �Ԃ�l:
//       true �Z�[�u����
//       false �Z�[�u���s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NMotionFile::StoreBvhFile(const char *fpath, const CFigure *fig, const CMotionData *mot)
{
	FILE *fout;
	if (fout = fopen(fpath, "w"), fout == NULL)
		return false;

	if (!NBvhFile::write_hierarchy(fout, fig))
		return false;
	if (!NBvhFile::write_motion(fout, fig, mot))
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���O���: NAsfAmcFile
// ASF-AMC�t�@�C����͗p�T�u�֐��Q
namespace NAsfAmcFile
{
	CJoint* parse_asf(FILE *fin, std::map<std::string, int> &orders, std::map<std::string, Matrix4f> &axes);
	CJoint* parse_hierarchy(FILE *fin, std::map<std::string, CJoint*> &joints);
	CJoint* parse_asf_root(FILE *fin, std::map<std::string, int> &order);
	bool parse_asf_header(FILE *fin);
	bool parse_asf_bone(FILE *fin, std::map<std::string, CJoint*> &joints, std::map<std::string, int> &orders, std::map<std::string, Matrix4f> &axes);
	bool parse_root_order(int &order);
	bool parse_bone_order(int &order);
	bool parse_axis(Matrix4f &m, const Vector3f &axis);
	Matrix4f calc_axis_transform(CJoint *node, const std::map<std::string, Matrix4f> &axes);

	bool parse_amc(FILE *fin, CMotionData *mot, CFigure *fig, const std::map<std::string, int> &orders, const std::map<std::string, Matrix4f> &axes);
	bool parse_root_coord(int order, Vector3f &v, Quaternionf &q);
	bool parse_joint_rotation(int order, Quaternionf &q);
	size_t parse_frame_count(FILE *fin);

	void change_skeletal_structure(CJoint *node, Vector3f &v, std::map<std::string, Matrix4f> &axes);
};

//-----------------------------------------------------------------------------
// ����: �֐߉�]�����̉��
// ����: 
//       order [in] ��]�I�[�_�[���ʎq
//       q [out] ��]�N�H�[�^�j�I��
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_joint_rotation(int order, Quaternionf &q)
{
	char *token;
	Matrix4f m, mt;
	float value;

	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);
	for (int l = 100;l >= 1;l /= 10)
	{
		if (order >= l)
		{
			GET_TOKEN(token, return false);
			value = NEigenMath::ToRadian( static_cast<float>(atof(token)) );

			switch ((order / l)  % 10)
			{
			case ORDER_RX: NEigenMath::Matrix4fRotationX(&mt, value); break;
			case ORDER_RY: NEigenMath::Matrix4fRotationY(&mt, value); break;
			case ORDER_RZ: NEigenMath::Matrix4fRotationZ(&mt, value); break;
			default: return false;
			}
			m *= mt;
		}
	}
	//m = basis_transform(m);
	NEigenMath::QuaternionfRotationMatrix(&q, &m);

	return true;
}

//-----------------------------------------------------------------------------
// ����: ���[�g�ʒu�C��]�����̉��
// ����: 
//       order [in] ��]�I�[�_�[���ʎq
//       v [out] �ʒu�x�N�g��
//       q [out] ��]�N�H�[�^�j�I��
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_root_coord(int order, Vector3f &v, Quaternionf &q)
{
	char *token;
	Matrix4f m, mt;
	float value;

	v.x() = v.y() = v.z() = 0;
	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);

	for (int l = 100000;l >= 1;l /= 10)
	{
		if (order >= l)
		{
			GET_TOKEN(token, return false);
			value = static_cast<float>(atof(token));

			mt = Matrix4f::Identity();
			//Matrix4fIdentity(&mt);
			switch ((order / l) % 10)
			{
			case ORDER_TX: v.x() = value; break;
			case ORDER_TY: v.y() = value; break;
			case ORDER_TZ: v.z() = value; break;
			case ORDER_RX: NEigenMath::Matrix4fRotationX(&mt, NEigenMath::ToRadian(value)); break;
			case ORDER_RY: NEigenMath::Matrix4fRotationY(&mt, NEigenMath::ToRadian(value)); break;
			case ORDER_RZ: NEigenMath::Matrix4fRotationZ(&mt, NEigenMath::ToRadian(value)); break;
			default: return false;
			}
			m *= mt;
		}
	}
	//v = basis_transform(v);
	//m = basis_transform(m);
	NEigenMath::QuaternionfRotationMatrix(&q, &m);

	return true;
}

//-----------------------------------------------------------------------------
// ����: ASF�t�@�C���w�b�_�̉��
// ����: 
//       fin [in] ASF�t�@�C���ւ̃|�C���^
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_asf_header(FILE *fin)
{
	char buf[1024], *token;

	//version
	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':');
	if (token = strtok(buf, s_delim), token == NULL)
		return false;
	if (strncmp(token, ":version", 8) != 0) 
		return false;

	// name
	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':');
	if (token = strtok(buf, s_delim), token == NULL)
		return false;
	if (strncmp(token, ":name", 5) != 0)
		return false;

	// units
	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':');
	if (token = strtok(buf, s_delim), token == NULL)
		return false;
	if (strncmp(token, ":units", 6) != 0)
		return false;

	// documentation
	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':');
	if (token = strtok(buf, s_delim), token == NULL)
		return false;
	if (strncmp(token, ":documentation", 15) != 0)
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// ����: ���[�g�̈ʒu�C��]�I�[�_�[���ʎq�̉��
// ����: 
//       order [out] �ʒu�C��]�I�[�_�[���ʎq
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_root_order(int &order)
{
	order = 0;

	char *token;
	while (token = strtok(NULL, s_delim), token != NULL)
	{
		int norder = 0;
		if (strncmp(token, "TX", 2) == 0)
			norder = ORDER_TX;
		else if (strncmp(token, "TY", 2) == 0)
			norder = ORDER_TY;
		else if (strncmp(token, "TZ", 2) == 0)
			norder = ORDER_TZ;
		else if (strncmp(token, "RX", 2) == 0)
			norder = ORDER_RX;
		else if (strncmp(token, "RY", 2) == 0)
			norder = ORDER_RY;
		else if (strncmp(token, "RZ", 2) == 0)
			norder = ORDER_RZ;
		else
			return false;

		order = order * 10 + norder;
	}

	return true;
}

//-----------------------------------------------------------------------------
// ����: �֐߉�]�I�[�_�[���ʎq�̉��
// ����: 
//       order [out] ��]�I�[�_�[���ʎq
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_bone_order(int &order)
{
	order = 0;

	char *token;
	while (token = strtok(NULL, s_delim), token != NULL)
	{
		int norder = 0;
		if (strncmp(token, "rx", 2) == 0)
			norder = ORDER_RX;
		else if (strncmp(token, "ry", 2) == 0)
			norder = ORDER_RY;
		else if (strncmp(token, "rz", 2) == 0)
			norder = ORDER_RZ;
		else
			return false;

		order = order * 10 + norder;
	}

	return true;
}

//-----------------------------------------------------------------------------
// ����: ASF���[�g�m�[�h�̉��
// ����: 
//       fin [in] ASF�t�@�C���ւ̃|�C���^
//       orders [in/out] �֐ߖ�-��]�I�[�_�[�̑Ή��}�b�v
// �Ԃ�l:
//       �������ꂽ���[�g�m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NAsfAmcFile::parse_asf_root(FILE *fin, std::map<std::string, int> &orders)
{
	char buf[1024], *token;
	Vector3f offset, orientation;

	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':')
	{
		if (token = strtok(buf, s_delim), token == NULL)
			return NULL;

		if (strncmp(token, "order", 5) == 0)
		{
			int order;
			parse_root_order(order);
			orders["root"] = order;
		}
		else if (strncmp(token, "axis", 4) == 0)
		{
			// TODO: implement (?)
		}
		else if (strncmp(token, "position", 8) == 0)
		{
			if (!parse_vector_data(offset))
				return false;
			//offset = basis_transform(offset);
		}
		else if (strncmp(token, "orientation", 11) == 0)
		{
			if (!parse_vector_data(orientation))
				return false;
			//orientation = basis_transform(orientation);
		}
		else
			return NULL;
	}

	if (token = strtok(buf, s_delim), token == NULL || strncmp(token, ":bonedata", 9) != 0)
		return NULL;
	CJoint *root = new CJoint(offset, NULL);
	root->SetName("root");
	return root;
}

//-----------------------------------------------------------------------------
// ����: axis�I�[�_�[�̉�͂�axis�s��̎擾
// ����: 
//       m [out] axis�s��
//       axis [in] axis����
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_axis(Matrix4f &m, const Vector3f &axis)
{
	char *token;
	GET_TOKEN(token, false);

	Matrix4f mt;
	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);
	size_t cnt;
	for (cnt = 0;cnt < 3;++cnt)
	{
		if (token[cnt] == 'X')
			NEigenMath::Matrix4fRotationX(&mt, NEigenMath::ToRadian(axis[cnt]));
		else if (token[cnt] == 'Y')
		    NEigenMath::Matrix4fRotationY(&mt, NEigenMath::ToRadian(axis[cnt]));
		else if (token[cnt] == 'Z')
			NEigenMath::Matrix4fRotationZ(&mt, NEigenMath::ToRadian(axis[cnt]));
		else
			break;

		m *= mt;
	}
	//m = basis_transform(m);
	return cnt == 3 ? true : false;
}

//-----------------------------------------------------------------------------
// ����: ASF�{�[�����̉��
// ����: 
//       fin [in] ASF�t�@�C���ւ̃|�C���^
//       joints [in/out] �֐ߖ�-�֐߃m�[�h�|�C���^�̑Ή��}�b�v
//       orders [in/out] �֐ߖ�-�֐߉�]�I�[�_�[�̑Ή��}�b�v
//       axes [in/out] �֐ߖ�-�֐�axis�s��̑Ή��}�b�v
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_asf_bone(FILE *fin, std::map<std::string, CJoint*> &joints, std::map<std::string, int> &orders, std::map<std::string, Matrix4f> &axes)
{
	char buf[1024], *token, jname[256];
	CJoint *node;
	Vector3f dir, axis, v;
	Matrix4f maxis, m;
	float len;
	int order = 0;

	while (fgets(buf, 1024, fin) != NULL)
	{
		token = strtok(buf, s_delim);
		if (strncmp(token, "id", 2) == 0)
		{
			GET_TOKEN(token, break);
		}
		else if (strncmp(token, "name", 4) == 0)
		{
			GET_TOKEN(token, break);
			strcpy(jname, token);
		}
		else if (strncmp(token, "direction", 9) == 0)
		{
			if (!parse_vector_data(dir))
				break;
			//dir = basis_transform(dir);
		}
		else if (strncmp(token, "length", 6) == 0)
		{
			GET_TOKEN(token, break);
			len = static_cast<float>(atof(token));
		}
		else if (strncmp(token, "axis", 4) == 0)
		{
			if (!parse_vector_data(axis) || !parse_axis(maxis, axis))
				break;
			axes[jname] = maxis;
		}
		else if (strncmp(token, "dof", 3) == 0)
		{
			if (!parse_bone_order(order))
				break;
		}
		else if (strncmp(token, "limits", 6) == 0)
		{
			if (order >= 100)
				fgets(buf, 1024, fin);
			if (order >= 10)
				fgets(buf, 1024, fin);
		}
		else if (strncmp(token, "bodymass", 8) == 0)
		{
			// TODO: bodymass
		}
		else if (strncmp(token, "cofmass", 7) == 0)
		{
			// TODO: cofmass
		}
		else if (strncmp(token, "end", 3) == 0)
		{
			dir *= len;

			m = maxis.transpose();
			//Matrix4fTranspose(&m, &maxis);
			NEigenMath::Vector3fTransformCoord(&v, &dir, &m);
			//D3DXVec3TransformCoord(&v, &dir, &m);

			node = new CJoint(v, NULL);
			node->SetName(jname);
			joints[jname] = node;
			orders[jname] = order;

			return true;
		}
		else
			break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// ����: �֐ߊK�w�\���̉��
// ����: 
//       fin [in] ASF�t�@�C���ւ̃|�C���^
//       joints [in/out] �֐ߖ�-�֐߃m�[�h�|�C���^�̑Ή��}�b�v
// �Ԃ�l:
//       ���[�g�m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NAsfAmcFile::parse_hierarchy(FILE *fin, std::map<std::string, CJoint*> &joints)
{
	char buf[1024], *token;
	CJoint *root, *parent, *node;
	std::map<std::string, CJoint*>::iterator itj;

	if (fgets(buf, 1024, fin) == NULL)
		return NULL;
	if (token = strtok(buf, s_delim), token == NULL || strncmp(token, "begin", 5) != 0)
		return NULL;
	if (itj = joints.find("root"), itj == joints.end())
		return NULL;

	root = joints["root"];
	while (fgets(buf, 1024, fin) != NULL)
	{
		if (token = strtok(buf, s_delim), token == NULL)
			break;

		if (strncmp(token, "end", 3) == 0)
			return root;
		else
		{
			if (itj = joints.find(token), itj == joints.end())
				return NULL;
			parent = itj->second;
		}

		while (token = strtok(NULL, s_delim), token != NULL)
		{
			if (itj = joints.find(token), itj == joints.end())
				return NULL;
			node = itj->second;
			parent->AddChild(node);
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// ����: ASF�t�@�C���̉��
// ����: 
//       fin [in] ASF�t�@�C���ւ̃|�C���^
//       orders [out] �֐ߖ�-�֐߉�]�I�[�_�[�̑Ή��}�b�v
//       axes [out] �֐ߖ�-�֐�axis�s��̑Ή��}�b�v
// �Ԃ�l:
//       �������ꂽ���[�g�m�[�h�ւ̃|�C���^
// ���̑�: 
//-----------------------------------------------------------------------------
CJoint* NAsfAmcFile::parse_asf(FILE *fin, std::map<std::string, int> &orders, std::map<std::string, Matrix4f> &axes)
{
	char buf[1024], *token;
	CJoint *root;
	std::map<std::string, CJoint*> joints;
	Matrix4f m;
	
	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':');
	token = strtok(buf, s_delim);
	if (strncmp(token, ":root", 15) != 0)
		return NULL;

	if (root = parse_asf_root(fin, orders), root == NULL)
		return NULL;
	joints["root"] = root;
	m = Matrix4f::Identity();
	//Matrix4fIdentity(&m);
	axes["root"] = m;

	while (fgets(buf, 1024, fin) != NULL && buf[0] != ':')
	{
		if (token = strtok(buf, s_delim), token == NULL ||  strncmp(token, "begin", 5) != 0)
			break;
		if (!parse_asf_bone(fin, joints, orders, axes))
			break;
	}

	if (token = strtok(buf, s_delim), token == NULL || strncmp(token, ":hierarchy", 10) != 0)
	{
		root->Destroy();
		for (std::map<std::string, CJoint*>::iterator it = joints.begin();it != joints.end();++it)
			it->second->Destroy();
		return NULL;
	}

	return parse_hierarchy(fin, joints);
}

//-----------------------------------------------------------------------------
// ����: AMC���t���[�����̉��
// ����: 
//       fin [in] AMC�t�@�C���ւ̃|�C���^
// �Ԃ�l:
//       ���t���[����
// ���̑�: 
//-----------------------------------------------------------------------------
size_t NAsfAmcFile::parse_frame_count(FILE *fin)
{
	char buf[1024], *token;
	size_t tmp, frames = 0;

	fseek(fin, 0L, SEEK_SET);
	while (fgets(buf, 1024, fin) != NULL)
	{
		if (token = strtok(buf, s_delim), token == NULL)
			return 0;

		tmp = static_cast<size_t>(atoi(token));
		if (tmp > frames)
			frames = tmp;
	}

	return frames;
}

//-----------------------------------------------------------------------------
// ����: axis�����ɂ��g�����X�t�H�[���s��̌v�Z
// ����: 
//       node [in] �֐߃m�[�h�ւ̃|�C���^
//       axes [in] �֐ߖ�-�֐�axis�s��̑Ή��}�b�v
// �Ԃ�l:
//       axis�g�����X�t�H�[���s��
// ���̑�: 
//-----------------------------------------------------------------------------
Matrix4f NAsfAmcFile::calc_axis_transform(CJoint *node, const std::map<std::string, Matrix4f> &axes)
{
	const char *name1 = node->GetName();
	std::map<std::string, Matrix4f>::const_iterator cit;

	cit = axes.find(name1);
	Matrix4f m1 = cit->second;

	if (node->GetParent() != NULL)
	{
		const char *name2 = node->GetParent()->GetName();
		cit = axes.find(name2);
		Matrix4f m2, mt = cit->second;
		m2 = mt.transpose();
		//Matrix4fTranspose(&m2, &mt);
		m1 = m1 * m2;
	}
	return m1;
}

//-----------------------------------------------------------------------------
// ����: AMC�t�@�C���̉��
// ����: 
//       fin [in] AMC�t�@�C���ւ̃|�C���^
//       mot [out] ���[�V�����f�[�^
//       fig [in] �X�P���g���f�[�^
//       orders [in] �֐ߖ�-�֐߉�]�I�[�_�[�̑Ή��}�b�v
//       axes [in/out] �֐ߖ�-�֐�axis�s��̑Ή��}�b�v
// �Ԃ�l:
//       true ��͐���
//       false ��͎��s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NAsfAmcFile::parse_amc(FILE *fin, CMotionData *mot, CFigure *fig, const std::map<std::string, int> &orders, const std::map<std::string, Matrix4f> &axes)
{
	char buf[1024], *token;
	std::map<std::string, int>::const_iterator cit;
	size_t frames = parse_frame_count(fin), frm = 0;
	int jid;

	Quaternionf q;
	Matrix4f m;
	Vector3f v;

	fseek(fin, 0L, SEEK_SET);

	if (!mot->Initialize(frames, fig->NumJoints()))
		return false;

	while (fgets(buf, 1024, fin) != NULL)
	{
		if (buf[0] == '#' || buf[0] == ':')
			continue;

		if (token = strtok(buf, s_delim), token == NULL)
			return false;

		if (token[0] >= '0' && token[0] <= '9')
		{
			frm = static_cast<size_t>(atoi(token)) - 1;
			continue;
		}

		if (jid = fig->FindNode(token), jid < 0)
			return false;

		cit = orders.find(token);
		if (jid == 0)
		{
			parse_root_coord(cit->second, v, q);
			mot->SetPosition(frm, v);
			mot->SetRotation(frm, jid, q);
		}
		else
		{
			parse_joint_rotation(cit->second, q);
			NEigenMath::Matrix4fRotationQuaternion(&m, &q);
			m = m * calc_axis_transform( fig->GetJoint(jid), axes);
			NEigenMath::QuaternionfRotationMatrix(&q, &m);
			mot->SetRotation(frm, jid, q);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// ����: ASF�`���̃X�P���g���\����BVH�`���ɕϊ�
// ����: 
//       node [in] �֐߃m�[�h�ւ̃|�C���^
//       v [in] �I�t�Z�b�g�x�N�g��
//       axes [in/out] �֐ߖ�-�֐�axis�s��̑Ή��}�b�v
// �Ԃ�l:
// ���̑�: 
//-----------------------------------------------------------------------------
void change_skeletal_structure(CJoint *node, Vector3f &v, std::map<std::string, Matrix4f> &axes)
{
	static int ls_extent = 0;
	if (node->NumChildren() > 0)
	{
		for (size_t i = 0;i < node->NumChildren();++i)
			change_skeletal_structure(node->GetChild(i), node->GetOffset(), axes);
	}
	else
	{
		CJoint *ext = new CJoint(node->GetOffset(), node);
		node->AddChild(ext);

		char buf[6];
		sprintf(buf, "ext%02d", ls_extent++);
		ext->SetName(buf);

		Matrix4f m;
		m = Matrix4f::Identity();
		//Matrix4fIdentity(&m);
		axes[buf] = m;
	}
	node->SetOffset(v);
}

//-----------------------------------------------------------------------------
// ����: ASF-AMC�t�@�C���̃��[�h
// ����: 
//       asfpath [in] ASF�t�@�C���ւ̃p�X
//       amcpath [in] AMC�t�@�C���ւ̃p�X
//       fig [out] �X�P���g���f�[�^
//       mot [out] ���[�V�����f�[�^
// �Ԃ�l:
//       true ���[�h����
//       false ���[�h���s
// ���̑�: 
//-----------------------------------------------------------------------------
bool NMotionFile::LoadAsfAmcFile(const char *asfpath, const char *amcpath, CFigure *fig, CMotionData *mot)
{
	FILE *fasf, *famc;
	std::map<std::string, int> orders;
	std::map<std::string, Matrix4f> axes;
	CJoint *root;

	if (fasf = fopen(asfpath, "r"), fasf == NULL)
		return false;
	if (!NAsfAmcFile::parse_asf_header(fasf))
		return false;

	if (root = NAsfAmcFile::parse_asf(fasf, orders, axes), root == NULL)
		return false;
	fclose(fasf);

	change_skeletal_structure(root, Vector3f(0, 0, 0), axes);
	fig->Initialize(root);

	if (famc = fopen(amcpath, "r"), famc == NULL)
		return false;
	if (!NAsfAmcFile::parse_amc(famc, mot, fig, orders, axes))
		return false;
	fclose(famc);

	return true;
}
