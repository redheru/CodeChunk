#ifndef GLUTCOLOR_HPP
#define GLUTCOLOR_HPP

//STL, C++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//GLUT
#include <GL/glew.h>
#include <GL/glut.h>

//C
#include <time.h>
#include <math.h>

//Math
#include <KlMath/klMatrix.h>
#include <KlMath/klVector.h>

namespace GLUT
{

class GLUTColor
{

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
		white,
	}ColorEnum;

	static klVec4 getColor(ColorEnum c, double alpha = 1.0)
	{
		if( c == red) { return klVec4(0.55, 0, 0, alpha); }
		else if( c == green) { return klVec4(0, 0.39, 0, alpha); }
		else if( c == blue) { return klVec4(0.1, 0.1, 0.44, alpha); }
		else if( c == yerrow) { return klVec4(0.85, 0.66, 0.15, alpha); }

		
		else if( c == p_red) { return klVec4(1, 0.64, 0.64, alpha); }
		else if( c == pink) { return klVec4(1, 0.64, 1, alpha); }
		else if( c == purple) { return klVec4(1, 0.64, 1, alpha); }
		else if( c == purple_blue) { return klVec4(0.64, 0.64, 1, alpha);}
		else if( c == p_blue) { return klVec4(0.58, 0.79, 1, alpha); }
		else if( c == light_blue) { return klVec4(0.6, 1, 1, alpha); }
		else if( c == blue_green) { return klVec4(0.64, 1, 0.82, alpha); }
		else if( c == p_green) { return klVec4(0.64, 1, 0.64, alpha); }
		else if( c == yerrow_green ) { return klVec4(0.64, 1, 0.64, alpha);}
		else if( c == p_yerrow ) { return klVec4(1, 1, 0.66, alpha);}
		else if( c == brown ) { return klVec4(1, 0.82, 0.64, alpha);}

		else if( c == puppet_yerrow ) { return klVec4(0.85, 0.66, 0.15,alpha); }
		else if( c == puppet_blue ) { return klVec4(0.36, 0.56, 0.99,alpha); }
		else if( c == puppet_green ) { return klVec4(0.64, 0.80, 0.36, alpha); }
		else if( c == puppet_pink ) { return klVec4(01.00, 0.75, 0.80, alpha); }
	
		else if( c == back_ground ) { return klVec4(0.84, 0.81, 0.75, alpha); }
		else if( c == white ) { return klVec4(1, 1, 1, alpha); }

		else{ return klVec4(1, 1, 1, alpha); }
	}

	static klVec3 getColor3(ColorEnum c)
	{
		klVec4 temp = getColor( c, 1.0 );
		return klVec3( temp.r, temp.g, temp.b );
	}

	static void setMaterial(klVec4 color,
		double specular_rate = 0.0,
		double diffuse_rate = 0.7,
		double amb_rate = 0.5)
	{
		klVec4 spc = color * specular_rate;
		spc.a = color.a;

		klVec4 dif = color * diffuse_rate;
		dif.a = color.a;

		klVec4 amb = color * amb_rate;
		amb.a = color.a;

		glColor4fv(color);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, spc);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	}


		
	static void setMaterial(klVec3 color,
		double specular_rate = 0.0,
		double diffuse_rate = 0.7,
		double amb_rate = 0.5,
		double alpha = 1.0)

	{
		klVec4 color2 = klVec4(color.r, color.g, color.b, alpha);
		setMaterial(color2, specular_rate, diffuse_rate, amb_rate);
	}



	static void setMaterial(ColorEnum c,
		double specular_rate = 0.0,
		double diffuse_rate = 0.7,
		double amb_rate = 0.5,
		double alpha = 1.0)
	{
		klVec4 color = getColor(c);
		color.a = alpha;
		setMaterial(color, specular_rate, diffuse_rate, amb_rate);
	}



}; // class GLUTColor

} // namespace GLUT

#endif //GLUTColor.hpp