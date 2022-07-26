#ifndef __ARKHE_T_VECTOR_3_HPP__
#define __ARKHE_T_VECTOR_3_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTMath.hpp"
#include "ArkheException.h"

//std includes
#include <ostream>

namespace arkhe
{

namespace foundation
{

template<typename T> class TVector3
{
public:
	//constructors
	TVector3() {} //we do not initialize the values
	TVector3(const T &val) : x(val),y(val),z(val) {}
	TVector3(const T &_x,const T &_y,const T &_z) : x(_x),y(_y),z(_z) {}
	TVector3(const TVector3 &copy);
	//destructor
	~TVector3() {}
	//operator overloads
	TVector3 &operator=(const TVector3 &copy);
	operator const T*() const;
	operator T*();

	//compile errors in release?
	//const T operator[](unsigned int i) const;
	//T &operator[](unsigned int i);

	const TVector3 operator+(const TVector3 &v) const;
	TVector3 &operator+=(const TVector3 &v);
	const TVector3 operator-() const;
	const TVector3 operator-(const TVector3 &v) const;
	TVector3 &operator-=(const TVector3 &v);
	const TVector3 operator*(const T &t) const;
	TVector3 &operator*=(const T &t);
	const TVector3 operator/(const T &t) const;
	TVector3 &operator/=(const T &t);
	const bool operator==(const TVector3 &v) const;
	const bool operator!=(const TVector3 &v) const;

	//TODO - testing
	const bool operator<(const TVector3 &v) const
	{
		if(!TMath<T>::equal(x,v.x))
			return x<v.x;
		if(!TMath<T>::equal(y,v.x))
			return y<v.y;
		if(!TMath<T>::equal(z,v.z))
			return z<v.z;
	}
	const bool operator>(const TVector3 &v) const
	{
		return x>v.x;
	}

	template<typename T> friend std::ostream &operator<<(std::ostream &out,const TVector3 &v);
	//methods
	void clean();
	const bool isZero() const;
	const T getMinAbsComponent(int &index) const;
	const T getMaxAbsComponent(int &index) const;
	const T dot(const TVector3 &v) const;
	const TVector3 cross(const TVector3 &v) const;
	const T length() const;
	const T lengthSq() const;
	void normalize();
	const TVector3 parallelTo(const TVector3 &v,const bool normalized=false) const;
	const TVector3 perpendicularTo(const TVector3 &v,const bool normalized=false) const;
	//static methods
	static void orthogonalize(const TVector3 &v0,const TVector3 &v1,const TVector3 &v2,TVector3 &w0,TVector3 &w1,TVector3 &w2,const bool normalizeBasis=false);
	static const TVector3 random(const T &t);
	static const TVector3 random(const T &x,const T &y,const T &z);
	//debugging
#ifdef ARKHE_DEBUG
	void render(const TVector3<T> &p=TVector3<T>::ZERO); //defer to another source file
#endif //ARKHE_DEBUG
	//tests
#ifdef ARKHE_ENABLE_FOUNDATION_TESTS
	static void UNIT_TEST();
#endif //ARKHE_ENABLE_FOUNDATION_TESTS
	//static variables
	static const TVector3 ZERO;
	static const TVector3 UNIT_X;
	static const TVector3 UNIT_Y;
	static const TVector3 UNIT_Z;
	//variables
	union
	{
		struct
		{
			T x;
			T y;
			T z;
		};
		struct
		{
			T _align[3];
		};
	};
private:
	//methods
	void _orthogonalize(const TVector3 &v0,const TVector3 &v1,const TVector3 &v2,TVector3 &w0,TVector3 &w1,TVector3 &w2,const bool normalizeBasis=false);
protected:
	//
}; //class TVector3

template<typename T> TVector3<T>::TVector3(const TVector3<T> &copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

template<typename T> TVector3<T> &TVector3<T>::operator=(const TVector3<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	x = copy.x;
	y = copy.y;
	z = copy.z;
	return *this;
}

template<typename T> TVector3<T>::operator const T*() const { return &x; }

template<typename T> TVector3<T>::operator T*() { return &x; }

/*template<typename T> const T TVector3<T>::operator[](unsigned int i) const
{
	if(i >= 3) throw Exception(Exception::OUT_OF_RANGE);
	return *((&x)+i);
}

template<typename T> T &TVector3<T>::operator[](unsigned int i)
{
	if(i >= 3) throw Exception(Exception::OUT_OF_RANGE);
	return *((&x)+i);
}*/

template<typename T> const TVector3<T> TVector3<T>::operator+(const TVector3<T> &v) const
{
	return TVector3<T>(x+v.x,y+v.y,z+v.z);
}

template<typename T> TVector3<T> &TVector3<T>::operator+=(const TVector3<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template<typename T> const TVector3<T> TVector3<T>::operator-() const { return TVector3<T>(-x,-y,-z); }

template<typename T> const TVector3<T> TVector3<T>::operator-(const TVector3<T> &v) const
{
	return TVector3<T>(x-v.x,y-v.y,z-v.z);
}

template<typename T> TVector3<T> &TVector3<T>::operator-=(const TVector3<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template<typename T> const TVector3<T> TVector3<T>::operator*(const T &t) const
{
	return TVector3<T>(x*t,y*t,z*t);
}

template<typename T> TVector3<T> &TVector3<T>::operator*=(const T &t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

template<typename T> const TVector3<T> TVector3<T>::operator/(const T &t) const
{
	T recip = TMath<T>::reciprocal(t);
	return TVector3<T>(x*recip,y*recip,z*recip);
}

template<typename T> TVector3<T> &TVector3<T>::operator/=(const T &t)
{
	T recip = TMath<T>::reciprocal(t);
	x *= recip;
	y *= recip;
	z *= recip;
	return *this;
}

template<typename T> const bool TVector3<T>::operator==(const TVector3<T> &v) const
{
	//return x==v.x && y==v.y && z==v.z;
	return TMath<T>::equal(x,v.x) && TMath<T>::equal(y,v.y) && TMath<T>::equal(z,v.z);
}

template<typename T> const bool TVector3<T>::operator!=(const TVector3<T> &v) const
{
	//return x!=v.x || y!=v.y || z!=v.z;
	return !TMath<T>::equal(x,v.x) || !TMath<T>::equal(y,v.y) || !TMath<T>::equal(z,v.z);
}

template<typename T> std::ostream &operator<<(std::ostream &out,const TVector3<T> &v)
{
#ifdef RAW_STREAM_OUTPUT
	out << v.x << ' ' << v.y << ' ' << v.z;
#else
	out << '(' << v.x << ',' << v.y << ',' << v.z << ')';
#endif //RAW_STREAM_OUTPUT
	return out;
}

template<typename T> void TVector3<T>::clean()
{
	TMath<T>::clean(&x,3);
}

template<typename T> const bool TVector3<T>::isZero() const
{
	return TMath<T>::isZero(&x,3);
}

//get minimum component value
//store index and return the signed value
template<typename T> const T TVector3<T>::getMinAbsComponent(int &index) const
{
	//set to x-component
	index = 0;
	T value = x;
	//check other components
	if(TMath<T>::abs(value) > TMath<T>::abs(y))
	{
		index = 1;
		value = y;
	}
	if(TMath<T>::abs(value) > TMath<T>::abs(z))
	{
		index = 2;
		value = z;
	}
	return value;
}

//get maximum absolute component value
//store index and return the signed value
template<typename T> const T TVector3<T>::getMaxAbsComponent(int &index) const
{
	//set to x-component
	index = 0;
	T value = x;
	//check other components
	if(TMath<T>::abs(value) < TMath<T>::abs(y))
	{
		index = 1;
		value = y;
	}
	if(TMath<T>::abs(value) < TMath<T>::abs(z))
	{
		index = 2;
		value = z;
	}
	return value;
}

template<typename T> const T TVector3<T>::dot(const TVector3<T> &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

template<typename T> const TVector3<T> TVector3<T>::cross(const TVector3<T> &v) const
{
	return TVector3<T>
	(
		y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x
	);
}

template<typename T> const T TVector3<T>::length() const
{
	return TMath<T>::sqrt(x*x + y*y + z*z);
}

template<typename T> const T TVector3<T>::lengthSq() const
{
	return x*x + y*y + z*z;
}

template<typename T> void TVector3<T>::normalize()
{
	T recipLen = TMath<T>::reciprocal(length());
	x *= recipLen;
	y *= recipLen;
	z *= recipLen;
}

/*
//Project this onto v to get a vector parallel to v.
//
//The projection of a vector w onto a vector v is defined as:
//
//w_proj = [(v.w)/v.v]v
*/
template<typename T> const TVector3<T> TVector3<T>::parallelTo(const TVector3<T> &v,const bool normalized) const
{
	T factor = x*v.x + y*v.y + z*v.z;
	if(!normalized)
	{
		factor *= TMath<T>::reciprocal(v.x*v.x + v.y*v.y + v.z*v.z); //this could fail if v is a zero vector
	}
	return TVector3<T>(v.x*factor,v.y*factor,v.z*factor);
}

/*
//Project this onto v to get a vector perpendicular to v.
//
*/
template<typename T> const TVector3<T> TVector3<T>::perpendicularTo(const TVector3<T> &v,const bool normalized) const
{
	return *this - parallelTo(v,normalized); //this could fail if v is a zero vector
}

/*
//Orthogonalize the set of vectors v0,v1,v2 to the set w0,w1,w2 using the Gram–Schmidt process.
*/
template<typename T> void TVector3<T>::_orthogonalize
(
	const TVector3 &v0,const TVector3 &v1,const TVector3 &v2,
	TVector3 &w0,TVector3 &w1,TVector3 &w2,
	const bool normalizeBasis
)
{
	w0 = v0;
	if(normalizeBasis) w0.normalize();
	w1 = normalizeBasis ? v1 - v1.parallelTo(w0,true) : v1 - v1.parallelTo(w0);
	if(normalizeBasis) w1.normalize();
	w2 = normalizeBasis ? v2 - v2.parallelTo(w0,true) - v2.parallelTo(w1,true) : v2 - v2.parallelTo(w0) - v2.parallelTo(w1);
	if(normalizeBasis) w2.normalize();
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_VECTOR_3_HPP__
