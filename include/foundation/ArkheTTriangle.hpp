#ifndef __ARKHE_T_TRIANGLE_HPP__
#define __ARKHE_T_TRIANGLE_HPP__

//arkhe local includes (foundation)
#include "ArkheTTriangleReuse.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TTriangle
{
public:
	template<typename T> friend class TMatrix44;
	template<typename T> friend class TIntersection;
public:
	//constructor
	TTriangle() {} //not initialized
	TTriangle(const TVector3<T> &_p0,const TVector3<T> &_p1,const TVector3<T> &_p2) : p0(_p0),p1(_p1),p2(_p2) {}
	//methods
	const T area() const { return triangle::area<T>(p0,p1,p2); }
	void barycentric(const TVector3<T> &p,T &u,T &v,T &w) const { triangle::barycentric<T>(p0,p1,p2,p,u,v,w); }
	const bool containsPoint(const TVector3<T> &p) const { return triangle::containsPoint<T>(p0,p1,p2,p); }
	const TVector3<T> closestPoint(const TVector3<T> &p) const { return triangle::cloestPoint<T>(p0,p1,p2,p); }
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//variables
	TVector3<T> p0,p1,p2;
protected:
	//
}; //class TTriangle

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_TRIANGLE_HPP__
