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
using namespace arkhe::foundation;
using namespace arkhe::opengl;

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

struct Camera
{
	Camera() : pos(Vector3::ZERO),look(Vector3::ZERO),up(Vector3::UNIT_Y),keyIncr(0.1),pan(0),pitch(0),fov(Math::PI/4),a(1),n(0.1),f(30),sensitivity(0.1) {}
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
		Matrix44 L = Matrix44::lookAt(Vector3(0,0,1),Vector3::ZERO,up); //TRL
		return Matrix44::translate(-pos.x,pos.y,pos.z) * Matrix44::rotateY(Angle(pan,Angle::DEGREES)) * Matrix44::rotateX(Angle(pitch,Angle::DEGREES)) * L;
	}
	const Matrix44 proj() const
	{
		return Matrix44::perspective(fov,a,n,f);
	}
	//view data
	Vector3 pos;
	Vector3 look;
	Vector3 up;
	Real keyIncr;
	Real pan,pitch;
	//projection data
	Real fov,a,n,f;
	//motion data
	Real prevX,prevY,curX,curY,deltaX,deltaY;
	Real sensitivity;
};

class testApp : public Application
{
public:
	testApp() {}
private:
	bool inverse;
	bool persp,world_persp;
	Real m_fov,m_fov_world_persp;
	Real rincr;
	Camera m_cam;
	bool m_mouseLeftDown;
protected:
	void initialize()
	{
		inverse = persp = world_persp = false;
		rincr = 0.0;
		m_fov = m_fov_world_persp = 45;
		cout << "testApp::initialize" << endl;
		m_cam.pos = Vector3(0,0,-5);
		m_cam.a = (Real)m_width/m_height;
		m_cam.pan = m_cam.pitch = 0;
		m_mouseLeftDown = false;
		glClearColor(0.55f,0.65f,0.575f,1.0f);
	}
	void uninitialize() { cout << "testApp::uninitialize" << endl; }
	void update()
	{
		glutPostRedisplay();
	}
	void resize(int w,int h)
	{
		Application::resize(w,h);
		m_cam.a = (Real)m_width/m_height;
	}
	void setView()
	{
		glViewport(0,0,m_width,m_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float aspect = (double)m_width / (double)m_height;
		float n = 0.1f;
		float f = 30.0f;

		//if(persp) glMultMatrixf(Matrix44::perspective(Angle(m_fov,Angle::DEGREES),aspect,n,f));
		//else gluPerspective(m_fov,aspect,n,f);
		glMultMatrixf(m_cam.proj());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix(); //push the modelview matrix

		/*//////////////////////////////////////////////////////
		glPushMatrix();

		float aspect = (float)m_width/m_height;
		glMultMatrixf(Matrix44::translate(m_width/2,m_height/2,0) * Matrix44::scale(m_width/2,(m_height/2)*aspect,1));

		glColor3f(0.5,0.8,0.7);
		Matrix44 TM2 = Matrix44::scale(4,1.4,1) * Matrix44::rotateZ(rincr) * Matrix44::scale(0.1,0.1,0.1);
		rincr+=0.001;
		drawCube(TM2);

		glPopMatrix();
		//////////////////////////////////////////////////////*/

		//glMultMatrixf(Matrix44::lookAt(Vector3(7,3,1),Vector3(0,0,0),Vector3(0,1,0)));
		glMultMatrixf(m_cam.worldToView());
		
		glColor3f(1,0.75f,0.1f);
		glutWireCube(1);

		glColor3f(0.5,0.8,0.7);
		Matrix44 TM = Matrix44::scale(2,0.5,0.25) * Matrix44::rotate(rincr,rincr,rincr);
		rincr+=0.001;
		drawCube(TM);

		glPushMatrix();
		glTranslatef(2,2,2);
		glColor3f(0.8,0.3,0);
		glutWireCube(0.25);
		glPopMatrix();

		drawAxes();

		////////////////// BEGIN CAM //////////////////
		glPushMatrix();

		Matrix44 viewToWorld = Matrix44::lookAt(Vector3(2,0.25,2),Vector3(0,0,0),Vector3(0,1,0));
		inverse ? glMultMatrixf(viewToWorld.inverse()) : glMultMatrixf(viewToWorld);
		drawCam();

		glColor3f(0.3,0.8,0);
		glPushMatrix();
		glTranslatef(0,0,-1);
		glutWireCube(0.5);
		glPopMatrix();

		glPopMatrix();
		////////////////// END CAM //////////////////

		////////////////// BEGIN PERSP //////////////////
		glPushMatrix();

		if(world_persp) glMultMatrixf(Matrix44::perspective(Angle(m_fov_world_persp,Angle::RADIANS),1.0,2.0,5.1));

		glTranslatef(0,0,-4);
		glColor3f(0.8,0.1,0.1);
		glutWireSphere(1,16,16);
		glColor3f(1,1,0);
		glTranslatef(0,0,-1.125);
		glutWireSphere(0.25,16,16);

		glPopMatrix();
		////////////////// END PERSP //////////////////

		glPopMatrix(); //pop the modelview matrix

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
		case 'p':
			persp = !persp;
			if(persp) cout << "Using Arkhe perspective projection" << endl;
			else cout << "Using GLU perspective projection" << endl;
			break;
		case '+':
			m_fov++;
			setView();
			cout << m_fov << endl;
			break;
		case '-':
			m_fov--;
			setView();
			cout << m_fov << endl;
			break;
		case 'w':
			m_cam.pos.z += m_cam.keyIncr;
			break;
		case 's':
			m_cam.pos.z -= m_cam.keyIncr;
			break;
		case 'd':
			m_cam.pos.x += m_cam.keyIncr;
			break;
		case 'a':
			m_cam.pos.x -= m_cam.keyIncr;
			break;
		default:
			break;
		}
	}
	void mouse(int button,int state,int x,int y)
	{
		m_mouseX = x;
		m_mouseY = y;
		m_cam.curX = x;
		m_cam.curY = y;

		if(button == GLUT_LEFT_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
				m_mouseLeftDown = true;
			}
			else if(state == GLUT_UP)
			{
				m_mouseLeftDown = false;
			}
		}
	}
	void mouseDrag(int x,int y)
	{
		if(m_mouseLeftDown)
		{
			m_cam.prevX = m_cam.curX;
			m_cam.prevY = m_cam.curY;
		
			m_cam.curX = x;
			m_cam.curY = y;
		
			m_cam.deltaX = m_cam.curX - m_cam.prevX;
			m_cam.deltaY = m_cam.prevY - m_cam.curY; //Y is opposite to X

			m_cam.pitch += m_cam.deltaY * m_cam.sensitivity;
			m_cam.pan -= m_cam.deltaX * m_cam.sensitivity;
		
			cout << m_cam.deltaX << "," << m_cam.deltaY << endl;
			cout << m_cam.pitch << "," << m_cam.pan << endl;
			cout << "----------------------------" << endl;
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

		//while(!kbhit()) ;

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