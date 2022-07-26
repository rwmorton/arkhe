#ifndef __INTEGRATOR_APP_H__
#define __INTEGRATOR_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
using namespace arkhe::opengl;

Vector3 vectorValue(Real x,Real y)
{
	Real X,Y;
	//1/15 x^2 + y^7, y^7 - x^3
	//X = 5 - Math::pow(x,2)/15.0 + Math::pow(y,7);
	//Y = Math::pow(y,3) - Math::pow(x,3);
	//Sin[x] + Cos[y] - E^x y, -Cos[y] + Sin[x] + E^x
	//X = Math::sin(x) + Math::cos(y) - Math::pow(Math::e,x)*y;
	//Y = Math::sin(x) - Math::cos(y) + Math::pow(Math::e,x);
	//-y^2 + 11 Sin[x y], x y^3 - 9 Cos[x]
	X = -y*y + 11 * Math::sin(x*y);
	Y = x*y*y*y - 9 * Math::cos(x);
	return Vector3(X,Y,0);
}

void drawTrail(const vector<Vector3> &trail,double r=0,double g=1,double b=0)
{
	if(trail.size() != 0)
	{
		glBegin(GL_LINE_STRIP);
		double color_incr = Math::reciprocal(trail.size());
		color_incr = 1;
		for(int i=0; i<trail.size(); i++)
		{
			glColor3f(i*color_incr*r,i*color_incr*g,i*color_incr*b);
			glVertex3f(trail.at(i).x,trail.at(i).y,0);
		}
		glEnd();
	}
}

class integratorApp : public Application
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
	Particle initParticle;
	Particle euler1Particle,euler2Particle,midpointParticle,verletParticle,actualApproxParticle;
	vector<Vector3> euler1Trail,euler2Trail,midpointTrail,verletTrail,actualApproxTrail;
	ForceField forceField;
};

void integratorApp::initialize()
{
	m_camera.pos = Vector3(0,0,5);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.01;
	m_camera.strictOrtho = true;
	m_camera.zoom = 30;
	//
	initParticle.setMass(1.0);
	initParticle.setPosition(Vector3(-3,1,0)); //-3,1,0
	euler1Particle = euler2Particle = midpointParticle = verletParticle = actualApproxParticle = initParticle;
	forceField.addParticle(&euler1Particle);
	forceField.addParticle(&euler2Particle);
	forceField.addParticle(&midpointParticle);
	forceField.addParticle(&verletParticle);
	forceField.addParticle(&actualApproxParticle);
}

void integratorApp::uninitialize() {}

void integratorApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Matrix44::orthographic(-m_camera.zoom*m_camera.a,m_camera.zoom*m_camera.a,m_camera.zoom,-m_camera.zoom,m_camera.pos.x,m_camera.pos.z+Math::abs(m_camera.f-m_camera.n)));
}

void integratorApp::update()
{
	Application::updateCamera();

	Vector3 p;
	double t = 0.005;

	//subsep to approximate actual trajectory
	int STEPS = 10;
	for(int i=0; i<STEPS; i++)
	{
		//actual approx particle
		p = actualApproxParticle.getPosition();
		actualApproxParticle.integrateEuler2(t / (double)STEPS);
		if(!actualApproxParticle.getVelocity().isZero()) actualApproxTrail.push_back(p);
	}

	//euler 1 particle
	p = euler1Particle.getPosition();
	euler1Particle.integrateEuler1(t);
	if(!euler1Particle.getVelocity().isZero()) euler1Trail.push_back(p);

	//euler 2 particle
	p = euler2Particle.getPosition();
	euler2Particle.integrateEuler2(t);
	if(!euler2Particle.getVelocity().isZero()) euler2Trail.push_back(p);

	//midpoint particle
	p = midpointParticle.getPosition();
	midpointParticle.integrateMidpoint(t);
	if(!midpointParticle.getVelocity().isZero()) midpointTrail.push_back(p);

	//verlet particle
	p = verletParticle.getPosition();
	verletParticle.integrateVerlet(t);
	if(!verletParticle.getVelocity().isZero()) verletTrail.push_back(p);

	//EAT TIME!!!
	/*
	//randomize frame time
	double trunc = 1000000 - Math::random(truncFactor);
	int N = 1 * trunc;
	for(int i=0; i<N; i++)
	{
		Math::sin(i);
	}*/

	glutPostRedisplay();
}

void integratorApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	//draw vector field
	glLineWidth(1);
	glColor3f(0.25,0.25,0.25);
	Viz::render2DVectorField(30,1,vectorValue);

	//draw particles
	glPointSize(5);
	Viz::render(euler1Particle,1,0,0);
	Viz::render(euler2Particle,0,1,0);
	Viz::render(midpointParticle,0,0,1);
	Viz::render(verletParticle,1,1,0);
	Viz::render(actualApproxParticle,0,0,0);

	//draw trails
	glLineWidth(2);
	drawTrail(euler1Trail,1,0,0);
	drawTrail(euler2Trail,0,1,0);
	drawTrail(midpointTrail,0,0,1);
	drawTrail(verletTrail,1,1,0);
	drawTrail(actualApproxTrail,0,0,0);

	glPopMatrix(); //pop the modelview matrix

	glFlush();
	glutSwapBuffers();
}

void integratorApp::keyboard(unsigned char key,int x,int y)
{
	Application::keyboard(key,x,y);
	switch(key)
	{
	default:
		break;
	}
}

#endif //__INTEGRATOR_APP_H__
