//std includes
#include <iostream>
#include <conio.h>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

//arkhe includes
#include <foundation/ArkheFoundation.h>
#include <opengl/ArkheApplication.h>
#include <opengl/ArkheViz.h>
using namespace arkhe::foundation;
using namespace arkhe::opengl;

#pragma warning(disable : 4244 4305 4996)

struct vizNorm
{
	vizNorm(Vector3 p,Vector3 d) : pos(p),dir(d) {}
	Vector3 pos,dir;
};

void drawCube(const Matrix44 m=Matrix44::IDENTITY)
{
	//store our cube points and normals
	std::vector<Vector3> points;
	std::vector<vizNorm> faceNormals;
	//front
	points.push_back(Vector3(-1,-1,1));
	points.push_back(Vector3(1,-1,1));
	points.push_back(Vector3(1,1,1));
	points.push_back(Vector3(-1,1,1));
	faceNormals.push_back(vizNorm(Vector3(0,0,1),Vector3(0,0,1)));
	//back
	points.push_back(Vector3(-1,-1,-1));
	points.push_back(Vector3(1,-1,-1));
	points.push_back(Vector3(1,1,-1));
	points.push_back(Vector3(-1,1,-1));
	faceNormals.push_back(vizNorm(Vector3(0,0,-1),Vector3(0,0,-1)));
	//left
	points.push_back(Vector3(1,-1,1));
	points.push_back(Vector3(1,-1,-1));
	points.push_back(Vector3(1,1,-1));
	points.push_back(Vector3(1,1,1));
	faceNormals.push_back(vizNorm(Vector3(1,0,0),Vector3(1,0,0)));
	//right
	points.push_back(Vector3(-1,-1,-1));
	points.push_back(Vector3(-1,-1,1));
	points.push_back(Vector3(-1,1,1));
	points.push_back(Vector3(-1,1,-1));
	faceNormals.push_back(vizNorm(Vector3(-1,0,0),Vector3(-1,0,0)));
	//top
	points.push_back(Vector3(-1,1,1));
	points.push_back(Vector3(1,1,1));
	points.push_back(Vector3(1,1,-1));
	points.push_back(Vector3(-1,1,-1));
	faceNormals.push_back(vizNorm(Vector3(0,1,0),Vector3(0,1,0)));
	//bottom
	points.push_back(Vector3(-1,-1,1));
	points.push_back(Vector3(1,-1,1));
	points.push_back(Vector3(1,-1,-1));
	points.push_back(Vector3(-1,-1,-1));
	faceNormals.push_back(vizNorm(Vector3(0,-1,0),Vector3(0,-1,0)));

	//a test
	vector<Vector3> atest;
	atest.push_back(Vector3(0,1,0));
	atest.push_back(Vector3(0,1,-1));
	atest.push_back(Vector3(0,2,-1));
	atest.push_back(Vector3(0,2,0));

	//apply transform
	for(int i=0; i<points.size(); i++)
	{
		points[i] = m(points[i]);
	}
	for(int i=0; i<faceNormals.size(); i++)
	{
		faceNormals[i].pos = m(faceNormals[i].pos,Matrix44::POINT);
		//faceNormals[i].dir = m(faceNormals[i].dir,Matrix44::NORMAL,false);
		faceNormals[i].dir = m(faceNormals[i].dir,Matrix44::NORMAL,false);
		faceNormals[i].dir.normalize();
	}
	for(int i=0; i<atest.size(); i++)
	{
		atest[i] = m(atest[i]);
	}

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	for(int i=0; i<points.size(); i+=4)
	{
		glBegin(GL_POLYGON);
		glVertex3fv(points[i]);
		glVertex3fv(points[i+1]);
		glVertex3fv(points[i+2]);
		glVertex3fv(points[i+3]);
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(0,0,1);
	glLineWidth(3);
	for(int i=0; i<faceNormals.size(); i++)
	{
		glBegin(GL_LINES);
		glVertex3fv(faceNormals[i].pos);
		glVertex3fv(faceNormals[i].pos+faceNormals[i].dir);
		glEnd();
	}
	glLineWidth(1);
	//a test
	glColor3f(1,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(atest[0]);
	glVertex3fv(atest[1]);
	glVertex3fv(atest[2]);
	glVertex3fv(atest[3]);
	glEnd();
}

void drawAxes(int flipZ=1,float scale=1.0)
{
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(1*scale,0,0);
	glVertex3f(0,0,0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0,1,0);
	glVertex3f(0,1*scale,0);
	glVertex3f(0,0,0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0,0,1);
	glVertex3f(0,0,1*flipZ*scale);
	glVertex3f(0,0,0);
	glEnd();
}

void drawCam()
{
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.5,0.5,0.5);
	glVertex3f(0,0,0);
	glVertex3f(0.5,0.5,-1);
	glVertex3f(0.5,-0.5,-1);
	glVertex3f(0,0,0);
	glVertex3f(-0.5,0.5,-1);
	glVertex3f(-0.5,-0.5,-1);
	glVertex3f(0,0,0);
	glVertex3f(0.5,0.5,-1);
	glVertex3f(-0.5,0.5,-1);
	glVertex3f(0,0,0);
	glVertex3f(0.5,-0.5,-1);
	glVertex3f(-0.5,-0.5,-1);
	glVertex3f(0,0,0);
	glEnd();
	glLineWidth(3);
	drawAxes(/*-1*/1,0.5);
	glColor3f(0.25,0.25,0.25);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x0101);
	glBegin(GL_LINES);
	glVertex3f(0.5,0.5,-1);
	glVertex3f(5,5,-10);
	glVertex3f(-0.5,0.5,-1);
	glVertex3f(-5,5,-10);
	glVertex3f(0.5,-0.5,-1);
	glVertex3f(5,-5,-10);
	glVertex3f(-0.5,-0.5,-1);
	glVertex3f(-5,-5,-10);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(1);
}

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

struct Camera
{
	Camera() : pos(Vector3::ZERO),keyIncr(0.1),pan(0),pitch(0),fov(75),a(1),n(0.5),f(20),sensitivity(0.1) {}
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
		return Matrix44::translate(-pos.x,pos.y,pos.z) * Matrix44::rotateX(Angle(pitch,Angle::DEGREES)) * Matrix44::rotateY(Angle(pan,Angle::DEGREES));// * L;
	}
	const Matrix44 proj() const
	{
		return Matrix44::perspective(Angle(fov,Angle::DEGREES),a,n,f);
	}
	//view data
	Vector3 pos;
	Real keyIncr;
	Real pan,pitch;
	//projection data
	Real fov,a,n,f;
	//motion data
	Real sensitivity;
};

void drawSphere(float r,Vector3 p)
{
	glPushMatrix();
	glTranslatef(p.x,p.y,p.z);
	glutWireSphere(r,16,16);
	glPopMatrix();
}

void rotateAxis(const Vector3 &p,const Vector3 &axis,float angle,float *m)
{
	float s,c;
	Math::sinCos(angle,s,c);
	Vector3 n = axis;
	n.normalize();
	Vector3 v = p.perpendicularTo(n);
	v.normalize();
	Vector3 w = v.cross(n);
	Matrix44 Rz = Matrix44::rotateZ(angle);
	Matrix44 M
	(
		v.x,w.x,n.x,0,
		v.y,w.y,n.y,0,
		v.z,w.z,n.z,0,
		0,0,0,1
	);
	Matrix44 Ra = M*Rz; //Rz*M;
	for(int i=0; i<16; i++)
	{
		m[i] = Ra[i];
	}

	glColor3f(1,1,1);
	glLineWidth(5);
	Viz::setColor(1,0,0);
	Viz::render(v);
	Viz::setColor(0,1,0);
	Viz::render(w);
	Viz::setColor(0,0,1);
	Viz::render(n);
	glLineWidth(1);
}

class testApp : public Application
{
public:
	testApp() {}
private:
	bool inverse;
	bool persp,world_persp;
	Real rincr;
	Camera m_cam;
	Vector3 pos,pos2;
protected:
	void initialize()
	{
		//gl init
		glEnable(GL_DEPTH_TEST);

		//app init
		inverse = persp = world_persp = false;
		rincr = 0.0;
		m_cam.pos = Vector3(0,0,-5);
		m_cam.a = (Real)m_window.width/(Real)m_window.height;
		m_cam.pan = m_cam.pitch = 0;
		glClearColor(0.55f,0.55f,0.55f,1.0f);
		pos = pos2 = Vector3::ZERO;
		pos2.z = -3.5;
	}
	void uninitialize() {}
	void update()
	{
		//update camera
		if(m_mouse.leftDown)
		{
			m_cam.pitch -= m_mouse.deltaY * m_cam.sensitivity;
			m_cam.pan += m_mouse.deltaX * m_cam.sensitivity;
		}
		if(m_mouse.middleDown)
		{
			//we need to map the screen space motion to
			//the world space in order for the objects to track the mouse...
			m_cam.pos.x -= m_mouse.deltaX * m_cam.sensitivity * 0.5;
			m_cam.pos.y += m_mouse.deltaY * m_cam.sensitivity * 0.5;
		}
		if(m_mouse.rightDown)
		{
			m_cam.pos.z += m_mouse.deltaY * m_cam.sensitivity * 0.5;
		}

		//rincr += 0.01;
		glutPostRedisplay();
	}
	void resize(int w,int h)
	{
		Application::resize(w,h);
		m_cam.a = (Real)w/(Real)h;
		setView();
	}
	void setView()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//if(persp) glMultMatrixf(Matrix44::perspective(m_cam.fov,m_cam.a,m_cam.n,m_cam.f));
		//else gluPerspective(m_cam.fov,m_cam.a,m_cam.n,m_cam.f);
		//glMultMatrixf(m_cam.proj());

		if(persp) glMultMatrixf(Matrix44::perspective(m_cam.fov,m_cam.a,m_cam.n,m_cam.f));
		else glMultMatrixf(Matrix44::orthographic(-3*m_cam.a,3*m_cam.a,3,-3,m_cam.pos.x,m_cam.pos.z+Math::abs(m_cam.f-m_cam.n)));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix(); //push the modelview matrix

		glMultMatrixf(m_cam.worldToView());
		//glMultMatrixf(Matrix44::lookAt(Vector3(1.5,1.5,2),Vector3::ZERO,Vector3::UNIT_Y));

		Vector3 MINa(-3,0,-1),MINb(pos.x-1,pos.y-1,pos.z-1);
		Vector3 MAXa(1,1,1),MAXb(pos.x+1,pos.y+1,pos.z+1);
		AABB a(MINa,MAXa),b(MINb,MAXb);

		Ray3 r(Vector3(-1,-1,3),Vector3(0.25,0.5,-1));
		try
		{
			Intersection::ResultSet3D *resultSet = 0;
			if(Intersection::test(r,b,resultSet))
			{
				Viz::setColor(1,0,0);
				glLineWidth(2);
				Viz::render(r);
			}
			else
			{
				glLineWidth(1);
				Viz::setColor(1,1,0);
				Viz::render(r);
			}
		}
		catch(Exception &e)
		{
			cout << e.what() << endl;
		}

		//render boxes with intersection test...
		if(Intersection::test(a,b))
		{
			glLineWidth(2);
			Viz::setColor(1,0,0);
			Viz::render(a);
			Viz::render(b);
		}
		else
		{
			glLineWidth(1);
			Viz::setColor(0.5,0.5,0);
			Viz::render(a);
			Viz::setColor(0,0.5,0.5);
			Viz::render(b);
		}

		//SPHERE-SPHERE TEST
		Real Sar = 1,Sbr = 0.5;
		Vector3 Sap(2,0,0),Sbp(pos2.x,pos2.y,pos2.z);
		Sphere Sa(Sap,Sar),Sb(Sbp,Sbr);
		if(Intersection::test(Sa,Sb))
		{
			glLineWidth(2);
			Viz::setColor(1,0.5,0);
		}
		else
		{
			glLineWidth(1);
			Viz::setColor(0.5,1,0);
		}
		drawSphere(Sar,Sap);
		drawSphere(Sbr,Sbp);

		//RAY-SPHERE TEST
		Ray3 r2(Vector3(3,1,-1),Vector3(-1,1,-1));
		r2.direction = Vector3(Math::sin(rincr),Math::cos(rincr),Math::sin(rincr));
		if(Intersection::test(r2,Sb,false))
		{
			Viz::setColor(1,0,0);
			Viz::render(r2);
		}
		else
		{
			Viz::setColor(0,0,0);
			Viz::render(r2);
		}

		/*
		//////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		Vector3 AXIS(1,1,1); //AXIS(Math::sin(rincr),Math::cos(rincr),Math::sin(rincr));
		AXIS.normalize();
		Vector3 P(1,0,0);
		glColor3f(1,0,1);
		glLineWidth(5);
		Viz::render(AXIS);
		glLineWidth(1);
		///////////////
		glPushMatrix();
		float *mtx = new float[16];
		rotateAxis(P,AXIS,rincr*5,mtx);
		glMultMatrixf(mtx);
		//glMultMatrixf(Matrix44::scale(1,1,Math::sin(rincr)+1));
		delete[] mtx;
		glColor3f(1,1,0);
		Viz::render(P);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3fv(P);
		glEnd();
		glColor3f(0,1,0);
		//glutWireCube(1);
		glutWireCone(1,1,8,8);
		glPushMatrix();
		glTranslatef(0,0,1);
		glScalef(0.5,0.5,2);
		//glutWireCube(1);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		glColor3f(1,1,0);
		glMultMatrixf(Matrix44::rotate(AXIS,rincr*5));
		glutWireCone(1,1,8,8);
		glPopMatrix();
		//////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		*/

		ImplicitLine2 il(Vector2(1,2),Vector2(Math::cos(rincr),Math::sin(rincr)));
		il.n.normalize();
		ParametricLine2 pl(Vector2(-3,-1),Vector2(0,3));
		Viz::setColor(1,0,0);
		Viz::render(il);
		Viz::setColor(1,1,0);
		Viz::render(pl);

		Viz::setColor(1,1,1);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2fv(pl(0));
		glVertex2fv(pl(0.5));
		glVertex2fv(pl(1));
		glVertex2fv(pl(2));
		glEnd();

		glPointSize(10);
		Viz::setColor(1,0,1);
		Intersection::ResultSet2D r2d;
		if(Intersection::test(il,pl,&r2d))
		{
			glBegin(GL_POINTS);
			glVertex2fv(r2d.points[0]);
			glEnd();
		}

		glDisable(GL_DEPTH_TEST);
		glLineWidth(3);
		//drawAxes();
		glEnable(GL_DEPTH_TEST);
		glLineWidth(1);
		glColor3f(0.35,0.35,0.35);
		drawGrid();

		glPopMatrix(); //pop the modelview matrix

		glColor3f(1,1,1);
		drawString(30,30,Util::toString(m_cam.fov).c_str());

		glFlush();
		glutSwapBuffers();
	}
	void keyboard(unsigned char key,int x,int y)
	{
		Application::keyboard(key,x,y);
		switch(key)
		{
		case 'i':
			inverse = !inverse;
			break;
		case 'c':
			persp = !persp;
			if(persp) cout << "Using Arkhe perspective projection" << endl;
			else cout << "Using Arkhe orthographic projection" << endl;
			setView();
			break;
		case '+':
			m_cam.fov++;
			setView();
			cout << m_cam.fov << endl;
			break;
		case '-':
			m_cam.fov--;
			setView();
			cout << m_cam.fov << endl;
			break;
		case 'w':
			pos.z += 0.1;
			break;
		case 's':
			pos.z -= 0.1;
			break;
		case 'd':
			pos.x += 0.1;
			break;
		case 'a':
			pos.x -= 0.1;
			break;
		case 't':
			pos.y += 0.1;
			break;
		case 'g':
			pos.y -= 0.1;
			break;
		case 'p':
			pos2.x -= 0.1;
			break;
		case '[':
			pos2.x += 0.1;
			break;
		case 'l':
			pos2.y -=0.1;
			break;
		case ';':
			pos2.y += 0.1;
			break;
		case ',':
			pos2.z -= 0.1;
			break;
		case '.':
			pos2.z += 0.1;
			break;
		case 'j':
			rincr += 0.1;
			break;
		default:
			break;
		}
	}
};

#define USE_APP

int main(int argc,char **argv)
{

	try
	{
#ifndef USE_APP
		unsigned int M = 3;
		unsigned int N = 100000000; //100000000;
		double meanTime = 0.0;
		for(unsigned int i=0; i<M; i++)
		{
			Timer t;
			t.start();
			for(unsigned int j=0; j<N; j++)
			{
				Matrix44 P,Q;
				P = Matrix44
				(
					Math::sin(j) + 3,0,0,0,
					0,Math::cos(i) + 3,0,0,
					0,0,Math::sin(i) + 3,0,
					0,0,0,Math::cos(i) + 3
				);
				Q = P.inverse();
			}
			t.stop();
			double et = t.elapsed(Timer::SECONDS);
			cout << N << " 4x4 inverse calculations took " << et << " seconds." << endl;
			meanTime += et;
		}
		cout << "Mean compute time was " << meanTime / (double)M << " seconds." << endl;
#else
		Application::run(new testApp,argc,argv);

		cout << Vector3::UNIT_X.cross(Vector3::UNIT_Y) << endl;

		Matrix44 TM(1,0,0,2,0,1,0,3,0,0,1,4,0,0,0,1);
		cout << TM << endl;
		cout << TM.inverse() << endl;
		cout << TM * TM.inverse() << endl;

		//if(!Intersection::test(Ray3(),AABB())) cout << "Is weking!" << endl;
		//Intersection::ResultSet3D resultSet;
		//if(Intersection::test(Ray3(),AABB(),&resultSet)) cout << "We want to compute the result of the intersection..." << endl;
		//else cout << "We only care about whether we have an intersection or not..." << endl;

		cout << "Testing atan2" << endl;
		cout << Math::aTan2(3,1) << endl;
		cout << Math::aTan2(0,0) << endl;

		cout << "Cross..." << endl;
		cout << Vector3(-1,0,0).cross(Vector3(0,1,0)) << endl;
		cout << Vector3(1,0,0).cross(Vector3(0,1,0)) << endl;
		cout << Vector3(0,1,0).cross(Vector3(1,0,0)) << endl;
		cout << Vector3(1,0,0).cross(Vector3(0,0,1)) << endl;
		cout << Vector3(0,0,1).cross(Vector3(1,0,0)) << endl;
		cout << "--------------------" << endl;
		cout << Vector3(1,0,0).cross(Vector3(0,0,-1)) << endl;
		cout << Vector3(1,0,0).cross(Vector3(0,0,1)) << endl;

		cout << endl << endl;
		float *mtx = new float[16];
		Vector3 pt(1,0,0);
		Vector3 axis(1,1,1);
		rotateAxis(pt,axis,Math::PI/4,mtx);
		//
		delete[] mtx;

		while(!kbhit()) ;

		/*Matrix33 Rx,Ry,Rz,Rxyz;
		Rx = Matrix33::rotateX(23);
		Ry = Matrix33::rotateY(-10);
		Rz = Matrix33::rotateZ(33);
		Rxyz = Matrix33::rotate(23,-10,33);
		cout << "Rx" << endl;
		cout << Rx << endl;
		cout << "Ry" << endl;
		cout << Ry << endl;
		cout << "Rz" << endl;
		cout << Rz << endl;
		cout << "RxRyRz" << endl;
		cout << Rx*Ry*Rz << endl;
		//cout << "Rz*Ry*Rx" << endl;
		//cout << Rz*Ry*Rx << endl;
		cout << "Rxyz" << endl;
		cout << Rxyz << endl;
		cout << endl << endl;
		cout << Matrix33::rotateZ(Math::PI/4) << endl;
		cout << Matrix33::rotate(Vector3::UNIT_Z,Math::PI/4) << endl;

		cout << endl << endl;

		Matrix44 look = Matrix44::lookAt(Vector3(1,1,1),Vector3(0,0,0),Vector3(0,1,0));
		cout << look << endl;*/

#endif //USE_APP
	}
	catch(std::exception &e)
	{
		cerr << e.what() << endl;
	}

	return 0;
}

/*
		Application::run(new testApp,argc,argv);

		Timer timer;
		timer.start();
		Math::UNIT_TEST();
		timer.stop();
		double tval = timer.elapsedTime();
		cout << "Unit test took: " << tval << " microseconds." << endl;
		cout << "Unit test took: " << tval * 0.001 << " milliseconds." << endl;
		cout << "Unit test took: " << tval * 0.000001 << " seconds." << endl;

		FPSTimer fpsTimer;
		cout << "Testing FPS timer:" << endl;
		for(int i=0; i<10; i++)
		{
			cout << fpsTimer.update() << endl;
		}

		Vector2 v1(2,4),v2(1,1),v3(Vector2::UNIT_X);
		cout << v1 << endl;
		cout << v2 << endl;
		cout << v3 << endl;
		Real *r_ptr = v1;
		cout << r_ptr[0] << ' ' << r_ptr[1] << endl;
		cout << v3[0] << ' ' << v3[1] << endl;
		cout << v1 + v2 << endl;
		cout << v2 - v1 << endl;
		cout << -v1 << endl;
		v1.x = Math::EPSILON;
		v1.y = Math::EPSILON;
		v2.x = Math::PI;
		v2.y = Math::EPSILON/2;
		v3.x = Math::EPSILON/2;
		v3.y = Math::EPSILON/2;
		if(v1.isZero()) cout << "v1: " << v1 << " is zero..." << endl;
		if(v2.isZero()) cout << "v2: " << v2 << " is zero..." << endl;
		if(v3.isZero()) cout << "v3: " << v3 << " is zero..." << endl;
		cout << "cleaning v3..." << endl;
		v3.clean();
		if(v3.isZero()) cout << "v3: " << v3 << " is zero..." << endl;
		//
		v1.x = v1.y = 1;
		v1.normalize();
		cout << v1 << endl;
		cout << v1.perpendicularTo(Vector2::UNIT_X) << endl;
		cout << v1.parallelTo(Vector2::UNIT_X) << endl;
		v2.x = 3;
		v2.y = 1;
		v2.normalize();
		cout << v2 << endl;
		cout << v2.perpendicularTo(Vector2::UNIT_X) << endl;
		cout << v2.parallelTo(Vector2::UNIT_X) << endl;
		cout << "NEW" << endl;
		v3.x = 1;
		v3.y = 0.35;
		Vector2 w(0.5,-0.8);
		w.normalize();
		cout << "W NORM: " << w << endl;
		cout << "V ONTO: " << v3 << endl;
		cout << v3.perpendicularTo(w,true) << endl;
		cout << v3.parallelTo(w,true) << endl;

		Vector3 a(1,1,1),b(2,1,0.5);
		cout << a << endl;
		cout << b << endl;
		cout << a.cross(b) << endl;

		cout << sizeof(Vector2) << endl;
		cout << sizeof(Vector3) << endl;

		cout << a._align << endl;
		cout << a._align[0] << endl;
		cout << a._align[1] << endl;
		cout << a._align[2] << endl;

		Matrix22 M(1,2,3,4),N(2,2,3,3),P(2,5,2,5);
		cout << M << endl;
		cout << N << endl;
		cout << P << endl;
		cout << "In matrix M:" << endl;
		cout << "Column 1 = " << M(0,0) << " " << M(1,0) << endl;
		cout << "Column 2 = " << M(0,1) << " " << M(1,1) << endl;
		cout << "In matrix N:" << endl;
		cout << "Column 1 = " << N(0,0) << " " << N(1,0) << endl;
		cout << "Column 2 = " << N(0,1) << " " << N(1,1) << endl;
		cout << "In matrix P:" << endl;
		cout << "Column 1 = " << P(0,0) << " " << P(1,0) << endl;
		cout << "Column 2 = " << P(0,1) << " " << P(1,1) << endl;

		M = Matrix22(1,3,2,4);
		N = Matrix22(1,2,3,4);
		cout << "Matrix M:" << endl;
		cout << M << endl;
		cout << "Matrix N:" << endl;
		cout << N << endl;
		cout << "MxN:" << endl;
		cout << M*N << endl;
		cout << "NxM:" << endl;
		cout << N*M << endl;

		cout << "Rotation matrix, 45 degrees:" << endl;
		cout << Matrix22::rotate(Angle(45,Angle::DEGREES)) << endl;
		cout << Matrix22::rotate(Math::PI/4)(Vector2(1,1)) << endl;
		cout << Matrix22::rotate(Math::PI/4)(Vector2(Math::sin(Math::PI/4),Math::sin(Math::PI/4))) << endl;

		M = Matrix22(1,2,3,4);
		cout << "Matrix M and M transpose:" << endl;
		cout << M << endl;
		cout << M.transpose() << endl;

		Vector3 a1(1.5,0.4,-0.95);
		Vector3 a2(0.86,0.89,-.75);
		Vector3 a3(-0.84,-0.3,1.3);
		Vector3 b1,b2,b3;
		Vector3::orthogonalize
		(
			a1,a2,a3,
			b1,b2,b3,true
		);
		cout << b1 << endl;
		cout << b2 << endl;
		cout << b3 << endl;
		std::string mel = "xform -ws -t ";
		mel += Util::toString((Real *)a1,3);
		mel += " src_basis|b1;";
		mel += "xform -ws -t ";
		mel += Util::toString((Real *)a2,3);
		mel += " src_basis|b2;";
		mel += "xform -ws -t ";
		mel += Util::toString((Real *)a3,3);
		mel += " src_basis|b3;";
		mel += "xform -ws -t ";
		mel += Util::toString((Real *)b1,3);
		mel += " tgt_basis|b1;";
		mel += "xform -ws -t ";
		mel += Util::toString((Real *)b2,3);
		mel += " tgt_basis|b2;";
		mel += "xform -ws -t ";
		mel += Util::toString((Real *)b3,3);
		mel += " tgt_basis|b3;";
		cout << mel << endl;
		Util::writeFile(mel,"onb.mel");

		{
			cout << Matrix22(2,0,0,4) << endl;
			cout << Matrix22(2,0,0,4).inverse() << endl;
		}
*/