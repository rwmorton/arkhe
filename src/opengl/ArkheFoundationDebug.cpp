//arkhe local includes (opengl)
#include "ArkheGL.h"
#include "ArkheViz.h"
using namespace arkhe::opengl;

//arkhe includes (foundation)
#include <foundation/ArkheFoundation.h>

namespace arkhe
{

namespace foundation
{

//render 3d vector
void Vector3::render(const Vector3 &p)
{
	Real len = length();
	//form the basis
	Vector3 u = *this;
	u.normalize();
	Vector3 v = u;
	int index;
	v.getMinAbsComponent(index);
	v[index] = 0;
	if(index == 0)
	{
		Real tmp = v[1];
		v[1] = -v[2];
		v[2] = tmp;
	}
	else if(index == 1)
	{
		Real tmp = v[0];
		v[0] = -v[2];
		v[2] = tmp;
	}
	else
	{
		Real tmp = v[0];
		v[0] = -v[1];
		v[1] = tmp;
	}
	v.normalize();
	Vector3 w = u.cross(v);
	w.normalize();
	//Vector3::orthogonalize(u,v,w,u,v,w);
	Matrix44 m = Matrix44::translate(p) * Matrix44(u,v,w) * Matrix44::scale(len);
	glPushMatrix();
	glMultMatrixf(m);
	//render the vector
	Real arrowSize = (Real)0.1;
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(1 - arrowSize,0,0);
	glEnd();
	glPushMatrix();
	glTranslatef(1 - arrowSize,0,0);
	glRotatef(90.0,0,1,0);
	glutSolidCone(0.025,arrowSize,16,1);
	glPopMatrix();
	glPopMatrix();
}

//render 3x3 matrix
void Matrix33::render(const Vector3 &p)
{
	Vector3 col1,col2,col3;
	getColumns(col1,col2,col3);
	//col 1
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col1);
	glEnd();
	//col 2
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col2);
	glEnd();
	//col 3
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col3);
	glEnd();
}

//render 4x4 matrix
void Matrix44::render(const Vector3 &p)
{
	Vector3 col1,col2,col3;
	getColumns(col1,col2,col3);
	//col 1
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col1);
	glEnd();
	//col 2
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col2);
	glEnd();
	//col 3
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3fv(p);
	glVertex3fv(p+col3);
	glEnd();
}

//render quaternion
void Quaternion::render(const Vector3 &p)
{
	Vector3 v = Vector3(x,y,z);
	if(!v.isZero())
	{
		v.normalize();
		v.render(p);
		v *= w;
		glBegin(GL_LINES);
		glVertex3fv(p);
		glVertex3fv(p+v);
		glEnd();
	}
}

//render ray
void Ray::render()
{
	int scale = 1000;
	//draw ray origin
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3f(origin.x,origin.y,origin.z);
	glEnd();
	glPointSize(1);
	//draw ray direction vector
	Viz::render(direction,origin);
	//draw rest of ray
	glBegin(GL_LINES);
	glVertex3f(origin.x,origin.y,origin.z);
	glVertex3f(origin.x+direction.x*scale,origin.y+direction.y*scale,origin.z+direction.z*scale);
	glEnd();
}

//render line
void Line::render()
{
	Real scale = 1000;
	//draw line origin
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3f(origin.x,origin.y,origin.z);
	glEnd();
	glPointSize(1);
	//draw line direction vector
	Viz::render(direction,origin);
	//draw the rest of the line
	glBegin(GL_LINES);
	glVertex3f(origin.x+direction.x*-scale,origin.y+direction.y*-scale,origin.z+direction.z*-scale);
	glVertex3f(origin.x+direction.x*scale,origin.y+direction.y*scale,origin.z+direction.z*scale);
	glEnd();
}

//render line segment
void LineSegment::render()
{
	//draw endpoints
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3f(start.x,start.y,start.z);
	glVertex3f(end.x,end.y,end.z);
	glEnd();
	glPointSize(1);
	//draw segment
	glBegin(GL_LINES);
	glVertex3f(start.x,start.y,start.z);
	glVertex3f(end.x,end.y,end.z);
	glEnd();
}

//sphere debug render
void Sphere::render(unsigned slices,unsigned stacks)
{
	glPushMatrix();
	glTranslatef(center.x,center.y,center.z);
	glutWireSphere(radius,slices,stacks);
	glPopMatrix();
}

//plane debug render
void Plane::render()
{
	//todo
}

//AABB debug render
void AABB::render()
{
	glBegin(GL_LINE_LOOP);
	//quad 1 - lies in XY plane at AABB min Z
	glVertex3f(min.x,min.y,min.z);
	glVertex3f(max.x,min.y,min.z);
	glVertex3f(max.x,max.y,min.z);
	glVertex3f(min.x,max.y,min.z);
	glEnd();
	//quad 2 - lies in XY plane at AABB max Z
	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x,min.y,max.z);
	glVertex3f(max.x,min.y,max.z);
	glVertex3f(max.x,max.y,max.z);
	glVertex3f(min.x,max.y,max.z);
	glEnd();
	//quad 3 - lies in YZ plane at AABB min X
	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x,min.y,min.z);
	glVertex3f(min.x,min.y,max.z);
	glVertex3f(min.x,max.y,max.z);
	glVertex3f(min.x,max.y,min.z);
	glEnd();
	//quad 4 - lies in YZ plane at AABB max X
	glBegin(GL_LINE_LOOP);
	glVertex3f(max.x,min.y,min.z);
	glVertex3f(max.x,min.y,max.z);
	glVertex3f(max.x,max.y,max.z);
	glVertex3f(max.x,max.y,min.z);
	glEnd();
}

//triangle debug render
void Triangle::render()
{
	glBegin(GL_LINE_LOOP);
	glVertex3fv(p0);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glEnd();
	//draw normal
	Vector3 e1 = p1-p0;
	Vector3 e2 = p2-p0;
	Vector3 n = e1.cross(e2);
	if(!n.isZero()) n.normalize();
	glBegin(GL_LINES);
	glVertex3fv(p0 + e1*Math::ONE_OVER_THREE + e2*Math::ONE_OVER_THREE);
	glVertex3fv(p0 + e1*Math::ONE_OVER_THREE + e2*Math::ONE_OVER_THREE + n);
	glEnd();
}

//tetrahedron debug render
void Tetrahedron::render()
{
	//glBegin(GL_TRIANGLES);
	glBegin(GL_LINE_LOOP);
	//bottom face
	glVertex3fv(p0);
	glVertex3fv(p1);
	glVertex3fv(p2);
	//face 1
	glVertex3fv(p0);
	glVertex3fv(p1);
	glVertex3fv(p3);
	//face 2
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	//face 3
	glVertex3fv(p2);
	glVertex3fv(p0);
	glVertex3fv(p3);
	glEnd();
	//
	//render normals
	Real scale = Math::ONE_OVER_FOUR;
	Viz::render(n0*scale,(p1+p2+p3)*Math::ONE_OVER_THREE);
	Viz::render(n1*scale,(p0+p2+p3)*Math::ONE_OVER_THREE);
	Viz::render(n2*scale,(p0+p1+p3)*Math::ONE_OVER_THREE);
	Viz::render(n3*scale,(p0+p1+p2)*Math::ONE_OVER_THREE);
}

//3d grid debug render
void Grid3D::render()
{
	for(int i=0; i<m_resX*m_resY*m_resZ; i++)
	{
		Viz::renderCube(m_voxels[i].vertices);
	}
}

//triangle mesh debug render
void TriangleMesh::render(bool shade)
{
	std::vector<Tri *>::iterator i = triangles.begin();
	while(i != triangles.end())
	{
		if(shade) glBegin(GL_TRIANGLES);
		else glBegin(GL_LINE_LOOP);
		glNormal3fv((*i)->n0);
		glVertex3fv((*i)->v0);
		glNormal3fv((*i)->n1);
		glVertex3fv((*i)->v1);
		glNormal3fv((*i)->n2);
		glVertex3fv((*i)->v2);
		glEnd();
		i++; //next face
	}
}

//marching cubes mesh debug render
void MarchingCubesMesh::render(bool shade)
{
	std::vector<Tri *>::iterator i = triangles.begin();
	while(i != triangles.end())
	{
		if(shade) glBegin(GL_TRIANGLES);
		else glBegin(GL_LINE_LOOP);
		glNormal3fv((*i)->n0);
		glVertex3fv((*i)->v0);
		glNormal3fv((*i)->n1);
		glVertex3fv((*i)->v1);
		glNormal3fv((*i)->n2);
		glVertex3fv((*i)->v2);
		glEnd();
		i++; //next face
	}
}

//metaballs debug render
void Metaballs::render()
{
	std::vector<Metaballs::Metaball *>::iterator i = metaballs.begin();
	while(i != metaballs.end())
	{
		glColor3fv((*i)->color);
		glLineWidth(3);
		Viz::renderSphere((*i)->c,(*i)->r);
		glLineWidth(1);
		Viz::renderSphere((*i)->c,(*i)->R);
		i++;
	}
}

} //namespace foundation

} //namespace arkhe
