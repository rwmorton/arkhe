#ifndef __ARKHE_T_TRIANGLE_REUSE_HPP__
#define __ARKHE_T_TRIANGLE_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

//Definition of methods that are reused
namespace triangle
{

//compute triangle area
template<typename T> const T area(const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2)
{
	TVector3<T> c0,c1,c2;
	c0 = p0.cross(p1);
	c1 = p1.cross(p2);
	c2 = p2.cross(p0);
	return (c0 + c1 + c2).length() * TMath<T>::ONE_OVER_TWO;
}

//compute barycentric coordinates
template<typename T> void barycentric
(
	const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2,	//triangle
	const TVector3<T> &p,	//test point
	T &u,T &v,T &w	//coordinates
)
{
	TVector3<T> v0(p1-p0),v1(p2-p0),v2(p-p0);
	T d00 = v0.dot(v0);
	T d01 = v0.dot(v1);
	T d11 = v1.dot(v1);
	T det = d00*d11 - d01*d01;
	T recipDet = TMath<T>::reciprocal(det);

	T a = v0.dot(v2);
	T b = v1.dot(v2);

	v = a*d11 - d01*b;
	v *= recipDet;

	w = d00*b - a*d01;
	w *= recipDet;

	u = TMath<T>::ONE - v - w;
} //namespace triangle

//does triangle contain point?
template<typename T> const bool containsPoint
(
	const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2,	//triangle
	const TVector3<T> &p	//test point
)
{
	T u,v,w;
	barycentric(p0,p1,p2,p,u,v,w);
	if(!TMath<T>::inRange(u,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	if(!TMath<T>::inRange(v,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	if(!TMath<T>::inRange(w,TMath<T>::ZERO,TMath<T>::ONE)) return false;
	return true;
}

//compute closest point to triangle from given point
//
//HACKED DIRECTLY FROM ERICSONS BOOK - WILL REIMPLEMENT WHEN I GET MY HEAD AROUND THE IDEA
//
//TODO
//
//
template<typename T> const T Dot(const TVector3<T> &a,const TVector3<T> &b) { return a.dot(b); }
template<typename T> const TVector3<T> Cross(const TVector3<T> &a,const TVector3<T> &b) { return a.cross(b); }

template<typename T> const TVector3<T> closestPoint
(
	const TVector3<T> &p0,const TVector3<T> &p1,const TVector3<T> &p2,	//triangle
	const TVector3<T> &p	//test point
)
{
    TVector3<T> ab = p1 - p0;
    TVector3<T> ac = p2 - p0;
    TVector3<T> bc = p2 - p1;

    // Compute parametric position s for projection P' of P on AB,
    // P' = A + s*AB, s = snom/(snom+sdenom)
    T snom = Dot<T>(p - p0, ab), sdenom = Dot<T>(p - p1, p0 - p1);

    // Compute parametric position t for projection P' of P on AC,
    // P' = A + t*AC, s = tnom/(tnom+tdenom)
    T tnom = Dot<T>(p - p0, ac), tdenom = Dot<T>(p - p2, p0 - p2);

    if (snom <= 0.0f && tnom <= 0.0f) return p0; // Vertex region early out

    // Compute parametric position u for projection P' of P on BC,
    // P' = B + u*BC, u = unom/(unom+udenom)
    T unom = Dot<T>(p - p1, bc), udenom = Dot<T>(p - p2, p1 - p2);

    if (sdenom <= 0.0f && unom <= 0.0f) return p1; // Vertex region early out
    if (tdenom <= 0.0f && udenom <= 0.0f) return p2; // Vertex region early out


    // P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
    TVector3<T> n = Cross<T>(p1 - p0, p2 - p0);
    T vc = Dot<T>(n, Cross<T>(p0 - p, p1 - p));
    // If P outside AB and within feature region of AB,
    // return projection of P onto AB
    if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
        return p0 + ab * snom / (snom + sdenom);

    // P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
    T va = Dot<T>(n, Cross<T>(p1 - p, p2 - p));
    // If P outside BC and within feature region of BC,
    // return projection of P onto BC
    if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
        return p1 + bc * unom / (unom + udenom);

    // P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
    T vb = Dot<T>(n, Cross<T>(p2 - p, p0 - p));
    // If P outside CA and within feature region of CA,
    // return projection of P onto CA
    if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
        return p0 + ac * tnom / (tnom + tdenom);

    // P must project inside face region. Compute Q using barycentric coordinates
    T u = va / (va + vb + vc);
    T v = vb / (va + vb + vc);
    T w = 1.0f - u - v; // = vc / (va + vb + vc)
    return p0 * u + p1 * v + p2 * w;
}

} //namespace triangle

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_TRIANGLE_REUSE_HPP__
