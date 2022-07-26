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

class Circle
{
public:
	Circle() : m_vertices(0),m_normals(0) { build(16); }
	~Circle()
	{
		delete[] m_vertices;
		delete[] m_normals;
	}
	void draw()
	{
		glColor3f(1,0,0);
		//draw vertices
		glBegin(GL_POINTS);
		for(unsigned int i=0; i<m_N; i++)
		{
			glVertex2f(m_vertices[i].x,m_vertices[i].y);
		}
		glEnd();
		//draw edges
		glBegin(GL_LINE_LOOP);
		for(unsigned int i=0; i<m_N; i++)
		{
			glVertex2f(m_vertices[i].x,m_vertices[i].y);
		}
		glEnd();
		//draw normals
		glColor3f(0,0,1);
		for(unsigned int i=0; i<m_N; i++)
		{
			drawVector(m_normals[i],m_vertices[i].x,m_vertices[i].y);
		}
	}
	void build(const unsigned int N)
	{
		m_N = N;
		if(m_vertices) delete[] m_vertices;
		if(m_normals) delete[] m_normals;
		m_vertices = new Vector2[m_N];
		m_normals = new Vector2[m_N];
		Real a = Math::ZERO;
		Real a_incr = Math::TWO_PI / (Real)m_N;
		//build circle in object space
		for(unsigned int i=0; i<m_N; i++,a+=a_incr)
		{
			//compute vertex positions
			Real x = Math::cos(a);
			Real y = Math::sin(a);
			m_vertices[i].x = x;
			m_vertices[i].y = y;
			//compute normals
			m_normals[i].x = x;
			m_normals[i].y = y;
			m_normals[i].normalize();
		}
	}
	void transform(const Matrix22 &m)
	{
		for(unsigned int i=0; i<m_N; i++)
		{
			m_vertices[i] = m(m_vertices[i]);
			m_normals[i] = m(m_normals[i],Matrix22::NORMAL,false);
		}
	}
	void renormalize()
	{
		for(unsigned int i=0; i<m_N; i++) m_normals[i].normalize();
	}
private:
	void drawVector(const Vector2 &v,Real xPos,Real yPos)
	{
		glBegin(GL_LINES);
		glVertex2f(xPos,yPos);
		glVertex2f(v.x+xPos,v.y+yPos);
		glEnd();
	}
	unsigned int m_N;
	Vector2 *m_vertices;
	Vector2 *m_normals;
};

class testApp : public Application
{
public:
	testApp() {}
private:
	//
	//CAMERA TEST
	bool m_mouseLeftDown,m_mouseRightDown,m_mouseMiddleDown;
	float m_camDistance,m_camDistanceY;
	float m_camPan,m_camPitch;
	float m_camSensitivity;
	double m_zNear,m_zFar;
	Circle m_circle;
	unsigned int m_count;
	Matrix22 m_matrix;
	Real x1,y1,x2,y2;
	Real a,a_incr;
	//
protected:
	void initialize()
	{
		cout << "testApp::initialize" << endl;
		glClearColor(0.55f,0.65f,0.575f,1.0f);
		m_mouseLeftDown = m_mouseRightDown = m_mouseMiddleDown = false;
		m_camDistance = -3;
		m_camDistanceY = 0;
		m_camPan = m_camPitch = 0;
		m_camSensitivity = 0.1f;
		m_zNear = 0.1;
		m_zFar = 20.0;
		m_count = 100;
		m_circle.build(m_count);
		//m_matrix = Matrix22::scale(3);
		//m_matrix = Matrix22::shear(1,0);
		x1 = y2 = 1;
		x2 = y1 = 0;
		m_matrix = Matrix22(x1,x2,y1,y2);
		a = 0.0;
		a_incr = 0.01;
	}
	void uninitialize() { cout << "testApp::uninitialize" << endl; }
	void update()
	{
		a += a_incr;
		glutPostRedisplay();
	}
	void setView()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,(double)m_width/(double)m_height,m_zNear,m_zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix(); //push the modelview matrix

		//gluLookAt(3,3,3,0,0,0,0,1,0);
		//glMultMatrixf(Matrix44::lookAt(Vector3(0,0,-3),Vector3::ZERO,Vector3(0,1,0)));
		//glTranslatef(0,m_camDistanceY,m_camDistance);

		
#define MINE
#ifdef MINE
		//glMultMatrixf(Matrix44::translate(0,0,-7));
		//glMultMatrixf(Matrix44::rotateZ(Angle(45,Angle::DEGREES)));
		//glMultMatrixf(Matrix44::scale(2,0.5,0.5));

		Matrix44 T = Matrix44::translate(0,0,-7);
		Matrix44 R = Matrix44::rotateZ(Angle(45,Angle::DEGREES));
		Matrix44 S = Matrix44::scale(2,0.5,0.5);
		Matrix44 M = T(R(S)); //S(R(T));
		glMultMatrixf(M);
#else
		glTranslatef(0,0,-7);
		glRotatef(45,0,0,1);
		glScalef(2,0.5,0.5);
#endif //MINE
		
		//glRotatef(m_camPitch,1,0,0);
		//glRotatef(m_camPan,0,1,0);

		//glTranslatef(3,3,3);

		glPushMatrix();
		glTranslatef(5,5,1);
		glutWireSphere(1,8,8);
		glPopMatrix();

		glColor3f(1,0.75f,0.1f);
		//glutWireCube(1);
		//glutWireSphere(1,8,8);

		//draw circle
		//m_matrix = Matrix22(x1,x2,y1,y2);
		//m_matrix = Matrix22::shear(Math::sin(a)*0.5,Math::cos(a));
		//m_matrix *= Matrix22::rotate(a);
		m_circle.build(m_count); //NOT EFFICIENT I KNOW, BUT IM LAZY...
		//m_circle.transform(m_matrix);
		m_circle.renormalize();
		m_circle.draw();

		glPopMatrix(); //pop the modelview matrix

		glColor3f(0,0,0);
		drawString(4,20,(std::string("Camera distance: ") + Util::toString(m_camDistance)).c_str());
		drawString(4,40,(std::string("Camera heading: ") + Util::toString(m_camPan)).c_str());
		drawString(4,60,(std::string("Camera pitch: ") + Util::toString(m_camPitch)).c_str());
		drawString(4,80,(std::string("z near/far: ") + Util::toString(m_zNear) + std::string(", ") + Util::toString(m_zFar)).c_str());

		glFlush();
		glutSwapBuffers();
	}
	void mouse(int button,int state,int x,int y)
	{
		m_mouseX = x;
		m_mouseY = y;

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
		else if(button == GLUT_RIGHT_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
				m_mouseRightDown = true;
			}
			else if(state == GLUT_UP)
			{
				m_mouseRightDown = false;
			}
		}
		else if(button == GLUT_MIDDLE_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
				m_mouseMiddleDown = true;
			}
			else if(state == GLUT_UP)
			{
				m_mouseMiddleDown = false;
			}
		}
	}
	void mouseDrag(int x,int y)
	{
		if(m_mouseLeftDown)
		{
			m_camPan += (x - m_mouseX);
			m_camPitch += (y - m_mouseY);
			m_mouseX = x;
			m_mouseY = y;
		}
		if(m_mouseRightDown)
		{
			m_camDistance -= (y - m_mouseY) * m_camSensitivity;
			m_mouseY = y;
		}
		if(m_mouseMiddleDown)
		{
			m_camDistanceY -= (y - m_mouseY) * 0.005f;
			m_mouseY = y;
		}
	}
	void keyboard(unsigned char key,int x,int y)
	{
		Application::keyboard(key,x,y);
		switch(key)
		{
		case 'f':
			m_camDistance = -3;
			m_camDistanceY = 0;
			break;
		case 'r':
			m_camDistance = -3;
			m_camDistanceY = 0;
			m_camPan = m_camPitch = 0;
			m_camSensitivity = 0.1f;
			break;
		case '+':
			m_circle.build(++m_count);
			break;
		case '-':
			m_circle.build(--m_count);
			break;
		case 't':
			m_circle.transform(m_matrix);
			cout << m_matrix << endl;
			break;
		case 'i':
			m_circle.transform(m_matrix.inverse());
			cout << m_matrix.inverse() << endl;
			cout << m_matrix * m_matrix.inverse() << endl;
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

	while(!kbhit()) ;

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