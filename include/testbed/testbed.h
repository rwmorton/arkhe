#ifndef __ARKHE_TESTBED_H__
#define __ARKHE_TESTBED_H__

//arkhe includes
#include <foundation/ArkheFoundation.h>
#include <physics/ArkhePhysics.h>
#include <opengl/ArkheApplication.h>
#include <opengl/ArkheViz.h>
using namespace arkhe::foundation;
using namespace arkhe::physics;
using namespace arkhe::opengl;

#pragma warning(disable : 4244 4305 4996 4305)

void drawGrid(float scale=1.0f)
{
	glMultMatrixf(Matrix44::scale(scale,scale,scale));
	//draw grid
	float width = 5.0f;
	float space = 1.0f;
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i=-width; i<=width; i++)
	{
		if(i == 0)
		{
			glLineWidth(2.0f);
		}
		glVertex3f(-width,0.0f,i*space);
		glVertex3f(width,0.0f,i*space);
		glVertex3f(i*space,0.0f,-width);
		glVertex3f(i*space,0.0f,width);
		if(i == 0)
		{
			glLineWidth(1.0f);
		}
	}
	glEnd();
}

#endif //__ARKHE_TESTBED_H__
