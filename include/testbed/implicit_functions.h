#ifndef __IMPLICIT_FUNCTIONS_H__
#define __IMPLICIT_FUNCTIONS_H__

//arkhe includes
#include <foundation/ArkheFoundation.h>
using namespace arkhe::foundation;

const Real implicit(const Vector3 &p) { return p.length() - 4; }

const Vector3 gradient(const Vector3 &p) { return Vector3(2*p.x,2*p.y,2*p.z); }

const Real implicit2(const Vector3 &p)
{
	Real x = p.x;
	Real y = p.y;
	Real z = p.z;
	return Math::pow(x,4) + Math::pow(y,4) + Math::pow(z,4) - Math::pow(p.dot(p),2) + 3 * (p.dot(p)) - 3;
}

const Vector3 gradient2(const Vector3 &p)
{
	Real x = p.x;
	Real y = p.y;
	Real z = p.z;
	return Vector3
	(
		4*x*x*x - 4*x*(x*x + y*y + z*z) + 6*x,
		4*y*y*y - 4*y*(x*x + y*y + z*z) + 6*y,
		4*z*z*z - 4*z*(x*x + y*y + z*z) + 6*z
	);
}

const Real implicit3(const Vector3 &p)
{
	return (Math::cos(p.x)*Math::sin(p.y) + Math::cos(p.y)*Math::sin(p.z) + Math::cos(p.z)*Math::sin(p.x));
}

const Vector3 gradient3(const Vector3 &p)
{
	Real sx,sy,sz,cx,cy,cz;
	Math::sinCos(p.x,sx,cx);
	Math::sinCos(p.y,sy,cy);
	Math::sinCos(p.z,sz,cz);
	return Vector3
	(
		cx*cz - sx*sy,
		cx*cy - cy*sz,
		cy*cz - sx*sz
	);
}

const Real implicit4(const Vector3 &p) { return Math::pow(p.x,3) + Math::pow(p.y,3) + Math::pow(p.z,3) + 6*p.x*p.y*p.z -1; }

const Vector3 gradient4(const Vector3 &p)
{
	Real x = p.x;
	Real y = p.y;
	Real z = p.z;
	Vector3 g;
	g.x = 3*x*x + 6*y*z;
	g.y = 3*y*y + 6*x*z;
	g.z = 3*z*z + 6*x*y;
	return g;
}

const Real implicit5(const Vector3 &p) { return p.z*(Math::pow(Math::e,p.x) - p.y*p.y) - Math::sin(p.x*p.y); }

const Vector3 gradient5(const Vector3 &p)
{
	Real x = p.x;
	Real y = p.y;
	Real z = p.z;
	return Vector3
	(
		-((2 * (-1 + Math::pow(Math::e,(-x*x - y*y))) * x)/(x*x + y*y)*(x*x + y*y)) - (2 * Math::pow(Math::e,(-x*x - y*y)) * x)/(x*x + y*y),
		-((2 * (-1 + Math::pow(Math::e,(-x*x - y*y))) * y)/(x*x + y*y)*(x*x + y*y)) - (2 * Math::pow(Math::e,(-x*x - y*y)) * y)/(x*x + y*y),
		0
	);
}

const Real implicit6(const Vector3 &p)
{
	//return p.x*p.x + p.z*p.z - 1; //cylinder
	//return p.x*p.x + p.y*p.y - p.z*p.z; //cone
	//return p.x*p.x - p.y + p.z*p.z; //paraboloid
	//return p.x*p.x + p.y*p.y - p.z*p.z + 1; //hyperboloid
	//Real a = 3; //centers
	//Real b = 1.5; //radius
	//return Math::pow(p.dot(p) - (a*a + b*b),2) - 4*a*a*(b*b - p.z*p.z); //torus
	return p.x*p.x - p.y + p.z*p.z;
}

const Vector3 gradient6(const Vector3 &p)
{
	Real x = p.x;
	Real y = p.y;
	Real z = p.z;
	/*
	return Vector3
	(
		4*x*(-9 - 1.5*1.5 + x*x + y*y * z*z),
		4*y*(-9 - 1.5*1.5 + x*x + y*y * z*z),
		8*9*z + 4*z*(-9 - 1.5*1.5 + x*x + y*y * z*z)
	);
	*/
	return Vector3(2*x,-1,2*z);
}

#define IMPLICIT_COUNT 6

#endif //__IMPLICIT_FUNCTIONS_H__
