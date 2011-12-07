//-*-c++-*-
#include "klVector.h"

namespace KlMath
{

double klVec2::epsilon = 1e-12;
double klVec3::epsilon = 1e-12;
double klVec4::epsilon = 1e-12;

//---------------- コンストラクタ
klVec2::klVec2()
{
  x = 0.0, y = 0.0;
}

klVec2::klVec2(const klVec2 &u)
{
  x = u.x, y = u.y;
}

klVec2::klVec2(const double r[2])
{
  x = r[0], y = r[1];
}

klVec2::klVec2(const double _x, const double _y)
{
  x = _x, y = _y;
}

//---------------- 自身が変化する関数
// 大きさ1に正規化
klVec2& klVec2::Normalize()
{
  return *this /= Abs();
}

klVec2& klVec2::Clamp()
{
  if (x > 1.0) x = 1.0; else if (x < 0.0) x = 0.0;
  if (y > 1.0) y = 1.0; else if (y < 0.0) y = 0.0;
  return *this;
}

// 回転
klVec2& klVec2::Rotate(const double deg)
{
  double rad = deg * DEG_TO_RAD;
  double _x = x, _y = y;
  x = _x * cos(rad) - _y * sin(rad);
  y = _x * sin(rad) + _y * cos(rad);
  return *this;
}

klVec2& klVec2::Translate(const double _x, const double _y)
{
  x += _x, y += _y;
  return *this;
}

// 代入演算子
klVec2& klVec2::operator = (const klVec2 &u)
{
  x = u.x, y = u.y;
  return *this;
}

klVec2& klVec2::operator += (const klVec2 &u)
{
  x += u.x, y += u.y;
  return *this;
}

klVec2& klVec2::operator -= (const klVec2 &u)
{
  x -= u.x, y -= u.y;
  return *this;
}

klVec2& klVec2::operator *= (const double r)
{
  x *= r, y *= r;
  return *this;
}

klVec2& klVec2::operator /= (const double r)
{
  x /= r, y /= r;
  return *this;
}

//---------------- 自身が変化しない関数
// 絶対値
double klVec2::Abs() const
{
  return sqrt(x * x + y * y);
}

double klVec2::Length() const
{
  return Abs();
}

// 別のベクトルとのなす角
double klVec2::Angle(const klVec2 &u) const
{
  return acos(((*this) * u)/((*this).Abs() * u.Abs())) * RAD_TO_DEG;
}

double klVec2::Distance(const klVec2 &u) const
{
  return sqrt((x-u.x)*(x-u.x) + (y-u.y)*(y-u.y));
}

// 単項演算子
klVec2 klVec2::operator +() const
{
  return klVec2(x, y);
}

klVec2 klVec2::operator -() const
{
  return klVec2(-x, -y);
}

// 2項演算子
double klVec2::operator * (const klVec2 &u) const
{
  return x * u.x + y * u.y; // 内積
}

klVec2 klVec2::operator + (const klVec2 &u) const
{
  return klVec2(x + u.x, y + u.y);
}

klVec2 klVec2::operator - (const klVec2 &u) const
{
  return klVec2(x - u.x, y - u.y);
}

klVec2 klVec2::operator * (const double r) const
{
  return klVec2(x * r, y * r);
}

klVec2 klVec2::operator / (const double r) const
{
  return klVec2(x / r, y / r);
}

bool klVec2::operator == (const klVec2 &u) const
{
  return (abs(x - u.x) < epsilon) && (abs(y - u.y) < epsilon);
}

bool klVec2::operator != (const klVec2 &u) const
{
  return (abs(x - u.x) >= epsilon) || (abs(y - u.y) >= epsilon);
}

klVec2 operator * (const double r, const klVec2 &u)
{
  return klVec2(r * u.x, r * u.y);
}

istream &operator>>(istream &i, klVec2 &v)
{
  char c;
  i >> c;
  if (c != '(') perror("syntax error : '(' expected");
  
  i >> v.x >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.y >> c;
  if (c != ')') perror("syntax error : ')' expected");

  return i;
}

ostream &operator<<(ostream &o, klVec2 v)
{
  o << "(" << v.x << ", " << v.y << ")";
  return o;
}

//---------------- コンストラクタ
klVec3::klVec3()
{
  x = 0.0, y = 0.0, z = 0.0;
}

klVec3::klVec3(const klVec3 &u)
{
  x = u.x, y = u.y, z = u.z;
}

klVec3::klVec3(const double r[3])
{
  x = r[0], y = r[1], z = r[2];
}

klVec3::klVec3(const float r[3])
{
  x = r[0], y = r[1], z = r[2];
}

klVec3::klVec3(const double _x, const double _y, const double _z)
{
  x = _x, y = _y, z = _z;
}

//---------------- 自身が変化する関数
// 大きさ1に正規化
klVec3& klVec3::Normalize()
{
  return *this /= Abs();
}

klVec3& klVec3::Clamp()
{
  if (x > 1.0) x = 1.0; else if (x < 0.0) x = 0.0;
  if (y > 1.0) y = 1.0; else if (y < 0.0) y = 0.0;
  if (z > 1.0) z = 1.0; else if (z < 0.0) z = 0.0;
  return *this;
}

klVec3& klVec3::Rotate(const double degree,
                       const double _vx, const double _vy, const double _vz)
{
  double len = sqrt(_vx*_vx + _vy*_vy + _vz*_vz);
  double vx = _vx / len;
  double vy = _vy / len;
  double vz = _vz / len;
  double c  = cos(degree * DEG_TO_RAD);
  double s  = sin(degree * DEG_TO_RAD);

  double m[16];
  m[0] = vx*vx*(1-c) + c;
  m[1] = vy*vx*(1-c) + vz*s;
  m[2] = vz*vx*(1-c) - vy*s;

  m[4] = vx*vy*(1-c) - vz*s;
  m[5] = vy*vy*(1-c) + c;
  m[6] = vz*vy*(1-c) + vx*s;

  m[8] = vx*vz*(1-c) + vy*s;
  m[9] = vy*vz*(1-c) - vx*s;
  m[10]= vz*vz*(1-c) + c;

  klVec3 tmp;
  tmp.x = m[0] * x + m[4] * y + m[8] * z;
  tmp.y = m[1] * x + m[5] * y + m[9] * z;
  tmp.z = m[2] * x + m[6] * y + m[10]* z;
  /*
  tmp.x = x * m[0] + y * m[1] + z * m[2];
  tmp.y = x * m[4] + y * m[5] + z * m[6];
  tmp.z = x * m[8] + y * m[9] + z * m[10];
    */

  return *this = tmp;
}

klVec3& klVec3::Rotate(const double degree, const klVec3 &v)
{
  return Rotate(degree, v.x, v.y, v.z);
}

klVec3& klVec3::Translate(const double _x, const double _y, const double _z)
{
//  if (w != 0.0) perror("directional vector (w = 0.0) cannot be translated");
  x += _x, y += _y, z += _z;
  return *this;
}

klVec3& klVec3::Translate(const klVec3 &v)
{
  return Translate(v.x, v.y, v.z);
}

klVec3& klVec3::Scale(const double _x, const double _y, const double _z)
{
  x *= _x, y *= _y, z *= _z;
  return *this;
}

klVec3& klVec3::Scale(const klVec3 &v)
{
  return Scale(v.x, v.y, v.z);
}

// 代入演算子
klVec3& klVec3::operator = (const klVec3 &u)
{
  x = u.x, y = u.y, z = u.z;
  return *this;
}

klVec3& klVec3::operator += (const klVec3 &u)
{
  x += u.x, y += u.y, z += u.z;
  return *this;
}

klVec3& klVec3::operator -= (const klVec3 &u)
{
  x -= u.x, y -= u.y, z -= u.z;
  return *this;
}

klVec3& klVec3::operator %= (const klVec3 &u)	// 外積
{
  double _x = x;
  double _y = y;
  double _z = z;
  x = _y * u.z - _z * u.y;
  y = _z * u.x - _x * u.z;
  z = _x * u.y - _y * u.x;
  return *this;
}

klVec3& klVec3::operator *= (const double r)
{
  x *= r, y *= r, z *= r;
  return *this;
}

klVec3& klVec3::operator /= (const double r)
{
  x /= r, y /= r, z /= r;
  return *this;
}

//---------------- 自身が変化しない関数
// 絶対値
double klVec3::Abs() const
{
  return sqrt(x * x + y * y + z * z);
}

double klVec3::Length() const
{
  return Abs();
}

// 別のベクトルとのなす角
double klVec3::Angle(const klVec3 &u) const
{
  return acos(((*this) * u)/((*this).Abs() * u.Abs())) * RAD_TO_DEG;
}

double klVec3::Distance(const klVec3 &u) const
{
  return sqrt((x-u.x)*(x-u.x) + (y-u.y)*(y-u.y) + (z-u.z)*(z-u.z));
}

double klVec3::DistToLine(const klVec3 &p1, const klVec3 &p2) const
{
  return ((*this) - (*this).ProjToLine(p1, p2)).Length();
}

bool klVec3::OnLine(const klVec3 &p1, const klVec3 &p2) const
{
  return DistToLine(p1, p2) < epsilon;
}

klVec3 klVec3::ProjToLine(const klVec3 &p1, const klVec3 &p2) const
{
  // 直線 p1 + s(p2 - p1) 上の、垂線の足における s を求める
  klVec3 dir = p2 - p1;
  double s = (-p1 * dir + (*this) * dir) / (dir * dir);
  return klVec3(p1 + s * dir);
}

double klVec3::DistToPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  return ((*this) - (*this).ProjToPlane(p1, p2, p3)).Length();
}

bool klVec3::OnPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  return DistToPlane(p1, p2, p3) < epsilon;
}

klVec3 klVec3::ProjToPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  klVec3 norm = (p2 - p1) % (p3 - p1);
  return *this - ((norm * (*this - p1)) / (norm.Abs() * norm.Abs())) * norm;
}

// 単項演算子
klVec3 klVec3::operator +() const
{
  return klVec3 (x, y, z);
}

klVec3 klVec3::operator -() const
{
  return klVec3(-x, -y, -z);
}

// 2項演算子
double klVec3::operator * (const klVec3 &u) const
{
  return x * u.x + y * u.y + z * u.z; // 内積
}

double klVec3::operator * (const klVec4 &u) const
{
  return x * u.x + y * u.y + z * u.z; // 内積
}

klVec3 klVec3::operator + (const klVec3 &u) const
{
  return klVec3(x + u.x, y + u.y, z + u.z);
}

klVec3 klVec3::operator + (const klVec4 &u) const
{
  return klVec3(x + u.x, y + u.y, z + u.z);
}

klVec3 klVec3::operator - (const klVec3 &u) const
{
  return klVec3(x - u.x, y - u.y, z - u.z);
}

klVec3 klVec3::operator - (const klVec4 &u) const
{
  return klVec3(x - u.x, y - u.y, z - u.z);
}

klVec3 klVec3::operator % (const klVec3 &u) const // 外積
{
  return klVec3(y * u.z - z * u.y,
                z * u.x - x * u.z,
                x * u.y - y * u.x);
}

klVec3 klVec3::operator % (const klVec4 &u) const // 外積
{
  return klVec3(y * u.z - z * u.y,
                z * u.x - x * u.z,
                x * u.y - y * u.x);
}

klVec3 klVec3::operator * (const double r) const
{
  return klVec3(x * r, y * r, z * r);
}

klVec3 klVec3::operator / (const double r) const
{
  return klVec3(x / r, y / r, z / r);
}

bool klVec3::operator == (const klVec3 &u) const
{
  return (abs(x - u.x) < epsilon) &&
         (abs(y - u.y) < epsilon) &&
         (abs(z - u.z) < epsilon);
}

bool klVec3::operator != (const klVec3 &u) const
{
  return (abs(x - u.x) >= epsilon) ||
         (abs(y - u.y) >= epsilon) ||
         (abs(z - u.z) >= epsilon);
}

klVec3 operator * (const double r, const klVec3 &u)
{
  return klVec3(r * u.x, r * u.y, r * u.z);
}

istream &operator>>(istream &i, klVec3 &v)
{
  char c;
  i >> c;
  if (c != '(') perror("syntax error : '(' expected");
  
  i >> v.x >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.y >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.z >> c;
  if (c != ')') perror("syntax error : ')' expected");

  return i;
}

ostream &operator<<(ostream &o, klVec3 v)
{
  o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return o;
}


//---------------- コンストラクタ
klVec4::klVec4()
{
  x = 0.0, y = 0.0, z = 0.0, w = 1.0;
}

klVec4::klVec4(const klVec3 &u, const double _w)
{
  x = u.x, y = u.y, z = u.z, w = _w;
}

klVec4::klVec4(const klVec4 &u)
{
  x = u.x, y = u.y, z = u.z, w = u.w;
}

klVec4::klVec4(const double r[4])
{
  x = r[0], y = r[1], z = r[2], w = r[3];
}

klVec4::klVec4(const double _x, const double _y, const double _z, const double _w)
{
  x = _x, y = _y, z = _z, w = _w;
}

//---------------- 自身が変化する関数
// 大きさ1に正規化
klVec4& klVec4::Normalize()
{
  return *this /= Abs();
}

klVec4& klVec4::Clamp()
{
  if (x > 1.0) x = 1.0; else if (x < 0.0) x = 0.0;
  if (y > 1.0) y = 1.0; else if (y < 0.0) y = 0.0;
  if (z > 1.0) z = 1.0; else if (z < 0.0) z = 0.0;
  return *this;
}

klVec4& klVec4::Rotate(const double degree,
                       const double _vx, const double _vy, const double _vz)
{
  double len = sqrt(_vx*_vx + _vy*_vy + _vz*_vz);
  double vx = _vx / len;
  double vy = _vy / len;
  double vz = _vz / len;
  double c  = cos(degree * DEG_TO_RAD);
  double s  = sin(degree * DEG_TO_RAD);

  double m[16];
  m[0] = vx*vx*(1-c) + c;
  m[1] = vy*vx*(1-c) + vz*s;
  m[2] = vz*vx*(1-c) - vy*s;

  m[4] = vx*vy*(1-c) - vz*s;
  m[5] = vy*vy*(1-c) + c;
  m[6] = vz*vy*(1-c) + vx*s;

  m[8] = vx*vz*(1-c) + vy*s;
  m[9] = vy*vz*(1-c) - vx*s;
  m[10]= vz*vz*(1-c) + c;

  klVec3 tmp;
  tmp.x = m[0] * x + m[4] * y + m[8] * z;
  tmp.y = m[1] * x + m[5] * y + m[9] * z;
  tmp.z = m[2] * x + m[6] * y + m[10]* z;
  /*
  tmp.x = x * m[0] + y * m[1] + z * m[2];
  tmp.y = x * m[4] + y * m[5] + z * m[6];
  tmp.z = x * m[8] + y * m[9] + z * m[10];
    */

  return *this = tmp;
}

klVec4& klVec4::Rotate(const double degree, const klVec3 &v)
{
  return Rotate(degree, v.x, v.y, v.z);
}

klVec4& klVec4::Rotate(const double degree, const klVec4 &v)
{
  return Rotate(degree, v.x, v.y, v.z);
}

klVec4& klVec4::Translate(const double _x, const double _y, const double _z)
{
//  if (w != 0.0) perror("directional vector (w = 0.0) cannot be translated");
  x += _x, y += _y, z += _z;
  return *this;
}

klVec4& klVec4::Translate(const klVec3 &v)
{
  return Translate(v.x, v.y, v.z);
}

klVec4& klVec4::Translate(const klVec4 &v)
{
  return Translate(v.x, v.y, v.z);
}

klVec4& klVec4::Scale(const double _x, const double _y, const double _z)
{
  x *= _x, y *= _y, z *= _z;
  return *this;
}

klVec4& klVec4::Scale(const klVec3 &v)
{
  return Scale(v.x, v.y, v.z);
}

klVec4& klVec4::Scale(const klVec4 &v)
{
  return Scale(v.x, v.y, v.z);
}

// 代入演算子
klVec4& klVec4::operator = (const klVec3 &u)
{
  x = u.x, y = u.y, z = u.z, w = 1.0;
  return *this;
}

klVec4& klVec4::operator = (const klVec4 &u)
{
  x = u.x, y = u.y, z = u.z, w = u.w;
  return *this;
}

klVec4& klVec4::operator += (const klVec3 &u)
{
  x += u.x, y += u.y, z += u.z;
  return *this;
}

klVec4& klVec4::operator += (const klVec4 &u)
{
  x += u.x, y += u.y, z += u.z;
  return *this;
}

klVec4& klVec4::operator -= (const klVec3 &u)
{
  x -= u.x, y -= u.y, z -= u.z;
  return *this;
}

klVec4& klVec4::operator -= (const klVec4 &u)
{
  x -= u.x, y -= u.y, z -= u.z;
  return *this;
}

klVec4& klVec4::operator %= (const klVec3 &u)	// 外積
{
  double _x = x;
  double _y = y;
  double _z = z;
  x = _y * u.z - _z * u.y;
  y = _z * u.x - _x * u.z;
  z = _x * u.y - _y * u.x;
  return *this;
}

klVec4& klVec4::operator %= (const klVec4 &u)	// 外積
{
  double _x = x;
  double _y = y;
  double _z = z;
  x = _y * u.z - _z * u.y;
  y = _z * u.x - _x * u.z;
  z = _x * u.y - _y * u.x;
  return *this;
}

klVec4& klVec4::operator *= (const double r)
{
  x *= r, y *= r, z *= r;
  return *this;
}

klVec4& klVec4::operator /= (const double r)
{
  x /= r, y /= r, z /= r;
  return *this;
}

//---------------- 自身が変化しない関数
// 絶対値
double klVec4::Abs() const
{
  return sqrt(x * x + y * y + z * z);
}

double klVec4::Length() const
{
  return Abs();
}

// 別のベクトルとのなす角
double klVec4::Angle(const klVec3 &u) const
{
  return acos(((*this) * u)/((*this).Abs() * u.Abs())) * RAD_TO_DEG;
}

// 別のベクトルとのなす角
double klVec4::Angle(const klVec4 &u) const
{
  return acos(((*this) * u)/((*this).Abs() * u.Abs())) * RAD_TO_DEG;
}

double klVec4::Distance(const klVec3 &u) const
{
  return sqrt((x-u.x)*(x-u.x) + (y-u.y)*(y-u.y) + (z-u.z)*(z-u.z));
}

double klVec4::Distance(const klVec4 &u) const
{
  return sqrt((x-u.x)*(x-u.x) + (y-u.y)*(y-u.y) + (z-u.z)*(z-u.z));
}

double klVec4::DistToLine(const klVec3 &p1, const klVec3 &p2) const
{
  return ((*this) - (*this).ProjToLine(p1, p2)).Length();
}

double klVec4::DistToLine(const klVec4 &p1, const klVec4 &p2) const
{
  return ((*this) - (*this).ProjToLine(p1, p2)).Length();
}

bool klVec4::OnLine(const klVec3 &p1, const klVec3 &p2) const
{
  return DistToLine(p1, p2) < epsilon;
}

bool klVec4::OnLine(const klVec4 &p1, const klVec4 &p2) const
{
  return DistToLine(p1, p2) < epsilon;
}

klVec4 klVec4::ProjToLine(const klVec3 &p1, const klVec3 &p2) const
{
  // 直線 p1 + s(p2 - p1) 上の、垂線の足における s を求める
  klVec4 dir = p2 - p1;
  double s = (-p1 * dir + (*this) * dir) / (dir * dir);
  return klVec3(p1 + s * dir);
}

klVec4 klVec4::ProjToLine(const klVec4 &p1, const klVec4 &p2) const
{
  // 直線 p1 + s(p2 - p1) 上の、垂線の足における s を求める
  klVec4 dir = p2 - p1;
  double s = (-p1 * dir + (*this) * dir) / (dir * dir);
  return klVec4(p1 + s * dir);
}

double klVec4::DistToPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  return ((*this) - (*this).ProjToPlane(p1, p2, p3)).Length();
}

double klVec4::DistToPlane(const klVec4 &p1, const klVec4 &p2, const klVec4 &p3) const
{
  return ((*this) - (*this).ProjToPlane(p1, p2, p3)).Length();
}

bool klVec4::OnPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  return DistToPlane(p1, p2, p3) < epsilon;
}

bool klVec4::OnPlane(const klVec4 &p1, const klVec4 &p2, const klVec4 &p3) const
{
  return DistToPlane(p1, p2, p3) < epsilon;
}

klVec4 klVec4::ProjToPlane(const klVec3 &p1, const klVec3 &p2, const klVec3 &p3) const
{
  klVec4 norm = (p2 - p1) % (p3 - p1);
  return *this - ((norm * (*this - p1)) / (norm.Abs() * norm.Abs())) * norm;
}

klVec4 klVec4::ProjToPlane(const klVec4 &p1, const klVec4 &p2, const klVec4 &p3) const
{
  klVec4 norm = (p2 - p1) % (p3 - p1);
  return *this - ((norm * (*this - p1)) / (norm.Abs() * norm.Abs())) * norm;
}

// 単項演算子
klVec4 klVec4::operator +() const
{
  return klVec4 (x, y, z, w);
}

klVec4 klVec4::operator -() const
{
  return klVec4(-x, -y, -z, w);
}

// 2項演算子
double klVec4::operator * (const klVec3 &u) const
{
  return x * u.x + y * u.y + z * u.z; // 内積
}

double klVec4::operator * (const klVec4 &u) const
{
  return x * u.x + y * u.y + z * u.z; // 内積
}

klVec4 klVec4::operator + (const klVec3 &u) const
{
  return klVec4(x + u.x, y + u.y, z + u.z, w);
}

klVec4 klVec4::operator + (const klVec4 &u) const
{
  return klVec4(x + u.x, y + u.y, z + u.z, w);
}

klVec4 klVec4::operator - (const klVec3 &u) const
{
  return klVec4(x - u.x, y - u.y, z - u.z, w);
}

klVec4 klVec4::operator - (const klVec4 &u) const
{
  return klVec4(x - u.x, y - u.y, z - u.z, w);
}

klVec4 klVec4::operator % (const klVec3 &u) const // 外積
{
  return klVec4(y * u.z - z * u.y,
                z * u.x - x * u.z,
                x * u.y - y * u.x,
                w);
}

klVec4 klVec4::operator % (const klVec4 &u) const // 外積
{
  return klVec4(y * u.z - z * u.y,
                z * u.x - x * u.z,
                x * u.y - y * u.x,
                w);
}

klVec4 klVec4::operator * (const double r) const
{
  return klVec4(x * r, y * r, z * r, w);
}

klVec4 klVec4::operator / (const double r) const
{
  return klVec4(x / r, y / r, z / r, w);
}

bool klVec4::operator == (const klVec4 &u) const
{
  return (abs(x - u.x) < epsilon) &&
         (abs(y - u.y) < epsilon) &&
         (abs(z - u.z) < epsilon);
}

bool klVec4::operator != (const klVec4 &u) const
{
  return (abs(x - u.x) >= epsilon) ||
         (abs(y - u.y) >= epsilon) ||
         (abs(z - u.z) >= epsilon);
}

klVec4 operator * (const double r, const klVec4 &u)
{
  return klVec4(r * u.x, r * u.y, r * u.z, u.w);
}

istream &operator>>(istream &i, klVec4 &v)
{
  char c;
  i >> c;
  if (c != '(') perror("syntax error : '(' expected");
  
  i >> v.x >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.y >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.z >> c;
  if (c != ',') perror("syntax error : ',' expected");

  i >> v.w >> c;
  if (c != ')') perror("syntax error : ')' expected");

  return i;
}

ostream &operator<<(ostream &o, klVec4 v)
{
  o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return o;
}

} // namespace KlMath