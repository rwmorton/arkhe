#ifndef __ARKHE_T_QUATERNION_HPP__
#define __ARKHE_T_QUATERNION_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"
#include "ArkheTMatrix33.hpp"
#include "ArkheException.h"

//std includes
#include <ostream>

namespace arkhe
{

namespace foundation
{

template<typename T> class TQuaternion
{
public:
	//constructors
	TQuaternion() : w(1),x(0),y(0),z(0) {}
	TQuaternion(const T &_w,const T &_x,const T &_y,const T &_z) : w(_w),x(_x),y(_y),z(_z) {}
	TQuaternion(const T &xRotation,const T &yRotation,const T &zRotation);
	TQuaternion(const TVector3<T> &v);
	TQuaternion(const TVector3<T> &axis,const T &angle);
	TQuaternion(const TMatrix33<T> &m);
	TQuaternion(const TQuaternion &copy);
	//destructor
	~TQuaternion() {}
	//operator overloads
	TQuaternion &operator=(const TQuaternion &copy);
	const T operator[](unsigned int i) const;
	T &operator[](unsigned int i);
	const TQuaternion operator+(const TQuaternion &q) const;
	TQuaternion &operator+=(const TQuaternion &q);
	const TQuaternion operator-() const;
	const TQuaternion operator-(const TQuaternion &q) const;
	TQuaternion &operator-=(const TQuaternion &q);
	const TQuaternion operator*(const T &t) const;
	const TQuaternion operator*(const TQuaternion &q) const;
	TQuaternion &operator*=(const T &t);
	TQuaternion &operator*=(const TQuaternion &q);
	const bool operator==(const TQuaternion &q) const;
	const bool operator!=(const TQuaternion &q) const;
	template<typename T> friend std::ostream &operator<<(std::ostream &out,const TQuaternion &q);
	//methods
	void clean();
	const bool isZero() const;
	const bool isUnit() const;
	const bool isIdentity() const;
	const T dot(const TQuaternion &q) const;
	const T magnitude() const;
	const T norm() const;
	void normalize();
	const TQuaternion conjugate() const;
	const TQuaternion inverse() const;
	void getAxisAngle(TVector3<T> &axis,T &angle);
	const TVector3<T> rotate(const TVector3<T> &v) const;
	void addScaledVector(const TVector3<T> &v,const T &t);
	//static methods
	static const TQuaternion lerp(const TQuaternion &start,const TQuaternion &end,const T &t);
	static const TQuaternion slerp(const TQuaternion &start,const TQuaternion &end,const T &t);
	static const TQuaternion approxSlerp(const TQuaternion &start,const TQuaternion &end,const T &t);
	//debugging
#ifdef ARKHE_DEBUG
	void render(const TVector3<T> &p=TVector3<T>::ZERO); //defer to another source file
#endif //ARKHE_DEBUG
	//static variables
	static const TQuaternion ZERO;
	static const TQuaternion IDENTITY;
	//variables
	union
	{
		struct
		{
			T w;
			T x;
			T y;
			T z;
		};
		struct
		{
			T _align[4];
		};
	};
private:
	//
protected:
	//
}; //class TQuaternion

template<typename T> TQuaternion<T>::TQuaternion(const T &xRotation,const T &yRotation,const T &zRotation)
{
	T Sx,Cx,Sy,Cy,Sz,Cz;

	TMath<T>::sinCos(xRotation * TMath<T>::ONE_OVER_TWO,Sx,Cx);
	TMath<T>::sinCos(yRotation * TMath<T>::ONE_OVER_TWO,Sy,Cy);
	TMath<T>::sinCos(zRotation * TMath<T>::ONE_OVER_TWO,Sz,Cz);

    w = Cx*Cy*Cz - Sx*Sy*Sz;
    x = Sx*Cy*Cz + Cx*Sy*Sz;
    y = Cx*Sy*Cz - Sx*Cy*Sz;
    z = Cx*Cy*Sz + Sx*Sy*Cx;
}

template<typename T> TQuaternion<T>::TQuaternion(const TVector3<T> &v) : w(TMath<T>::ZERO),x(v.x),y(v.y),z(v.z) {}

template<typename T> TQuaternion<T>::TQuaternion(const TVector3<T> &axis,const T &angle)
{
	T lenSq = axis.lengthSq();
	if(TMath<T>::isZero(lenSq))
	{
		w = TMath<T>::ONE;
		x = y = z = TMath<T>::ZERO;
	}

	T s,c;
	TMath<T>::sinCos(angle * TMath<T>::ONE_OVER_TWO,s,c);
	T scale = s/TMath<T>::sqrt(lenSq);

	w = c;
	x = axis.x * scale;
	y = axis.y * scale;
	z = axis.z * scale;
}

template<typename T> TQuaternion<T>::TQuaternion(const TMatrix33<T> &m)
{
	throw Exception(Exception::NO_IMPLEMENTATION); //TODO
}

template<typename T> TQuaternion<T>::TQuaternion(const TQuaternion<T> &copy)
{
	w = copy.w;
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

template<typename T> TQuaternion<T> &TQuaternion<T>::operator=(const TQuaternion<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	w = copy.w;
	x = copy.x;
	y = copy.y;
	z = copy.z;
	return *this;
}

template<typename T> const T TQuaternion<T>::operator[](unsigned int i) const
{
	if(i >= 4) throw Exception(Exception::OUT_OF_RANGE);
	return (&w)[i];
}

template<typename T> T &TQuaternion<T>::operator[](unsigned int i)
{
	if(i >= 4) throw Exception(Exception::OUT_OF_RANGE);
	return (&w)[i];
}

template<typename T> const TQuaternion<T> TQuaternion<T>::operator+(const TQuaternion<T> &q) const
{
	return TQuaternion<T>(w+q.w,x+q.x,y+q.y,z+q.z);
}

template<typename T> TQuaternion<T> &TQuaternion<T>::operator+=(const TQuaternion<T> &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

template<typename T> const TQuaternion<T> TQuaternion<T>::operator-() const
{
	return TQuaternion<T>(-w,-x,-y,-z);
}

template<typename T> const TQuaternion<T> TQuaternion<T>::operator-(const TQuaternion<T> &q) const
{
	return TQuaternion<T>(w-q.w,x-q.x,y-q.y,z-q.z);
}

template<typename T> TQuaternion<T> &TQuaternion<T>::operator-=(const TQuaternion<T> &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

template<typename T> const TQuaternion<T> TQuaternion<T>::operator*(const T &t) const
{
	return TQuaternion<T>(w*t,x*t,y*t,z*t);
}

template<typename T> const TQuaternion<T> TQuaternion<T>::operator*(const TQuaternion<T> &q) const
{
    return TQuaternion<T>
	(
		w*q.w - x*q.x - y*q.y - z*q.z,
		w*q.x + x*q.w + y*q.z - z*q.y,
		w*q.y + y*q.w + z*q.x - x*q.z,
		w*q.z + z*q.w + x*q.y - y*q.x
	);
}

template<typename T> TQuaternion<T> &TQuaternion<T>::operator*=(const T &t)
{
	w *= t;
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

template<typename T> TQuaternion<T> &TQuaternion<T>::operator*=(const TQuaternion<T> &q)
{
	TQuaternion<T> res = *this * q;
	*this = res;
	return *this;
}

template<typename T> const bool TQuaternion<T>::operator==(const TQuaternion<T> &q) const
{
	return w==q.w && x==q.x && y==q.y && z==q.z;
}

template<typename T> const bool TQuaternion<T>::operator!=(const TQuaternion<T> &q) const
{
	return w!=q.w || x!=q.x || y!=q.y || z!=q.z;
}

template<typename T> std::ostream &operator<<(std::ostream &out,const TQuaternion<T> &q)
{
#ifdef RAW_STREAM_OUTPUT
	out << q.w << ' ' << q.x << ' ' << q.y << ' ' << q.z;
#else
	out << '(' << q.w << ',' << q.x << ',' << q.y << ',' << q.z << ')';
#endif //RAW_OUTPUT
	return out;
}

template<typename T> void TQuaternion<T>::clean()
{
	TMath<T>::clean(&w,4);
}

template<typename T> const bool TQuaternion<T>::isZero() const
{
	return TMath<T>::isZero(w*w + x*x + y*y + z*z);
}

template<typename T> const bool TQuaternion<T>::isUnit() const
{
	return TMath<T>::isZero(TMath<T>::ONE - w*w - x*x - y*y - z*z);
}

template<typename T> const bool TQuaternion<T>::isIdentity() const
{
	return TMath<T>::equal(w,TMath<T>::ONE) && TMath<T>::isZero(&x,3);
}

template<typename T> const T TQuaternion<T>::dot(const TQuaternion &q) const
{
	return w*q.w + x*q.x + y*q.y + z*q.z;
}

template<typename T> const T TQuaternion<T>::magnitude() const
{
	return TMath<T>::sqrt(w*w + x*x + y*y + z*z);
}

template<typename T> const T TQuaternion<T>::norm() const
{
	return w*w + x*x + y*y + z*z;
}

template<typename T> void TQuaternion<T>::normalize()
{
	T recipNorm = TMath<T>::reciprocal(norm());
	w *= recipNorm;
	x *= recipNorm;
	y *= recipNorm;
	z *= recipNorm;
}

template<typename T> const TQuaternion<T> TQuaternion<T>::conjugate() const
{
	return TQuaternion<T>(w,-x,-y,-z);
}

template<typename T> const TQuaternion<T> TQuaternion<T>::inverse() const
{
	T norm = norm();
	if(isZero()) return TQuaternion<T>::IDENTITY;
	T normRecip = TMath<T>::reciprocal(norm);
	return TQuaternion<T>(w * normRecip,-x * normRecip,-y * normRecip,-z * normRecip);
}

template<typename T> void TQuaternion<T>::getAxisAngle(TVector3<T> &axis,T &angle)
{
	angle = TMath<T>::TWO * TMath<T>::aCos(w);
	T len = TMath<T>::sqrt(TMath<T>::ONE - w*w);
	
	if(TMath<T>::isZero(len))
	{
		axis = TVector3<T>::ZERO;
	}
	else
	{
		lenRecip = TMath<T>::reciprocal(len);
		axis.x = x * lenRecip;
		axis.y = y * lenRecip;
		axis.z = z * lenRecip;
	}
}

//Assumes quaternion is normalized
template<typename T> const TVector3<T> TQuaternion<T>::rotate(const TVector3<T> &v) const
{
	if(!isUnit()) throw Exception(Exception::LOGIC_ERROR,"not a unit quaternion");
	T a = (x*v.x + y*v.y + z*v.z) * TMath<T>::TWO;
	T b = w * TMath<T>::TWO;
	T c = b*w - TMath<T>::ONE;

	return TVector3<T>
	(
		c*v.x + a*x + b*(y*v.z - z*v.y),
		c*v.y + a*y + b*(z*v.x - x*v.z),
		c*v.z + a*z + b*(x*v.y - y*v.x)
	);
}

template<typename T> void TQuaternion<T>::addScaledVector(const TVector3<T> &v,const T &t)
{
	TQuaternion<T> q(TMath<T>::ZERO,v.x*t,v.y*t,v.z*t);
	q *= *this;
	w += q.w * TMath<T>::ONE_OVER_TWO;
	x += q.x * TMath<T>::ONE_OVER_TWO;
	y += q.y * TMath<T>::ONE_OVER_TWO;
	z += q.z * TMath<T>::ONE_OVER_TWO;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_QUATERNION_HPP__
