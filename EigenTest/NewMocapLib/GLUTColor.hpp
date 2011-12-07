//-----------------------------------------------------------------------------
//                             GLUTColor.h
//
// GLUTで描画する際の色を管理するクラス
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/06 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_COLOR_HEADER__
#define GLUT_COLOR_HEADER__

////STL, C++
//#include <vector>
//#include <map>
//#include <string>
//#include <algorithm>
//#include <fstream>
//#include <iostream>
//
////GLUT
//#include <GL/glew.h>
//#include <GL/glut.h>
//
////C
//#include <time.h>
//#include <math.h>
//
////Math
//#include <KlMath/klMatrix.h>
//#include <KlMath/klVector.h>

//#include "GLUTBase.hpp"

// 名前空間: NGLUT
// GLUTで描画するためのクラス群
namespace NGLUT
{

// 説明:
// GLUTで色を管理するクラス
class GLUTColor
{
// 説明:
// 列挙型
public:
	typedef enum
	{
		p_red, pink, purple, purple_blue,
		p_blue, light_blue, blue_green, p_green,
		yerrow_green, p_yerrow, brown,

		red, blue, green,
		yerrow,
		
		puppet_yerrow,
		puppet_blue,
		puppet_green,
		puppet_pink,

		back_ground,
		white
	}ColorEnum;

// 説明:
// 設定関数
public:
	static Vector4f getColor(ColorEnum c, float alpha = 1.0)
	{
		if( c == red) { return Vector4f(0.55, 0, 0, alpha); }
		else if( c == green) { return Vector4f(0, 0.39, 0, alpha); }
		else if( c == blue) { return Vector4f(0.1, 0.1, 0.44, alpha); }
		else if( c == yerrow) { return Vector4f(0.85, 0.66, 0.15, alpha); }
		
		else if( c == p_red) { return Vector4f(1, 0.64, 0.64, alpha); }
		else if( c == pink) { return Vector4f(1, 0.64, 1, alpha); }
		else if( c == purple) { return Vector4f(1, 0.64, 1, alpha); }
		else if( c == purple_blue) { return Vector4f(0.64, 0.64, 1, alpha);}
		else if( c == p_blue) { return Vector4f(0.58, 0.79, 1, alpha); }
		else if( c == light_blue) { return Vector4f(0.6, 1, 1, alpha); }
		else if( c == blue_green) { return Vector4f(0.64, 1, 0.82, alpha); }
		else if( c == p_green) { return Vector4f(0.64, 1, 0.64, alpha); }
		else if( c == yerrow_green ) { return Vector4f(0.64, 1, 0.64, alpha);}
		else if( c == p_yerrow ) { return Vector4f(1, 1, 0.66, alpha);}
		else if( c == brown ) { return Vector4f(1, 0.82, 0.64, alpha);}

		else if( c == puppet_yerrow ) { return Vector4f(0.85, 0.66, 0.15,alpha); }
		else if( c == puppet_blue ) { return Vector4f(0.36, 0.56, 0.99,alpha); }
		else if( c == puppet_green ) { return Vector4f(0.64, 0.80, 0.36, alpha); }
		else if( c == puppet_pink ) { return Vector4f(01.00, 0.75, 0.80, alpha); }
	
		else if( c == back_ground ) { return Vector4f(0.84, 0.81, 0.75, alpha); }
		else if( c == white ) { return Vector4f(1, 1, 1, alpha); }

		else{ return Vector4f(1, 1, 1, alpha); }
	}

	static Vector3f getColor3(ColorEnum c)
	{
		Vector4f temp = getColor( c, 1.0 );
		return Vector3f( temp.x(), temp.y(), temp.z() );
	}

	static void setMaterial(const Vector4f& color,
		float specular_rate = 0.0,
		float diffuse_rate = 0.7,
		float amb_rate = 0.5)
	{
		Vector4f spc = color * specular_rate;
		spc.w() = color.w();

		Vector4f dif = color * diffuse_rate;
		dif.w() = color.w();

		Vector4f amb = color * amb_rate;
		amb.w() = color.w();

		glColor4fv(color.data());
		glMaterialfv(GL_FRONT, GL_SPECULAR, spc.data()); // GLSLシェーダを使う場合は無効となる
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif.data());  // 
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb.data());  // 
	}
		
	static void setMaterial(Vector3f color,
		float specular_rate = 0.0,
		float diffuse_rate = 0.7,
		float amb_rate = 0.5,
		float alpha = 1.0)

	{
		Vector4f color2 = Vector4f(color.x(), color.y(), color.z(), alpha);
		setMaterial(color2, specular_rate, diffuse_rate, amb_rate);
	}

	static void setMaterial(ColorEnum c,
		float specular_rate = 0.0,
		float diffuse_rate = 0.7,
		float amb_rate = 0.5,
		float alpha = 1.0)
	{
		Vector4f color = getColor(c);
		color.w() = alpha;
		setMaterial(color, specular_rate, diffuse_rate, amb_rate);
	}



}; // class GLUTColor

} // namespace GLUT

#endif //GLUTColor.hpp