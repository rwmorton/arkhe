#ifndef __ARKHE_T_LINE_SEGMENT_REUSE_HPP__
#define __ARKHE_T_LINE_SEGMENT_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

namespace line_segment
{

template<typename T> const TVector3<T> closestPoint(const TVector3<T> &start,const TVector3<T> &end,const TVector3<T> &p)
{
	TVector3<T> d = end - start;
	T recipDot = TMath<T>::reciprocal(d.dot(d)); //will throw an exception if divide by zero occurs
	T t = (p - start).dot(d) * recipDot;
	if(t < 0) t = 0;
	else if(t > 1) t = 1;
	return start + (end - start) * t;
}

//Get closest points between the two line segments passed in. Point associated with the first
//line segment is p and the point q is associated with the second line segment passed in.
//If the line segment are parallel this method returns false, otherwise it returns true.
template<typename T> const T closestPoints
(
	const TVector3<T> &s0,const TVector3<T> &e0,	//line segment 1
	const TVector3<T> &s1,const TVector3<T> &e1,	//line segment 2
	TVector3<T> &p,TVector3<T> &q	//closest points
)
{
	TVector3<T> d1 = e0 - s0;
	TVector3<T> d2 = e1 - s1;

	T a = d1.dot(d1);
	T e = d2.dot(d2);

	if(TMath<T>::isZero(a) && TMath<T>::isZero(e))
	{
		p = s0;
		q = s1;
		TVector3<T> v = p - q;
		return v.lengthSq();
	}

	TVector3<T> r = s0 - s1;
	T f = d2.dot(r);
	T s,t;

	if(TMath<T>::isZero(a))
	{
		s = 0;
		t = f / e;
		t = TMath<T>::clamp(t,0,1);
	}
	else
	{
		T c = d1.dot(r);
		if(TMath<T>::isZero(c))
		{
			t = 0;
			s = TMath<T>::clamp(-c/a,0,1);
		}
		else
		{
			T b = d1.dot(d2);
			T denom = a*e - b*b;

			if(!TMath<T>::isZero(denom))
			{
				s = TMath<T>::clamp((b*f-c*e)/denom,0,1);
			}
			else
			{
				s = 0;
			}
			t = (b*s + f)/e;

			if(t < 0)
			{
				t = 0;
				s = TMath<T>::clamp(-c/a,0,1);
			}
			else if(t > 1)
			{
				t = 1;
				s = TMath<T>::clamp((b-c)/a,0,1);
			}
		}
	}

	p = s0 + d1*s;
	q = s1 + d2*t;
	TVector3<T> v = p - q;
	return v.lengthSq();
}

} //namespace line_segment

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_LINE_SEGMENT_REUSE_HPP__
