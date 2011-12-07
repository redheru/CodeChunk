//-*-c++-*-
#ifndef _klMatrix_h_
#define _klMatrix_h_

#include "klVector.h"

namespace KlMath
{

//	y/x 0     1     2      3
//	0 m[0], m[4], m[8],  m[12],         +----------+ X
//	1 m[1], m[5], m[9],  m[13],   <==>  | Rotation | Y
//	2 m[2], m[6], m[10], m[14],         +----------+ Z
//	3 m[3], m[7], m[11], m[15]           0   0   0   1

// m[11], m[12] m[13] m[14]
// m[21], m[22], ...
// m[31], ...
// m[41], ...

// M = TRz(x)Ry(y)Rx(z)
//
//      [ cos x -sin x 0 0 ][ cos y 0 sin y 0 ][ 1     0      0 0 ]
//   = T[ sin x  cos x 0 0 ][     0 1     0 0 ][ 0 cos z -sin z 0 ]
//      [     0      0 1 0 ][-sin y 0 cos y 0 ][ 0 sin z  cos z 0 ]
//      [     0      0 0 1 ][     0 0     0 1 ][ 0     0      0 1 ]
//
//      [ cx*cy  -sx*cz + cx*sy*sz   sx*sz + cx*sy*cz  0 ]
//   = T[ sx*cy   cx*cz + sx*sy*sz  -cx*sz + sx*sy*cz  0 ]
//      [   -sy              cy*sz              cy*cz  0 ]
//      [     0                  0                  0  1 ]
//
// cy = 0 �̂Ƃ�,�����s��ɂȂ�̂� z = 0 (cz = 1, sz = 0) �ƌ��߂���D

//class klQuaternion;
class klMat44
{
 public:
  // �v�f�ɕʖ���^����
  union {
    double m[16];
    struct { // �s�E�񂪋t�Ȃ��Ƃɒ���
      double rxx, rxy, rxz, rxw;
      double ryx, ryy, ryz, ryw;
      double rzx, rzy, rzz, rzw;
      double x,  y,  z,  w;
    };
    struct { // �s�E�񂪋t�Ȃ��Ƃɒ���
      double m11, m21, m31, m41;
      double m12, m22, m32, m42;
      double m13, m23, m33, m43;
      double m14, m24, m34, m44;
    };
  };

  // double �ւ̃L���X�g - ����ɂ�� glMultMatrixd(m); �ȂǂƏ�����
  operator double *() {
    return m;
  };
  // float �ւ̃L���X�g - ����ɂ�� glMultMatrixf(m); �ȂǂƏ�����
  operator float *() {
    for (int i=0;i<16;i++) f[i] = (float)m[i];
    return f;
  };

  // �R���X�g���N�^
  klMat44 ();
  klMat44 (const klMat44 &M);
  klMat44 (const double m[16]);
  klMat44 (const double r0,const double r1,const double r2,const double r3,
            const double r4,const double r5,const double r6,const double r7,
            const double r8,const double r9,const double rA,const double rB,
            const double rC,const double rD,const double rE,const double rF);
  klMat44 (const klVec3 &rx, const klVec3 &ry, const klVec3 &rz, const klVec3 &t);
  klMat44 (const klVec4 &rx, const klVec4 &ry, const klVec4 &rz, const klVec4 &t);
//  klMat44 (const klQuaternion &q, const klVec3 &pos);
//  klMat44 (const klQuaternion &q, const klVec4 &pos);

  //=======================================================================
  // �������ω����郁�\�b�h
  //=======================================================================
  // ������
  klMat44& Clear();			// ��s��ɂ���
  klMat44& LoadIdentity();		// �P�ʍs��ɂ���
  klMat44& NormalizeW();		// �����s��Ƃ��Đ��K��
  klMat44& SetPos   (const klMat44 &m);
  klMat44& SetPos   (const klVec3  &v);
  klMat44& SetPos   (const klVec4  &v);
  klMat44& SetPos   (double  x, double y, double z);

  klMat44& SetRot   (const klMat44 &m);
  klMat44& SetRot   (const klVec3  &v);			// �x
  klMat44& SetRot   (const klVec4  &v);			// �x
  klMat44& SetRot   (double  x, double y, double z);	// �x
//  klMat44& SetRot   (klQuaternion&);

  klMat44& Rotate(const double degree,
                  const double vx,
                  const double vy,
                  const double vz);	// ��]
  klMat44& Rotate(const double degree, const klVec3 &v);// ��]
  klMat44& Rotate(const double degree, const klVec4 &v);// ��]
  klMat44& Translate(const double x,
                     const double y,
                     const double z);	// ���i
  klMat44& Translate(const klVec3 &v);	// ���i
  klMat44& Translate(const klVec4 &v);	// ���i
  klMat44& Scale(const double x, const double y, const double z);
  klMat44& Scale(const klVec3 &v);
  klMat44& Scale(const klVec4 &v);

  // ������Z�q
  klMat44& operator  = (const klMat44 &m);	// ���
  klMat44& operator  = (const double m[16]);	// ���
  klMat44& operator += (const klMat44 &m);	// �a
  klMat44& operator += (const double m[16]);	// �a
  klMat44& operator -= (const klMat44 &m);	// ��
  klMat44& operator -= (const double m[16]);	// ��
  klMat44& operator *= (const klMat44 &m);	// ��
  klMat44& operator *= (const double m[16]);	// ��
  klMat44& operator *= (const double r);	// ��
  klMat44& operator /= (const double r);	// ��
  klMat44& operator ^= (const int i);		// �p��

  //=======================================================================
  // �������ω����Ȃ����\�b�h
  //=======================================================================
  // ��r���Z�q
  bool operator == (const klMat44 &m) const;	// ��r�i�m��j
  bool operator == (const double m[16]) const;	// ��r�i�m��j
  bool operator != (const klMat44 &m) const;	// ��r�i�ے�j
  bool operator != (const double m[16]) const;	// ��r�i�ے�j

  // �񍀉��Z�q
  klMat44 operator + (const klMat44 &m) const;	// �a
  klMat44 operator + (const double m[16]) const;// �a
  klMat44 operator - (const klMat44 &m) const;	// ��
  klMat44 operator - (const double m[16]) const;// ��
  klMat44 operator * (const klMat44 &m) const;	// ��
  klMat44 operator * (const double m[16]) const;// ��
  klVec3  operator * (const klVec3 &v) const;	// ��
  klVec4  operator * (const klVec4 &v) const;	// ��
  klMat44 operator * (const double r) const;	// ��
  klMat44 operator / (const double r) const;	// ��
  klMat44 operator ^ (const int i) const;	// �p��

  friend klMat44 operator * (const double m1[16], const klMat44 &m2); // ��
  friend klMat44 operator * (const double r, const klMat44 &m); // ��

  klMat44 operator ! () const;			// �t�s��
  klMat44 operator ~ () const;			// �]�u�s��

  double Det () const;				// �s��

  // ���s�ړ�����, X, Y, Z �����̉�]�p
  double A() const { return (a()); };
  double E() const { return (m[2] * m[2] == 1)
                       ? ((m[2] == 1) ? -90.0 : 90.0)
                       : ((cos(a()) != 0)
                          ? atan2(-m[2], m[0] / cos(a()))
                          : atan2(-m[2], m[1] / sin(a()))) ; };
  double R() const { return (m[2] * m[2] == 1)
                       ? 0
                       : atan2(m[6], m[10]) ; };

// kiyokiyo
  // ���s�ړ�����, ��]���� �݂̂��܂񂾍s���Ԃ�
  klMat44 Pos() const;
  klMat44 Rot() const;

  // �l�̎擾�i�����ɒl���������ށD�Ԃ�l�� *this�j
  // �ł��C�������g�͕ω����Ȃ�����const
  klMat44 GetPos   (klMat44 &m) const;
  klMat44 GetPos   (klVec3  &v) const;
  klMat44 GetPos   (klVec4  &v) const;
  klMat44 GetPos   (double  &x, double &y, double &z) const;
  klVec3  GetPos   () const;
  klVec4  GetPos4  () const;

  klMat44 GetRot   (klMat44 &m) const;
  klMat44 GetRot   (klVec3  &v) const;
  klMat44 GetRot   (klVec4  &v) const;
  klMat44 GetRot   (double  &x, double &y, double &z) const;
  klMat44 GetEulerAngle   (double  &x, double &y, double &z) const;
  klMat44 GetEulerAngle(float &x, float &y, float &z) const;
//  klMat44 GetRot   (klQuaternion&) const;

  friend istream &operator >> (istream &i, klMat44 &m);
  friend ostream &operator << (ostream &o, const klMat44 &m);

  static double epsilon;

protected:
  float f[16];
  double a() const {
    return (m[2] * m[2] == 1)
      ? atan2(-m[4], m[5])
        : atan2( m[1], m[0]);
  };
};

//---------------------------------------------------------------------------//
//	End of klMat44.h
//---------------------------------------------------------------------------//
} // namespace KlMath

#endif
