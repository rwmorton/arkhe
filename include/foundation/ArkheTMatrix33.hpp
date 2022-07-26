#ifndef __ARKHE_T_MATRIX_33_HPP__
#define __ARKHE_T_MATRIX_33_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheMatrixPreproc.h"
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"
#include "ArkheTRay.hpp"

//std includes
#include <ostream>

namespace arkhe
{

namespace foundation
{

//forward class declarations
template<typename T> class TQuaternion;
template<typename T> class TMatrix44;

template<typename T> class TMatrix33
{
public:
	//enums
	enum TransformAs
	{
		VECTOR = 0, //default
		//there is no option for points as this
		//is the same as vector transformation
		NORMAL //normals require specific transformation
	}; //enum TransformAs
	enum InvertAs
	{
		FULL = 0, //full inverse
		ORTHOGONAL, //invert as orthogonal
		DIAGONAL //invert as diagonal
	}; //enum InvertAs
	//constructors
	TMatrix33() {} //we do not initialize the values
	TMatrix33
	(
		const T &_m00,const T &_m01,const T &_m02,
		const T &_m10,const T &_m11,const T &_m12,
		const T &_m20,const T &_m21,const T &_m22
	);
	TMatrix33(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3) { setColumns(col1,col2,col3); }
	TMatrix33(const TMatrix33 &copy);
	TMatrix33(const TMatrix44<T> &m4x4);
	//destructor
	~TMatrix33() {}
	//operator overloads
	TMatrix33 &operator=(const TMatrix33 &copy);
	operator const T*() const;
	operator T*();
	const T operator()(unsigned int i,unsigned int j) const;
	T &operator()(unsigned int i,unsigned int j);
	const TVector3<T> operator()(const TVector3<T> &v) const; //strictly vectors - for performance - no switch logic etc.
	const TVector3<T> operator()(const TVector3<T> &v,unsigned int transformAs,const bool isOrthogonal=false) const;
	const TRay<T> operator()(const TRay<T> &r) const;
	//TODO BOX etc.
	const TMatrix33<T> operator()(const TMatrix33<T> &m) const;
	const TMatrix33 operator+(const TMatrix33 &m) const;
	TMatrix33 &operator+=(const TMatrix33 &m);
	const TMatrix33 operator-() const;
	const TMatrix33 operator-(const TMatrix33 &m) const;
	TMatrix33 &operator-=(const TMatrix33 &m);
	const TMatrix33 operator*(const T &t) const;
	const TMatrix33 operator*(const TMatrix33 &m) const;
	TMatrix33 &operator*=(const T &t);
	TMatrix33 &operator*=(const TMatrix33 &m);
	const TMatrix33 operator/(const T &t) const;
	TMatrix33 &operator/=(const T &t);
	const bool operator==(const TMatrix33 &m) const;
	const bool operator!=(const TMatrix33 &m) const;
	template<typename T> friend std::ostream &operator<<(std::ostream &out,const TMatrix33<T> &m);
	//methods
	void clean();
	const bool isZero() const;
	void getColumns(TVector3<T> &col1,TVector3<T> &col2,TVector3<T> &col3) const;
	void setColumns(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3);
	const T trace() const;
	const T determinant() const;
	const TMatrix33 transpose() const;
	const TMatrix33 adjugate() const;
	const TMatrix33 inverse() const; //strictly full inverse - no case logic etc.
	const TMatrix33 inverse(unsigned int invertAs) const;
	//static methods
	static const TMatrix33 rotateX(const T &angle);
	static const TMatrix33 rotateY(const T &angle);
	static const TMatrix33 rotateZ(const T &angle);
	static const TMatrix33 rotate(const T &x,const T &y,const T &z);
	static const TMatrix33 rotate(const TVector3<T> &axis,const T &angle);
	static const TMatrix33 rotate(const TQuaternion<T> &q);
	static const TMatrix33 scale(const T &factor);
	static const TMatrix33 scale(const T &x,const T &y,const T &z);
	//static variables
	static const TMatrix33 ZERO;
	static const TMatrix33 IDENTITY;
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
		// |m00 m01 m02|
		// |m10 m11 m12|
		// |m20 m21 m22|
		struct
		{
			T m[9];
		};
		struct
		{
			//column 1
			T m00;
			T m10;
			T m20;
			//column 2
			T m01;
			T m11;
			T m21;
			//column 3
			T m02;
			T m12;
			T m22;
		};
	};
protected:
	//
}; //class TMatrix33

template<typename T> TMatrix33<T>::TMatrix33
(
	const T &_m00,const T &_m01,const T &_m02,
	const T &_m10,const T &_m11,const T &_m12,
	const T &_m20,const T &_m21,const T &_m22
) : m00(_m00),m01(_m01),m02(_m02),m10(_m10),m11(_m11),m12(_m12),m20(_m20),m21(_m21),m22(_m22) {}

template<typename T> TMatrix33<T>::TMatrix33(const TMatrix33<T> &copy)
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
}

template<typename T> TMatrix33<T>::TMatrix33(const TMatrix44<T> &m4x4)
{
	//col 1
	m[0] = m4x4.m[0];
	m[1] = m4x4.m[1];
	m[2] = m4x4.m[2];
	//col 2
	m[3] = m4x4.m[4];
	m[4] = m4x4.m[5];
	m[5] = m4x4.m[6];
	//col 3
	m[6] = m4x4.m[8];
	m[7] = m4x4.m[9];
	m[8] = m4x4.m[10];
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator=(const TMatrix33<T> &copy)
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
	return *this;
}

template<typename T> TMatrix33<T>::operator const T*() const { return &m[0]; }

template<typename T> TMatrix33<T>::operator T*() { return &m[0]; }

template<typename T> const T TMatrix33<T>::operator()(unsigned int i,unsigned int j) const
{
	if(i >= s_dimension || j >= s_dimension) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*s_dimension) + j];
}

template<typename T> T &TMatrix33<T>::operator()(unsigned int i,unsigned int j)
{
	if(i >= s_dimension || j >= s_dimension) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*s_dimension) + j];
}

template<typename T> const TVector3<T> TMatrix33<T>::operator()(const TVector3<T> &v) const
{
	return TVector3<T>
	(
		m00*v.x + m01*v.y + m02*v.z,
		m10*v.x + m11*v.y + m12*v.z,
		m20*v.x + m21*v.y + m22*v.z
	);
}

template<typename T> const TVector3<T> TMatrix33<T>::operator()(const TVector3<T> &v,unsigned int transformAs,const bool isOrthogonal) const
{
	TVector3<T> vPrime;
	if(transformAs == TMatrix33<T>::NORMAL && !isOrthogonal)
	{
		TMatrix33<T> it;
		ARKHE_MATRIX33_INVERSE_TRANSPOSE(T,m,it.m);
		ARKHE_MATRIX33_X_VECTOR3(it.m,v,vPrime);
	}
	else
	{
		ARKHE_MATRIX33_X_VECTOR3(m,v,vPrime);
	}
	return vPrime;
}

template<typename T> const TRay<T> TMatrix33<T>::operator()(const TRay<T> &r) const
{
	TRay<T> rPrime;
	ARKHE_MATRIX33_X_VECTOR3(m,r.origin,rPrime.origin);
	ARKHE_MATRIX33_X_VECTOR3(m,r.direction,rPrime.direction);
	return rPrime;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator()(const TMatrix33<T> &mtx) const
{
	return TMatrix33<T>
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22
	);
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator+(const TMatrix33<T> &mtx) const
{
	return TMatrix33<T>
	(
		m00+mtx.m00,m01+mtx.m01,m02+mtx.m02,
		m10+mtx.m10,m11+mtx.m11,m12+mtx.m12,
		m20+mtx.m20,m21+mtx.m21,m22+mtx.m22
	);
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator+=(const TMatrix33<T> &mtx)
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
	return *this;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator-() const
{
	return TMatrix33<T>
	(
		-m00,-m01,-m02,
		-m10,-m11,-m12,
		-m20,-m21,-m22
	);
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator-(const TMatrix33<T> &mtx) const
{
	return TMatrix33<T>
	(
		m00-mtx.m00,m01-mtx.m01,m02-mtx.m02,
		m10-mtx.m10,m11-mtx.m11,m12-mtx.m12,
		m20-mtx.m20,m21-mtx.m21,m22-mtx.m22
	);
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator-=(const TMatrix33<T> &mtx)
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
	return *this;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator*(const T &t) const
{
	return TMatrix33<T>
	(
		m00*t,m01*t,m02*t,
		m10*t,m11*t,m12*t,
		m20*t,m21*t,m22*t
	);
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator*(const TMatrix33<T> &mtx) const
{
	return TMatrix33<T>
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22
	);
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator*=(const T &t)
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
	return *this;
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator*=(const TMatrix33<T> &mtx)
{
	TMatrix33<T> res
	(
		m00*mtx.m00 + m01*mtx.m10 + m02*mtx.m20,
		m00*mtx.m01 + m01*mtx.m11 + m02*mtx.m21,
		m00*mtx.m02 + m01*mtx.m12 + m02*mtx.m22,
		m10*mtx.m00 + m11*mtx.m10 + m12*mtx.m20,
		m10*mtx.m01 + m11*mtx.m11 + m12*mtx.m21,
		m10*mtx.m02 + m11*mtx.m12 + m12*mtx.m22,
		m20*mtx.m00 + m21*mtx.m10 + m22*mtx.m20,
		m20*mtx.m01 + m21*mtx.m11 + m22*mtx.m21,
		m20*mtx.m02 + m21*mtx.m12 + m22*mtx.m22
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
	return *this;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::operator/(const T &t) const
{
	T recip = TMath<T>::reciprocal(t);
	return TMatrix33<T>
	(
		m00*recip,m01*recip,m02*recip,
		m10*recip,m11*recip,m12*recip,
		m20*recip,m21*recip,m22*recip
	);
}

template<typename T> TMatrix33<T> &TMatrix33<T>::operator/=(const T &t)
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
	return *this;
}

template<typename T> const bool TMatrix33<T>::operator==(const TMatrix33<T> &mtx) const
{
	return	m[0]==mtx.m[0] && m[1]==mtx.m[1] && m[2]==mtx.m[2] &&
			m[3]==mtx.m[3] && m[4]==mtx.m[4] && m[5]==mtx.m[5] &&
			m[6]==mtx.m[6] && m[7]==mtx.m[7] && m[8]==mtx.m[8];
}

template<typename T> const bool TMatrix33<T>::operator!=(const TMatrix33<T> &mtx) const
{
	return	m[0]!=mtx.m[0] || m[1]!=mtx.m[1] || m[2]!=mtx.m[2] ||
			m[3]!=mtx.m[3] || m[4]!=mtx.m[4] || m[5]!=mtx.m[5] ||
			m[6]!=mtx.m[6] || m[7]!=mtx.m[7] || m[8]!=mtx.m[8];
}

template<typename T> std::ostream &operator<<(std::ostream &out,const TMatrix33<T> &mtx)
{
#ifdef RAW_STREAM_OUTPUT
	for(int i=0; i<8; i++) out << mtx.m[i] << ' ';
	out << mtx.m[8];
#else
	out.precision(3);
	out.setf(std::ios::fixed);
	out << '|' << mtx.m00 << " " << mtx.m01 << " " << mtx.m02 << "|\n";
	out << '|' << mtx.m10 << " " << mtx.m11 << " " << mtx.m12 << "|\n";
	out << '|' << mtx.m20 << " " << mtx.m21 << " " << mtx.m22 << "|\n";
#endif //RAW_STREAM_OUTPUT
	return out;
}

template<typename T> void TMatrix33<T>::clean()
{
	TMath<T>::clean(m,s_numElements);
}

template<typename T> const bool TMatrix33<T>::isZero() const
{
	return TMath<T>::isZero(m,s_numElements);
}

template<typename T> void TMatrix33<T>::getColumns(TVector3<T> &col1,TVector3<T> &col2,TVector3<T> &col3) const
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

template<typename T> void TMatrix33<T>::setColumns(const TVector3<T> &col1,const TVector3<T> &col2,const TVector3<T> &col3)
{
	//col 1
	m00 = col1.x;
	m10 = col1.y;
	m20 = col1.z;
	//col 2
	m01 = col2.x;
	m11 = col2.y;
	m21 = col2.z;
	//col 3
	m02 = col3.x;
	m12 = col3.y;
	m22 = col3.z;
}

template<typename T> const T TMatrix33<T>::trace() const
{
	return m[0] + m[4] + m[8];
}

template<typename T> const T TMatrix33<T>::determinant() const
{
	T det;
	ARKHE_MATRIX33_DETERMINANT(m,det);
	return det;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::transpose() const
{
	return TMatrix33<T>
	(
		m00,m10,m20,
		m01,m11,m21,
		m02,m12,m22
	);
}

template<typename T> const TMatrix33<T> TMatrix33<T>::adjugate() const
{
	TMatrix33<T> res;
	ARKHE_MATRIX33_ADJUGATE(m,res.m);
	return res;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::inverse() const
{
	TMatrix33<T> res;
	ARKHE_MATRIX33_INVERSE(T,m,res.m);
	return res;
}

template<typename T> const TMatrix33<T> TMatrix33<T>::inverse(unsigned int invertAs) const
{
	TMatrix33 res;
	if(invertAs == TMatrix33<T>::FULL)
	{
		ARKHE_MATRIX33_INVERSE(T,m,res.m);
	}
	else if(invertAs == TMatrix33<T>::ORTHOGONAL)
	{
		res = TMatrix33<T>
		(
			m00,m10,m20,
			m01,m11,m21,
			m02,m12,m22
		);
	}
	else
	{
		//diagonal inverse
		res = TMatrix33<T>
		(
			TMath<T>::reciprocal(m[0]),TMath<T>::ZERO,TMath<T>::ZERO,
			TMath<T>::ZERO,TMath<T>::reciprocal(m[4]),TMath<T>::ZERO,
			TMath<T>::ZERO,TMath<T>::ZERO,TMath<T>::reciprocal(m[8])
		);
	}
	return res;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MATRIX_33_HPP__
