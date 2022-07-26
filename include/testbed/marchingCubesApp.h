#ifndef __MARCHING_CUBES_APP_H__
#define __MARCHING_CUBES_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
using namespace arkhe::opengl;

#include "implicit_functions.h"

class marchingCubesApp : public Application
{
public:
	//methods
	void initialize();
	void uninitialize();
	void setView();
	void update();
	void display();
	void keyboard(unsigned char key,int x,int y);
	//variables
	Real bound;
	int res;
	TriangleMesh *mesh;
	int triIndex;
	AABB aabb;
	int implicitIndex;
	bool shade;
	bool wire;
};

void marchingCubesApp::initialize()
{
	m_camera.pos = Vector3(0,0,15);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.05; //0.01;
	m_camera.strictOrtho = false;
	m_camera.zoom = 2;
	//
	bound = 3;
	res = 16;
	aabb = AABB(Vector3(-bound),Vector3(bound));
	mesh = MarchingCubes::build(aabb,res,implicit,gradient);

	triIndex = 0;
	implicitIndex = 0;
	shade = true;
	wire = false;
}

void marchingCubesApp::uninitialize()
{
	if(mesh) delete mesh;
}

void marchingCubesApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Matrix44::perspective(m_camera.fov,m_camera.a,m_camera.n,m_camera.f));
}

void marchingCubesApp::update()
{
	Application::updateCamera();

	//

	glutPostRedisplay();
}

void marchingCubesApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	//draw aabb
	glColor3f(0,0,0);
	aabb.render();

	if(shade)
	{
		float spec[] = {0.25,0.25,0.25,1}; //{0.8,0.4,0,1};
		float shin[] = {5};
		//float lpos[] = {xVal,yVal,10,0};
		Vector3 pos = m_camera.worldToView().inverse()(m_camera.pos,Matrix44::TransformAs::POINT);
		float lpos[] = {pos.x,pos.y,pos.z,0};
		float lcol[] = {1,1,1,1}; //{1,0.8,0,1};
		float amb[] = {0.1f,0.1f,0.1f,1.0f};
		glShadeModel(GL_SMOOTH);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shin);
		glLightfv(GL_LIGHT0,GL_POSITION,lpos);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,lcol);
		glLightfv(GL_LIGHT0,GL_SPECULAR,lcol);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
	}

	glColor3f(1,0.8,0);
	if(mesh) mesh->render(shade);

	if(shade) glDisable(GL_LIGHTING);

	if(shade && wire)
	{
		glLineWidth(3);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0,0,0);
		if(mesh) mesh->render(false);
		glLineWidth(1);
	}

	//draw grid
	glColor3f(0.8,0.8,0.8);
	drawGrid(1);

	//draw axes
	{
		glLineWidth(3);
		Matrix33 m = Matrix33::IDENTITY;
		m.render();
		glLineWidth(1);
	}

	glPopMatrix(); //pop modelview matrix

	glFlush();
	glutSwapBuffers();
}

void marchingCubesApp::keyboard(unsigned char key,int x,int y)
{
	Application::keyboard(key,x,y);
	switch(key)
	{
	case 'z':
		shade = !shade;
		break;
	case 'x':
		wire = !wire;
		break;
	case '+':
		res += 1;
		cout << "res = " << res << endl;
		break;
	case '-':
		if((int)res-1 < 0) res = 0;
		cout << "res = " << res << endl;
		break;
	case 'i':
		implicitIndex++;
		if(implicitIndex >= IMPLICIT_COUNT) implicitIndex = 0;
		//rebuild
		if(mesh) delete mesh;
		if(implicitIndex == 0) mesh = MarchingCubes::build(aabb,res,implicit,gradient);
		else if(implicitIndex == 1) mesh = MarchingCubes::build(aabb,res,implicit2,gradient2);
		else if(implicitIndex == 2) mesh = MarchingCubes::build(aabb,res,implicit3,gradient3);
		else if(implicitIndex == 3) mesh = MarchingCubes::build(aabb,res,implicit4,gradient4);
		else if(implicitIndex == 4) mesh = MarchingCubes::build(aabb,res,implicit5,gradient5);
		else if(implicitIndex == 5) mesh = MarchingCubes::build(aabb,res,implicit6,gradient6);
		cout << mesh->numTriangles() << " triangles were generated" << endl;
		triIndex = 0; //reset
		break;
	case 'r':
		if(mesh) delete mesh;
		if(implicitIndex == 0) mesh = MarchingCubes::build(aabb,res,implicit,gradient);
		else if(implicitIndex == 1) mesh = MarchingCubes::build(aabb,res,implicit2,gradient2);
		else if(implicitIndex == 2) mesh = MarchingCubes::build(aabb,res,implicit3,gradient3);
		else if(implicitIndex == 3) mesh = MarchingCubes::build(aabb,res,implicit4,gradient4);
		else if(implicitIndex == 4) mesh = MarchingCubes::build(aabb,res,implicit5,gradient5);
		else if(implicitIndex == 5) mesh = MarchingCubes::build(aabb,res,implicit6,gradient6);
		cout << mesh->numTriangles() << " triangles were generated" << endl;
		triIndex = 0; //reset
	case '.':
		triIndex++;
		if(triIndex >= mesh->numTriangles()) triIndex = mesh->numTriangles()-1;
		//cout << "triagle index = " << triIndex << " and has " << mesh->triangles.at(triIndex).neighbors.size() << " neighbors" << endl;
		break;
	case ',':
		triIndex--;
		if(triIndex < 0) triIndex = 0;
		//cout << "triagle index = " << triIndex << " and has " << mesh->triangles.at(triIndex).neighbors.size() << " neighbors" << endl;
		break;
	default:
		break;
	}
}

#endif //__MARCHING_CUBES_APP_H__
