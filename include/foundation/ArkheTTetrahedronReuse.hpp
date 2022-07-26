#ifndef __ARKHE_T_TETRAHEDRON_REUSE_HPP__
#define __ARKHE_T_TETRAHEDRON_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTVector3.hpp"
#include "ArkheTMatrix33.hpp"
#include "ArkheTMath.hpp"

namespace arkhe
{

namespace foundation
{

namespace tetrahedron
{

//LOOK INTO MANIFOLD FLIPPING ISSUE... NEEDS TO BE COMPATIBILE WITH NORMALS...
//WINDING NEEDS TO BE CONSISTENT...
//
//TODO
//

template<typename T> void barycentric
(
	const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2,const TVector3<T> &p3,	//tetrahedron
	const TVector3<T> &p,	//test point
	T &t,T &u,T &v,T &w	//barycentric coordinates
)
{
	TMatrix33<T> m
	(
		p1.x-p0.x,p2.x-p0.x,p3.x-p0.x,
		p1.y-p0.y,p2.y-p0.y,p3.y-p0.y,
		p1.z-p0.z,p2.z-p0.z,p3.z-p0.z
	);
	TMatrix33<T> i = m.inverse();
	TVector3<T> B(p.x-p0.x,p.y-p0.y,p.z-p0.z);
	TVector3<T> X = i(B);
	u = X.x;
	v = X.y;
	w = X.z;
	t = TMath<T>::ONE - u - v - w;
}

template<typename T> const bool containsPoint
(
	const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2,const TVector3<T> &p3,	//tetrahedron
	const TVector3<T> &p	//test point
)
{
	T t,u,v,w;
	barycentric(p0,p1,p2,p3,p,t,u,v,w);
	if(!TMath<T>::inRange(t,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	if(!TMath<T>::inRange(u,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	if(!TMath<T>::inRange(v,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	if(!TMath<T>::inRange(w,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	return true;
}

} //namespace tetrahedron

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_TETRAHEDRON_REUSE_HPP__
