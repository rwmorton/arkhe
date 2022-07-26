#ifndef __MARCHING_CUBES_GRID_APP_H__
#define __MARCHING_CUBES_GRID_APP_H__

//arkhe includes
#include <opengl/ArkheApplication.h>
using namespace arkhe::opengl;

#include "implicit_functions.h"

class marchingCubesGridApp : public Application
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
	MarchingCubesMesh *mesh;
	int triIndex;
	AABB aabb;
	int implicitIndex;
	bool shade;
	bool wire;
	Grid3D grid;
	int i,j,k;
	bool vox;
	Real isoLevel;
};

void marchingCubesGridApp::initialize()
{
	m_camera.pos = Vector3(0,0,15);
	m_camera.a = (Real)m_window.width/(Real)m_window.height;
	m_camera.n = 0.001;
	m_camera.f = 100;
	m_camera.pan = m_camera.pitch = 0;
	m_camera.sensitivity = 0.1; //0.01;
	m_camera.strictOrtho = false;
	m_camera.zoom = 2;
	//
	bound = 5;
	res = 16;
	aabb = AABB(Vector3(-bound),Vector3(bound));
	grid.setBounds(aabb);
	grid.setResolution(res,res,res);
	grid.build();
	i = j = k = 0;
	mesh = MarchingCubesGrid::build(grid,implicit,gradient);
	cout << "generated vertex normals from gradient function" << endl;
	cout << mesh->numTriangles() << " triangles were generated" << endl;
	cout << "computing neigbhors for mesh" << endl;
	//mesh->computeNeighbors();
	cout << "neighbors computed" << endl;

	triIndex = 0;
	implicitIndex = 0;
	shade = true;
	wire = false;
	vox = false;
	isoLevel = 0;//-1.4;//0;
}

void marchingCubesGridApp::uninitialize()
{
	//if(mesh) delete mesh;
}

void marchingCubesGridApp::setView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Matrix44::perspective(m_camera.fov,m_camera.a,m_camera.n,m_camera.f));
}

void marchingCubesGridApp::update()
{
	Application::updateCamera();

	//

	glutPostRedisplay();
}

void marchingCubesGridApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push the modelview matrix
	glMultMatrixf(m_camera.worldToView());

	//draw aabb
	glColor3f(0,0,0);
	aabb.render();

	//render grid
	//grid.render();

	Voxel voxel = grid(i,j,k);
	if(vox)
	{
		//draw current voxel
		glLineWidth(1);
		glColor3f(1,0,0);
		Viz::renderCube(voxel.vertices);
		//draw neighbors
		glLineWidth(1);
		glColor3f(0,1,0);
		//std::vector<Voxel *> neighbors = grid.getNeighborsTo(i,j,k);
		std::vector<Voxel *> neighbors = grid.getNeighborsTo(voxel.id);
		std::vector<Voxel *>::iterator it = neighbors.begin();
		while(it != neighbors.end())
		{
			Viz::renderCube((*it)->vertices);
			it++;
		}
		glLineWidth(1);
	}

	//draw current triangle by index
	glColor3f(1,0,1);
	glBegin(GL_TRIANGLES);
	glVertex3fv(mesh->triangles.at(triIndex)->v0);
	glVertex3fv(mesh->triangles.at(triIndex)->v1);
	glVertex3fv(mesh->triangles.at(triIndex)->v2);
	glEnd();
	//draw neighbors
	glColor3f(1,0.5,0.5);
	/*if(mesh->triangles.at(triIndex)->en0)
	{
		glVertex3fv(mesh->triangles.at(triIndex)->en0->v0);
		glVertex3fv(mesh->triangles.at(triIndex)->en0->v1);
		glVertex3fv(mesh->triangles.at(triIndex)->en0->v2);
	}
	if(mesh->triangles.at(triIndex)->en1)
	{
		glVertex3fv(mesh->triangles.at(triIndex)->en1->v0);
		glVertex3fv(mesh->triangles.at(triIndex)->en1->v1);
		glVertex3fv(mesh->triangles.at(triIndex)->en1->v2);
	}
	if(mesh->triangles.at(triIndex)->en2)
	{
		glVertex3fv(mesh->triangles.at(triIndex)->en2->v0);
		glVertex3fv(mesh->triangles.at(triIndex)->en2->v1);
		glVertex3fv(mesh->triangles.at(triIndex)->en2->v2);
	}*/

	//draw triangles belonging to this voxel
	bool empty = true;
	if(mesh->voxelTriangles[voxel.id].size() > 0) empty = false;
	if(!empty)
	{
		glLineWidth(5);
		glColor3f(1,0,0);
		std::vector<MarchingCubesMesh::Tri *>::iterator iter = mesh->voxelTriangles[voxel.id].begin();
		while(iter != mesh->voxelTriangles[voxel.id].end())
		{
			//render triangles
			glBegin(GL_LINE_LOOP);
			glVertex3fv((*iter)->v0);
			glVertex3fv((*iter)->v1);
			glVertex3fv((*iter)->v2);
			glEnd();
			iter++;
		}
		//draw neighboring triangles
		std::vector<Voxel *> voxNeighbors = grid.getNeighborsTo(i,j,k);
		std::vector<Voxel *>::iterator voxIter = voxNeighbors.begin();
		glColor3f(0,1,0);
		while(voxIter != voxNeighbors.end())
		{
			iter = mesh->voxelTriangles[(*voxIter)->id].begin();
			while(iter != mesh->voxelTriangles[(*voxIter)->id].end())
			{
				//render triangles
				glBegin(GL_LINE_LOOP);
				glVertex3fv((*iter)->v0);
				glVertex3fv((*iter)->v1);
				glVertex3fv((*iter)->v2);
				glEnd();
				iter++;
			}
			voxIter++;
		}
	}
	glLineWidth(1);

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

void marchingCubesGridApp::keyboard(unsigned char key,int x,int y)
{
	Application::keyboard(key,x,y);
	switch(key)
	{
	case 'v':
		vox = !vox;
		break;
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
		else res--;
		cout << "res = " << res << endl;
		break;
	case 'i':
		implicitIndex++;
		if(implicitIndex >= IMPLICIT_COUNT) implicitIndex = 0;
		//rebuild
		if(mesh) delete mesh;
		if(implicitIndex == 0) mesh = MarchingCubesGrid::build(grid,implicit,gradient,isoLevel);
		else if(implicitIndex == 1) mesh = MarchingCubesGrid::build(grid,implicit2,gradient2,isoLevel);
		else if(implicitIndex == 2) mesh = MarchingCubesGrid::build(grid,implicit3,gradient3,isoLevel);
		else if(implicitIndex == 3) mesh = MarchingCubesGrid::build(grid,implicit4,gradient4,isoLevel);
		else if(implicitIndex == 4) mesh = MarchingCubesGrid::build(grid,implicit5,gradient5,isoLevel);
		else if(implicitIndex == 5) mesh = MarchingCubesGrid::build(grid,implicit6,gradient6,isoLevel);
		cout << mesh->numTriangles() << " triangles were generated" << endl;
		triIndex = 0; //reset
		break;
	case 'r':
		if(mesh) delete mesh;
		grid.setResolution(res,res,res);
		grid.build();
		i = j = k = 0;
		if(implicitIndex == 0) mesh = MarchingCubesGrid::build(grid,implicit,gradient,isoLevel);
		else if(implicitIndex == 1) mesh = MarchingCubesGrid::build(grid,implicit2,gradient2,isoLevel);
		else if(implicitIndex == 2) mesh = MarchingCubesGrid::build(grid,implicit3,gradient3,isoLevel);
		else if(implicitIndex == 3) mesh = MarchingCubesGrid::build(grid,implicit4,gradient4,isoLevel);
		else if(implicitIndex == 4) mesh = MarchingCubesGrid::build(grid,implicit5,gradient5,isoLevel);
		else if(implicitIndex == 5) mesh = MarchingCubesGrid::build(grid,implicit6,gradient6,isoLevel);
		cout << mesh->numTriangles() << " triangles were generated" << endl;
		triIndex = 0; //reset
	case '.':
		triIndex++;
		if(triIndex >= mesh->numTriangles()) triIndex = mesh->numTriangles()-1;
		cout << "triIndex = " << triIndex << endl;
		break;
	case ',':
		triIndex--;
		if(triIndex < 0) triIndex = 0;
		cout << "triIndex = " << triIndex << endl;
		break;
	case '6':
		i++;
		if(i >= res) i = res-1;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case '4':
		i--;
		if(i < 0) i = 0;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case '8':
		j++;
		if(j >= res) j = res-1;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case '5':
		j--;
		if(j < 0) j = 0;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case 'n':
		k++;
		if(k >= res) k = res-1;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case 'h':
		k--;
		if(k < 0) k = 0;
		cout << "(i,j,k) = (" << i << ',' << j << ',' << k << ')' << endl;
		break;
	case 'q':
		isoLevel += 0.1;
		cout << isoLevel << endl;
		break;
	case 'a':
		isoLevel -= 0.1;
		cout << isoLevel << endl;
		break;
	default:
		break;
	}
}

#endif //__MARCHING_CUBES_GRID_APP_H__
