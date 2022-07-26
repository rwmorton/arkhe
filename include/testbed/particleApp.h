#ifndef __ARKHE_PARTICLEAPP_H__
#define __ARKHE_PARTICLEAPP_H__

//arkhe includes
#include "testbed.h"
#include <physics/ArkheMassAggregates.h>

class particleApp : public Application
{
public:
	particleApp() {}
private:
	bool persp;
	MA_Cube *cube,*cube2,*cube3;
	double F;
	bool full;
	MA_String *string;
	bool high_speed;
	MA_Sphere *sphere;
	Timer timer;
	int timeDir;
protected:
	void initialize()
	{
		cout << "Press n,m,f to decrease F, increase F and apply F" << endl;
		cout << "Press p for display mode" << endl;
		F = 10;
		full = false;
		high_speed = false;
		timeDir = 1;
		//gl init
		glEnable(GL_DEPTH_TEST);

		//app init
		persp = true;
		m_camera.pos = Vector3(1,3,6);
		m_camera.a = (Real)m_window.width/(Real)m_window.height;
		m_camera.n = 0.001;
		m_camera.f = 100;
		m_camera.pan = m_camera.pitch = 0;
		glClearColor(0.55f,0.55f,0.55f,1.0f);
		//
		cube = new MA_Cube(Vector3(0,5,0));
		cube->setGlobalAcceleration(G);
		//cube->setRotation(Matrix33::rotate(Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI)));
		if(!high_speed) cube->setK(400);
		else cube->setK(1000);
		cube2 = new MA_Cube(Vector3(-3,5,0));
		cube2->setGlobalAcceleration(G);
		cube2->setRotation(Matrix33::rotate(Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI)));
		cube2->setK(150);
		cube3 = new MA_Cube(Vector3(3,5,0));
		cube3->setGlobalAcceleration(G);
		cube3->setRotation(Matrix33::rotate(Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI),Math::randomBinomial(Math::PI)));
		cube3->setK(50);
		//
		string = new MA_String(Vector3(0,10,0),Vector3(5,10,0),100);
		string->setGlobalAcceleration(G);
		string->first()->setMass(INFINITE_MASS);
		string->first()->setInfiniteMass();
		string->first()->setGlobalAcceleration(Vector3::ZERO);
		if(!high_speed) string->setK(1500);
		else string->setK(1000000);
		//
		int RES = 8;
		sphere = new MA_Sphere(Vector3(0,5,3),1,RES,RES);
		sphere->setRotation(Matrix33::rotate(Math::HALF_PI,Math::randomBinomial(Math::TWO_PI),Math::randomBinomial(Math::TWO_PI)));
		sphere->setGlobalAcceleration(G);
		sphere->setK(50);
		//
		timer.start();
	}
	void uninitialize()
	{
		delete cube;
		delete cube2;
		delete cube3;
		delete string;
		delete sphere;
	}
	void resize(int w,int h)
	{
		Application::resize(w,h);
	}
	void setView()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(persp) glMultMatrixf(m_camera.proj());
		else glMultMatrixf(Matrix44::orthographic(-3*m_camera.a,3*m_camera.a,3,-3,m_camera.pos.x,m_camera.pos.z+Math::abs(m_camera.f-m_camera.n)));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void update()
	{
		updateCamera();
		
		//do stuff...
		double t;
		if(high_speed) t = 0.005; //500 fps
		else t = 0.0166666666666667; //timer.delta(Timer::SECONDS); - fix to 60 frames per second
		t *= timeDir;
		cube->integrate(t);
		//cube2->integrate(t);
		//cube3->integrate(t);
		//string->integrate(t);
		//sphere->integrate(t);

		glutPostRedisplay();
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix(); //push the modelview matrix
		glMultMatrixf(m_camera.worldToView());

		//do stuff...
		glPointSize(5);
		Viz::render(*cube,full);
		//Viz::render(*cube2,full);
		//Viz::render(*cube3,full);
		//glLineWidth(2);
		//Viz::render(*string,full);
		//Viz::render(*sphere,full);

		//glLineWidth(1);
		//glBegin(GL_LINES);
		//glColor3f(0.7,0.7,0.7);
		//glVertex3f(0,10,0);
		//glVertex3f(0,5,0);
		//glEnd();

		glEnable(GL_DEPTH_TEST);
		glLineWidth(1);
		glColor3f(0.35,0.35,0.35);
		drawGrid();

		glPopMatrix(); //pop the modelview matrix

		glColor3f(1,1,1);
		drawString(30,30,Util::toString(m_camera.fov).c_str());
		double fps = 1.0 / timer.delta(Timer::SECONDS);
		drawString(30,50,Util::toString(fps).c_str());

		glFlush();
		glutSwapBuffers();
	}
	void keyboard(unsigned char key,int x,int y)
	{
		Application::keyboard(key,x,y);
		switch(key)
		{
		case 'c':
			persp = !persp;
			if(persp) cout << "Using Arkhe perspective projection" << endl;
			else cout << "Using Arkhe orthographic projection" << endl;
			setView();
			break;
		case '+':
			m_camera.fov++;
			setView();
			cout << m_camera.fov << endl;
			break;
		case '-':
			m_camera.fov--;
			setView();
			cout << m_camera.fov << endl;
			break;
		case 'n':
			F -= 10;
			cout << "F = " << F << endl;
			break;
		case 'm':
			F += 10;
			cout << "F = " << F << endl;
			break;
		case 'f':
			cube->addForce(Vector3(0,F,0));
			cube2->addForce(Vector3(0,F,0));
			cube3->addForce(Vector3(0,F,0));
			sphere->addForce(Vector3(0,F,0));
			break;
		case 'h':
			string->addForce(Vector3(F,0,F));
			break;
		case 'p':
			full = !full;
			break;
		case 't':
			timeDir *= -1;
			cout << "TIME DIR : " << timeDir << endl;
			break;
		default:
			break;
		}
	}
};

#endif //__ARKHE_PARTICLEAPP_H__
