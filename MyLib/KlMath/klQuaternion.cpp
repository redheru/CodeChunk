//-*-c++-*-
#include "klQuaternion.h"

namespace KlMath
{

double klQuat::epsilon = 1e-12;


//---------------- コンストラクタ
klQuat::klQuat()
{
  x = 0.0, y = 0.0, z = 0.0, w = 1.0;
}

klQuat::klQuat(const klQuat &u)
{
  x = u.x, y = u.y, z = u.z, w = u.w;
}

klQuat::klQuat(const klVec4 &u)
{
  x = u.x, y = u.y, z = u.z, w = u.w;
}

klQuat::klQuat(const double r[4])
{
  x = r[0], y = r[1], z = r[2], w = r[3];
}

klQuat::klQuat(const double _x, const double _y, const double _z, const double _w)
{
  x = _x, y = _y, z = _z, w = _w;
}

klQuat::klQuat(const double theta,const klVec3 &axis)
{
	w = cos(theta / 2.0);
	double s = sin(theta / 2.0);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}


klQuat::klQuat(vector<double> vec)
{
	if( (int)vec.size() != 4)
	{
		Clear();
		return;
	}
	else
	{
		x = vec[0];
		y = vec[1];
		z = vec[2];
		w = vec[3];
	}
}


//---------------- 自身が変化する関数
//初期化
klQuat& klQuat::Clear() {
	x = 0.0, y = 0.0, z = 0.0, w = 1.0;
	return *this;
}

// 大きさ1に正規化
klQuat& klQuat::Normalize()
{
  double len = x * x + y * y + z * z + w * w;
  klQuat tmp;
  if (len == 0) {
	  tmp.x = 0;
	  tmp.y = 0;
	  tmp.z = 0;
	  tmp.w = 1;
  } else {
	  len = sqrt(len);
	  assert(len != 0);
	  tmp.x = x / len;
	  tmp.y = y / len;
	  tmp.z = z / len;
	  tmp.w = w /len;
  }
  return *this = tmp;
}

// 成分を[0-1]にクランプ
klQuat& klQuat::Clamp()
{
  if (x > 1.0) x = 1.0; else if (x < 0.0) x = 0.0;
  if (y > 1.0) y = 1.0; else if (y < 0.0) y = 0.0;
  if (z > 1.0) z = 1.0; else if (z < 0.0) z = 0.0;
  return *this;
}


// Euler -> Quaternion
klQuat& klQuat::FromEuler(const double pitch, const double yaw, const double roll)
{
	return *this = fromEuler(pitch, yaw, roll);
}

// Matrix -> Quaternion
klQuat& klQuat::FromklMat(const klMat44& R)
{
	return *this = fromklMat(R);
}

// Matrix -> Quaternion
//klQuat& klQuat::FromklMat2(const klMat44 &R) // from TMPS wiki
//{
//	return *this = fromklMat2(R);
//}

// Rotation Vector -> Quaternion
klQuat& klQuat::FromRotVec(const klVec3 &v, const double &a)
{
	return *this = fromRotVec(v, a);
}

// Rotation Vector -> Quaternion
klQuat& klQuat::FromRotVec(const klVec4 &v)
{
	return *this = fromRotVec(v);
}

// Rotation between two vector -> Quaternion
klQuat& klQuat::FromTwoVec(const klVec3 &src, const klVec3 &dist)
{
	return *this = fromTwoVec(src, dist);
}




klQuat& klQuat::Rotate(const double degree,
                       const double _vx, const double _vy, const double _vz)
{
	w = cos( (degree * DEG_TO_RAD) / 2.0 );
	double s = sin( (degree * DEG_TO_RAD) / 2.0 );
	x = _vx * s;
	y = _vy * s;
	z = _vz * s;
	return *this;
}

klQuat& klQuat::Rotate(const double degree, const klVec3 &v)
{
	w = cos( (degree * DEG_TO_RAD) / 2.0 );
	double s = sin( (degree * DEG_TO_RAD) / 2.0 );
	x = v.x * s;
	y = v.y * s;
	z = v.z * s;
	return Rotate(degree, v.x, v.y, v.z);
}

klQuat& klQuat::Rotate(const klVec3 &from, const klVec3 &to)
{
	klVec3 perp = from % to;//外積
	double len = perp.Length();
	klQuat ret;
	if(len == 0){
		ret.Clear();
	}else{
		perp /= len;
		double c = from * to;
		double theta = atan2(len, c);

		ret.w = cos(theta / 2.0);
		double s = sin(theta / 2.0);
		ret.x = perp.x * s;
		ret.y = perp.y * s;
		ret.z = perp.z * s;

		//あったほうがいいかもしれないけどとりあえず消しておく
		//Vector< NUM, 3 > test = rotate(from, ret);
		//if (length(test - to) > 0.001) {
		//	std::cout << "ERROR" << std::endl;
		//	std::cout << " From: " << from << std::endl;
		//	std::cout << "   To: " << to << std::endl;
		//	std::cout << " Test: " << test << std::endl;
		//}	}
	}
	return *this = ret;
}

//代入演算子
klQuat& klQuat::operator = (const klQuat &u) // 代入
{
  x = u.x, y = u.y, z = u.z, w = u.w;
  return *this;
}

klQuat& klQuat::operator *= (const klQuat &u) // 積
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w * u.w - x * u.x - y * u.y - z * u.z;

	//xyz is this.w*r.xyz+r.w*this.xyz+cross_product(this.xyz,r.xyz)
	ret.x = y * u.z - u.y * z + w * u.x + u.w * x;
	ret.y = z * u.x - u.z * x + w * u.y + u.w * y;
	ret.z = x * u.y - u.x * y + w * u.z + u.w * z;
	return *this = ret;
}

klQuat& klQuat::operator *= (const double &u) // 定数倍
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w * u;
	ret.x = x * u;
	ret.y = y * u;
	ret.z = z * u;
	return *this = ret;
}

klQuat& klQuat::operator /= (const double &u) // 定数倍
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w / u;
	ret.x = x / u;
	ret.y = y / u;
	ret.z = z / u;
	return *this = ret;
}


klQuat& klQuat::operator += (const klQuat &u) // 和
{
	klQuat ret = (*this).Abs();
	klQuat tmp = u.Abs();
	ret.w = ret.w + tmp.w;
	ret.x = ret.x + tmp.x;
	ret.y = ret.y + tmp.y;
	ret.z = ret.z + tmp.z;
	return *this = ret;
}

//---------------- 自身が変化しない関数
// 絶対値
klQuat klQuat::Abs() const{
	klQuat tmp = (*this);
	if(tmp.w < 0){
		tmp.x = -1 * tmp.x;
		tmp.y = -1 * tmp.y;
		tmp.z = -1 * tmp.z;
		tmp.w = -1 * tmp.w;
	}
	return tmp;
}

// 共役クォータニオン
klQuat klQuat::Conjugate() const{
	klQuat tmp = (*this);
	tmp.x = -tmp.x; 
	tmp.y = -tmp.y;
	tmp.z = -tmp.z;
	return tmp;
}

//ノルム
double klQuat::Norm() const{	
	klQuat tmp = (*this);
	return sqrt( tmp.w * tmp.w + tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);	
}

//逆クォータニオン
klQuat klQuat::Inverse() const
{
	klQuat tmp = (*this);
	double norm = tmp.Norm();
	tmp = tmp.Conjugate();
	return tmp / norm;
}

// Yaw
double klQuat::GetYawAngle() const
{
	klVec3 test = Rotate(klVec3(1.0, 0.0, 0.0));
	return atan2(-test.z, test.x);	
}

// Roll
double klQuat::GetRoll() const
{
  return atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
}

// Pitch
double klQuat::GetPitch() const
{
  return asin(-2*(x*z - w*y));
}

// Yaw
double klQuat::GetYaw() const
{
  return atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
}

//Quaternion -> klMat
klMat44 klQuat::ToklMat44() const
{
	return toklMat44(*this);
}

//Quaternion -> Matrix
void klQuat::ToMatrix(double* mat) const
{
	toMatrix(*this, mat);
	return;
}


//Quaternion -> Euler
klVec3 klQuat::ToEulerZxy() const
{
	return toEulerZxy(*this);
}

//Quaternion -> Rotation Vector
klVec4 klQuat::ToRotVec() const
{
	return toRotVec(*this);
}

//Compute rotation Quaternion to distination vector from source vector
klVec3 klQuat::ToRotVec2() const
{
	return toRotVec2(*this);
}

// 単項演算子
klQuat klQuat::operator !() const
{
	double len = x * x + y * y + z * z + w * w;
	klQuat ret;
	if(len == 0){
		ret.x = 0;
		ret.y = 0;
		ret.z = 0;
		ret.w = 1;
	} else {
		assert(len != 0);
		ret.x = -x / len;
		ret.y = -y / len;
		ret.z = -z / len;
		ret.w = -w / len;
	}
	return ret;
}

klQuat klQuat::operator ~() const
{
	klQuat ret;
	ret.x = -x;
	ret.y = -y;
	ret.z = -z;
	ret.w = w;
	return ret;
}

// 2項演算子
// 積
klQuat klQuat::operator * (const klQuat &u) const
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w * u.w - x * u.x - y * u.y - z * u.z;

	//xyz is this.w*r.xyz+r.w*this.xyz+cross_product(this.xyz,r.xyz)
	ret.x = y * u.z - u.y * z + w * u.x + u.w * x;
	ret.y = z * u.x - u.z * x + w * u.y + u.w * y;
	ret.z = x * u.y - u.x * y + w * u.z + u.w * z;
	return ret;
}

// 定数倍
klQuat klQuat::operator * (const double &u) const
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w * u;
	ret.x = w * x;
	ret.y = w * y;
	ret.z = w * z;
	return ret;
}

// 定数倍
klQuat klQuat::operator / (const double &u) const
{
	klQuat ret;
	//w is this.w * u.w - this..xyz * u.xyz
	ret.w = w / u;
	ret.x = w / x;
	ret.y = w / y;
	ret.z = w / z;
	return ret;
}

// 和
klQuat klQuat::operator + (const klQuat &u)  const
{
	klQuat ret = (*this).Abs();
	klQuat tmp = u.Abs();
	ret.w = ret.w + tmp.w;
	ret.x = ret.x + tmp.x;
	ret.y = ret.y + tmp.y;
	ret.z = ret.z + tmp.z;
	return ret;
}

bool klQuat::operator == (const klQuat &u) const
{
  return (abs(x - u.x) < epsilon) &&
         (abs(y - u.y) < epsilon) &&
         (abs(z - u.z) < epsilon) &&
         (abs(w - u.w) < epsilon);
}

bool klQuat::operator != (const klQuat &u) const
{
  return (abs(x - u.x) >= epsilon) ||
         (abs(y - u.y) >= epsilon) ||
         (abs(z - u.z) >= epsilon) ||
         (abs(w - u.w) >= epsilon);
}

istream &operator>>(istream &i, klQuat &v)
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

ostream &operator<<(ostream &o, klQuat v)
{
  o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return o;
}

klVec3 klQuat::Rotate(const klVec3 &v) const
{
	klQuat tmp;
	tmp.w = 0;
	tmp.x = v.x;
	tmp.y = v.y;
	tmp.z = v.z;
	tmp = (*this) * (tmp * (~(*this)));
	return klVec3(tmp.x, tmp.y, tmp.z);		
}

//---------------- 静的関数
void klQuat::toMatrix(const klQuat &q, double* mat)
{
	double wx = q.w * q.x;
	double wy = q.w * q.y;
	double wz = q.w * q.z;

	double xx = q.x * q.x;
	double xy = q.x * q.y;
	double xz = q.x * q.z;

	double yy = q.y * q.y;
	double yz = q.y * q.z;

	double zz = q.z * q.z;

	double res[16] = {
		1 - 2*yy - 2*zz,	2 * xy + 2 * wz,	2 * xz - 2 * wy,	0,
		2 * xy - 2 * wz,	1 - 2*xx - 2*zz,	2 * yz + 2 * wx,	0,
		2 * xz + 2 * wy,	2 * yz - 2 * wx,	1 - 2*xx - 2*yy,	0,
		0,					0,					0,					1
	};
	
	for(unsigned int i = 0; i < 16; ++i)
	{
		mat[i] = res[i];
	}
}

//Quaternion -> klMat
klMat44 klQuat::toklMat44(const klQuat &q) 
{
    double sx = q.x * q.x;
	double sy = q.y * q.y;
	double sz = q.z * q.z;
	double cx = q.y * q.z;
	double cy = q.x * q.z;
	double cz = q.x * q.y;
	double wx = q.w * q.x;
	double wy = q.w * q.y;
	double wz = q.w * q.z;

	klMat44 res = klMat44(
    1.0 - 2.0 * (sy + sz),
    2.0 * (cz + wz),
    2.0 * (cy - wy),
	0,
    
	2.0 * (cz - wz),
    1.0 - 2.0 * (sx + sz),
    2.0 * (cx + wx),
	0,
    
	2.0 * (cy + wy),
    2.0 * (cx - wx),
    1.0 - 2.0 * (sx + sy),
	0,

    0.0,
    0.0,
    0.0,
	1.0
	);

	return res;
}

//Quaternion -> Euler
klVec3 klQuat::toEulerZxy(const klQuat &q)
{
	klMat44 R = toklMat44(q);

	klVec3 e;
	double rad_to_deg = 180.0 / M_PI;

	////e.x = asin( m.m32 );
	////e.z = atan2( -m.m12, m.m22 );
	////e.y = atan2( -m.m31, m.m33 );

	if( R.m32 > 0.999999 )
	{
		e.x = M_PI/2.0;
		e.y = 0;
		e.z = atan2( R.m21, R.m11 );
	}
	else if( R.m32 < -0.999999 )
	{
		e.x = -M_PI/2.0;
		e.y = 0;
		e.z = atan2( R.m21, R.m11 );
	}
	else
	{
		e.x = asin( R.m32 );
		e.z = atan2( -R.m12, R.m22 );
		e.y = atan2( -R.m31, R.m33 );
	}
	e.x *= rad_to_deg;
	e.y *= rad_to_deg;
	e.z *= rad_to_deg;

	return e;
}

//Quaternion -> Rotation Vector
klVec4 klQuat::toRotVec(const klQuat &q)
{
	klVec4 v;
	v.w = acos(q.w) * 2.0f;
	if( sin( v.w * 0.5 ) < 0.001 )
	{
		v.x = 0;
		v.y = 0;
		v.z = 0;
	}
	else
	{
		v.x = q.x / sin(v.w * 0.5);
		v.y = q.y / sin(v.w * 0.5);
		v.z = q.z / sin(v.w * 0.5);		
	}
	return v;
}

//Compute rotation Quaternion to distination vector from source vector
klVec3 klQuat::toRotVec2(const klQuat &q)
{
	double norm = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
	double angle = 2 * atan2( (double)norm, (double)q.w );

	klVec3 ret_vec;
	if(norm == 0)
	{
		ret_vec.v[0] = 0;
		ret_vec.v[1] = 0;
		ret_vec.v[2] = 0;
	}
	else
	{
		ret_vec.v[0] = angle * q.x / norm;
		ret_vec.v[1] = angle * q.y / norm;
		ret_vec.v[2] = angle * q.z / norm;
	}
	return ret_vec;
}



// Euler -> Quaternion
klQuat klQuat::fromEuler(const double pitch, const double yaw, const double roll)
{
	klQuat ret;
	double cos_z_2 = cos(0.5*yaw);
	double cos_y_2 = cos(0.5*pitch);
	double cos_x_2 = cos(0.5*roll);

	double sin_z_2 = sin(0.5*yaw);
	double sin_y_2 = sin(0.5*pitch);
	double sin_x_2 = sin(0.5*roll);

	// and now compute quaternion
	ret.w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
	ret.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
	ret.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
	ret.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

	return ret;
}

// Matrix -> Quaternion
klQuat klQuat::fromklMat(const klMat44& R)
{
	klQuat q;
	double tmp = fabs( R.m11 + R.m22 + R.m33 + 1 );
	q.w = 0.5 * sqrt(tmp);
 
	if(q.w > klQuat::epsilon)
	{
		q.x = ( R.m32 -R.m23 )/(4*q.w);
		q.y = ( R.m13 -R.m31 )/(4*q.w);
		q.z = ( R.m21 -R.m12 )/(4*q.w);
	}
	else
	{
		// |w| <= 1/2
		static int s_iNext[3] = { 2, 3, 1 };
		int i = 1;
		if( R.m22 > R.m11 ) i = 2;
		if( R.m33 > R.m22 ) i = 3;
		int j = s_iNext[i-1];
		int k = s_iNext[j-1];


		double before_sqrt = R.m[4*i+i]-R.m[4*j+j]-R.m[4*k+k] + 1.0;
		// 特異点？みたいなのの処理(これでいいのか果たして)
		if( before_sqrt < 0.0000 )
		{
			q.x = 0.7;
			q.y = -0.7;
			q.z = 0;
			q.w = 0.006;
			return q;
		}
		double fRoot = sqrt( before_sqrt );


		double* tmp[3] = { &q.x, &q.y, &q.z };
		*tmp[i-1] = 0.5*fRoot;
		fRoot = 0.5/fRoot;
		q.w = (R.m[4*j+k]-R.m[4*k+j])*fRoot;
		*tmp[j-1] = (R.m[4*i+j]+R.m[4*j+i])*fRoot;
		*tmp[k-1] = (R.m[4*j+k]+R.m[4*k+i])*fRoot;
	}
	return q;
}


// Rotation Vector -> Quaternion
klQuat klQuat::fromRotVec(const klVec3 &v, const double &a)
{
	klQuat q;
	const double sina = sin(a * 0.5f), cosa = cos(a * 0.5f);
	q.x = sina * v.x;
	q.y = sina * v.y;
	q.z = sina * v.z;
	q.w = cosa;
	return q;
}

// Rotation Vector -> Quaternion
klQuat klQuat::fromRotVec(const klVec4 &v)
{
	klQuat q;
	const double sina = sin(v.w * 0.5f), cosa = cos(v.w * 0.5f);
	q.x = sina * v.x;
	q.y = sina * v.y;
	q.z = sina * v.z;
	q.w = cosa;
	return q;
}

// Rotation between two vector -> Quaternion
klQuat klQuat::fromTwoVec(const klVec3 &src, const klVec3 &dist)
{
	klQuat q;

	klVec3 cross = src % dist;

	//no difference
	if(cross.Abs() == 0)
	{
		q.Clear();
		return q;
	}
	klVec3 axis_vec = cross / cross.Abs();
	
	double dot = src * dist;
	double cos_theta =  dot / ( src.Abs() * dist.Abs() );

	double cos_theta_div2 = sqrt( 0.5 * (1.0 + cos_theta) );
	double sin_theta_div2 = 0.5 * (1.0 - cos_theta);

	if( sin_theta_div2 >= 0)
	{
		sin_theta_div2 = sqrt( sin_theta_div2 );
	}
	else
	{
		sin_theta_div2 = 0.0;
	}
	
	q.w = cos_theta_div2;
	q.x = axis_vec.x * sin_theta_div2;
	q.y = axis_vec.y * sin_theta_div2;
	q.z = axis_vec.z * sin_theta_div2;

	return q;
}

} //namespace KlMath