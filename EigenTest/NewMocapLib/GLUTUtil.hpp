//-----------------------------------------------------------------------------
//                             GLUTUtil.h
//
// GLUTÇ≈ï`âÊÇ∑ÇÈï‚èïÉNÉâÉX
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/11/06 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef GLUT_UTIL_HEADER__
#define GLUT_UTIL_HEADER__

//Math
#include "EigenMath.h"

//GLUT
#include "GLUTColor.hpp"

//GLUT
#include <GL/glew.h>
#include <GL/glut.h>

//C
#include <time.h>

// ñºëOãÛä‘: NGLUT
// GLUTÇ≈ï`âÊÇ∑ÇÈÇΩÇﬂÇÃÉNÉâÉXåQ
namespace NGLUT
{
// ê‡ñæ:
// GLUTÇ≈ï`âÊÇ∑ÇÈï‚èïÉNÉâÉX
class GLUTUtil
{
// ê‡ñæ:
// óÒãìå^
public:
	typedef enum
	{
		x, y, z
	}AxisName;

	static Vector3f getAxis(AxisName axis_name)
	{
		if( axis_name == x )
		{
			return Vector3f(1, 0, 0);
		}
		else if( axis_name == y )
		{
			return Vector3f(0, 1, 0);
		}		
		else if( axis_name == z )
		{
			return Vector3f(0, 0, 1);
		}
	}

	static void DrawAxes(float length = 0.2, float width = 3.0)
	{
		glLineWidth(width);
		float distance = length * 0.08;

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

	static void DrawLine(Vector3f p1, Vector3f p2, float width = 3.0)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex3fv(p1.data());
		glVertex3fv(p2.data());
		glEnd();
	}

	static void DrawLine(float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z, float width = 3.0)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex3f(p1_x, p1_y, p1_z);
		glVertex3f(p2_x, p2_y, p2_z);
		glEnd();
	}

	static void DrawPlane(Vector3f axis, Vector3f pos, Vector3f color, float size = 2.f, float alpha = 0.5f, bool spin = false)
	{
		GLUTColor::setMaterial(color, alpha, 1, 1, 1);
		color.w() = alpha;
		Vector3f a1 = axis.normalized();

		glPushMatrix();
		{
			//glTranslated(0.5,0,-0.2);
			glTranslated(pos.x(), pos.y(), pos.z());
	

			Vector3f temp_vec = Vector3f( 0, 0, 1);
			if( temp_vec.dot(axis) > 0.999 )
			{
				temp_vec = Vector3f(1, 0, 0);
			}

			Vector3f a2 = temp_vec.cross(a1);
			Vector3f a3 = a1.cross(a2);
			a2 = a2.normalized();
			a3 = a3.normalized();

			Vector3f p1, p2, p3, p4;
			p1 = ( a2 * size ) + ( a3 * (-size) );
			p2 = ( a2 * size ) + ( a3 * size );
			p3 = ( a2 * (-size) ) + ( a3 * size );
			p4 = ( a2 * (-size) ) + ( a3 * (-size) );
			

			GLUTColor::setMaterial(color, 0.f, 0.7f, 0.7f);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if(spin){
				Matrix4f mat; mat = Matrix4f::Identity();
				//float mat[16];
				NEigenMath::Matrix4fRotationAxis(&mat, &axis, NEigenMath::ToRadian(45));
				//Quaternionf q; q = AngleAxisf(NEigenMath::ToRadian(45), axis);
				//q.ToMatrix(mat);
				glMultMatrixf(mat.data());			
				//glRotate(axis, 45);
			}

			glBegin(GL_POLYGON);
			
			glVertex3fv(p1.data());
			glVertex3fv(p2.data());
			glVertex3fv(p3.data());
			glVertex3fv(p4.data());
			glEnd();

			glEnable(GL_CULL_FACE);
			glDisable(GL_BLEND);
			

			//Color::setMaterial( Color::getColor(Color::red) );
			//Util::DrawLine(Vector3f(0,0,0), a1 * size / 5.0 );
			//
			//Color::setMaterial( Color::getColor(Color::green) );
			//Util::DrawLine(Vector3f(0,0,0), a2 * size / 5.0 );

			//Color::setMaterial( Color::getColor(Color::blue) );
			//Util::DrawLine(Vector3f(0,0,0), a3 * size / 5.0 );

		}
		glPopMatrix();	
	}


	static void DrawPoint(Vector3f p, float size)
	{
		glEnable(GL_POINT_SMOOTH);
		glPointSize(size);
		glBegin(GL_POINTS);
		glVertex3fv(p.data());
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}

	static void DrawSphere(Vector3f p, float size = 0.01, float slice = 30, bool is_alpha = false )
	{
		glPushMatrix();
		{
			if( is_alpha )
			{
				glEnable(GL_BLEND);
				glDisable(GL_CULL_FACE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			glTranslatef( p.x(), p.y(), p.z() );
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

	static void DrawArrow(  Vector3f from, Vector3f to, float size = 3.0, float head_size = 0.15)
	{
		glPushMatrix();
		{
			glTranslatef( from.x(), from.y(), from.z() );
			Vector3f dist = to - from;

			Matrix4f mat; mat = Matrix4f::Identity();
			NEigenMath::fromTwoVector3f(&mat, from, to);
			glMultMatrixf(mat.data());
			//Quaternionf q = Quaternionf::fromTwoVec(Vector3f(0,0,1), dist);

			////glRotated(q);
			//float mat[16];
			//Quaternionf::toMatrix(q, mat);
			//glMultMatrixd(mat);

			float distance = 0.04;
			float length = dist.norm() * (1.0 - distance*2.0);
			glTranslatef( 0, 0, distance * length );
			
			GLUTUtil::DrawArrow( length, size, head_size);
		}
		glPopMatrix();
	}

	static void DrawArrow( float length, float size, float head_size = 0.05)
	{
		GLUTUtil::DrawLine(Vector3f(0,0,0), Vector3f(0,0,length), size);
		glPushMatrix();
		{
			glTranslatef(0,0,length);
			
			//head1
			glPushMatrix();
			{
				glRotatef(150, 1, 0 ,0);
				GLUTUtil::DrawLine(Vector3f(0,0,0), Vector3f(0,0,length * head_size), size);
			}
			glPopMatrix();
			
			//head2
			glPushMatrix();
			{
				glRotatef(-150, 1, 0 ,0);
				GLUTUtil::DrawLine(Vector3f(0,0,0), Vector3f(0,0,length * head_size), size);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

	
	static void DrawEllipsoid(Vector3f p, Vector3f v1, Vector3f v2, Vector3f v3, int sect) { 
		GLfloat mat[16]; 
		mat[0] = v1.x();mat[1] = v1.y(); mat[2] = v1.z();mat[3] = 0;
		mat[4] = v2.x(); mat[5] = v2.y(); mat[6] = v2.z();
		mat[7] = 0; mat[8] = v3.x(); mat[9] = v3.y();
		mat[10] = v3.z(); mat[11] = 0; mat[12] = p.x();
		mat[13] = p.y(); mat[14] = p.z(); mat[15] = 1;
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

	static void DrawEllipsoidalBody(Vector3f pos1, Vector3f pos2, int sect, float radius) {
		Vector3f p,v1,v2,v3;
		p = (pos1 + pos2) /2;
		v1 = (pos1 - pos2) /2;
		
		//auxiliary vector(ï‚èï)
		Vector3f temp(1,1,1);

		v2 = v1.cross(temp);
		v2 = v2.normalized();
		v2 = v2 * radius;
		v3 = v1.cross(v2);
		v3 = v3.normalized();
		v3 = v3 * radius;

		DrawEllipsoid(p,v1,v2,v3,sect);
	}




}; // class GLUTUtil

} // namespace GLUT

#endif //GLUTUtil.hpp