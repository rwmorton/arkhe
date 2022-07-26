#ifndef __ARKHE_T_RAY_HPP__
#define __ARKHE_T_RAY_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TRay
{
public:
	//constructors
	TRay() : min(TMath<T>::RAY_EPSILON),max(TMath<T>::MAX) {}
	TRay(const TVector3<T> &o,const TVector3<T> &d) : origin(o),direction(d),min(TMath<T>::RAY_EPSILON),max(TMath<T>::MAX) {}
	TRay(const TRay &copy);
	//destructor
	~TRay() {}
	//operator overloads
	TRay &operator=(const TRay &copy);
	const TVector3<T> operator()(const T &t) const;
	template<typename T> friend std::ostream &operator<<(std::ostream &out,const TRay &r);
	//methods
	const TVector3<T> closestPoint(const TVector3<T> &p) const;
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
	//variables
	TVector3<T> origin;
	TVector3<T> direction;
	T min,max;
private:
	//
protected:
	//
}; //class TRay

template<typename T> TRay<T>::TRay(const TRay<T> &copy) : origin(copy.origin),direction(copy.direction) {}

template<typename T> TRay<T> &TRay<T>::operator=(const TRay<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	origin = copy.origin;
	direction = copy.direction;
	min = copy.min;
	max = copy.max;
	return *this;
}

template<typename T> const TVector3<T> TRay<T>::operator()(const T &t) const
{
	return origin + (direction * t);
}

template<typename T> std::ostream &operator<<(std::ostream &out,const TRay<T> &r)
{
#ifdef RAW_STREAM_OUTPUT
	out << r.origin << ' ' << r.direction;
#else
	out << r.origin << ',' << r.direction;
#endif //RAW_STREAM_OUTPUT
	return out;
}

template<typename T> const TVector3<T> TRay<T>::closestPoint(const TVector3<T> &p) const
{
	T recipDot = TMath<T>::reciprocal(direction.dot(direction)); //will throw an exception if divide by zero occurs
	T t = (p - origin).dot(direction) * recipDot;
	if(t < 0) t = 0;
	return (*this)(t);
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_RAY_HPP__
