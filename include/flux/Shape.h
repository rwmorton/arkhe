#ifndef __SHAPE_H__
#define __SHAPE_H__

//local includes
#include "Base.h"

struct HitRecord
{
	Vector p;
	Vector nn;
};

//simple material
struct Material
{
	Material() : r(0.0),g(0.0),b(0.0) {}
	Material(double _r,double _g,double _b) : r(_r),g(_g),b(_b) {}
	Material(const Material &copy) : r(copy.r),g(copy.g),b(copy.b) {}
	double r,g,b; //range allowed [0,1]
};

class Shape
{
public:
	//constructor
	Shape(const Matrix &l2w,const Material &m) : m_localToWorld(l2w),m_worldToLocal(l2w.inverse()),m_material(m) {}
	//destructor
	virtual ~Shape() {}
	//methods
	virtual bool hit(const Ray &r,double *tHit,HitRecord &rec) const = 0;
	virtual bool shadowHit(const Ray &r) const = 0;
//private:
	//
//protected:
	//data
	Material m_material;
	Matrix m_localToWorld,m_worldToLocal;
}; //class Shape

class Sphere : public Shape
{
public:
	//constructor
	Sphere(const Matrix &l2w,double r,const Material &m) : Shape(l2w,m),m_radius(r) {}
	//methods
	bool hit(const Ray &r,double *tHit,HitRecord &rec) const;
	bool shadowHit(const Ray &r) const;
private:
	//
protected:
	//data
	double m_radius;
}; //class Sphere

#endif //__SHAPE_H__
