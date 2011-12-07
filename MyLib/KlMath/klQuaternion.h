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
// klQuat クラス
// 概要: Quaternionのクラス
//-------------------------------------------------------------------
class klQuat
{
	public:
	//要素に別名を与える
	union {
		double q[4];
		struct { 
			double x, y, z, w;
		};
	};
  //共用体を使っている
  //double q[4]とdouble x,y,z,w に同じ領域が割り当てられる
  //つまり２つの違う表わし方ができる

  //[]での参照を可能にする
  double & operator[](int const &i) {
	  return q[i];
  }

  //---------------- コンストラクタ
  klQuat();
  klQuat(const klQuat &);
  klQuat(const klVec4 &);
  klQuat(const double r[4]);
  klQuat(const double x, const double y, const double z, const double w);
  klQuat(vector<double> vec);
  klQuat(const double theta, const klVec3 &axis);

  //---------------- 自身が変化するメソッド（返値は自分自身の参照）
  klQuat& Clear();//初期化
  klQuat& Normalize();				// 大きさ正規化
  klQuat& Clamp();				// 成分を[0-1]にクランプ

  ////Convertメソッド
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
                 const double vz);	// 回転
  klQuat& Rotate(const double degree, const klVec3 &v);// 回転
  klQuat& Rotate(const klVec3 &from, const klVec3 &to);// 回転


  //klVec4& Translate(const double x,
  //                  const double y,
  //                  const double z);	// 並進
  //klVec4& Translate(const klVec3 &v);	// 並進
  //klVec4& Translate(const klVec4 &v);	// 並進
  //klVec4& Scale(const double x, const double y, const double z);
  //klVec4& Scale(const klVec3 &v);
  //klVec4& Scale(const klVec4 &v);
  
  // 代入演算子
  //klVec4& operator =  (const klVec3 &u);	// 代入; w = 1.0
  klQuat& operator =  (const klQuat &u);	// 代入
  klQuat& operator +=  (const klQuat &u); // 和
  klQuat& operator *=  (const klQuat &u); // 積
  klQuat& operator *=  (const double &u); // 定数倍
  klQuat& operator /=  (const double &u); // 定数倍

  //klVec4& operator += (const klVec3 &u);	// 和
  //klVec4& operator += (const klVec4 &u);	// 和
  //klVec4& operator -= (const klVec3 &u);	// 差
  //klVec4& operator -= (const klVec4 &u);	// 差
  //klVec4& operator %= (const klVec3 &u);	// 外積
  //klVec4& operator %= (const klVec4 &u);	// 外積
  //klVec4& operator *= (const double r);		// 定数倍
  //klVec4& operator /= (const double r);		// 定数倍

  //---------------- 自身が変化しないメソッド
  klQuat Abs() const;//絶対値
  klQuat Conjugate() const;//共役
  double Norm() const;//ノルム
  klQuat Inverse() const;//逆クォータニオン
  double GetYawAngle() const;//Yaw
  double GetRoll() const;//Roll
  double GetPitch() const;//Pitch
  double GetYaw() const;//Yaw

  ////Convertメソッド
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

  ////回転メソッド
  //Compute vector rotated by quaternion
  klVec3 Rotate(const klVec3 &v) const;

  ////補間メソッド
  klQuat Lerp(const klQuat &a, const klQuat & b, const double amt) const;// 線形補間

  //double Abs() const;				// 絶対値
  //double Length() const;			// 長さ
  //double Angle(const klVec3 &) const;		// 他のベクトルと成す角
  //double Angle(const klVec4 &) const;		// 他のベクトルと成す角
  //double Distance(const klVec3 &) const;	// 他のベクトルとの距離
  //double Distance(const klVec4 &) const;	// 他のベクトルとの距離
  //double DistToLine(const klVec3 &,
  //                  const klVec3 &) const;	// 2点が成す直線との距離
  //double DistToLine(const klVec4 &,
  //                  const klVec4 &) const;	// 2点が成す直線との距離
  //bool OnLine(const klVec3 &,
  //            const klVec3 &) const;		// 2点が成す直線上にあるか
  //bool OnLine(const klVec4 &,
  //            const klVec4 &) const;		// 2点が成す直線上にあるか
  //klVec4 ProjToLine(const klVec3 &,
  //                  const klVec3 &) const;	// 2点が成す直線へ下ろした足
  //klVec4 ProjToLine(const klVec4 &,
  //                  const klVec4 &) const;	// 2点が成す直線へ下ろした足
  //double DistToPlane(const klVec3 &,
  //                   const klVec3 &,
  //                   const klVec3 &) const;	// 3点が成す平面との距離
  //double DistToPlane(const klVec4 &,
  //                   const klVec4 &,
  //                   const klVec4 &) const;	// 3点が成す平面との距離
  //bool OnPlane(const klVec3 &,
  //             const klVec3 &,
  //             const klVec3 &) const;		// 3点が成す平面上にあるか
  //bool OnPlane(const klVec4 &,
  //             const klVec4 &,
  //             const klVec4 &) const;		// 3点が成す平面上にあるか
  //klVec4 ProjToPlane(const klVec3 &,
  //                   const klVec3 &,
  //                   const klVec3 &) const;	// 3点が成す平面へ下ろした足
  //klVec4 ProjToPlane(const klVec4 &,
  //                   const klVec4 &,
  //                   const klVec4 &) const;	// 3点が成す平面へ下ろした足

  // 単項演算子
  klQuat operator !() const;	// 逆元 
  klQuat operator ~() const;	// 共役

  // 2項演算子 
  klQuat operator * (const klQuat &u) const; //積
  klQuat operator * (const double &u) const; //定数倍
  klQuat operator / (const double &u) const; //定数倍
  klQuat operator + (const klQuat &u) const; //和

  //double operator * (const klVec3 &u) const;	// 内積
  //double operator * (const klVec4 &u) const;	// 内積
  //klVec4 operator + (const klVec3 &u) const;	// 和
  //klVec4 operator + (const klVec4 &u) const;	// 和
  //klVec4 operator - (const klVec3 &u) const;	// 差
  //klVec4 operator - (const klVec4 &u) const;	// 差
  //klVec4 operator % (const klVec3 &u) const;	// 外積
  //klVec4 operator % (const klVec4 &u) const;	// 外積
  //klVec4 operator * (const double r) const;	// 定数倍
  //klVec4 operator / (const double r) const;	// 定数倍

  //friend klVec4 operator * (const double r, const klVec4 &v);// 定数倍

  // 比較演算子
  bool operator == (const klQuat &u) const;	// 一致
  bool operator != (const klQuat &u) const;	// 不一致

  friend istream &operator>>(istream &i, klQuat &v);
  friend ostream &operator<<(ostream &o, klQuat v);

  static double epsilon;

  //---------------- 静的関数
public:

  //Convert関数

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
