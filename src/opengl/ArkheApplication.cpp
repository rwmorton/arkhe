//arkhe local includes (opengl)
#include "ArkheApplication.h"

//arkhe includes (foundation)
#include <foundation/ArkheException.h>
using namespace arkhe::foundation;

#include <iostream>

namespace arkhe
{

namespace opengl
{

//static member declarations
Application *Application::s_application = 0;

Application::~Application() {}

void Application::run(Application *app,int argc,char **argv)
{
	if(!app)
	{
		throw Exception(Exception::NULL_POINTER);
	}

	if(s_application)
	{
		delete s_application;
	}

	s_application = app;

	s_application->_initializeCore(argc,argv); //initialize the application core
	s_application->initialize(); //initialize the derived application

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS); //freeglut implementation only
	glutMainLoop(); //enter main loop

	s_application->uninitialize(); //uninitialize the derived application
	s_application->_uninitializeCore(); //uninitialize the application core
}

void Application::_initializeCore(int argc,char **argv)
{
	//initialize GLUT
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(m_window.width,m_window.height);
	glutInitWindowPosition(m_window.x,m_window.y);

	/*
	//Specify the type of OpenGL context. This is OpenGL's internal data
	//structure for keeping track of state settings and operations.
	*/
	glutInitContextVersion(3,3); //(3,3); //(4,3); former for laptop, latter for workstation
	/*
	//Profile selects whether we are using the latest features or
	//whether we are including features that are backward compatible etc.
	*/
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE;
	if(glewInit())
	{
		throw Exception("Unable to initialize GLEW");
	}

	//register GLUT callback functions
	glutIdleFunc(&Application::_updateCallback);
	glutDisplayFunc(&Application::_displayCallback);
	glutReshapeFunc(&Application::_resizeCallback);
	glutMouseFunc(&Application::_mouseCallback);
	glutMotionFunc(&Application::_mouseDragCallback);
	glutKeyboardFunc(&Application::_keyboardCallback);
}

void Application::_uninitializeCore()
{
	if(!s_application)
	{
		throw Exception(Exception::NULL_POINTER);
	}
	delete s_application;
	s_application = 0;
}

void Application::_updateCallback() { s_application->update(); }
void Application::_displayCallback() { s_application->display(); }
void Application::_resizeCallback(int w,int h) { s_application->resize(w,h); }
void Application::_mouseCallback(int button,int state,int x,int y) { s_application->mouse(button,state,x,y); }
void Application::_mouseDragCallback(int x,int y) { s_application->mouseDrag(x,y); }
void Application::_keyboardCallback(unsigned char key,int x,int y) { s_application->keyboard(key,x,y); }

void Application::updateCamera()
{
	//update camera
	if(!m_camera.strictOrtho)
	{
		if(m_mouse.leftDown)
		{
			m_camera.pitch -= m_mouse.deltaY * m_camera.sensitivity * (Real)0.051;
			m_camera.pan += m_mouse.deltaX * m_camera.sensitivity * (Real)0.05;
		}
	}
	if(m_mouse.middleDown)
	{
		//we need to map the screen space motion to
		//the world space in order for the objects to track the mouse...
		m_camera.pos.x -= m_mouse.deltaX * m_camera.sensitivity * (Real)0.5;
		m_camera.pos.y -= m_mouse.deltaY * m_camera.sensitivity * (Real)0.5;
	}
	if(m_mouse.rightDown)
	{
		m_camera.pos.z -= m_mouse.deltaY * m_camera.sensitivity * (Real)0.5;
		m_camera.zoom += m_mouse.deltaY * m_camera.sensitivity * (Real)0.5;
	}
	setView();
}

void Application::resize(int w,int h)
{
	m_window.width = w;
	m_window.height = h;
	glViewport(0,0,w,h);
	m_camera.a = (Real)w/(Real)h;
	setView();
}

void Application::mouse(int button,int state,int x,int y)
{
	m_mouse.curX = x;
	m_mouse.curY = y;

	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			m_mouse.leftDown = true;
		}
		else if(state == GLUT_UP)
		{
			m_mouse.leftDown = false;
		}
	}
	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			m_mouse.middleDown = true;
		}
		else if(state == GLUT_UP)
		{
			m_mouse.middleDown = false;
		}
	}
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			m_mouse.rightDown = true;
		}
		else if(state == GLUT_UP)
		{
			m_mouse.rightDown = false;
		}
	}
}

void Application::mouseDrag(int x,int y)
{
	m_mouse.prevX = m_mouse.curX;
	m_mouse.prevY = m_mouse.curY;
	m_mouse.curX = x;
	m_mouse.curY = y;
	m_mouse.deltaX = x - m_mouse.prevX;
	m_mouse.deltaY = m_mouse.prevY - y; //Y is opposite to X
}

void Application::keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27:
		glutLeaveMainLoop(); //freeglut implementation only
		break;
	default:
		break;
	}
}

void Application::drawString(float x,float y,const char *string)
{
	glDisable(GL_DEPTH_TEST);

	//create a temporary 2D projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0,m_window.width,m_window.height,0); //flip the Y coordinate

	glRasterPos2f(x,y);
	for(const char *c=&string[0]; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
	}

	glPopMatrix(); //pop the temporary projection

	glEnable(GL_DEPTH_TEST);
}

} //namespace opengl

} //namespace arkhe
