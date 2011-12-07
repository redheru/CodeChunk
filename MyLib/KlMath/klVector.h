//-*-c++-*-
#ifndef _klVector_h_
#define _klVector_h_

#include <iostream>
using namespace std;

namespace KlMath
{

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926536
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (M_PI / 180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0 / M_PI)
#endif

class klVec2;
class klVec3;
class klVec4;

//-------------------------------------------------------------------
// klVec2 �N���X
// �T�v: �񎟌��x�N�g���̃N���X
//-------------------------------------------------------------------
class klVec2
{
public:
  // �v�f�ɕʖ���^����
  union {
    double v[2];
	struct {
      double x, y;
    };
  };
  //[]�ł̎Q�Ƃ��\�ɂ���
  double & operator[](int const &i) {
	  return v[i];
  }

  //���p�̂��g���Ă���
  //double v[2]��double x,y �ɓ����̈悪���蓖�Ă���
  //�܂�Q�̈Ⴄ�\�킵�����ł���
  
  //---------------- �R���X�g���N�^
  klVec2();
  klVec2(const klVec2 &);
  klVec2(const double r[2]);
  klVec2(const double x, const double y);

  // double* �ւ̃L���X�g - ����ɂ�� glVertex3dv(v); �ȂǂƏ�����
  operator double *() {
    return v;
  };
  // float* �ւ̃L���X�g - ����ɂ�� glVertex3fv(v); �ȂǂƏ�����
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1];
    return f;
  };

  //---------------- ���g���ω����郁�\�b�h�i�Ԓl�͎������g�̎Q�Ɓj
  klVec2& Normalize();				// �傫�����K��
  klVec2& Clamp();				// ������[0-1]�ɃN�����v
  klVec2& Rotate(const double degree);		// ��]
  klVec2& Translate(const double x, const double y);	// ���i
  
  // ������Z�q
  klVec2& operator =  (const klVec2 &u);	// ���
  klVec2& operator += (const klVec2 &u);	// �a
  klVec2& operator -= (const klVec2 &u);	// ��
  klVec2& operator *= (const double r);		// �萔�{
  klVec2& operator /= (const double r);		// �萔�{

  //---------------- ���g���ω����Ȃ����\�b�h
  double Abs() const;				// ��Βl
  double Length() const;			// ����
  double Angle(const klVec2 &) const;		// ���̃x�N�g���Ɛ����p
  double Distance(const klVec2 &) const;	// ���̃x�N�g���Ƃ̋���

  // �P�����Z�q
  klVec2 operator +() const;
  klVec2 operator -() const;

  // 2�����Z�q 
  double operator * (const klVec2 &u) const;	// ����
  klVec2 operator + (const klVec2 &u) const;	// �a
  klVec2 operator - (const klVec2 &u) const;	// ��
  klVec2 operator * (const double r) const;	// �萔�{
  klVec2 operator / (const double r) const;	// �萔�{

  // double ���O�ɗ���2�����Z�q
  friend klVec2 operator * (const double r, const klVec2 &v);// �萔�{

  // ��r���Z�q
  bool operator == (const klVec2 &u) const;	// ��v
  bool operator != (const klVec2 &u) const;	// �s��v

  friend istream &operator>>(istream &i, klVec2 &v);
  friend ostream &operator<<(ostream &o, klVec2 v);

  static double epsilon;

protected:
  float f[2];
};
  
class klVec3
{
public:
  // �v�f�ɕʖ���^����
  union {
    double v[3];
    struct {
      double x, y, z;
    };
    struct {
      double r, g, b;
    };
  };
  //[]�ł̎Q�Ƃ��\�ɂ���
  double & operator[](int const &i) {
	  return v[i];
  }

  // double �ւ̃L���X�g - ����ɂ�� glVertex3dv(v); �ȂǂƏ�����
  operator double *() {
    return v;
  };
  // float �ւ̃L���X�g - ����ɂ�� glVertex3fv(v); �ȂǂƏ�����
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1], f[2] = (float)v[2];
    return f;
  };

  //---------------- �R���X�g���N�^
  klVec3();
  klVec3(const klVec3 &);
  klVec3(const double r[3]);
  klVec3(const float r[3]);
  klVec3(const double x, const double y, const double z);

  //---------------- ���g���ω����郁�\�b�h�i�Ԓl�͎������g�̎Q�Ɓj
  klVec3& Normalize();				// �傫�����K��
  klVec3& Clamp();				// ������[0-1]�ɃN�����v
  klVec3& Rotate(const double degree,
                 const double vx,
                 const double vy,
                 const double vz);	// ��]
  klVec3& Rotate(const double degree, const klVec3 &v);// ��]
  klVec3& Translate(const double x,
                    const double y,
                    const double z);	// ���i
  klVec3& Translate(const klVec3 &v);	// ���i
  klVec3& Scale(const double x, const double y, const double z);
  klVec3& Scale(const klVec3 &v);
  
  // ������Z�q
  klVec3& operator =  (const klVec3 &u);	// ���
  klVec3& operator += (const klVec3 &u);	// �a
  klVec3& operator -= (const klVec3 &u);	// ��
  klVec3& operator %= (const klVec3 &u);	// �O��
  klVec3& operator *= (const double r);		// �萔�{
  klVec3& operator /= (const double r);		// �萔�{

  //---------------- ���g���ω����Ȃ����\�b�h
  double Abs() const;				// ��Βl
  double Length() const;			// ����
  double Angle(const klVec3 &) const;		// ���̃x�N�g���Ɛ����p
  double Distance(const klVec3 &) const;	// ���̃x�N�g���Ƃ̋���
  double DistToLine(const klVec3 &,
                    const klVec3 &) const;	// 2�_�����������Ƃ̋���
  bool OnLine(const klVec3 &,
              const klVec3 &) const;		// 2�_������������ɂ��邩
  klVec3 ProjToLine(const klVec3 &,
                    const klVec3 &) const;	// 2�_�����������։��낵����
  double DistToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3�_���������ʂƂ̋���
  bool OnPlane(const klVec3 &,
               const klVec3 &,
               const klVec3 &) const;		// 3�_���������ʏ�ɂ��邩
  klVec3 ProjToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3�_���������ʂ։��낵����

  // �P�����Z�q
  klVec3 operator +() const;
  klVec3 operator -() const;

  // 2�����Z�q 
  double operator * (const klVec3 &u) const;	// ����
  double operator * (const klVec4 &u) const;	// ����
  klVec3 operator + (const klVec3 &u) const;	// �a
  klVec3 operator + (const klVec4 &u) const;	// �a
  klVec3 operator - (const klVec3 &u) const;	// ��
  klVec3 operator - (const klVec4 &u) const;	// ��
  klVec3 operator % (const klVec3 &u) const;	// �O��
  klVec3 operator % (const klVec4 &u) const;	// �O��
  klVec3 operator * (const double r) const;	// �萔�{
  klVec3 operator / (const double r) const;	// �萔�{

  friend klVec3 operator * (const double r, const klVec3 &v);// �萔�{

  // ��r���Z�q
  bool operator == (const klVec3 &u) const;	// ��v
  bool operator != (const klVec3 &u) const;	// �s��v

  friend istream &operator>>(istream &i, klVec3 &v);
  friend ostream &operator<<(ostream &o, klVec3 v);

  static double epsilon;

protected:
  float f[3];
};
  
class klVec4
{
public:
  union {
    double v[4];
    struct {
      double x, y, z, w;
    };
    struct {
      double r, g, b, a;
    };
  };
  //[]�ł̎Q�Ƃ��\�ɂ���
  double & operator[](int const &i) {
	  return v[i];
  }

  // double �ւ̃L���X�g - ����ɂ�� glVertex3dv(v); �ȂǂƏ�����
  operator double *() {
    return v;
  };
  // float �ւ̃L���X�g - ����ɂ�� glVertex3fv(v); �ȂǂƏ�����
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1], f[2] = (float)v[2], f[3] = (float)v[3];
    return f;
  };

  //---------------- �R���X�g���N�^
  klVec4();
  klVec4(const klVec3 &, const double w = 1.0);
  klVec4(const klVec4 &);
  klVec4(const double r[4]);
  klVec4(const double x, const double y, const double z, const double w = 1.0);

  //---------------- ���g���ω����郁�\�b�h
  klVec4& Normalize();				// �傫�����K��
  klVec4& Clamp();				// ������[0-1]�ɃN�����v
  klVec4& Rotate(const double degree,
                 const double vx,
                 const double vy,
                 const double vz);	// ��]
  klVec4& Rotate(const double degree, const klVec3 &v);// ��]
  klVec4& Rotate(const double degree, const klVec4 &v);// ��]
  klVec4& Translate(const double x,
                    const double y,
                    const double z);	// ���i
  klVec4& Translate(const klVec3 &v);	// ���i
  klVec4& Translate(const klVec4 &v);	// ���i
  klVec4& Scale(const double x, const double y, const double z);
  klVec4& Scale(const klVec3 &v);
  klVec4& Scale(const klVec4 &v);
  
  // ������Z�q
  klVec4& operator =  (const klVec3 &u);	// ���; w = 1.0
  klVec4& operator =  (const klVec4 &u);	// ���
  klVec4& operator += (const klVec3 &u);	// �a
  klVec4& operator += (const klVec4 &u);	// �a
  klVec4& operator -= (const klVec3 &u);	// ��
  klVec4& operator -= (const klVec4 &u);	// ��
  klVec4& operator %= (const klVec3 &u);	// �O��
  klVec4& operator %= (const klVec4 &u);	// �O��
  klVec4& operator *= (const double r);		// �萔�{
  klVec4& operator /= (const double r);		// �萔�{

  //---------------- ���g���ω����Ȃ����\�b�h
  double Abs() const;				// ��Βl
  double Length() const;			// ����
  double Angle(const klVec3 &) const;		// ���̃x�N�g���Ɛ����p
  double Angle(const klVec4 &) const;		// ���̃x�N�g���Ɛ����p
  double Distance(const klVec3 &) const;	// ���̃x�N�g���Ƃ̋���
  double Distance(const klVec4 &) const;	// ���̃x�N�g���Ƃ̋���
  double DistToLine(const klVec3 &,
                    const klVec3 &) const;	// 2�_�����������Ƃ̋���
  double DistToLine(const klVec4 &,
                    const klVec4 &) const;	// 2�_�����������Ƃ̋���
  bool OnLine(const klVec3 &,
              const klVec3 &) const;		// 2�_������������ɂ��邩
  bool OnLine(const klVec4 &,
              const klVec4 &) const;		// 2�_������������ɂ��邩
  klVec4 ProjToLine(const klVec3 &,
                    const klVec3 &) const;	// 2�_�����������։��낵����
  klVec4 ProjToLine(const klVec4 &,
                    const klVec4 &) const;	// 2�_�����������։��낵����
  double DistToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3�_���������ʂƂ̋���
  double DistToPlane(const klVec4 &,
                     const klVec4 &,
                     const klVec4 &) const;	// 3�_���������ʂƂ̋���
  bool OnPlane(const klVec3 &,
               const klVec3 &,
               const klVec3 &) const;		// 3�_���������ʏ�ɂ��邩
  bool OnPlane(const klVec4 &,
               const klVec4 &,
               const klVec4 &) const;		// 3�_���������ʏ�ɂ��邩
  klVec4 ProjToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3�_���������ʂ։��낵����
  klVec4 ProjToPlane(const klVec4 &,
                     const klVec4 &,
                     const klVec4 &) const;	// 3�_���������ʂ։��낵����

  // �P�����Z�q
  klVec4 operator +() const;
  klVec4 operator -() const;

  // 2�����Z�q 
  double operator * (const klVec3 &u) const;	// ����
  double operator * (const klVec4 &u) const;	// ����
  klVec4 operator + (const klVec3 &u) const;	// �a
  klVec4 operator + (const klVec4 &u) const;	// �a
  klVec4 operator - (const klVec3 &u) const;	// ��
  klVec4 operator - (const klVec4 &u) const;	// ��
  klVec4 operator % (const klVec3 &u) const;	// �O��
  klVec4 operator % (const klVec4 &u) const;	// �O��
  klVec4 operator * (const double r) const;	// �萔�{
  klVec4 operator / (const double r) const;	// �萔�{

  friend klVec4 operator * (const double r, const klVec4 &v);// �萔�{

  // ��r���Z�q
  bool operator == (const klVec4 &u) const;	// ��v
  bool operator != (const klVec4 &u) const;	// �s��v

  friend istream &operator>>(istream &i, klVec4 &v);
  friend ostream &operator<<(ostream &o, klVec4 v);

  static double epsilon;

protected:
  float f[4];
};

} //namespace KlMath
  
#endif
