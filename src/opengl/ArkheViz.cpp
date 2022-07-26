//arkhe local includes (opengl)
#include "ArkheViz.h"
#include "ArkheGL.h"

namespace arkhe
{

namespace opengl
{

void Viz::render(const Vector3 &vec,const Vector3 &pos)
{
	Real len = vec.length();
	//form the basis
	Vector3 u = vec;
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
	Matrix44 m = Matrix44::translate(pos) * Matrix44(u,v,w) * Matrix44::scale(len);
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

void Viz::render2DVectorField(int extentsFromOrigin,Real scale,Vector3 (*vectorFunction)(Real,Real),bool realScale)
{
	Real bounds = (Real)extentsFromOrigin;
	Real incr = Math::SQRT_2 * scale;
	Vector3 v;
	std::vector<Vector3> positions,directions;
	Real curLen,maxLen(0),recipMaxLen;
	for(Real x=-bounds; x<=bounds; x+=incr)
	{
		for(Real y=-bounds; y<=bounds; y+=incr)
		{
			v = vectorFunction(x,y);
			positions.push_back(Vector3(x,y,0));
			directions.push_back(v);
			curLen = v.length();
			if(maxLen < curLen) maxLen = curLen;
		}
	}
	//render
	recipMaxLen = Math::reciprocal(maxLen);
	std::vector<Vector3>::iterator p = positions.begin();
	std::vector<Vector3>::iterator d = directions.begin();
	while(p != positions.end())
	{
		v = *d;
		if(realScale) v *= recipMaxLen;
		else
		{
			v.normalize();
			v *= scale;
		}
		render(v,*p);
		p++; d++;
	}
}

void Viz::render3DVectorField(int extentsFromOrigin,Real scale,Vector3 (*vectorFunction)(Real,Real,Real),bool realScale)
{
	Real bounds = (Real)extentsFromOrigin;
	Real incr = Math::SQRT_2 * scale;
	Vector3 v;
	std::vector<Vector3> positions,directions;
	Real curLen,maxLen(0),recipMaxLen;
	for(Real x=-bounds; x<=bounds; x+=incr)
	{
		for(Real y=-bounds; y<=bounds; y+=incr)
		{
			for(Real z=-bounds; z<=bounds; z+=incr)
			{
				v = vectorFunction(x,y,z);
				positions.push_back(Vector3(x,y,z));
				directions.push_back(v);
				curLen = v.length();
				if(maxLen < curLen) maxLen = curLen;
			}
		}
	}
	//render
	recipMaxLen = Math::reciprocal(maxLen);
	std::vector<Vector3>::iterator p = positions.begin();
	std::vector<Vector3>::iterator d = directions.begin();
	while(p != positions.end())
	{
		v = *d;
		if(realScale) v *= recipMaxLen;
		else v.normalize();
		render(v,*p);
		p++; d++;
	}
}

void Viz::render3DVectorField(const Vector3 &min,const Vector3 &max,Real scale,Vector3 (*vectorFunction)(Real,Real,Real),bool realScale)
{
	Real incr = Math::SQRT_2 * scale;
	Vector3 v;
	std::vector<Vector3> positions,directions;
	Real curLen,maxLen(0),recipMaxLen;
	for(Real x=min.x; x<=max.x; x+=incr)
	{
		for(Real y=min.y; y<=max.y; y+=incr)
		{
			for(Real z=min.z; z<=max.z; z+=incr)
			{
				v = vectorFunction(x,y,z);
				positions.push_back(Vector3(x,y,z));
				directions.push_back(v);
				curLen = v.length();
				if(maxLen < curLen) maxLen = curLen;
			}
		}
	}
	//render
	recipMaxLen = Math::reciprocal(maxLen);
	std::vector<Vector3>::iterator p = positions.begin();
	std::vector<Vector3>::iterator d = directions.begin();
	while(p != positions.end())
	{
		v = *d;
		if(realScale) v *= recipMaxLen;
		else v.normalize();
		render(v,*p);
		p++; d++;
	}

	AABB box(min,max);
	box.render();
}

void Viz::renderSphere(const Vector3 &p,Real radius)
{
	glPushMatrix();
	glTranslatef(p.x,p.y,p.z);
	//xy-plane
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; i++)
		glVertex3f(Math::cos(((Real)i/64)*Math::TWO_PI)*radius,Math::sin(((Real)i/64)*Math::TWO_PI)*radius,0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	//xz-plane
	for(int i=0; i<64; i++)
		glVertex3f(Math::cos(((Real)i/64)*Math::TWO_PI)*radius,0,Math::sin(((Real)i/64)*Math::TWO_PI)*radius);
	glEnd();
	//yz-plane
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; i++)
		glVertex3f(0,Math::cos(((Real)i/64)*Math::TWO_PI)*radius,Math::sin(((Real)i/64)*Math::TWO_PI)*radius);
	glEnd();
	glPopMatrix();
}

void Viz::renderCube(const Vector3 *vertices)
{
	if(!vertices) throw Exception(Exception::NULL_POINTER);

	//bottom face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[1]);
	glEnd();
	//top face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[5]);
	glEnd();
	//front face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glEnd();
	//back face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[4]);
	glEnd();
	//left face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[7]);
	glEnd();
	//right face
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glEnd();
}

/*
m_data->vertices[0] = Vector3(p.x,p.y,p.z); //the current vertex (x,y,z)
m_data->vertices[1] = Vector3(p.x+xSize,p.y,p.z);
m_data->vertices[2] = Vector3(p.x+xSize,p.y,p.z+zSize);
m_data->vertices[3] = Vector3(p.x,p.y,p.z+zSize);
m_data->vertices[4] = Vector3(p.x,p.y+ySize,p.z);
m_data->vertices[5] = Vector3(p.x+xSize,p.y+ySize,p.z);
m_data->vertices[6] = Vector3(p.x+xSize,p.y+ySize,p.z+zSize);
m_data->vertices[7] = Vector3(p.x,p.y+ySize,p.z+zSize);
*/

void Viz::render(const Ray &ray)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3fv(ray.origin);
	glEnd();
	glLineWidth(2);
	render(ray.direction,ray.origin);
	Real scale = 1000;
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x1111);
	render(ray.direction*scale,ray.origin);
	glDisable(GL_LINE_STIPPLE);
	glPointSize(1);
	glLineWidth(1);
}

void Viz::render(const Particle &p,Real r,Real g,Real b)
{
	glColor3f(r,g,b);
	Vector3 pos = p.getPosition();
	glBegin(GL_POINTS);
	glVertex3d(pos.x,pos.y,pos.z);
	glEnd();
}

void Viz::render(const Spring &s,Real r,Real g,Real b)
{
	glColor3f(r,g,b);
	Vector3 m = s.getAPosition();
	Vector3 n = s.getBPosition();
	glBegin(GL_LINES);
	glVertex3d(m.x,m.y,m.z);
	glVertex3d(n.x,n.y,n.z);
	glEnd();
}

void Viz::render(const MA_Cube &c,bool full)
{
	if(full)
	{
		vector<Particle *>::const_iterator i = c.particles.begin();
		while(i != c.particles.end())
		{
			Viz::render(**i);
			i++;
		}
		vector<Spring>::const_iterator j = c.springs.begin();
		while(j != c.springs.end())
		{
			Viz::render(*j);
			j++;
		}
	}
	else
	{
		glColor3f(1,1,1);
		Vector3 a,b,C,d;
		glBegin(GL_LINES);
		a = c.particles.at(0)->getPosition();
		b = c.particles.at(1)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(1)->getPosition();
		b = c.particles.at(2)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(2)->getPosition();
		b = c.particles.at(3)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(3)->getPosition();
		b = c.particles.at(0)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(4)->getPosition();
		b = c.particles.at(5)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(5)->getPosition();
		b = c.particles.at(6)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(6)->getPosition();
		b = c.particles.at(7)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(7)->getPosition();
		b = c.particles.at(4)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(1)->getPosition();
		b = c.particles.at(4)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(2)->getPosition();
		b = c.particles.at(5)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(3)->getPosition();
		b = c.particles.at(6)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_LINES);
		a = c.particles.at(0)->getPosition();
		b = c.particles.at(7)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glEnd();
		glBegin(GL_POLYGON);
		//face 1
		a = c.particles.at(0)->getPosition();
		b = c.particles.at(1)->getPosition();
		C = c.particles.at(2)->getPosition();
		d = c.particles.at(3)->getPosition();
		glVertex3d(a.x,a.y,a.z);
		glVertex3d(b.x,b.y,b.z);
		glVertex3d(C.x,C.y,C.z);
		glVertex3d(d.x,d.y,d.z);
		glEnd();
	}
}

void Viz::render(const MA_String &s,bool full)
{
	if(full)
	{
		vector<Particle *>::const_iterator i = s.particles.begin();
		while(i != s.particles.end())
		{
			Viz::render(**i);
			i++;
		}
		vector<Spring>::const_iterator j = s.springs.begin();
		while(j != s.springs.end())
		{
			Viz::render(*j);
			j++;
		}
	}
	else
	{
		vector<Spring>::const_iterator j = s.springs.begin();
		while(j != s.springs.end())
		{
			Viz::render(*j,1,1,1);
			j++;
		}
	}
}

void Viz::render(const MA_Sphere &s,bool full)
{
	if(full)
	{
		vector<Particle *>::const_iterator i = s.particles.begin();
		while(i != s.particles.end())
		{
			Viz::render(**i);
			i++;
		}
		vector<Spring>::const_iterator j = s.springs.begin();
		while(j != s.springs.end())
		{
			Viz::render(*j);
			j++;
		}
	}
	else
	{
		vector<Particle *>::const_iterator i = s.particles.begin();
		while(i != s.particles.end())
		{
			Viz::render(**i,1,1,1);
			i++;
		}
	}
}

void Viz::render(const Assemblage &a)
{
	//draw elements
	glColor3f(1,0,0);
	glLineWidth(2);
	glBegin(GL_LINES);
	for(int i=0; i<a.elements.size(); i++)
	{
		glVertex3f(a.elements.at(i)->A->x,0,0);
		glVertex3f(a.elements.at(i)->B->x,0,0);
	}
	glEnd();
	//draw nodes
	glPointSize(5);
	glBegin(GL_POINTS);
	for(int i=0; i<a.nodes.size(); i++)
	{
		if(a.nodes.at(i)->boundary)
		{
			glColor3f(0,0,0);
		}
		else
		{
			glColor3f(1,1,0);
		}
		glVertex3f(a.nodes.at(i)->x,0,0);
	}
	glEnd();
}

} //namespace opengl

} //namespace arkhe
