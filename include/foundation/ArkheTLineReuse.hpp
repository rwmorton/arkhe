#ifndef __ARKHE_T_LINE_REUSE_HPP__
#define __ARKHE_T_LINE_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

namespace line
{

template<typename T> const TVector3<T> closestPoint(const TVector3<T> &origin,const TVector3<T> &direction,const TVector3<T> &p)
{
	T recipDot = TMath<T>::reciprocal(direction.dot(direction)); //will throw an exception if divide by zero occurs
	T t = (p - origin).dot(direction) * recipDot;
	return origin + direction * t;
}

//Get closest points between the two lines passed in. Point associated with the first
//line is p and the point q is associated with the second line passed in.
//If the lines are parallel this method returns false, otherwise it returns true.
template<typename T> const bool closestPoints
(
	const TVector3<T> &o1,const TVector3<T> &d1,	//line 1
	const TVector3<T> &o2,const TVector3<T> &d2,	//line 2
	TVector3<T> &p,TVector3<T> &q	//closest points
)
{
	T a = d1.dot(d1);
	T b = d1.dot(d2);
	T e = d2.dot(d2);

	T d = a*e - b*b; //>= 0
	if(TMath<T>::isZero(d)) return false; //lines are parallel

	TVector3<T> r = o1 - o2;
	T c = d1.dot(r);
	T f = d2.dot(r);

	T recipD = TMath<T>::reciprocal(d);
	T s = (b*f - c*e) * recipD;
	T t = (a*f - b*c) * recipD;

	p = (*this)(s);
	q = line(t);

	return true;
}

} //namespace line

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_LINE_REUSE_HPP__
