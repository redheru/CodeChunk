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
// klVec2 クラス
// 概要: 二次元ベクトルのクラス
//-------------------------------------------------------------------
class klVec2
{
public:
  // 要素に別名を与える
  union {
    double v[2];
	struct {
      double x, y;
    };
  };
  //[]での参照を可能にする
  double & operator[](int const &i) {
	  return v[i];
  }

  //共用体を使っている
  //double v[2]とdouble x,y に同じ領域が割り当てられる
  //つまり２つの違う表わし方ができる
  
  //---------------- コンストラクタ
  klVec2();
  klVec2(const klVec2 &);
  klVec2(const double r[2]);
  klVec2(const double x, const double y);

  // double* へのキャスト - これにより glVertex3dv(v); などと書ける
  operator double *() {
    return v;
  };
  // float* へのキャスト - これにより glVertex3fv(v); などと書ける
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1];
    return f;
  };

  //---------------- 自身が変化するメソッド（返値は自分自身の参照）
  klVec2& Normalize();				// 大きさ正規化
  klVec2& Clamp();				// 成分を[0-1]にクランプ
  klVec2& Rotate(const double degree);		// 回転
  klVec2& Translate(const double x, const double y);	// 並進
  
  // 代入演算子
  klVec2& operator =  (const klVec2 &u);	// 代入
  klVec2& operator += (const klVec2 &u);	// 和
  klVec2& operator -= (const klVec2 &u);	// 差
  klVec2& operator *= (const double r);		// 定数倍
  klVec2& operator /= (const double r);		// 定数倍

  //---------------- 自身が変化しないメソッド
  double Abs() const;				// 絶対値
  double Length() const;			// 長さ
  double Angle(const klVec2 &) const;		// 他のベクトルと成す角
  double Distance(const klVec2 &) const;	// 他のベクトルとの距離

  // 単項演算子
  klVec2 operator +() const;
  klVec2 operator -() const;

  // 2項演算子 
  double operator * (const klVec2 &u) const;	// 内積
  klVec2 operator + (const klVec2 &u) const;	// 和
  klVec2 operator - (const klVec2 &u) const;	// 差
  klVec2 operator * (const double r) const;	// 定数倍
  klVec2 operator / (const double r) const;	// 定数倍

  // double が前に来る2項演算子
  friend klVec2 operator * (const double r, const klVec2 &v);// 定数倍

  // 比較演算子
  bool operator == (const klVec2 &u) const;	// 一致
  bool operator != (const klVec2 &u) const;	// 不一致

  friend istream &operator>>(istream &i, klVec2 &v);
  friend ostream &operator<<(ostream &o, klVec2 v);

  static double epsilon;

protected:
  float f[2];
};
  
class klVec3
{
public:
  // 要素に別名を与える
  union {
    double v[3];
    struct {
      double x, y, z;
    };
    struct {
      double r, g, b;
    };
  };
  //[]での参照を可能にする
  double & operator[](int const &i) {
	  return v[i];
  }

  // double へのキャスト - これにより glVertex3dv(v); などと書ける
  operator double *() {
    return v;
  };
  // float へのキャスト - これにより glVertex3fv(v); などと書ける
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1], f[2] = (float)v[2];
    return f;
  };

  //---------------- コンストラクタ
  klVec3();
  klVec3(const klVec3 &);
  klVec3(const double r[3]);
  klVec3(const float r[3]);
  klVec3(const double x, const double y, const double z);

  //---------------- 自身が変化するメソッド（返値は自分自身の参照）
  klVec3& Normalize();				// 大きさ正規化
  klVec3& Clamp();				// 成分を[0-1]にクランプ
  klVec3& Rotate(const double degree,
                 const double vx,
                 const double vy,
                 const double vz);	// 回転
  klVec3& Rotate(const double degree, const klVec3 &v);// 回転
  klVec3& Translate(const double x,
                    const double y,
                    const double z);	// 並進
  klVec3& Translate(const klVec3 &v);	// 並進
  klVec3& Scale(const double x, const double y, const double z);
  klVec3& Scale(const klVec3 &v);
  
  // 代入演算子
  klVec3& operator =  (const klVec3 &u);	// 代入
  klVec3& operator += (const klVec3 &u);	// 和
  klVec3& operator -= (const klVec3 &u);	// 差
  klVec3& operator %= (const klVec3 &u);	// 外積
  klVec3& operator *= (const double r);		// 定数倍
  klVec3& operator /= (const double r);		// 定数倍

  //---------------- 自身が変化しないメソッド
  double Abs() const;				// 絶対値
  double Length() const;			// 長さ
  double Angle(const klVec3 &) const;		// 他のベクトルと成す角
  double Distance(const klVec3 &) const;	// 他のベクトルとの距離
  double DistToLine(const klVec3 &,
                    const klVec3 &) const;	// 2点が成す直線との距離
  bool OnLine(const klVec3 &,
              const klVec3 &) const;		// 2点が成す直線上にあるか
  klVec3 ProjToLine(const klVec3 &,
                    const klVec3 &) const;	// 2点が成す直線へ下ろした足
  double DistToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3点が成す平面との距離
  bool OnPlane(const klVec3 &,
               const klVec3 &,
               const klVec3 &) const;		// 3点が成す平面上にあるか
  klVec3 ProjToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3点が成す平面へ下ろした足

  // 単項演算子
  klVec3 operator +() const;
  klVec3 operator -() const;

  // 2項演算子 
  double operator * (const klVec3 &u) const;	// 内積
  double operator * (const klVec4 &u) const;	// 内積
  klVec3 operator + (const klVec3 &u) const;	// 和
  klVec3 operator + (const klVec4 &u) const;	// 和
  klVec3 operator - (const klVec3 &u) const;	// 差
  klVec3 operator - (const klVec4 &u) const;	// 差
  klVec3 operator % (const klVec3 &u) const;	// 外積
  klVec3 operator % (const klVec4 &u) const;	// 外積
  klVec3 operator * (const double r) const;	// 定数倍
  klVec3 operator / (const double r) const;	// 定数倍

  friend klVec3 operator * (const double r, const klVec3 &v);// 定数倍

  // 比較演算子
  bool operator == (const klVec3 &u) const;	// 一致
  bool operator != (const klVec3 &u) const;	// 不一致

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
  //[]での参照を可能にする
  double & operator[](int const &i) {
	  return v[i];
  }

  // double へのキャスト - これにより glVertex3dv(v); などと書ける
  operator double *() {
    return v;
  };
  // float へのキャスト - これにより glVertex3fv(v); などと書ける
  operator float *() {
    f[0] = (float)v[0], f[1] = (float)v[1], f[2] = (float)v[2], f[3] = (float)v[3];
    return f;
  };

  //---------------- コンストラクタ
  klVec4();
  klVec4(const klVec3 &, const double w = 1.0);
  klVec4(const klVec4 &);
  klVec4(const double r[4]);
  klVec4(const double x, const double y, const double z, const double w = 1.0);

  //---------------- 自身が変化するメソッド
  klVec4& Normalize();				// 大きさ正規化
  klVec4& Clamp();				// 成分を[0-1]にクランプ
  klVec4& Rotate(const double degree,
                 const double vx,
                 const double vy,
                 const double vz);	// 回転
  klVec4& Rotate(const double degree, const klVec3 &v);// 回転
  klVec4& Rotate(const double degree, const klVec4 &v);// 回転
  klVec4& Translate(const double x,
                    const double y,
                    const double z);	// 並進
  klVec4& Translate(const klVec3 &v);	// 並進
  klVec4& Translate(const klVec4 &v);	// 並進
  klVec4& Scale(const double x, const double y, const double z);
  klVec4& Scale(const klVec3 &v);
  klVec4& Scale(const klVec4 &v);
  
  // 代入演算子
  klVec4& operator =  (const klVec3 &u);	// 代入; w = 1.0
  klVec4& operator =  (const klVec4 &u);	// 代入
  klVec4& operator += (const klVec3 &u);	// 和
  klVec4& operator += (const klVec4 &u);	// 和
  klVec4& operator -= (const klVec3 &u);	// 差
  klVec4& operator -= (const klVec4 &u);	// 差
  klVec4& operator %= (const klVec3 &u);	// 外積
  klVec4& operator %= (const klVec4 &u);	// 外積
  klVec4& operator *= (const double r);		// 定数倍
  klVec4& operator /= (const double r);		// 定数倍

  //---------------- 自身が変化しないメソッド
  double Abs() const;				// 絶対値
  double Length() const;			// 長さ
  double Angle(const klVec3 &) const;		// 他のベクトルと成す角
  double Angle(const klVec4 &) const;		// 他のベクトルと成す角
  double Distance(const klVec3 &) const;	// 他のベクトルとの距離
  double Distance(const klVec4 &) const;	// 他のベクトルとの距離
  double DistToLine(const klVec3 &,
                    const klVec3 &) const;	// 2点が成す直線との距離
  double DistToLine(const klVec4 &,
                    const klVec4 &) const;	// 2点が成す直線との距離
  bool OnLine(const klVec3 &,
              const klVec3 &) const;		// 2点が成す直線上にあるか
  bool OnLine(const klVec4 &,
              const klVec4 &) const;		// 2点が成す直線上にあるか
  klVec4 ProjToLine(const klVec3 &,
                    const klVec3 &) const;	// 2点が成す直線へ下ろした足
  klVec4 ProjToLine(const klVec4 &,
                    const klVec4 &) const;	// 2点が成す直線へ下ろした足
  double DistToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3点が成す平面との距離
  double DistToPlane(const klVec4 &,
                     const klVec4 &,
                     const klVec4 &) const;	// 3点が成す平面との距離
  bool OnPlane(const klVec3 &,
               const klVec3 &,
               const klVec3 &) const;		// 3点が成す平面上にあるか
  bool OnPlane(const klVec4 &,
               const klVec4 &,
               const klVec4 &) const;		// 3点が成す平面上にあるか
  klVec4 ProjToPlane(const klVec3 &,
                     const klVec3 &,
                     const klVec3 &) const;	// 3点が成す平面へ下ろした足
  klVec4 ProjToPlane(const klVec4 &,
                     const klVec4 &,
                     const klVec4 &) const;	// 3点が成す平面へ下ろした足

  // 単項演算子
  klVec4 operator +() const;
  klVec4 operator -() const;

  // 2項演算子 
  double operator * (const klVec3 &u) const;	// 内積
  double operator * (const klVec4 &u) const;	// 内積
  klVec4 operator + (const klVec3 &u) const;	// 和
  klVec4 operator + (const klVec4 &u) const;	// 和
  klVec4 operator - (const klVec3 &u) const;	// 差
  klVec4 operator - (const klVec4 &u) const;	// 差
  klVec4 operator % (const klVec3 &u) const;	// 外積
  klVec4 operator % (const klVec4 &u) const;	// 外積
  klVec4 operator * (const double r) const;	// 定数倍
  klVec4 operator / (const double r) const;	// 定数倍

  friend klVec4 operator * (const double r, const klVec4 &v);// 定数倍

  // 比較演算子
  bool operator == (const klVec4 &u) const;	// 一致
  bool operator != (const klVec4 &u) const;	// 不一致

  friend istream &operator>>(istream &i, klVec4 &v);
  friend ostream &operator<<(ostream &o, klVec4 v);

  static double epsilon;

protected:
  float f[4];
};

} //namespace KlMath
  
#endif
