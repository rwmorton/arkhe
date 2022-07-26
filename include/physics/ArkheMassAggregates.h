#ifndef __ARKHE_MASS_AGGREGATES_H__
#define __ARKHE_MASS_AGGREGATES_H__

//arkhe local includes (physics)
#include "ArkheParticle.h"
#include "ArkheSpring.h"

//std includes
#include <vector>
#include <iostream>
using namespace std;

namespace arkhe
{

namespace physics
{

class MA
{
public:
	//destructor
	virtual ~MA();
	//methods
	void setK(Real k);
	void setGlobalAcceleration(const Vector3 &a);
	Real getMass() const;
	Vector3 getAcceleration() const;
	void addForce(const Vector3 &f);
	void integrate(Real t);
	//variables
	vector<Particle *> particles;
	vector<Spring> springs;
private:
	//
protected:
	//
}; //class MA

class MA_Cube : public MA
{
public:
	//constructor
	MA_Cube(const Vector3 &c) : center(c) { buildCube(); }
	//methods
	void setRotation(const Matrix33 &m);
private:
	//methods
	void buildCube();
	//variables
	static const Vector3 v1,v2,v3,v4,v5,v6,v7,v8; //vertices
	Vector3 center;
}; //class MA_Cube

class MA_String : public MA
{
public:
	//constructor
	MA_String(const Vector3 &start,const Vector3 &end,int npts) { buildString(start,end,npts); };
	//methods
	Particle *first() { return particles.at(0); }
	Particle *last() { return particles.at(particles.size()-1); }
private:
	//methods
	void buildString(const Vector3 &s,const Vector3 &e,int numPoints);
protected:
	//
}; //class MA_String

class MA_Sphere : public MA
{
public:
	//constructor
	MA_Sphere(const Vector3 &c,Real r,int vslices,int hslices) : center(c),radius(r) { buildSphere(vslices,hslices); }
	//methods
	void setRotation(const Matrix33 &m);
private:
	//methods
	void buildSphere(int vslices,int hslices);
	//variables
	Vector3 center;
	Real radius;
protected:
	//
}; //class MA_Sphere

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_MASS_AGGREGATES_H__
