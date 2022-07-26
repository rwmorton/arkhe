#ifndef __ARKHE_T_PLANE_HPP__
#define __ARKHE_T_PLANE_HPP__

//arkhe local includes (foundation)
#include "ArkheTPlaneReuse.hpp"

namespace arkhe
{

namespace foundation
{

//The plane normal must always be normalized.
template<typename T> class TPlane
{
public:
	template<typename T> friend class TIntersection;
public:
	//constructors
	TPlane() {}
	TPlane(const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2);
	TPlane(const TVector3<T> &n,const T &d) : normal(n),distance(d) { normal.normalize(); } //make sure normal is normalized
	TPlane(const TPlane &copy) : distance(copy.distance),normal(copy.normal) {}
	//destructor
	~TPlane() {}
	//operator overloads
	TPlane &operator=(const TPlane &copy);
	//methods
	const TVector3<T> closestPoint(const TVector3<T> &p) const { return plane::closestPoint<T>(normal,distance,p); }
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//variables
	TVector3<T> normal;
	T distance;
protected:
	//
}; //class TPlane

//assume that points are ordered ccw
template<typename T> TPlane<T>::TPlane(const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2)
{
	normal = (p1-p0).cross(p2-p0);
	normal.normalize();
	distance = normal.dot(p0);
}

template<typename T> TPlane<T> &TPlane<T>::operator=(const TPlane<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	distance = copy.distance;
	normal = copy.normal;
	return *this;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_PLANE_HPP__
