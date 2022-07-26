//local includes
#include "Shape.h"

bool Sphere::hit(const Ray &ray,double *tHit,HitRecord &rec) const
{
	Ray r = m_worldToLocal(ray); //copy ray and transform to local space of sphere

	double A = r.direction.dot(r.direction);
	double B = 2.0 * r.origin.dot(r.direction);
	double C = r.origin.dot(r.origin) - m_radius*m_radius;

	//compute quadratic coefficients
	double t0,t1;
	int roots = Math::solveQuadratic(A,B,C,&t0,&t1);

	if(roots == Math::NO_ROOTS)
	{
		return false; //no intersection
	}

	//check that hit lies within ray
	if(t1 < r.min || t0 > r.max)
	{
		return false;
	}
	*tHit = t0;
	if(t0 < r.min)
	{
		if(t1 > r.max)
		{
			return false;
		}
		*tHit = t1;
	}

	rec.p = m_localToWorld(r(*tHit));
	rec.nn = r(*tHit);
	rec.nn.normalize();

	return true;
}

bool Sphere::shadowHit(const Ray &ray) const
{
	Ray r = m_worldToLocal(ray); //copy ray and transform to local space of sphere

	double A = r.direction.dot(r.direction);
	double B = 2.0 * r.origin.dot(r.direction);
	double C = r.origin.dot(r.origin) - m_radius*m_radius;

	//compute quadratic coefficients
	double t0,t1;
	int roots = Math::solveQuadratic(A,B,C,&t0,&t1);

	if(roots == Math::NO_ROOTS)
	{
		return false; //no intersection
	}

	//check that hit lies within ray
	if(t1 < r.min || t0 > r.max)
	{
		return false;
	}
	if(t0 < r.min)
	{
		if(t1 > r.max)
		{
			return false;
		}
	}

	return true;
}
