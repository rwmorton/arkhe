#ifndef __BOX_APP_H__
#define __BOX_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
using namespace arkhe::opengl;

class boxApp : public Application
{
public:
	//methods
	void initialize();
	void uninitialize();
	void setView();
	void update();
	void display();
};

void boxApp::initialize()
{
	m_camera.pos = Vector3(0,0,5);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.01;
	m_camera.strictOrtho = true;
}

void boxApp::uninitialize() {}

void boxApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Matrix44::orthographic(-m_camera.zoom*m_camera.a,m_camera.zoom*m_camera.a,m_camera.zoom,-m_camera.zoom,m_camera.pos.x,m_camera.pos.z+Math::abs(m_camera.f-m_camera.n)));
}

void boxApp::update()
{
	Application::updateCamera();

	//

	glutPostRedisplay();
}

void boxApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	glColor3f(1,0,0);
	::glutWireCube(1);

	glColor3f(0,1,0);
	int NUM_SEGS = 32;
	double incr = Math::TWO_PI / NUM_SEGS;
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<NUM_SEGS; i++)
	{
		//glVertex2f(Math::cos(i*incr),Math::sin(i*incr));
		glVertex3f(Math::cos(i*incr),Math::sin(i*incr),0);
	}
	glEnd();

	glPopMatrix(); //pop the modelview matrix

	glFlush();
	glutSwapBuffers();
}

#endif //__BOX_APP_H__
