//-*-c++-*-
// made by Numaguchi
#ifndef _klQuaternion_h_
#define _klQuaternion_h_

#include "klVector.h"
#include "klMatrix.h"
#include <vector>
#include <assert.h>

namespace KlMath
{

class klQuat;

//-------------------------------------------------------------------
// klQuat �N���X
// �T�v: Quaternion�̃N���X
//-------------------------------------------------------------------
class klQuat
{
	public:
	//�v�f�ɕʖ���^����
	union {
		double q[4];
		struct { 
			double x, y, z, w;
		};
	};
  //���p�̂��g���Ă���
  //double q[4]��double x,y,z,w �ɓ����̈悪���蓖�Ă���
  //�܂�Q�̈Ⴄ�\�킵�����ł���

  //[]�ł̎Q�Ƃ��\�ɂ���
  double & operator[](int const &i) {
	  return q[i];
  }

  //---------------- �R���X�g���N�^
  klQuat();
  klQuat(const klQuat &);
  klQuat(const klVec4 &);
  klQuat(const double r[4]);
  klQuat(const double x, const double y, const double z, const double w);
  klQuat(vector<double> vec);
  klQuat(const double theta, const klVec3 &axis);

  //---------------- ���g���ω����郁�\�b�h�i�Ԓl�͎������g�̎Q�Ɓj
  klQuat& Clear();//������
  klQuat& Normalize();				// �傫�����K��
  klQuat& Clamp();				// ������[0-1]�ɃN�����v

  ////Convert���\�b�h
  // klQuat <- Euler
  klQuat& FromEuler(const double pitch, const double yaw, const double roll);
  // klQuat <- klMat
  klQuat& FromklMat(const klMat44 &R);
  //klQuat& FromklMat2(const klMat44 &R); // from TMPS wiki
  // klQuat <- RotVec
  klQuat& FromRotVec(const klVec3 &v, const double &a);
  klQuat& FromRotVec(const klVec4 &v);
  // klQuat <- Two Vec
  klQuat& FromTwoVec(const klVec3 &src, const klVec3 &dst);


  klQuat& Rotate(const double degree,
                 const double vx,
                 const double vy,
                 const double vz);	// ��]
  klQuat& Rotate(const double degree, const klVec3 &v);// ��]
  klQuat& Rotate(const klVec3 &from, const klVec3 &to);// ��]


  //klVec4& Translate(const double x,
  //                  const double y,
  //                  const double z);	// ���i
  //klVec4& Translate(const klVec3 &v);	// ���i
  //klVec4& Translate(const klVec4 &v);	// ���i
  //klVec4& Scale(const double x, const double y, const double z);
  //klVec4& Scale(const klVec3 &v);
  //klVec4& Scale(const klVec4 &v);
  
  // ������Z�q
  //klVec4& operator =  (const klVec3 &u);	// ���; w = 1.0
  klQuat& operator =  (const klQuat &u);	// ���
  klQuat& operator +=  (const klQuat &u); // �a
  klQuat& operator *=  (const klQuat &u); // ��
  klQuat& operator *=  (const double &u); // �萔�{
  klQuat& operator /=  (const double &u); // �萔�{

  //klVec4& operator += (const klVec3 &u);	// �a
  //klVec4& operator += (const klVec4 &u);	// �a
  //klVec4& operator -= (const klVec3 &u);	// ��
  //klVec4& operator -= (const klVec4 &u);	// ��
  //klVec4& operator %= (const klVec3 &u);	// �O��
  //klVec4& operator %= (const klVec4 &u);	// �O��
  //klVec4& operator *= (const double r);		// �萔�{
  //klVec4& operator /= (const double r);		// �萔�{

  //---------------- ���g���ω����Ȃ����\�b�h
  klQuat Abs() const;//��Βl
  klQuat Conjugate() const;//����
  double Norm() const;//�m����
  klQuat Inverse() const;//�t�N�H�[�^�j�I��
  double GetYawAngle() const;//Yaw
  double GetRoll() const;//Roll
  double GetPitch() const;//Pitch
  double GetYaw() const;//Yaw

  ////Convert���\�b�h
  //klQuat -> klMat
  klMat44 ToklMat44() const; //original is Quat_to_klMat
  //klMat44 ToklMat44_2() const;
  //klQuat -> Matrix
  void ToMatrix(double* mat) const; //original is quat_to_gl_matrix
  //klQuat -> Euler
  klVec3 ToEulerZxy() const;
  //klQuat -> RotVec
  klVec4 ToRotVec() const;
  klVec3 ToRotVec2() const; //original is toklVec3

  ////��]���\�b�h
  //Compute vector rotated by quaternion
  klVec3 Rotate(const klVec3 &v) const;

  ////��ԃ��\�b�h
  klQuat Lerp(const klQuat &a, const klQuat & b, const double amt) const;// ���`���

  //double Abs() const;				// ��Βl
  //double Length() const;			// ����
  //double Angle(const klVec3 &) const;		// ���̃x�N�g���Ɛ����p
  //double Angle(const klVec4 &) const;		// ���̃x�N�g���Ɛ����p
  //double Distance(const klVec3 &) const;	// ���̃x�N�g���Ƃ̋���
  //double Distance(const klVec4 &) const;	// ���̃x�N�g���Ƃ̋���
  //double DistToLine(const klVec3 &,
  //                  const klVec3 &) const;	// 2�_�����������Ƃ̋���
  //double DistToLine(const klVec4 &,
  //                  const klVec4 &) const;	// 2�_�����������Ƃ̋���
  //bool OnLine(const klVec3 &,
  //            const klVec3 &) const;		// 2�_������������ɂ��邩
  //bool OnLine(const klVec4 &,
  //            const klVec4 &) const;		// 2�_������������ɂ��邩
  //klVec4 ProjToLine(const klVec3 &,
  //                  const klVec3 &) const;	// 2�_�����������։��낵����
  //klVec4 ProjToLine(const klVec4 &,
  //                  const klVec4 &) const;	// 2�_�����������։��낵����
  //double DistToPlane(const klVec3 &,
  //                   const klVec3 &,
  //                   const klVec3 &) const;	// 3�_���������ʂƂ̋���
  //double DistToPlane(const klVec4 &,
  //                   const klVec4 &,
  //                   const klVec4 &) const;	// 3�_���������ʂƂ̋���
  //bool OnPlane(const klVec3 &,
  //             const klVec3 &,
  //             const klVec3 &) const;		// 3�_���������ʏ�ɂ��邩
  //bool OnPlane(const klVec4 &,
  //             const klVec4 &,
  //             const klVec4 &) const;		// 3�_���������ʏ�ɂ��邩
  //klVec4 ProjToPlane(const klVec3 &,
  //                   const klVec3 &,
  //                   const klVec3 &) const;	// 3�_���������ʂ։��낵����
  //klVec4 ProjToPlane(const klVec4 &,
  //                   const klVec4 &,
  //                   const klVec4 &) const;	// 3�_���������ʂ։��낵����

  // �P�����Z�q
  klQuat operator !() const;	// �t�� 
  klQuat operator ~() const;	// ����

  // 2�����Z�q 
  klQuat operator * (const klQuat &u) const; //��
  klQuat operator * (const double &u) const; //�萔�{
  klQuat operator / (const double &u) const; //�萔�{
  klQuat operator + (const klQuat &u) const; //�a

  //double operator * (const klVec3 &u) const;	// ����
  //double operator * (const klVec4 &u) const;	// ����
  //klVec4 operator + (const klVec3 &u) const;	// �a
  //klVec4 operator + (const klVec4 &u) const;	// �a
  //klVec4 operator - (const klVec3 &u) const;	// ��
  //klVec4 operator - (const klVec4 &u) const;	// ��
  //klVec4 operator % (const klVec3 &u) const;	// �O��
  //klVec4 operator % (const klVec4 &u) const;	// �O��
  //klVec4 operator * (const double r) const;	// �萔�{
  //klVec4 operator / (const double r) const;	// �萔�{

  //friend klVec4 operator * (const double r, const klVec4 &v);// �萔�{

  // ��r���Z�q
  bool operator == (const klQuat &u) const;	// ��v
  bool operator != (const klQuat &u) const;	// �s��v

  friend istream &operator>>(istream &i, klQuat &v);
  friend ostream &operator<<(ostream &o, klQuat v);

  static double epsilon;

  //---------------- �ÓI�֐�
public:

  //Convert�֐�

  //klQuat <- Euler
  static klQuat fromEuler(const double pitch, const double yaw, const double roll);
  //klQuat <- klMat44
  static klQuat fromklMat(const klMat44& R);
  //static klQuat fromklMat2(const klMat44 &R);//original is Quat_to_klMat
  //klQuat <- Rot Vec
  static klQuat fromRotVec(const klVec3 &v, const double &a);
  static klQuat fromRotVec(const klVec4 &v);
  //klQuat <- Two Vec
  static klQuat fromTwoVec(const klVec3 &src, const klVec3 &dist);

  //klQuat -> klMat
  static klMat44 toklMat44(const klQuat &q); //original is Quat_to_klMat
  //static klMat44 toklMat44_2(const klQuat &q);
  //klQuat -> Mat
  static void toMatrix(const klQuat &q, double* mat); //original is quat_to_gl_matrix
  //klQuat -> Euler
  static klVec3 toEulerZxy(const klQuat &q);
  //klQuat -> RotVec
  static klVec4 toRotVec(const klQuat &q);
  static klVec3 toRotVec2(const klQuat &q); //original is toklVec3

protected:
  float f[4];
};
} //namespace KlMath

#endif
