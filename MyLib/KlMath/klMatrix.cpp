//-*-c++-*-
#include "klMatrix.h"

namespace KlMath
{

double klMat44::epsilon = 1e-12;
//---------------------------------------------------------------------------//
//  処理   : コンストラクタ
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44::klMat44()
{
  LoadIdentity();
}

klMat44::klMat44(const klMat44 &M)
{
  (*this) = M;
}

klMat44::klMat44(const double src[16])
{
  (*this) = src;
}

klMat44::klMat44
(const double r0,const double r1,const double r2,const double r3,
 const double r4,const double r5,const double r6,const double r7,
 const double r8,const double r9,const double rA,const double rB,
 const double rC,const double rD,const double rE,const double rF)
{
  m[ 0] = r0, m[ 1] = r1, m[ 2] = r2, m[ 3] = r3;
  m[ 4] = r4, m[ 5] = r5, m[ 6] = r6, m[ 7] = r7;
  m[ 8] = r8, m[ 9] = r9, m[10] = rA, m[11] = rB;
  m[12] = rC, m[13] = rD, m[14] = rE, m[15] = rF;
}

klMat44::klMat44
(const klVec3 &rx, const klVec3 &ry, const klVec3 &rz, const klVec3 &t)
{
  rxx = rx.x, rxy = rx.y, rxz = rx.z, rxw = 0.0;
  ryx = ry.x, ryy = ry.y, ryz = ry.z, ryw = 0.0;
  rzx = rz.x, rzy = rz.y, rzz = rz.z, rzw = 0.0;
  x   = t.x,  y   = t.y,  z   = t.z,  w   = 1.0;
}

klMat44::klMat44
(const klVec4 &rx, const klVec4 &ry, const klVec4 &rz, const klVec4 &t)
{
  rxx = rx.x, rxy = rx.y, rxz = rx.z, rxw = rx.w;
  ryx = ry.x, ryy = ry.y, ryz = ry.z, ryw = ry.w;
  rzx = rz.x, rzy = rz.y, rzz = rz.z, rzw = rz.w;
  x   = t.x,  y   = t.y,  z   = t.z,  w   = t.w;
}

//---------------------------------------------------------------------------//
//  処理   : 初期化など
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44& klMat44::Clear()
{
  (*this) = klMat44(0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0);
  return *this;
}

klMat44& klMat44::LoadIdentity()
{
  (*this) = klMat44(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
  return *this;
}

klMat44& klMat44::NormalizeW()
{
  if (w == 0.0) return *this;
  if (w != 1.0){
    for (int i = 0; i < 15; i++) m[i] /= w;
    w = 1.0;
  }
  return *this;
}

klMat44& klMat44::SetPos(const klMat44 &M)
{
  x = M.x, y = M.x, z = M.z;
  return *this;
}

klMat44& klMat44::SetPos(const klVec3 &v)
{
  x = v.x, y = v.y, z = v.z;
  return *this;
}

klMat44& klMat44::SetPos(const klVec4 &v)
{
  x = v.x, y = v.y, z = v.z;
  return *this;
}

klMat44& klMat44::SetPos(double _x, double _y, double _z)
{
  x = _x, y = _y, z = _z;
  return *this;
}

klMat44& klMat44::SetRot(const klMat44 &M)
{
  rxx = M.rxx, rxy = M.rxy, rxz = M.rxz;
  ryx = M.ryx, ryy = M.ryy, ryz = M.ryz;
  rzx = M.rzx, rzy = M.rzy, rzz = M.rzz;
  return *this;
}

klMat44& klMat44::SetRot(const klVec3 &v)
{
  return SetRot(v.x, v.y, v.z);
}

klMat44& klMat44::SetRot(const klVec4 &v)
{
  return SetRot(v.x, v.y, v.z);
}

klMat44& klMat44::SetRot(const double x, const double y, const double z)
{
  double cx = cos(x * DEG_TO_RAD);
  double sx = sin(x * DEG_TO_RAD);
  double cy = cos(y * DEG_TO_RAD);
  double sy = sin(y * DEG_TO_RAD);
  double cz = cos(z * DEG_TO_RAD);
  double sz = sin(z * DEG_TO_RAD);
  
  m[0] =  cx * cy;
  m[1] =  sx * cy;
  m[2] =      -sy;

  m[4] = -sx * cz + cx * sy * sz;
  m[5] =  cx * cz + sx * sy * sz;
  m[6] =  cy * sz;

  m[8] =  sx * sz + cx * sy * cz;
  m[9] = -cx * sz + sx * sy * cz;
  m[10]=  cy * cz;

  //m[3] = m[7] = m[11] = 0;
  //m[15] = 1;

  return *this;
}

/*
klMat44& klMat44::SetRot(const klQuaternion& q)
{
  double xx = q.q[0] * q.q[0];
  double xy = q.q[0] * q.q[1];
  double xz = q.q[0] * q.q[2];
  double xw = q.q[0] * q.q[3];
  
  double yy = q.q[1] * q.q[1];
  double yz = q.q[1] * q.q[2];
  double yw = q.q[1] * q.q[3];

  double zz = q.q[2] * q.q[2];
  double zw = q.q[2] * q.q[3];
  
  m[0] = 1.0 - 2.0 * (yy + zz);
  m[1] =       2.0 * (xy - zw);
  m[2] =       2.0 * (xz + yw);

  m[4] =       2.0 * (xy + zw); 
  m[5] = 1.0 - 2.0 * (xx + zz);
  m[6] =       2.0 * (yz - xw);
  
  m[8] =       2.0 * (xz - yw);
  m[9] =       2.0 * (yz + xw);
  m[10]= 1.0 - 2.0 * (xx + yy);
  return *this;
}
*/

klMat44& klMat44::Rotate(const double degree,
                         const double _vx, const double _vy, const double _vz)
{
  double len = sqrt(_vx*_vx + _vy*_vy + _vz*_vz);
  double vx = _vx / len;
  double vy = _vy / len;
  double vz = _vz / len;
  double c  = cos(degree * DEG_TO_RAD);
  double s  = sin(degree * DEG_TO_RAD);

  klMat44 tmp;
  tmp.m[0] = vx*vx*(1-c) + c;
  tmp.m[1] = vy*vx*(1-c) + vz*s;
  tmp.m[2] = vz*vx*(1-c) - vy*s;

  tmp.m[4] = vx*vy*(1-c) - vz*s;
  tmp.m[5] = vy*vy*(1-c) + c;
  tmp.m[6] = vz*vy*(1-c) + vx*s;

  tmp.m[8] = vx*vz*(1-c) + vy*s;
  tmp.m[9] = vy*vz*(1-c) - vx*s;
  tmp.m[10]= vz*vz*(1-c) + c;

  return *this = (*this) * tmp;
}

klMat44& klMat44::Rotate(const double degree, const klVec3 &v)
{
  return Rotate(degree, v.x, v.y, v.z);
}

klMat44& klMat44::Rotate(const double degree, const klVec4 &v)
{
  return Rotate(degree, v.x, v.y, v.z);
}

klMat44& klMat44::Translate(const double _x, const double _y, const double _z)
{
  klMat44 tmp;
  tmp.x = _x, tmp.y = _y, tmp.z = _z;
  return *this = (*this) * tmp;
}

klMat44& klMat44::Translate(const klVec3 &v)
{
  return Translate(v.x, v.y, v.z);
}

klMat44& klMat44::Translate(const klVec4 &v)
{
  return Translate(v.x, v.y, v.z);
}

klMat44& klMat44::Scale(const double _x, const double _y, const double _z)
{
  klMat44 tmp;
  tmp.m[0] = _x, tmp.m[5] = _y, tmp.m[10] = _z;
  return *this = (*this) * tmp;
}

klMat44& klMat44::Scale(const klVec3 &v)
{
  return Scale(v.x, v.y, v.z);
}

klMat44& klMat44::Scale(const klVec4 &v)
{
  return Scale(v.x, v.y, v.z);
}

//---------------------------------------------------------------------------//
//  処理   : 代入演算子
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44& klMat44::operator = (const klMat44 &src)
{
  return (*this = (double*)src.m);
}

klMat44& klMat44::operator = (const double src[16])
{
  for (int i=0;i<16;i++) m[i] = src[i];
  return *this;
}

klMat44& klMat44::operator += (const klMat44& src)
{
  return (*this += (double*)src.m);
}

klMat44& klMat44::operator += (const double src[16])
{
  for (int i=0;i<16;i++) m[i] += src[i];
  return *this;
}

klMat44& klMat44::operator -= (const klMat44& src)
{
  return (*this -= (double*)src.m);
}

klMat44& klMat44::operator -= (const double src[16])
{
  for (int i=0;i<16;i++) m[i] -= src[i];
  return *this;
}

klMat44& klMat44::operator *= (const klMat44& src)
{
  return (*this *= (double*)src.m);
}

klMat44& klMat44::operator *= (const double src[16])
{
  int x, y;
  double tmp[16];
  for (x = 0; x <= 3; x++){
    for (y = 0; y <= 3; y++){
      tmp[4*x + y] =
        m[ 0 + y] * src[4*x + 0] +
        m[ 4 + y] * src[4*x + 1] +
        m[ 8 + y] * src[4*x + 2] +
        m[12 + y] * src[4*x + 3];
    }
  }
  return (*this = tmp);
}

klMat44& klMat44::operator *= (const double r)
{
  for (int i=0;i<16;i++) m[i] *= r;
  return *this;
}

klMat44& klMat44::operator /= (const double r)
{
  for (int i=0;i<16;i++) m[i] /= r;
  return *this;
}

klMat44& klMat44::operator ^= (const int i)
{
  if (i > 0){
    klMat44 tmp = *this;
    for (int j=i;j>1;j--) *this = *this * tmp;
  } else if (i < 0){
    *this = !(*this);
    klMat44 tmp = *this;
    for (int j=i;j<-1;j++) *this = *this * tmp;
  } else {
    LoadIdentity();
  }
  return *this;
}

//---------------------------------------------------------------------------//
//  処理   : 比較演算子
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
bool klMat44::operator == (const klMat44& src) const
{
  return (*this == (double*)src.m);
}

bool klMat44::operator == (const double n[16]) const
{
  for (int i=0;i<16;i++){
    if (abs(m[i] - n[i]) >= epsilon) return false;
  }
  return true;
}

bool klMat44::operator != (const klMat44& src) const
{
  return !(*this == src.m);
}

bool klMat44::operator != (const double src[16]) const
{
  return !(*this == src);
}

//---------------------------------------------------------------------------//
//  処理   : 二項演算子
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44 klMat44::operator + (const klMat44& src) const
{
  return (*this + (double*)src.m);
}

klMat44 klMat44::operator + (const double src[16]) const
{
  klMat44 tmp(*this);
  return tmp += src;
}

klMat44 klMat44::operator - (const klMat44& src) const
{
  return (*this - (double*)src.m);
}

klMat44 klMat44::operator - (const double src[16]) const
{
  klMat44 tmp(*this);
  return tmp -= src;
}

klMat44 klMat44::operator * (const klMat44& src) const
{
  return (*this * (double*)src.m);
}

klMat44 klMat44::operator * (const double src[16]) const
{
  int x, y;
  klMat44 tmp;
  for (x = 0; x <= 3; x++){
    for (y = 0; y <= 3; y++){
      tmp.m[4*x + y] =
        m[ 0 + y] * src[4*x + 0] +
        m[ 4 + y] * src[4*x + 1] +
        m[ 8 + y] * src[4*x + 2] +
        m[12 + y] * src[4*x + 3];
    }
  }
  return tmp;
}

klMat44 klMat44::operator * (const double r) const
{
  klMat44 tmp(*this);
  return tmp *= r;
}

klVec3 klMat44::operator * (const klVec3 &v) const
{
  klVec3 tmp(m[0]*v.v[0] + m[4]*v.v[1] + m[ 8]*v.v[2] + m[12],
             m[1]*v.v[0] + m[5]*v.v[1] + m[ 9]*v.v[2] + m[13],
             m[2]*v.v[0] + m[6]*v.v[1] + m[10]*v.v[2] + m[14]);
  return tmp;
}

klVec4 klMat44::operator * (const klVec4 &v) const
{
  klVec4 tmp(m[0]*v.v[0] + m[4]*v.v[1] + m[ 8]*v.v[2] + m[12]*v.v[3],
             m[1]*v.v[0] + m[5]*v.v[1] + m[ 9]*v.v[2] + m[13]*v.v[3],
             m[2]*v.v[0] + m[6]*v.v[1] + m[10]*v.v[2] + m[14]*v.v[3],
             m[3]*v.v[0] + m[7]*v.v[1] + m[11]*v.v[2] + m[15]*v.v[3]);
  return tmp;
}

klMat44 klMat44::operator / (const double r) const
{
  klMat44 tmp(*this);
  return tmp /= r;
}

klMat44 klMat44::operator ^ (const int i) const
{
  klMat44 t1(*this);
  if (i > 0){
    klMat44 t2 = t1;
    for (int j=i;j>1;j--) t1 = t1 * t2;
  } else if (i < 0){
    t1 = !(t1);
    klMat44 t2 = t1;
    for (int j=i;j<-1;j++) t1 = t1 * t2;
  } else {
    t1.LoadIdentity();
  }
  return t1;
}

klMat44 operator * (const double m1[16], const klMat44 &m2)
{
  klMat44 tmp(m1);
  return tmp *= m2;
}

klMat44 operator * (const double r, const klMat44 &M)
{
  klMat44 tmp(M);
  return tmp *= r;
}

//---------------------------------------------------------------------------//
//  処理   : 逆行列
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44 klMat44::operator ! () const
{
  static double t[256];
  klMat44 ans;

  double det;
  if ((det = Det()) == 0.0){
    perror("warning: Det = 0.");
    cerr << *this << endl;
    return *this;
  }

  // a b c d      
  // e f g h e f g
  // i j k l i j k
  // m n o p m n o
  //   b c d a b c
  //   f g h e f g
  //   j k l i j k
                                         t[8*4 + 4] = m[4*0 + 0]; // a
                            t[8*4 + 1] = t[8*4 + 5] = m[4*0 + 1]; // b
                            t[8*4 + 2] = t[8*4 + 6] = m[4*0 + 2]; // c
                            t[8*4 + 3]              = m[4*0 + 3]; // d
               t[8*1 + 4]              = t[8*5 + 4] = m[4*1 + 0]; // e
  t[8*1 + 1] = t[8*1 + 5] = t[8*5 + 1] = t[8*5 + 5] = m[4*1 + 1]; // f
  t[8*1 + 2] = t[8*1 + 6] = t[8*5 + 2] = t[8*5 + 6] = m[4*1 + 2]; // g
  t[8*1 + 3]              = t[8*5 + 3]              = m[4*1 + 3]; // h
               t[8*2 + 4]              = t[8*6 + 4] = m[4*2 + 0]; // i 
  t[8*2 + 1] = t[8*2 + 5] = t[8*6 + 1] = t[8*6 + 5] = m[4*2 + 1]; // j
  t[8*2 + 2] = t[8*2 + 6] = t[8*6 + 2] = t[8*6 + 6] = m[4*2 + 2]; // k
  t[8*2 + 3]              = t[8*6 + 3]              = m[4*2 + 3]; // l
               t[8*3 + 4]                           = m[4*3 + 0]; // m
  t[8*3 + 1] = t[8*3 + 5]                           = m[4*3 + 1]; // n
  t[8*3 + 2] = t[8*3 + 6]                           = m[4*3 + 2]; // o
  t[8*3 + 3]                                        = m[4*3 + 3]; // p

  int x, y;
  for (x = 0; x <= 3; x++){
    for (y = 0; y <= 3; y++){
      ans.m[4*y + x] = (((x + y) % 2 == 0) ? 1.0 : -1.0) *
        (t[8*(x+1) + (y+1)] * t[8*(x+2) + (y+2)] * t[8*(x+3) + (y+3)] +
         t[8*(x+1) + (y+2)] * t[8*(x+2) + (y+3)] * t[8*(x+3) + (y+1)] +
         t[8*(x+1) + (y+3)] * t[8*(x+2) + (y+1)] * t[8*(x+3) + (y+2)] -
         t[8*(x+1) + (y+3)] * t[8*(x+2) + (y+2)] * t[8*(x+3) + (y+1)] -
         t[8*(x+1) + (y+2)] * t[8*(x+2) + (y+1)] * t[8*(x+3) + (y+3)] -
         t[8*(x+1) + (y+1)] * t[8*(x+2) + (y+3)] * t[8*(x+3) + (y+2)])
        / det;
    }
  }
  // 同次座標行列として使うなら m[15] == 1.0 のはず
  /*
  if (ans.m[4*3 + 3] != 1.0){
    for (x = 0; x <= 3; x++){
      for (y = 0; y <= 3; y++){
        ans.m[4*y + x] /= ans.m[4*3 + 3];
      }
    }
    ans.m[4*3 + 3] = 1.0;
  }
  */
  return ans;
}

//---------------------------------------------------------------------------//
//  処理   : 転置行列
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44 klMat44::operator ~ () const
{
  klMat44 tmp;
  for (int x = 0; x <= 3; x++)
    for (int y = 0; y <= 3; y++)
      tmp.m[4*x + y] = m[4*y + x];
  return tmp;
}

//---------------------------------------------------------------------------//
//  処理   : 行列式
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
double klMat44::Det () const
{
  return m[15] * (m[0] * m[5] * m[10] +
                  m[4] * m[9] * m[ 2] +
                  m[8] * m[1] * m[ 6] -
                  m[0] * m[9] * m[ 6] -
                  m[4] * m[1] * m[10] -
                  m[8] * m[5] * m[ 2]);
}

// kiyokiyo
//---------------------------------------------------------------------------//
//  処理   : 平行移動成分, 回転成分 のみを含んだ行列を返す
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44 klMat44::Pos() const
{
  klMat44 tmp;
  tmp.x = x, tmp.y = y, tmp.z = z;
  return tmp;
}

klMat44 klMat44::Rot() const
{
  klMat44 tmp(*this);
  tmp.x = tmp.y = tmp.z = 0.0;
  return tmp;
}

//---------------------------------------------------------------------------//
//  処理   : 値の取得
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
klMat44 klMat44::GetPos(klMat44 &M) const
{
  M.x = x, M.y = y, M.z = z;
  return *this;
}

klMat44 klMat44::GetPos(klVec3 &v) const
{
  v.x = x, v.y = y, v.z = z;
  return *this;
}

klMat44 klMat44::GetPos(klVec4 &v) const
{
  v.x = x, v.y = y, v.z = z;
  return *this;
}

klMat44 klMat44::GetPos(double &_x, double &_y, double &_z) const
{
  _x = x, _y = y, _z = z;
  return *this;
}

klVec3 klMat44::GetPos() const
{
	return klVec3(x,y,z);
}

klVec4 klMat44::GetPos4() const
{
	return klVec4(x,y,z,1);
}


klMat44 klMat44::GetRot(klMat44 &M) const
{
  M.m[0] = m[0];  M.m[1] = m[1];  M.m[ 2] = m[ 2];
  M.m[4] = m[4];  M.m[5] = m[5];  M.m[ 6] = m[ 6];
  M.m[8] = m[8];  M.m[9] = m[9];  M.m[10] = m[10];
  return *this;
}

klMat44 klMat44::GetRot(klVec3 &v) const
{
  v.x = A(), v.y = E(), v.z = R();
  return *this;
}

klMat44 klMat44::GetRot(klVec4 &v) const
{
  v.x = A(), v.y = E(), v.z = R();
  return *this;
}

klMat44 klMat44::GetRot(double &_x, double &_y, double &_z) const
{
  _x = A(), _y = E(), _z = R();
  return *this;
}


      //double rxx, rxy, rxz, rxw;
      //double ryx, ryy, ryz, ryw;
      //double rzx, rzy, rzz, rzw;
klMat44 klMat44::GetEulerAngle(double &_x, double &_y, double &_z) const{
	_x = -asin(rzy);
	
	if(cos(_x) != 0){
		_y = atan2(rzx,rzz);

		double temp = rxy/cos(_x);
		if( temp < -0.9999 ) _z = M_PI * 3.0 / 2.0;
		else if( temp > 0.9999 ) _z = M_PI / 2.0;
		else _z = asin(temp);
		if(ryy<0){
			_z = 180 - _z;
			}
		}
	else{
		_y = atan2(-rzx,rxx);
		_z = 0;
		}
	_x = _x * RAD_TO_DEG;
	_y = _y * RAD_TO_DEG;
	_z = _z * RAD_TO_DEG;
	
	return *this;
	}

klMat44 klMat44::GetEulerAngle(float &_x, float &_y, float &_z) const{
	_x = -(float)asin(rzy);
	
	if(cos(_x) != 0){
		_y = (float)atan2(rzx,rzz);
		_z = asin((float)rxy/cos(_x));
		if(ryy<0){
			_z = 180 - _z;
			}
		}
	else{
		_y = (float)atan2(-rzx,rxx);
		_z = 0;
		}
	_x = (float)(_x * RAD_TO_DEG);
	_y = (float)(_y * RAD_TO_DEG);
	_z = (float)(_z * RAD_TO_DEG);
	
	return *this;
	}


//---------------------------------------------------------------------------//
//  処理   : ストリーム
//  返り値 : 
//  備考   : 
//---------------------------------------------------------------------------//
istream &operator>>(istream &in,  klMat44 &m)
{
  in >> m.m[0];  in >> m.m[4];  in >> m.m[ 8];  in >> m.m[12];
  in >> m.m[1];  in >> m.m[5];  in >> m.m[ 9];  in >> m.m[13];
  in >> m.m[2];  in >> m.m[6];  in >> m.m[10];  in >> m.m[14];
  in >> m.m[3];  in >> m.m[7];  in >> m.m[11];  in >> m.m[15];
  return in;
};

ostream &operator<<(ostream &out, const klMat44 &m)
{
  out.width(10);  out.setf(ios::left);
  out << m.m[0] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[4] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[8] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[12]<< endl;
  out.width(10);  out.setf(ios::left);
  out << m.m[1] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[5] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[9] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[13] << endl;
  out.width(10);  out.setf(ios::left);
  out << m.m[2] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[6] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[10] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[14] << endl;
  out.width(10);  out.setf(ios::left);
  out << m.m[3] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[7] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[11] << ' ';
  out.width(10);  out.setf(ios::left);
  out << m.m[15] << endl;
  return out;
};

//---------------------------------------------------------------------------//
//	End of klMatrix.cpp
//---------------------------------------------------------------------------//

} //namespace KlMath