//local includes
#include "ArkheMassAggregates.h"

//TEMP
#include <iostream>
using namespace std;

namespace arkhe
{

namespace physics
{

const Vector3 MA_Cube::v1(-0.5,-0.5,0.5);
const Vector3 MA_Cube::v2(0.5,-0.5,0.5);
const Vector3 MA_Cube::v3(0.5,0.5,0.5);
const Vector3 MA_Cube::v4(-0.5,0.5,0.5);
const Vector3 MA_Cube::v5(0.5,-0.5,-0.5);
const Vector3 MA_Cube::v6(0.5,0.5,-0.5);
const Vector3 MA_Cube::v7(-0.5,0.5,-0.5);
const Vector3 MA_Cube::v8(-0.5,-0.5,-0.5);

////////////////// MA //////////////////

MA::~MA()
{
	vector<Particle *>::iterator i = particles.begin();
	while(i != particles.end())
	{
		delete *i;
		i++;
	}
}

void MA::setK(Real k)
{
	vector<Spring>::iterator i = springs.begin();
	while(i != springs.end())
	{
		i->setK(k);
		i++;
	}
}

Real MA::getMass() const
{
	vector<Particle *>::const_iterator i = particles.begin();
	Real mass = 0.0;
	while(i != particles.end())
	{
		mass += (*i)->getMass();
		i++;
	}
	return mass;
}

Vector3 MA::getAcceleration() const
{
	vector<Particle *>::const_iterator i = particles.begin();
	Vector3 accel = Vector3::ZERO;
	while(i != particles.end())
	{
		accel += (*i)->getAcceleration();
		i++;
	}
	return accel;
}

void MA::setGlobalAcceleration(const Vector3 &a)
{
	vector<Particle *>::iterator i = particles.begin();
	while(i != particles.end())
	{
		(*i)->setGlobalAcceleration(a);
		i++;
	}
}

void MA::addForce(const Vector3 &f)
{
	vector<Particle *>::iterator i = particles.begin();
	while(i != particles.end())
	{
		(*i)->addForce(f);
		i++;
	}
}

void MA::integrate(Real t)
{
	vector<Particle *>::iterator i = particles.begin();
	Real depth;
	while(i != particles.end())
	{
		(*i)->integrate(t);
		//////////////////// TEST ////////////////////
		//check for y<0 (collision with y-plane) - just a test...
		depth = (*i)->getPosition().y;
		if(depth < 0)
		{
			(*i)->addForce(Vector3(0,-depth*1000,0)); //JUST A DIRTY HACK

			Vector3 a = getAcceleration();
			if(Math::isNan(a.x) || Math::isNan(a.y) || Math::isNan(a.z))
			{
				cout << "UH OH, beeg problem... getAcceleration() returning trash..." << endl;
				cout << "TRASH = " << a << endl;
				a = Vector3::ZERO;
				cout << "SET TO ZERO" << endl;
			}
			//Vector3 f = a * (*i)->getMass(); //getMass();
			//(*i)->addForce(-f);

			//Vector3 p = (*i)->getPosition();
			//p.y = 0;
			//(*i)->setPosition(p);
		}
		//////////////////// END TEST ////////////////////
		i++;
	}
	vector<Spring>::iterator j = springs.begin();
	while(j != springs.end())
	{
		j->update();
		j++;
	}
}

////////////////// MA_Cube //////////////////

void MA_Cube::setRotation(const Matrix33 &m)
{
	vector<Particle *>::iterator i = particles.begin();
	while(i != particles.end())
	{
		//subtract center
		//apply rotation
		//add center
		(*i)->setPosition(m((*i)->getPosition()-center)+center);
		i++;
	}
}

void MA_Cube::buildCube()
{
	Real m = 1.0;
	particles.push_back(new Particle(m,center + v1,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v2,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v3,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v4,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v5,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v6,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v7,Vector3::ZERO));
	particles.push_back(new Particle(m,center + v8,Vector3::ZERO));
	//
	Spring s;
	//spring for each pair
	for(int i=0; i<8; i++)
	{
		for(int j=0; j<8; j++)
		{
			if(i != j)
			{
				s.setEndpoints(particles.at(i),particles.at(j));
				springs.push_back(s);
			}
		}
	}
}

////////////////// MA_String //////////////////

void MA_String::buildString(const Vector3 &s,const Vector3 &e,int numPoints)
{
	Real m = 1; //being lazy and limited...
	Vector3 dir = e - s; //vector from start to end
	Real len = dir.length();
	Real incr = len / numPoints; //no check for div-by-zero - being lazy here
	Real t; //parameter
	for(int i=0; i<numPoints; i++)
	{
		t = (i * incr) / len;
		particles.push_back(new Particle(m,s*(1-t) + e*t,Vector3::ZERO));
	}
	Spring spring;
	for(int i=1; i<particles.size(); i++)
	{
		spring.setEndpoints(particles.at(i-1),particles.at(i));
		springs.push_back(spring);
	}
}

////////////////// MA_Sphere //////////////////

void MA_Sphere::setRotation(const Matrix33 &m)
{
	vector<Particle *>::iterator i = particles.begin();
	while(i != particles.end())
	{
		//subtract center
		//apply rotation
		//add center
		(*i)->setPosition(m((*i)->getPosition()-center)+center);
		i++;
	}
}

void MA_Sphere::buildSphere(int vslices,int hslices)
{
	Real m = 1;
	//no check for div-by-zero etc. being lazy...
	//
	//slices go here...
	Real y_incr = (2 * radius) / hslices;
	for(Real y=radius; y>=-radius; y-=y_incr)
	{
		if(y==radius || y==-radius)
		{
			//single vertex at the poles
			particles.push_back(new Particle(m,Vector3(0,y,0)+center,Vector3::ZERO));
		}
		else
		{
			//generate vertices around the circumference
			Real rad = Math::sqrt(radius*radius - y*y);
			Real c_incr = (2 * Math::PI) / vslices;
			for(int i=0; i<vslices; i++)
			{
				Vector3 pt(Math::cos(i*c_incr) * rad,y,Math::sin(i*c_incr) * rad);
				particles.push_back(new Particle(m,pt+center,Vector3::ZERO));
			}
		}
	}
	//
	Spring s;
	//spring for each pair
	for(int i=0; i<particles.size(); i++)
	{
		for(int j=0; j<particles.size(); j++)
		{
			if(i != j)
			{
				s.setEndpoints(particles.at(i),particles.at(j));
				springs.push_back(s);
			}
		}
	}
}

} //namespace physics

} //namespace arkhe
