#ifndef __ARKHE_T_MATRIX_44_HPP__
#define __ARKHE_T_MATRIX_44_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheMatrixPreproc.h"
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"
#include "ArkheTRay.hpp"
#include "ArkheTLine.hpp"
#include "ArkheTLineSegment.hpp"
#include "ArkheTTriangle.hpp"
#include "ArkheTSphere.hpp"
#include "ArkheTTetrahedron.hpp"

//std includes
#include <ostream>

namespace arkhe
{

namespace foundation
{

//forward class declarations
template<typename T> class TQuaternion;
template<typename T> class TMatrix33;

template<typename T> class TMatrix44
{
public:
	//enums
	enum TransformAs
	{
		VECTOR = 0, //default
		POINT, //use homogenous coordinate
		NORMAL //normals require specific transformation
	}; //enum TransformAs
	enum InvertAs
	{
		FULL = 0, //full inverse
		ORTHOGONAL, //invert as orthogonal
		DIAGONAL //invert as diagonal
	}; //enum InvertAs
	//constructors
	TMatrix44() {} //we do not initialize the values
	TMatrix44
	(
		const T &_m00,const T &_m01,const T &_m02,const T &_m03,
		const T &_m10,const T &_m11,const T &_m12,const T &_m13,
		const T &_m20,const T &_m21,const T &_m22,const T &_m23,
		const T &_m30,const T &_m31,const T &_m32,const T &_m33
	);
	TMatrix44(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3) { setColumns(col1,col2,col3); }
	TMatrix44(const TMatrix33<T> &m3x3);
	TMatrix44(const TMatrix44 &copy);
	//destructor
	~TMatrix44() {}
	//operator overloads
	TMatrix44 &operator=(const TMatrix44 &copy);
	operator const T*() const;
	operator T*();
	const T operator()(unsigned int i,unsigned int j) const;
	T &operator()(unsigned int i,unsigned int j);	
	const TVector3<T> operator()(const TVector3<T> &v) const; //strictly vectors - for performance - no switch logic etc.
	const TVector3<T> operator()(const TVector3<T> &v,unsigned int transformAs,const bool isOrthogonal=false) const;
	const TRay<T> operator()(const TRay<T> &r) const;
	//
	const TLine<T> operator()(const TLine<T> &l) const;
	const TLineSegment<T> operator()(const TLineSegment<T> &l) const;
	const TTriangle<T> operator()(const TTriangle<T> &t) const;
	const TSphere<T> operator()(const TSphere<T> &s) const;
	const TTetrahedron<T> operator()(const TTetrahedron<T> &t) const;
	//
	const TMatrix44<T> operator()(const TMatrix44<T> &m) const;
	const TMatrix44 operator+(const TMatrix44 &m) const;
	TMatrix44 &operator+=(const TMatrix44 &m);
	const TMatrix44 operator-() const;
	const TMatrix44 operator-(const TMatrix44 &m) const;
	TMatrix44 &operator-=(const TMatrix44 &m);
	const TMatrix44 operator*(const T &t) const;
	const TMatrix44 operator*(const TMatrix44 &m) const;
	TMatrix44 &operator*=(const T &t);
	TMatrix44 &operator*=(const TMatrix44 &m);
	const TMatrix44 operator/(const T &t) const;
	TMatrix44 &operator/=(const T &t);
	const bool operator==(const TMatrix44 &m) const;
	const bool operator!=(const TMatrix44 &m) const;
	template<typename T> friend std::ostream &operator<<(std::ostream &out,const TMatrix44<T> &m);
	//methods
	void clean();
	const bool isZero() const;
	void getColumns(TVector3<T> &col1,TVector3<T> &col2,TVector3<T> &col3) const;
	void setColumns(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3);
	const T trace() const;
	const T determinant() const;
	const TMatrix44 transpose() const;
	const TMatrix44 adjugate() const;
	const TMatrix44 inverse() const; //strictly full inverse - no case logic etc.
	const TMatrix44 inverse(unsigned int invertAs) const;
	//static methods
	static const TMatrix44 rotateX(const T &angle);
	static const TMatrix44 rotateY(const T &angle);
	static const TMatrix44 rotateZ(const T &angle);
	static const TMatrix44 rotate(const T &x,const T &y,const T &z);
	static const TMatrix44 rotate(const TVector3<T> &axis,const T &angle);
	static const TMatrix44 rotate(const TQuaternion<T> &q);
	static const TMatrix44 scale(const T &factor);
	static const TMatrix44 scale(const T &x,const T &y,const T &z);
	static const TMatrix44 translate(const T &x,const T &y,const T &z);
	static const TMatrix44 translate(const TVector3<T> &t);
	static const TMatrix44 lookAt(const TVector3<T> &eye,const TVector3<T> &lookAt,const TVector3<T> &up);
	static const TMatrix44 orthographic(const T &left,const T &right,const T &top,const T &bottom,const T &near,const T &far);
	static const TMatrix44 perspective(const T &fov,const T &aspect,const T &near,const T &far); //fov is in degrees! makes more sense that way...
	//static variables
	static const TMatrix44 ZERO;
	static const TMatrix44 IDENTITY;
	//debugging
#ifdef ARKHE_DEBUG
	void render(const TVector3<T> &p=TVector3<T>::ZERO); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//static variables
	static const unsigned int s_dimension;
	static const unsigned int s_numElements;
	//variables
	union
	{
		//store as column-major
		// |m00 m01 m02 m03|
		// |m10 m11 m12 m13|
		// |m20 m21 m22 m23|
		// |m30 m31 m32 m33|
		struct
		{
			T m[16];
		};
		struct
		{
			//column 1
			T m00;
			T m10;
			T m20;
			T m30;
			//column 2
			T m01;
			T m11;
			T m21;
			T m31;
			//column 3
			T m02;
			T m12;
			T m22;
			T m32;
			//column 4
			T m03;
			T m13;
			T m23;
			T m33;
		};
	};
protected:
	//
}; //class TMatrix44

template<typename T> TMatrix44<T>::TMatrix44
(
	const T &_m00,const T &_m01,const T &_m02,const T &_m03,
	const T &_m10,const T &_m11,const T &_m12,const T &_m13,
	const T &_m20,const T &_m21,const T &_m22,const T &_m23,
	const T &_m30,const T &_m31,const T &_m32,const T &_m33
) : m00(_m00),m01(_m01),m02(_m02),m03(_m03),m10(_m10),m11(_m11),m12(_m12),m13(_m13),m20(_m20),m21(_m21),m22(_m22),m23(_m23),m30(_m30),m31(_m31),m32(_m32),m33(_m33) {}

template<typename T> TMatrix44<T>::TMatrix44(const TMatrix33<T> &m3x3)
{
	//row 1
	m00 = m3x3.m00;
	m01 = m3x3.m01;
	m02 = m3x3.m02;
	m03 = TMath<T>::ZERO;
	//row 2
	m10 = m3x3.m10;
	m11 = m3x3.m11;
	m12 = m3x3.m12;
	m13 = TMath<T>::ZERO;
	//row 3
	m20 = m3x3.m20;
	m21 = m3x3.m21;
	m22 = m3x3.m22;
	m23 = TMath<T>::ZERO;
	//row 4
	m30 = m31 = m32 = TMath<T>::ZERO;
	m33 = TMath<T>::ONE;
}

template<typename T> TMatrix44<T>::TMatrix44(const TMatrix44<T> &copy)
{
	m[0] = copy.m[0];
	m[1] = copy.m[1];
	m[2] = copy.m[2];
	m[3] = copy.m[3];
	m[4] = copy.m[4];
	m[5] = copy.m[5];
	m[6] = copy.m[6];
	m[7] = copy.m[7];
	m[8] = copy.m[8];
	m[9] = copy.m[9];
	m[10] = copy.m[10];
	m[11] = copy.m[11];
	m[12] = copy.m[12];
	m[13] = copy.m[13];
	m[14] = copy.m[14];
	m[15] = copy.m[15];
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator=(const TMatrix44<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	m[0] = copy.m[0];
	m[1] = copy.m[1];
	m[2] = copy.m[2];
	m[3] = copy.m[3];
	m[4] = copy.m[4];
	m[5] = copy.m[5];
	m[6] = copy.m[6];
	m[7] = copy.m[7];
	m[8] = copy.m[8];
	m[9] = copy.m[9];
	m[10] = copy.m[10];
	m[11] = copy.m[11];
	m[12] = copy.m[12];
	m[13] = copy.m[13];
	m[14] = copy.m[14];
	m[15] = copy.m[15];
	return *this;
}

template<typename T> TMatrix44<T>::operator const T*() const { return &m[0]; }

template<typename T> TMatrix44<T>::operator T*() { return &m[0]; }

template<typename T> const T TMatrix44<T>::operator()(unsigned int i,unsigned int j) const
{
	if(i >= s_dimension || j >= s_dimension) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*s_dimension) + j];
}

template<typename T> T &TMatrix44<T>::operator()(unsigned int i,unsigned int j)
{
	if(i >= s_dimension || j >= s_dimension) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*s_dimension) + j];
}

template<typename T> const TVector3<T> TMatrix44<T>::operator()(const TVector3<T> &v) const
{
	return TVector3<T>
	(
		m00*v.x + m01*v.y + m02*v.z,
		m10*v.x + m11*v.y + m12*v.z,
		m20*v.x + m21*v.y + m22*v.z
	);
}

template<typename T> const TVector3<T> TMatrix44<T>::operator()(const TVector3<T> &v,unsigned int transformAs,const bool isOrthogonal) const
{
	TVector3<T> vPrime;
	if(transformAs == TMatrix44<T>::POINT)
	{
		ARKHE_MATRIX44_X_HOMOGENOUS_UNIT_W_POINT(m,v,vPrime); //transform as point, treat w as 1.
	}
	else if(transformAs == TMatrix44<T>::NORMAL && !isOrthogonal)
	{
		TMatrix44<T> it;
		ARKHE_MATRIX44_INVERSE_TRANSPOSE(T,m,it.m);
		ARKHE_MATRIX44_X_VECTOR3(it.m,v,vPrime);
	}
	else
	{
		ARKHE_MATRIX44_X_VECTOR3(m,v,vPrime);
	}
	return vPrime;
}

template<typename T> const TRay<T> TMatrix44<T>::operator()(const TRay<T> &r) const
{
	TRay<T> rPrime;
	rPrime.origin = (*this)(r.origin,TMatrix44<T>::POINT);
	rPrime.direction = (*this)(r.direction);
	return rPrime;
}

template<typename T> const TLine<T> TMatrix44<T>::operator()(const TLine<T> &l) const
{
	TLine<T> lPrime;
	lPrime.origin = (*this)(l.origin,TMatrix44<T>::POINT);
	lPrime.direction = (*this)(l.direction);
	return lPrime;
}

template<typename T> const TLineSegment<T> TMatrix44<T>::operator()(const TLineSegment<T> &l) const
{
	TLineSegment<T> lPrime;
	lPrime.start = (*this)(l.start,TMatrix44<T>::POINT);
	lPrime.end = (*this)(l.end,TMatrix44<T>::POINT);
	return lPrime;
}

template<typename T> const TTriangle<T> TMatrix44<T>::operator()(const TTriangle<T> &t) const
{
	TTriangle<T> tPrime;
	tPrime.p0 = (*this)(t.p0,TMatrix44<T>::POINT);
	tPrime.p1 = (*this)(t.p1,TMatrix44<T>::POINT);
	tPrime.p2 = (*this)(t.p2,TMatrix44<T>::POINT);
	return tPrime;
}

template<typename T> const TSphere<T> TMatrix44<T>::operator()(const TSphere<T> &s) const
{
	TSphere<T> sPrime;
	sPrime.center = (*this)(s.center,TMatrix44<T>::POINT);
	return sPrime;
}

template<typename T> const TTetrahedron<T> TMatrix44<T>::operator()(const TTetrahedron<T> &t) const
{
	TTetrahedron<T> tPrime;
	//transform points
	tPrime.p0 = (*this)(t.p0,TMatrix44<T>::POINT);
	tPrime.p1 = (*this)(t.p1,TMatrix44<T>::POINT);
	tPrime.p2 = (*this)(t.p2,TMatrix44<T>::POINT);
	tPrime.p3 = (*this)(t.p3,TMatrix44<T>::POINT);
	//transform normals
	tPrime.n0 = (*this)(t.n0,TMatrix44<T>::NORMAL);
	tPrime.n1 = (*this)(t.n1,TMatrix44<T>::NORMAL);
	tPrime.n2 = (*this)(t.n2,TMatrix44<T>::NORMAL);
	tPrime.n3 = (*this)(t.n3,TMatrix44<T>::NORMAL);
	//renormalize normals
	tPrime.n0.normalize();
	tPrime.n1.normalize();
	tPrime.n2.normalize();
	tPrime.n3.normalize();
	return tPrime;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator()(const TMatrix44<T> &mtx) const
{
	return TMatrix44<T>
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20 + m03*mtx.m30,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21 + m03*mtx.m31,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22 + m03*mtx.m32,
		m00*mtx.m03 + m01*mtx.m13 + m02*mtx.m23 + m03*mtx.m33,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20 + m13*mtx.m30,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21 + m13*mtx.m31,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22 + m13*mtx.m32,
		m10*mtx.m03 + m11*mtx.m13 + m12*mtx.m23 + m13*mtx.m33,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20 + m23*mtx.m30,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21 + m23*mtx.m31,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22 + m23*mtx.m32,
		m20*mtx.m03 + m21*mtx.m13 + m22*mtx.m23 + m23*mtx.m33,
		m30*mtx.m00 + m31*mtx.m10 + m32*mtx.m20 + m33*mtx.m30,
		m30*mtx.m01 + m31*mtx.m11 + m32*mtx.m21 + m33*mtx.m31,
		m30*mtx.m02 + m31*mtx.m12 + m32*mtx.m22 + m33*mtx.m32,
		m30*mtx.m03 + m31*mtx.m13 + m32*mtx.m23 + m33*mtx.m33
	);
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator+(const TMatrix44<T> &mtx) const
{
	return TMatrix44<T>
	(
		m00+mtx.m00,m01+mtx.m01,m02+mtx.m02,m03+mtx.m03,
		m10+mtx.m10,m11+mtx.m11,m12+mtx.m12,m13+mtx.m13,
		m20+mtx.m20,m21+mtx.m21,m22+mtx.m22,m23+mtx.m23,
		m30+mtx.m30,m31+mtx.m31,m32+mtx.m32,m33+mtx.m33
	);
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator+=(const TMatrix44<T> &mtx)
{
	m[0] += mtx.m[0];
	m[1] += mtx.m[1];
	m[2] += mtx.m[2];
	m[3] += mtx.m[3];
	m[4] += mtx.m[4];
	m[5] += mtx.m[5];
	m[6] += mtx.m[6];
	m[7] += mtx.m[7];
	m[8] += mtx.m[8];
	m[9] += mtx.m[9];
	m[10] += mtx.m[10];
	m[11] += mtx.m[11];
	m[12] += mtx.m[12];
	m[13] += mtx.m[13];
	m[14] += mtx.m[14];
	m[15] += mtx.m[15];
	return *this;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator-() const
{
	return TMatrix44<T>
	(
		-m00,-m01,-m02,-m03,
		-m10,-m11,-m12,-m13,
		-m20,-m21,-m22,-m23,
		-m30,-m31,-m32,-m33
	);
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator-(const TMatrix44<T> &mtx) const
{
	return TMatrix44<T>
	(
		m00-mtx.m00,m01-mtx.m01,m02-mtx.m02,m03-mtx.m03,
		m10-mtx.m10,m11-mtx.m11,m12-mtx.m12,m13-mtx.m13,
		m20-mtx.m20,m21-mtx.m21,m22-mtx.m22,m23-mtx.m23,
		m30-mtx.m30,m31-mtx.m31,m32-mtx.m32,m33-mtx.m33
	);
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator-=(const TMatrix44<T> &mtx)
{
	m[0] -= mtx.m[0];
	m[1] -= mtx.m[1];
	m[2] -= mtx.m[2];
	m[3] -= mtx.m[3];
	m[4] -= mtx.m[4];
	m[5] -= mtx.m[5];
	m[6] -= mtx.m[6];
	m[7] -= mtx.m[7];
	m[8] -= mtx.m[8];
	m[9] -= mtx.m[9];
	m[10] -= mtx.m[10];
	m[11] -= mtx.m[11];
	m[12] -= mtx.m[12];
	m[13] -= mtx.m[13];
	m[14] -= mtx.m[14];
	m[15] -= mtx.m[15];
	return *this;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator*(const T &t) const
{
	return TMatrix44<T>
	(
		m00*t,m01*t,m02*t,m03*t,
		m10*t,m11*t,m12*t,m13*t,
		m20*t,m21*t,m22*t,m23*t,
		m30*t,m31*t,m32*t,m33*t
	);
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator*(const TMatrix44<T> &mtx) const
{
	return TMatrix44<T>
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20 + m03*mtx.m30,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21 + m03*mtx.m31,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22 + m03*mtx.m32,
		m00*mtx.m03 + m01*mtx.m13 + m02*mtx.m23 + m03*mtx.m33,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20 + m13*mtx.m30,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21 + m13*mtx.m31,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22 + m13*mtx.m32,
		m10*mtx.m03 + m11*mtx.m13 + m12*mtx.m23 + m13*mtx.m33,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20 + m23*mtx.m30,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21 + m23*mtx.m31,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22 + m23*mtx.m32,
		m20*mtx.m03 + m21*mtx.m13 + m22*mtx.m23 + m23*mtx.m33,
		m30*mtx.m00 + m31*mtx.m10 + m32*mtx.m20 + m33*mtx.m30,
		m30*mtx.m01 + m31*mtx.m11 + m32*mtx.m21 + m33*mtx.m31,
		m30*mtx.m02 + m31*mtx.m12 + m32*mtx.m22 + m33*mtx.m32,
		m30*mtx.m03 + m31*mtx.m13 + m32*mtx.m23 + m33*mtx.m33
	);
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator*=(const T &t)
{
	m[0] *= t;
	m[1] *= t;
	m[2] *= t;
	m[3] *= t;
	m[4] *= t;
	m[5] *= t;
	m[6] *= t;
	m[7] *= t;
	m[8] *= t;
	m[9] *= t;
	m[10] *= t;
	m[11] *= t;
	m[12] *= t;
	m[13] *= t;
	m[14] *= t;
	m[15] *= t;
	return *this;
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator*=(const TMatrix44<T> &mtx)
{
	return TMatrix44<T> res
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20 + m03*mtx.m30,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21 + m03*mtx.m31,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22 + m03*mtx.m32,
		m00*mtx.m03 + m01*mtx.m13 + m02*mtx.m23 + m03*mtx.m33,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20 + m13*mtx.m30,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21 + m13*mtx.m31,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22 + m13*mtx.m32,
		m10*mtx.m03 + m11*mtx.m13 + m12*mtx.m23 + m13*mtx.m33,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20 + m23*mtx.m30,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21 + m23*mtx.m31,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22 + m23*mtx.m32,
		m20*mtx.m03 + m21*mtx.m13 + m22*mtx.m23 + m23*mtx.m33,
		m30*mtx.m00 + m31*mtx.m10 + m32*mtx.m20 + m33*mtx.m30,
		m30*mtx.m01 + m31*mtx.m11 + m32*mtx.m21 + m33*mtx.m31,
		m30*mtx.m02 + m31*mtx.m12 + m32*mtx.m22 + m33*mtx.m32,
		m30*mtx.m03 + m31*mtx.m13 + m32*mtx.m23 + m33*mtx.m33
	);
	m[0] = res.m[0];
	m[1] = res.m[1];
	m[2] = res.m[2];
	m[3] = res.m[3];
	m[4] = res.m[4];
	m[5] = res.m[5];
	m[6] = res.m[6];
	m[7] = res.m[7];
	m[8] = res.m[8];
	m[9] = res.m[9];
	m[10] = res.m[10];
	m[11] = res.m[11];
	m[12] = res.m[12];
	m[13] = res.m[13];
	m[14] = res.m[14];
	m[15] = res.m[15];
	return *this;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::operator/(const T &t) const
{
	T recip = TMath<T>::reciprocal(t);
	return TMatrix44<T>
	(
		m00*recip,m01*recip,m02*recip,m03*recip,
		m10*recip,m11*recip,m12*recip,m13*recip,
		m20*recip,m21*recip,m22*recip,m23*recip,
		m30*recip,m31*recip,m32*recip,m33*recip
	);
}

template<typename T> TMatrix44<T> &TMatrix44<T>::operator/=(const T &t)
{
	T recip = TMath<T>::reciprocal(t);
	m[0] *= recip;
	m[1] *= recip;
	m[2] *= recip;
	m[3] *= recip;
	m[4] *= recip;
	m[5] *= recip;
	m[6] *= recip;
	m[7] *= recip;
	m[8] *= recip;
	m[9] *= recip;
	m[10] *= recip;
	m[11] *= recip;
	m[12] *= recip;
	m[13] *= recip;
	m[14] *= recip;
	m[15] *= recip;
	return *this;
}

template<typename T> const bool TMatrix44<T>::operator==(const TMatrix44<T> &mtx) const
{
	return	m[0]==mtx.m[0] && m[1]==mtx.m[1] && m[2]==mtx.m[2] && m[3]==mtx.m[3] &&
			m[4]==mtx.m[4] && m[5]==mtx.m[5] && m[6]==mtx.m[6] && m[7]==mtx.m[7] &&
			m[8]==mtx.m[8] && m[9]==mtx.m[9] && m[10]==mtx.m[10] && m[11]==mtx.m[11] &&
			m[12]==mtx.m[12] && m[13]==mtx.m[13] && m[14]==mtx.m[14] && m[15]==mtx.m[15];
}

template<typename T> const bool TMatrix44<T>::operator!=(const TMatrix44<T> &mtx) const
{
	return	m[0]!=mtx.m[0] || m[1]!=mtx.m[1] || m[2]!=mtx.m[2] || m[3]!=mtx.m[3] ||
			m[4]!=mtx.m[4] || m[5]!=mtx.m[5] || m[6]!=mtx.m[6] || m[7]!=mtx.m[7] ||
			m[8]!=mtx.m[8] || m[9]!=mtx.m[9] || m[10]!=mtx.m[10] || m[11]!=mtx.m[11] ||
			m[12]!=mtx.m[12] || m[13]!=mtx.m[13] || m[14]!=mtx.m[14] || m[15]!=mtx.m[15];
}

template<typename T> std::ostream &operator<<(std::ostream &out,const TMatrix44<T> &mtx)
{
#ifdef RAW_STREAM_OUTPUT
	for(int i=0; i<15; i++) out << mtx.m[i] << ' ';
	out << mtx.m[15];
#else
	out.precision(3);
	out.setf(std::ios::fixed);
	out << '|' << mtx.m00 << " " << mtx.m01 << " " << mtx.m02 << " " << mtx.m03 << "|\n";
	out << '|' << mtx.m10 << " " << mtx.m11 << " " << mtx.m12 << " " << mtx.m13 << "|\n";
	out << '|' << mtx.m20 << " " << mtx.m21 << " " << mtx.m22 << " " << mtx.m23 << "|\n";
	out << '|' << mtx.m30 << " " << mtx.m31 << " " << mtx.m32 << " " << mtx.m33 << "|\n";
#endif //RAW_STREAM_OUTPUT
	return out;
}

template<typename T> void TMatrix44<T>::clean()
{
	TMath<T>::clean(m,s_numElements);
}

template<typename T> const bool TMatrix44<T>::isZero() const
{
	return TMath<T>::isZero(m,s_numElements);
}

template<typename T> void TMatrix44<T>::getColumns(TVector3<T> &col1,TVector3<T> &col2,TVector3<T> &col3) const
{
	//col 1
	col1.x = m00;
	col1.y = m10;
	col1.z = m20;
	//col 2
	col2.x = m01;
	col2.y = m11;
	col2.z = m21;
	//col 3
	col3.x = m02;
	col3.y = m12;
	col3.z = m22;
}

template<typename T> void TMatrix44<T>::setColumns(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3)
{
	//col 1
	m00 = col1.x;
	m10 = col1.y;
	m20 = col1.z;
	m30 = TMath<T>::ZERO;
	//col 2
	m01 = col2.x;
	m11 = col2.y;
	m21 = col2.z;
	m31 = TMath<T>::ZERO;;
	//col 3
	m02 = col3.x;
	m12 = col3.y;
	m22 = col3.z;
	m32 = TMath<T>::ZERO;;
	//col 4
	m03 = m13 = m23 = TMath<T>::ZERO;
	m33 = TMath<T>::ONE;
}

template<typename T> const T TMatrix44<T>::trace() const
{
	return m[0] + m[5] + m[10] + m[15];
}

template<typename T> const T TMatrix44<T>::determinant() const
{
	T det;
	ARKHE_MATRIX44_DETERMINANT(m,det);
	return det;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::transpose() const
{
	return TMatrix44<T>
	(
		m00,m10,m20,m30,
		m01,m11,m21,m31,
		m02,m12,m22,m32,
		m03,m13,m23,m33
	);
}

template<typename T> const TMatrix44<T> TMatrix44<T>::adjugate() const
{
	TMatrix44<T> res;
	ARKHE_MATRIX44_ADJUGATE(T,m,res.m);
	return res;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::inverse() const
{
	TMatrix44<T> res;
	ARKHE_MATRIX44_INVERSE(T,m,res.m);
	return res;
}

template<typename T> const TMatrix44<T> TMatrix44<T>::inverse(unsigned int invertAs) const
{
	TMatrix44 res;
	if(invertAs == TMatrix44<T>::FULL)
	{
		ARKHE_MATRIX44_INVERSE(T,m,res.m);
	}
	else if(invertAs == TMatrix44<T>::ORTHOGONAL)
	{
		res = TMatrix44<T>
		(
			m00,m10,m20,m30,
			m01,m11,m21,m31,
			m02,m12,m22,m32,
			m03,m13,m23,m33
		);
	}
	else
	{
		//diagonal inverse
		res = TMatrix44<T>
		(
			TMath<T>::reciprocal(m[0]),TMath<T>::ZERO,TMath<T>::ZERO,TMath<T>::ZERO,
			TMath<T>::ZERO,TMath<T>::reciprocal(m[5]),TMath<T>::ZERO,TMath<T>::ZERO,
			TMath<T>::ZERO,TMath<T>::ZERO,TMath<T>::reciprocal(m[10]),TMath<T>::ZERO,
			TMath<T>::ZERO,TMath<T>::ZERO,TMath<T>::ZERO,TMath<T>::reciprocal(m[15])
		);
	}
	return res;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MATRIX_44_HPP__
