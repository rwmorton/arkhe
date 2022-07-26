#ifndef __FEM_APP_H__
#define __FEM_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
using namespace arkhe::opengl;

class femApp : public Application
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
	Vector3 p,d;
	std::vector<Vector3> trail;
	Real count;
	Real angle;
	bool toggle;
	Vector3 axis;
};

void femApp::initialize()
{
	m_camera.pos = Vector3(0,0,-5);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.1; //0.01;
	m_camera.strictOrtho = false;
	m_camera.zoom = 2;
	//
	p = Vector3(1,2,0.5);
	count = 0;
	angle = 0;
	toggle = false;
	axis.x = Math::randomBinomial(3);
	axis.y = Math::randomBinomial(3);
	axis.z = Math::randomBinomial(3);
}

void femApp::uninitialize() {}

void femApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glMultMatrixf(Matrix44::orthographic(-m_camera.zoom*m_camera.a,m_camera.zoom*m_camera.a,m_camera.zoom,-m_camera.zoom,m_camera.pos.x,m_camera.pos.z+Math::abs(m_camera.f-m_camera.n)));
	glMultMatrixf(Matrix44::perspective(m_camera.fov,m_camera.a,m_camera.n,m_camera.f));
}

void femApp::update()
{
	Application::updateCamera();

	//
	//p.x = Math::sin(count) * 3;
	//p.y = Math::cos(count) * 2;
	//p.z = Math::sin(count) * Math::cos(count) * 2;
	count += 0.01;

	glutPostRedisplay();
}

Vector3 vectorValue(Real x,Real y,Real z)
{
	Real X,Y,Z;
	//1/15 x^2 + y^7, y^7 - x^3
	X = 5 - Math::pow(x,2)/15.0 + Math::pow(y,7);
	Y = Math::pow(y,3) - Math::pow(x,3);
	Z = Math::pow(z,3);
	//Sin[x] + Cos[y] - E^x y, -Cos[y] + Sin[x] + E^x
	X = Math::sin(x) + Math::cos(y) - Math::pow(Math::e,x)*y;
	Y = Math::sin(x) - Math::cos(y) + Math::pow(Math::e,x);
	//-y,x
	//double X = -y;
	//double Y = x;
	//X = Math::cos(y);
	//Y = Math::sin(z);
	//Z = Math::cos(x);
	return Vector3(X,Y,Z);
	//return Vector3(x,y,z);
}

void femApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	//
	Ray r(p,d);
	//AABB box(Vector3(-2,-2,-1),Vector3(1,3,2));
	Triangle tri(Vector3(0,0,-3)*3,Vector3(0,3,0)*3,Vector3(0,0,2)*2);
	//Triangle tri(Vector3(0,0,-3)*3,Vector3(0,0,2)*2,Vector3(0,3,0)*3);

	bool hit = false;
	Real pt;
	//hit = Intersection::Ray_AABB(r,box,pt);
	hit = Intersection::Ray_Triangle(r,tri,pt);

	if(hit) glColor3f(1,0,0);
	else glColor3f(0.75,0.75,0.75);
	r.render();
	//box.render();
	tri.render();

	if(hit)
	{
		glColor3f(1,1,0);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3f(r(pt).x,r(pt).y,r(pt).z);
		glEnd();
	}

	Quaternion q(Vector3(1,1,1),count);
	q.render();
	Vector3 x(2,2,1);
	glColor3f(1,0,0);
	x.render();
	Matrix33 m = Matrix33::rotate(q);
	Vector3 y = m(x);
	glColor3f(0,1,0);
	y.render();

	trail.push_back(y);
	glColor3f(1,0.8,0.2);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<trail.size(); i++)
	{
		Vector3 a = trail.at(i);
		glVertex3f(a.x,a.y,a.z);
	}
	glEnd();

	glColor3f(1,0,1);
	LineSegment ls(Vector3(1,1,1)*-10,Vector3(1,1,1)*10);
	ls.render();
	Vector3 cp2 = ls.closestPoint(y);
	LineSegment ls2(y,cp2);
	glColor3f(0,0,0);
	ls2.render();

	cout << q << endl;

	/*
	//test closest points on two lines
	LineSegment l1(Vector3(-2,0,0),p);
	LineSegment l2(Vector3(3,1,0),Vector3(-1,-1,1));
	glColor3f(1,0,0);
	l1.render();
	glColor3f(0,1,0);
	l2.render();

	Vector3 P,Q;
	l1.closestPoints(l2,P,Q);

	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3fv(P);
	glVertex3fv(Q);
	glEnd();
	*/

	/*
	//
	glColor3f(0.75,0.75,0.75);
	//glColor3f(0,0,0);
	//Viz::render3DVectorField(5,0.5,vectorValue,toggle);
	Vector3 a(-1,-1,-0.1);
	Vector3 b(1,1,0.1);
	//a = Vector3::ZERO;
	a *= 5;
	b *= 5;
	Viz::render3DVectorField(a,b,0.5,vectorValue,toggle);

	//
	Matrix44 m = Matrix44::rotate(axis,angle);
	//m.render();
	Vector3 pPrime = m(p);

	Viz::render(axis);
	Vector3 A = p;
	A.normalize();
	Viz::render(A);

	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(axis.x*10,axis.y*10,axis.z*10);
	glEnd();
	//
	glLineWidth(3);
	glColor3f(1,0,0);
	Vector3 pProj = p.parallelTo(axis);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(p.x,p.y,p.z);
	glEnd();
	//
	glLineWidth(2);
	glColor3f(0.7,1,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(pProj.x,pProj.y,pProj.z);
	//
	glVertex3f(pProj.x,pProj.y,pProj.z);
	glVertex3f(p.x,p.y,p.z);
	//
	glEnd();
	glColor3f(1,0.7,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(pPrime.x,pPrime.y,pPrime.z);
	glVertex3f(pProj.x,pProj.y,pProj.z);
	glVertex3f(pPrime.x,pPrime.y,pPrime.z);
	glEnd();

	glLineWidth(1);

	Line line(Vector3(-1,-1,-1),Vector3(1,0,1));
	line.render();

	LineSegment lineS(Vector3(-1,-1,0),Vector3(0,1,2));
	lineS.render();

	Vector3 dir = Vector3(1,1,1);
	dir.normalize();
	Ray ray(Vector3(-2,1,0),dir);
	ray.render();

	trail.push_back(pPrime);
	glColor3f(1,0.8,0.2);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<trail.size(); i++)
	{
		Vector3 a = trail.at(i);
		glVertex3f(a.x,a.y,a.z);
	}
	glEnd();
	*/

	/*AABB box(Vector3(-1,-1,-1),Vector3(1,1,1));
	//box.render();
	Ray ray(Vector3(-1,0,0),Vector3(1,0,0));
	Triangle tri(Vector3(-1,2,3),Vector3(1,0,-1),Vector3(1,-1,5));
	glLineWidth(3);
	glColor3f(1,0,0);
	//ray.render();
	tri.render();
	glLineWidth(1);

	glPointSize(5);
	glColor3f(0,0,0);
	glBegin(GL_POINTS);
	glVertex3f(p.x,p.y,p.z);
	glEnd();

	Vector3 q = tri.closestPoint(p); //box.closestPoint(p);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(p.x,p.y,p.z);
	glVertex3f(q.x,q.y,q.z);
	glEnd();

	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3f(p.x,p.y,p.z);
	//glVertex3f(ray.origin.x,ray.origin.y,ray.origin.z);
	//glVertex3f(tri.p0.x,tri.p0.y,tri.p0.z);
	glEnd();*/

	/*trail.push_back(q);
	glColor3f(1,0.8,0.2);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<trail.size(); i++)
	{
		Vector3 a = trail.at(i);
		glVertex3f(a.x,a.y,a.z);
	}
	glEnd();*/

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

void femApp::keyboard(unsigned char key,int x,int y)
{
	Application::keyboard(key,x,y);
	switch(key)
	{
	case 'x':
		axis.x = Math::randomBinomial(3);
		axis.y = Math::randomBinomial(3);
		axis.z = Math::randomBinomial(3);
		p.x = Math::randomBinomial(3);
		p.y = Math::randomBinomial(3);
		p.z = Math::randomBinomial(3);
		d.x = Math::randomBinomial(3);
		d.y = Math::randomBinomial(3);
		d.z = Math::randomBinomial(3);
		d.normalize();
		trail.clear();
		break;
	case 't':
		toggle = !toggle;
		break;
	case 'r':
		angle += 0.1;
		break;
	case '+':
		p.y += 0.1;
		break;
	case '-':
		p.y -= 0.1;
		break;
	case 'w':
		p.z += 0.1;
		break;
	case 's':
		p.z -= 0.1;
		break;
	case 'a':
		p.x -= 0.1;
		break;
	case 'd':
		p.x += 0.1;
		break;
	default:
		break;
	}
}

#endif //__FEM_APP_H__
