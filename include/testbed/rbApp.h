#ifndef __RB_APP_H__
#define __RB_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
#include <physics/ArkhePhysics.h>
using namespace arkhe::opengl;
using namespace arkhe::physics;

class RB_Cube : public RigidBody
{
public:
	//constructor
	RB_Cube() : size(1) { initDefaults(); }
	//methods
	void render()
	{
		glPushMatrix();
		glMultMatrixf(transformMatrix);
		glutWireCube(size);
		glPopMatrix();
	}
	//variables
	Real size;
private:
	//
protected:
	//
}; //class RB_Cube

class rbApp : public Application
{
public:
	//methods
	void initialize();
	void uninitialize();
	void setView();
	void update();
	void display();
	void keyboard(unsigned char key,int x,int y);
	//data
	RB_Cube cube;
	Vector3 p,f;
};

void rbApp::initialize()
{
	m_camera.pos = Vector3(0,3,12);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.1; //0.01;
	m_camera.strictOrtho = false;
	m_camera.zoom = 2;
	//
	cube.position.y = 5;
	//cube.acceleration = G;
	cube.calculateDerivedData();
	cube.linearDamping = 0.9;
	cube.angularDamping = 0.9;
	//set inertia tensor
	Real diag = (Real)1.0/12.0 * Math::reciprocal(cube.inverseMass) * (cube.size*cube.size + cube.size*cube.size);
	Matrix33 it
	(
		diag,0,0,
		0,diag,0,
		0,0,diag
	);
	cube.setInertiaTensor(it);
	p = Vector3::ZERO;
	f = Vector3::ZERO;
}

void rbApp::uninitialize() {}

void rbApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Matrix44::perspective(m_camera.fov,m_camera.a,m_camera.n,m_camera.f));
}

void rbApp::update()
{
	Application::updateCamera();

	//
	Real t = Math::reciprocal(60);
	cube.integrate(t);

	//cout << cube.inverseInertiaTensor << endl;
	//cout << cube.inverseInertiaTensorWorld << endl;

	glutPostRedisplay();
}

void rbApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	//
	glColor3f(1,1,0);
	cube.render();

	glPointSize(5);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	glVertex3fv(cube.transformMatrix(p,Matrix44::TransformAs::POINT));
	glEnd();

	if(!f.isZero()) f.render(cube.transformMatrix(p,Matrix44::TransformAs::POINT)-f);

	//draw grid
	glColor3f(0.8,0.8,0.8);
	drawGrid(1);

	//draw axes
	{
		Matrix33 m = Matrix33::IDENTITY;
		m.render();
	}

	glPopMatrix(); //pop modelview matrix

	glFlush();
	glutSwapBuffers();
}

void rbApp::keyboard(unsigned char key,int x,int y)
{
	Application::keyboard(key,x,y);
	switch(key)
	{
	case 'r':
		p.x = Math::ONE_OVER_TWO * Math::randomSign();
		p.y = Math::ONE_OVER_TWO * Math::randomSign();
		p.z = Math::ONE_OVER_TWO * Math::randomSign();
		break;
	case 'f':
		f = Vector3(0,100,0);
		cube.addForceAtBodyPoint(p,f);
		break;
	default:
		break;
	}
}

#endif //__RB_APP_H__
