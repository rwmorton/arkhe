#ifndef __ARKHE_APPLICATION_H__
#define __ARKHE_APPLICATION_H__

//arkhe local includes (opengl)
#include "ArkheGL.h" //must be included before any other headers

//arkhe includes (foundation)
#include <foundation/ArkheFoundation.h>
using namespace arkhe::foundation;

namespace arkhe
{

namespace opengl
{

/*
TODO: add options to control the OpenGL context, ie. single/double buffering,
depth testing enabled/disabled etc.

LIST:
DISPLAY MODE:
	GLUR_RGBA,GLUT_RGB,GLUT_INDEX
	GLUT_SINGLE,GLUT_DOUBLE
	GLUT_ACCUM,GLUT_DEPTH,GLUT_STENCIL

WINDOW SIZE
WINDOW POSITION

REFERENCE:
	int glutGetModifiers(void);
	GLUT_ACTIVE_SHIFT,GLUT_ACTIVE_CTRL,GLUT_ACTIVE_ALT
	ie.
	mod = glutGetModifiers();
	if (mod == (GLUT_ACTIVE_CTRL|GLUT_ACTIVE_ALT)) ...

	void glutKeyboardUpFunc(void (*func)(u_char key,int x,int y));
*/
class Application
{
public:
	//constructors
	Application() {}
	Application(unsigned int w,unsigned int h) : m_window(w,h) {}
	//destructors
	virtual ~Application();
	//methods
	/*
	//Takes ownership of the Application pointer passed in.
	*/
	static void run(Application *app,int argc,char **argv);
private:
	//make non-copyable
	Application(const Application &);
	Application &operator=(const Application &);
	//methods
	/*
	//The following two methods are purely for freeglut and GLEW only.
	*/
	void _initializeCore(int argc,char **argv);
	void _uninitializeCore();
	//static methods for GLUT callbacks
	static void __cdecl _updateCallback();
	static void __cdecl _displayCallback();
	static void __cdecl _resizeCallback(int w,int h);
	static void __cdecl _mouseCallback(int button,int state,int x,int y);
	static void __cdecl _mouseDragCallback(int x,int y);
	static void __cdecl _keyboardCallback(unsigned char key,int x,int y);
protected:
	//structs
	struct Window
	{
		Window()
		{
			x = 400; y = 200;
			width = 800;
			height = 600;
		}
		Window(int w,int h) : width(w),height(h),x(0),y(0) {}
		int x,y;
		int width,height;
	}; //struct Window
	struct Mouse
	{
		Mouse()
		{
			prevX = prevY = curX = curY = deltaX = deltaY = 0;
			leftDown = middleDown = rightDown = false;
		}
		int prevX,prevY,curX,curY,deltaX,deltaY;
		bool leftDown,middleDown,rightDown;
	}; //struct Mouse
	struct Camera
	{
		Camera() : pos(Vector3::ZERO),keyIncr((Real)0.1),pan(0),pitch(0),fov(45),a(1),n(0.5),f(20),sensitivity((Real)0.1),zoom(1),strictOrtho(false) {}
		const Matrix44 worldToView() const
		{
			Real Cx,Cy,Sx,Sy;
			Math::sinCos(pitch,Sx,Cx);
			Math::sinCos(pan,Sy,Cy);
			Matrix44 TR = Matrix44
			(
				Cy,0,Sy,pos.x,
				Sx*Sy,Cx,-Sx*Cy,pos.y,
				-Cx*Sy,Sx,Cx*Cy,pos.z,
				0,0,0,1
			);
			Matrix44 L = Matrix44::lookAt(Vector3(0,0,1),Vector3::ZERO,Vector3::UNIT_Y); //TRL
			return Matrix44::translate(-pos.x,-pos.y,-pos.z) * Matrix44::rotateX(pitch) * Matrix44::rotateY(pan);// * L;
		}
		const Matrix44 proj() const
		{
			return Matrix44::perspective(fov,a,n,f);
		}
		//view data
		Vector3 pos;
		Real keyIncr;
		Real pan,pitch;
		//projection data
		Real fov,a,n,f;
		//motion data
		Real sensitivity;
		Real zoom; //for ortho zoom
		bool strictOrtho;
	}; //struct Camera
	//member variables
	Window m_window;
	Mouse m_mouse;
	Camera m_camera;
	static Application *s_application;
	//methods
	virtual void initialize() = 0;
	virtual void uninitialize() = 0;
	/*
	//Make sure to glutPostRedisplay()...
	*/
	virtual void update() = 0;
	virtual void setView() = 0; // REPLACE THIS WITH A CAMERA SYSTEM...
	/*
	//In the derived implementation:
	//Make sure to update the OpenGL state ie.
	//glFlush();
	//glutSwapBuffers();
	*/
	virtual void updateCamera();
	virtual void display() = 0;
	virtual void resize(int w,int h);
	virtual void mouse(int button,int state,int x,int y);
	virtual void mouseDrag(int x,int y);
	virtual void keyboard(unsigned char key,int x,int y);
	virtual void drawString(float x,float y,const char *string);
}; //class Application

} //namespace opengl

} //namespace arkhe

#endif //__ARKHE_APPLICATION_H__
