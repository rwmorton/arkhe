#ifndef __ARKHE_T_MATRIX_NN_HPP__
#define __ARKHE_T_MATRIX_NN_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheMatrixPreproc.h"
#include "ArkheTMath.hpp"

//std includes
#include <ostream>

namespace arkhe
{

namespace foundation
{

//Not a great design, will haev issues with stack for large matrices...
//Only for research, will move to a production library later...
template<typename T,unsigned N> class TMatrixNN
{
public:
	//enums
	enum InvertAs
	{
		FULL = 0, //full inverse
		ORTHOGONAL, //invert as orthogonal
		DIAGONAL, //invert as diagonal
		SYMMETRIC //symmetric
		//etc.
	}; //enum InvertAs
	//constructors
	TMatrixNN() {} //we do not initialize the values
	TMatrixNN(const TMatrixNN &copy);
	//destructor
	~TMatrixNN() {}
	//operator overloads
	TMatrixNN &operator=(const TMatrixNN &copy);
	operator const T*() const;
	operator T*();
	const T operator()(unsigned int i,unsigned int j) const;
	T &operator()(unsigned int i,unsigned int j);	
	const TMatrixNN<T,N> operator()(const TMatrixNN<T,N> &m) const;	
	const TMatrixNN operator+(const TMatrixNN &m) const;
	TMatrixNN &operator+=(const TMatrixNN &m);
	const TMatrixNN operator-() const;
	const TMatrixNN operator-(const TMatrixNN &m) const;
	TMatrixNN &operator-=(const TMatrixNN &m);
	const TMatrixNN operator*(const T &t) const;
	const TMatrixNN operator*(const TMatrixNN &m) const;
	TMatrixNN &operator*=(const T &t);
	TMatrixNN &operator*=(const TMatrixNN &m);
	const TMatrixNN operator/(const T &t) const;
	TMatrixNN &operator/=(const T &t);
	const bool operator==(const TMatrixNN &m) const;
	const bool operator!=(const TMatrixNN &m) const;
	template<typename T,unsigned N> friend std::ostream &operator<<(std::ostream &out,const TMatrixNN<T,N> &m);
	//methods
	void clean();
	const bool isZero() const;
	const bool isIdentity() const;
	const T trace() const;
	const T determinant() const;
	void setZero();
	void setIdentity();
	const TMatrixNN transpose() const;
	const TMatrixNN adjugate() const;
	const TMatrixNN inverse() const; //strictly full inverse - no case logic etc.
	const TMatrixNN inverse(unsigned int invertAs) const;
private:
	//methods
	const TMatrixNN gaussSeidel() const;
	void swapRows(unsigned int i,unsigned int j);
	void scaleRow(unsigned int row,const T &scaleFactor);
	void addScaledRows(unsigned int dstRow,unsigned int srcRow,const T &scaleFactor);
	unsigned int maxRowAtCol(unsigned int col,T &value) const;
	const T rowDotCol(unsigned int row,const TMatrixNN<T,N> &A,unsigned int col,const TMatrixNN<T,N> &B) const;
	//variables
	T m[N*N]; //store as column-major
protected:
	//
}; //class TMatrixNN

#define MATRIX_NN_LOOP for(int i=0; i<N*N; i++)

template<typename T,unsigned N> TMatrixNN<T,N>::TMatrixNN(const TMatrixNN<T,N> &copy)
{
	MATRIX_NN_LOOP m[i] = copy.m[i];
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator=(const TMatrixNN<T,N> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	MATRIX_NN_LOOP m[i] = copy.m[i];
	return *this;
}

template<typename T,unsigned N> TMatrixNN<T,N>::operator const T*() const { return &m[0]; }

template<typename T,unsigned N> TMatrixNN<T,N>::operator T*() { return &m[0]; }

template<typename T,unsigned N> const T TMatrixNN<T,N>::operator()(unsigned int i,unsigned int j) const
{
	if(i >= N || j >= N) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*N) + j];
}

template<typename T,unsigned N> T &TMatrixNN<T,N>::operator()(unsigned int i,unsigned int j)
{
	if(i >= N || j >= N) throw Exception(Exception::OUT_OF_RANGE);
	return m[(i*N) + j];
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator()(const TMatrixNN<T,N> &mtx) const
{
	return (*this) * mtx;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator+(const TMatrixNN<T,N> &mtx) const
{
	TMatrixNN<T,N> res;
	MATRIX_NN_LOOP res.m[i] = m[i] + mtx.m[i];
	return res;
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator+=(const TMatrixNN<T,N> &mtx)
{
	MATRIX_NN_LOOP m[i] += mtx.m[i];
	return *this;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator-() const
{
	TMatrixNN<T,N> res;
	MATRIX_NN_LOOP res.m[i] = -m[i];
	return res;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator-(const TMatrixNN<T,N> &mtx) const
{
	TMatrixNN<T,N> res;
	MATRIX_NN_LOOP res.m[i] = m[i] - mtx.m[i];
	return res;
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator-=(const TMatrixNN<T,N> &mtx)
{
	MATRIX_NN_LOOP m[i] -= mtx.m[i];
	return *this;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator*(const T &t) const
{
	TMatrixNN<T,N> res;
	MATRIX_NN_LOOP res.m[i] = m[i] * t;
	return res;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator*(const TMatrixNN<T,N> &mtx) const
{
	TMatrixNN<T,N> res;
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			res(i,j) = rowDotCol(i,*this,j,mtx);
		}
	}
	return res;
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator*=(const T &t)
{
	MATRIX_NN_LOOP m[i] *= t;
	return *this;
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator*=(const TMatrixNN<T,N> &mtx)
{
	TMatrixNN<T,N> res = (*this) * mtx;
	return *this;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::operator/(const T &t) const
{
	T recip = TMath<T>::reciprocal(t);
	TMatrixNN<T,N> res;
	MATRIX_NN_LOOP res.m[i] = m[i] * recip;
	return res;
}

template<typename T,unsigned N> TMatrixNN<T,N> &TMatrixNN<T,N>::operator/=(const T &t)
{
	T recip = TMath<T>::reciprocal(t);
	MATRIX_NN_LOOP m[i] *= recip;
	return *this;
}

template<typename T,unsigned N> const bool TMatrixNN<T,N>::operator==(const TMatrixNN<T,N> &mtx) const
{
	MATRIX_NN_LOOP if(!TMath<T>::equal(m[i],mtx.m[i],TMath<T>::GAUSS_SEIDEL_EPSILON)) return false;
	return true;
}

template<typename T,unsigned N> const bool TMatrixNN<T,N>::operator!=(const TMatrixNN<T,N> &mtx) const
{
	MATRIX_NN_LOOP if(!TMath<T>::equal(m[i],mtx.m[i],TMath<T>::GAUSS_SEIDEL_EPSILON)) return true;
	return false;
}

template<typename T,unsigned N> std::ostream &operator<<(std::ostream &out,const TMatrixNN<T,N> &mtx)
{
	/* NOT USED
#ifdef MATHEMATICA_STREAM_OUTPUT
	out << '{';
	for(int i=0; i<N; i++)
	{
		out << '{';
		for(int j=0; j<N; j++)
		{
			cout << mtx(i,j);
			if(j < N-1) out << ',';
		}
		if(i < N-1) out << "},";
		else out << '}';
	}
	out << '}';
#endif //MATHEMATICA_STREAM_OUTPUT
	*/
#ifdef RAW_STREAM_OUTPUT
	for(int i=0; i<(N*N)-1; i++) out << mtx.m[i] << ' ';
	out << mtx.m[(N*N)-1];
#else //normal output
	out.precision(3);
	out.setf(std::ios::fixed);
	for(int i=0; i<N; i++)
	{
		out << '|';
		for(int j=0; j<N; j++)
		{
			cout << mtx(i,j);
			if(j < N-1) out << '\t';
		}
		if(i == N-1) out << '|';
		else out << "|\n";
	}
#endif //RAW_STREAM_OUTPUT
	return out;
}

template<typename T,unsigned N> void TMatrixNN<T,N>::clean()
{
	TMath<T>::clean(m,N*N,TMath<T>::GAUSS_SEIDEL_EPSILON);
}

template<typename T,unsigned N> const bool TMatrixNN<T,N>::isZero() const
{
	return TMath<T>::isZero(m,N*N,TMath<T>::GAUSS_SEIDEL_EPSILON);
}

template<typename T,unsigned N> const bool TMatrixNN<T,N>::isIdentity() const
{
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			if(i == j && (*this)(i,j) != TMath<T>::ONE) return false;
			else if(!TMath<T>::isZero((*this)(i,j))) return false;
		}
	}
	return true;
}

template<typename T,unsigned N> void TMatrixNN<T,N>::setZero()
{
	MATRIX_NN_LOOP m[i] = TMath<T>::ZERO;
}

template<typename T,unsigned N> void TMatrixNN<T,N>::setIdentity()
{
	for(unsigned int i=0; i<N; i++)
	{
		for(unsigned int j=0; j<N; j++)
		{
			if(i == j) (*this)(i,j) = TMath<T>::ONE;
			else (*this)(i,j) = TMath<T>::ZERO;
		}
	}
}

template<typename T,unsigned N> const T TMatrixNN<T,N>::trace() const
{
	T ret = TMath<T>::ZERO;
	for(int i=0; i<N; i++) ret += (*this)(i,i);
	return ret;
}

template<typename T,unsigned N> const T TMatrixNN<T,N>::determinant() const
{
	T det;
	//TODO
	throw Exception(Exception::NO_IMPLEMENTATION);
	return det;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::transpose() const
{
	TMatrixNN<T,N> res;
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			res(j,i) = (*this)(i,j);
		}
	}
	return res;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::adjugate() const
{
	TMatrixNN<T,N> res;
	//TODO
	throw Exception(Exception::NO_IMPLEMENTATION);
	return res;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::inverse() const
{
	return gaussSeidel();
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::inverse(unsigned int invertAs) const
{
	TMatrixNN res;
	if(invertAs == TMatrixNN<T,N>::FULL)
	{
		//TODO
		throw Exception(Exception::NO_IMPLEMENTATION);
	}
	else if(invertAs == TMatrixNN<T,N>::ORTHOGONAL)
	{
		res = transpose();
	}
	else
	{
		//diagonal inverse
		throw Exception(Exception::NO_IMPLEMENTATION);
	}
	return res;
}

template<typename T,unsigned N> const TMatrixNN<T,N> TMatrixNN<T,N>::gaussSeidel() const
{
	TMatrixNN<T,N> A = *this;
	TMatrixNN<T,N> I; //inverse
	I.setIdentity();

	unsigned int entry;
	T value;
	T scaleFactor;
	unsigned int thisRow = 0;

	for(int col=0; col<N; col++)
	{
		//get maximum entry
		entry = A.maxRowAtCol(col,value);

		//is this a singular matrix?
		if(TMath<T>::isZero(value))
		{
			I.setZero();
			return I;
		}

		if(entry != thisRow)
		{
			A.swapRows(col,entry);
			I.swapRows(col,entry);
		}

		if(!TMath<T>::isZero(value))
		{
			A.scaleRow(thisRow,TMath<T>::reciprocal(value));
			I.scaleRow(thisRow,TMath<T>::reciprocal(value));
		}

		for(int row=col+1; row<N; row++)
		{
			scaleFactor = -A(row,col);
			A.addScaledRows(row,col,scaleFactor);
			I.addScaledRows(row,col,scaleFactor);
		}

		thisRow++;
	}

	//back substitute
	for(int row=N-1; row>=0; row--)
	{
		for(int i=0; i<row; i++)
		{
			scaleFactor = -A(i,row);
			A.addScaledRows(i,row,scaleFactor);
			I.addScaledRows(i,row,scaleFactor);
		}
	}

	return I;
}

template<typename T,unsigned N> void TMatrixNN<T,N>::swapRows(unsigned int i,unsigned int j)
{
	T tmp;
	for(int k=0; k<N; k++)
	{
		tmp = (*this)(i,k);
		(*this)(i,k) = (*this)(j,k);
		(*this)(j,k) = tmp;
	}
}

template<typename T,unsigned N> void TMatrixNN<T,N>::scaleRow(unsigned int row,const T &scaleFactor)
{
	for(int i=0; i<N; i++) (*this)(row,i) *= scaleFactor;
}

template<typename T,unsigned N> void TMatrixNN<T,N>::addScaledRows(unsigned int dstRow,unsigned int srcRow,const T &scaleFactor)
{
	for(int i=0; i<N; i++)
	{
		(*this)(dstRow,i) += (*this)(srcRow,i) * scaleFactor;
	}
}

template<typename T,unsigned N> unsigned int TMatrixNN<T,N>::maxRowAtCol(unsigned int col,T &value) const
{
	value = (*this)(col,col); //initialize
	unsigned int index = col;
	for(int i=col+1; i<N; i++)
	{
		if((*this)(i,col) > value)
		{
			value = (*this)(i,col);
			index = i;
		}
	}
	return index;
}

//do dot product of row of A and column of B
template<typename T,unsigned N> const T TMatrixNN<T,N>::rowDotCol(unsigned int row,const TMatrixNN<T,N> &A,unsigned int col,const TMatrixNN<T,N> &B) const
{
	T dot = TMath<T>::ZERO;
	for(int i=0; i<N; i++)
	{
		dot += A(row,i) * B(i,col);
	}
	return dot;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MATRIX_NN_HPP__
