#ifndef GLUTUTIL_HPP
#define GLUTUTIL_HPP

//Math
#include <KlMath/klMath.h>

//GLUT
#include "GLUTColor.hpp"

//GLUT
//#include <GL/glew.h>
//#include <GL/glut.h>

//C
#include <time.h>

namespace GLUT
{

	//using namespace Math;

class GLUTUtil
{
public:
	typedef enum
	{
		x, y, z
	}AxisName;

	static klVec3 getAxis(AxisName axis_name)
	{
		if( axis_name == x )
		{
			return klVec3(1, 0, 0);
		}
		else if( axis_name == y )
		{
			return klVec3(0, 1, 0);
		}		
		else if( axis_name == z )
		{
			return klVec3(0, 0, 1);
		}
	}
	
	//static Vector3f getAxisVector3f(AxisName axis_name)
	//{
	//	Vector3f result;
	//	result.x = 0;
	//	result.y = 0;
	//	result.z = 0;

	//	if( axis_name == x )
	//	{
	//		result.x = 1;
	//	}
	//	else if( axis_name == y )
	//	{
	//		result.y = 1;
	//	}		
	//	else if( axis_name == z )
	//	{
	//		result.z = 1;
	//	}
	//	return result;
	//}

	static void DrawAxes(double length = 0.2, double width = 3.0)
	{
		glLineWidth(width);
		double distance = length * 0.08;

		GLUTColor::setMaterial(GLUTColor::red, 0, 1, 1.0);
		glBegin(GL_LINES);
		glVertex3d(distance, distance, distance);
		glVertex3d(distance + length, distance, distance);
		glEnd();

		
		GLUTColor::setMaterial(GLUTColor::green, 0, 1, 1.0);
		glBegin(GL_LINES);		
		glVertex3d(distance, distance, distance);
		glVertex3d(distance, distance + length, distance);
		glEnd();

		
		GLUTColor::setMaterial(GLUTColor::blue, 0, 1, 1.0);
		glBegin(GL_LINES);		
		glVertex3d(distance, distance, distance);
		glVertex3d(distance, distance, distance + length);
		glEnd();
	}

	static void DrawLine(klVec3 p1, klVec3 p2, double width = 3.0)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex3dv(p1);
		glVertex3dv(p2);
		glEnd();
	}

	static void DrawLine(double p1_x, double p1_y, double p1_z, double p2_x, double p2_y, double p2_z, double width = 3.0)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex3d(p1_x, p1_y, p1_z);
		glVertex3d(p2_x, p2_y, p2_z);
		glEnd();
	}
	static void DrawLine(klVec4 p1, klVec4 p2, double width = 3.0)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex3d(p1.x, p1.y, p1.z);
		glVertex3d(p2.x, p2.y, p2.z);
		glEnd();
	}


	static void DrawPlane(klVec3 axis, klVec3 pos, klVec4 color, double size = 2.0, double alpha = 0.5, bool spin = false)
	{
		GLUTColor::setMaterial(color, 1, 1, 1);
		color.a = alpha;
		klVec3 a1 = axis.Normalize();

		glPushMatrix();
		{
			//glTranslated(0.5,0,-0.2);
			glTranslated(pos.x, pos.y, pos.z);
	

			klVec3 temp_vec = klVec3( 0, 0, 1);
			if( temp_vec * axis > 0.999 )
			{
				temp_vec = klVec3(1, 0, 0);
			}

			klVec3 a2 = temp_vec % a1;
			klVec3 a3 = a1 % a2;
			a2 = a2.Normalize();
			a3 = a3.Normalize();

			klVec3 p1, p2, p3, p4;
			p1 = ( a2 * size ) + ( a3 * (-size) );
			p2 = ( a2 * size ) + ( a3 * size );
			p3 = ( a2 * (-size) ) + ( a3 * size );
			p4 = ( a2 * (-size) ) + ( a3 * (-size) );
			

			GLUTColor::setMaterial(color, 0, 0.7, 0.7);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if(spin){
				double mat[16];
				klQuat q(45 * M_PI / 180, axis);
				q.ToMatrix(mat);
				glMultMatrixd(mat);			
				//glRotate(axis, 45);
			}

			glBegin(GL_POLYGON);
			
			glVertex3dv(p1);
			glVertex3dv(p2);
			glVertex3dv(p3);
			glVertex3dv(p4);
			glEnd();

			glEnable(GL_CULL_FACE);
			glDisable(GL_BLEND);
			

			//Color::setMaterial( Color::getColor(Color::red) );
			//Util::DrawLine(klVec3(0,0,0), a1 * size / 5.0 );
			//
			//Color::setMaterial( Color::getColor(Color::green) );
			//Util::DrawLine(klVec3(0,0,0), a2 * size / 5.0 );

			//Color::setMaterial( Color::getColor(Color::blue) );
			//Util::DrawLine(klVec3(0,0,0), a3 * size / 5.0 );

		}
		glPopMatrix();	
	}


	static void DrawPoint(klVec3 p, double size)
	{
		glEnable(GL_POINT_SMOOTH);
		glPointSize(size);
		glBegin(GL_POINTS);
		glVertex3dv(p);
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}

	static void DrawSphere(klVec3 p, double size = 0.01, double slice = 30, bool is_alpha = false )
	{
		glPushMatrix();
		{
			if( is_alpha )
			{
				glEnable(GL_BLEND);
				glDisable(GL_CULL_FACE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			glTranslated( p.x, p.y, p.z );
			//glTranslate(p);
			glEnable(GL_NORMALIZE);
			glutSolidSphere(size, slice, slice);
			glDisable(GL_NORMALIZE);

			if( is_alpha )
			{
				glEnable(GL_CULL_FACE);
				glDisable(GL_BLEND);
			}
		}
		glPopMatrix();
	}

	
	//length;  ñÓëSëÃÇÃí∑Ç≥
	//tradius; îˆÇÃîºåa
	//hradius; ì™ÇÃîºåa
	//hlength; ì™ÇÃí∑Ç≥
	//slices;  ëΩäpå`ãﬂéóÇÃäpêî
	//static void DrawArrow( klVec3 from, klVec3 to, double size = 0.1, double slice = 20)
	//					   //double tradius, double hradius,
	//                       //double hlength, double slices = 20)
	//{
	//	glPushMatrix();
	//	glTranslated( from.x, from.y, from.z );
	//	klVec3 dist = to - from;
	//	Quatf q = Converter::TwoVec_to_Quat( klVec3(0, 0, 1), dist);
	//	
	//	

	//	glRotate(q);
	//	//glTranslated(0.5*size, 0.5*size, 0.5*size);
	//	glTranslated(0, 0, 0.25*size);
	//	double length = dist.Length() * 0.92;
	//	double tradius = 0.2 * size;
	//	double hradius = 0.5 * size;
	//	double hlength = 0.5 * size;

	//	//DrawArrow( length, tradius, hradius, hlength, slice );
	//	Util::DrawArrow( length, size, 0.05);
	//	glPopMatrix();
	//}

	static void DrawArrow(  klVec3 from, klVec3 to, double size = 3.0, double head_size = 0.15)
	{
		glPushMatrix();
		{
			glTranslated( from.x, from.y, from.z );
			klVec3 dist = to - from;
			
			klQuat q = klQuat::fromTwoVec(klVec3(0,0,1), dist);

			//glRotated(q);
			double mat[16];
			klQuat::toMatrix(q, mat);
			glMultMatrixd(mat);

			double distance = 0.04;
			double length = dist.Length() * (1.0 - distance*2.0);
			glTranslated( 0, 0, distance * length );
			
			GLUTUtil::DrawArrow( length, size, head_size);
		}
		glPopMatrix();
	}

	static void DrawArrow( double length, double size, double head_size = 0.05)
	{
		GLUTUtil::DrawLine(klVec3(0,0,0), klVec3(0,0,length), size);
		glPushMatrix();
		{
			glTranslated(0,0,length);
			
			//head1
			glPushMatrix();
			{
				glRotated(150, 1, 0 ,0);
				GLUTUtil::DrawLine(klVec3(0,0,0), klVec3(0,0,length * head_size), size);
			}
			glPopMatrix();
			
			//head2
			glPushMatrix();
			{
				glRotated(-150, 1, 0 ,0);
				GLUTUtil::DrawLine(klVec3(0,0,0), klVec3(0,0,length * head_size), size);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

	
	static void DrawEllipsoid(klVec3 p, klVec3 v1, klVec3 v2, klVec3 v3, int sect) { 
		GLfloat mat[16]; 
		mat[0] = v1.x;mat[1] = v1.y; mat[2] = v1.z;mat[3] = 0;
		mat[4] = v2.x; mat[5] = v2.y; mat[6] = v2.z;
		mat[7] = 0; mat[8] = v3.x; mat[9] = v3.y;
		mat[10] = v3.z; mat[11] = 0; mat[12] = p.x;
		mat[13] = p.y; mat[14] = p.z; mat[15] = 1;
		//glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glMultMatrixf(mat);
		glEnable(GL_NORMALIZE);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutSolidSphere(1, sect, sect);
		glDisable(GL_BLEND);

		glDisable(GL_NORMALIZE);
		glPopMatrix();
	} 

	static void DrawEllipsoidalBody(klVec3 pos1, klVec3 pos2, int sect, float radius) {
		klVec3 p,v1,v2,v3;
		p = (pos1 + pos2) /2;
		v1 = (pos1 - pos2) /2;
		
		//auxiliary vector(ï‚èï)
		klVec3 temp(1,1,1);

		v2 = v1 % temp;
		v2.Normalize();
		v2 = v2 * radius;
		v3 = v1 % v2;
		v3.Normalize();
		v3 = v3 * radius;

		DrawEllipsoid(p,v1,v2,v3,sect);
	}




}; // class GLUTUtil

} // namespace GLUT

#endif //GLUTUtil.hpp