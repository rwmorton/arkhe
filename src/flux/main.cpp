#pragma warning(disable : 4244)

/** THIS MUST BE INCLUDED FIRST **/
//GLEW include
#include <GL/glew.h>
//freeglut include
#include <GL/freeglut.h>

//local includes
#include "Base.h"
#include "Film.h"
#include "Shape.h"
#include "Camera.h"
#include "Scene.h"

//std includes
#include <iostream>
#include <conio.h>
using namespace std;

Film film(800,600);

void updateCallback()
{
	////////////////////////////////////////////
	///////////// UPDATE SCENE RENDERING ///////
	float r,g,b;
	for(int i=0;i<film.getWidth(); i++)
	{
		for(int j=0; j<film.getHeight(); j++)
		{
			r = (float)i / film.getWidth();
			g = (float)j / film.getHeight();
			b = 0.0f;
			film.setPixel(i,j,r,g,b);
		}
	}
	////////////////////////////////////////////
	glutPostRedisplay();
}

void resizeCallback(int w,int h) { glutReshapeWindow(film.getWidth(),film.getHeight()); } //do not allow resizing }

void displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluOrtho2D(0,film.getWidth(),0,film.getHeight());
	//draw the film
	glDrawPixels(film.getWidth(),film.getHeight(),GL_RGB,GL_FLOAT,film.getBuffer());
	glFlush();
	glutSwapBuffers();
}

int main(int argc,char **argv)
{
	try
	{
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
		glutInitWindowSize(film.getWidth(),film.getHeight());
		glutInitWindowPosition(0,0);
		glutCreateWindow(argv[0]);
		//register GLUT callback functions
		glutIdleFunc(&updateCallback);
		glutDisplayFunc(&displayCallback);
		glutReshapeFunc(&resizeCallback);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS); //freeglut implementation only
		glutMainLoop(); //enter main loop
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	//Sphere s(Matrix::translate(0,0,-5),1,Material(1,0,0));
	Sphere s(Matrix::IDENTITY,1,Material(1,0,0));
	Ray r(Vector(0,0,6),Vector(0,0,-1));

	if(s.shadowHit(r)) cout << "Got a hit..." << endl;
	else cout << "No hit..." << endl;

	cout << r << endl;
	cout << s.m_worldToLocal(r) << endl;

	while(!kbhit()) ;

	return 0;
}
