#ifndef __ARKHE_T_TETRAHEDRON_HPP__
#define __ARKHE_T_TETRAHEDRON_HPP__

//arkhe local includes (foundation)
#include "ArkheTTetrahedronReuse.hpp"

namespace arkhe
{

namespace foundation
{

/*
n0 : p1,p2,p3
n1 : p0,p2,p3
n2 : p0,p1,p3
n3 : p0,p1,p2
*/
template<typename T> class TTetrahedron
{
public:
	template<typename T> friend class TMatrix44;
	template<typename T> friend class TIntersection;
public:
	//constructor
	TTetrahedron() {} //not initialized
	TTetrahedron(const TVector3<T> &_p0,const TVector3<T> &_p1,const TVector3<T> &_p2,const TVector3<T> &_p3) : p0(_p0),p1(_p1),p2(_p2),p3(_p3) {}
	//methods
	void barycentric(const TVector3<T> &p,T &t,T &u,T &v,T &w) const { tetrahedron::barycentric(p0,p1,p2,p3,p,t,u,v,w); }
	const bool containsPoint(const TVector3<T> &p) const { return tetrahedron::containsPoint<T>(p0,p1,p2,p3,p); }
	void generateNormals();
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//variables
	TVector3<T> p0,p1,p2,p3;
	TVector3<T> n0,n1,n2,n3;
protected:
	//
}; //class TTetrahedron

template<typename T> void TTetrahedron<T>::generateNormals()
{
	n0 = (p1-p3).cross(p2-p3);
	n0.normalize();
	n1 = (p0-p2).cross(p3-p2);
	n1.normalize();
	n2 = (p3-p1).cross(p0-p1);
	n2.normalize();
	n3 = (p2-p0).cross(p1-p0);
	n3.normalize();
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_TETRAHEDRON_HPP__
