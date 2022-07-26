#ifndef __ARKHE_T_MATH_HPP__
#define __ARKHE_T_MATH_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheUtil.h"
#include "ArkheMathTables.h"

namespace arkhe
{

namespace foundation
{

template<typename T> class TMath
{
public:
	enum QuadraticRoots
	{
		NON_EQUAL_ROOTS, //two roots
		EQUAL_ROOTS, //one root
		NO_ROOTS
	};
	//static methods
	//basic functions
	static const bool equal(const T &a,const T &b,const T &epsilon=TMath<T>::EPSILON);
	static const bool inRange(const T &x,const T &a,const T &b);
	static const T minimum(const T &a,const T &b); //name clashes if using min or max ???
	static const T maximum(const T &a,const T &b); //as above
	static const T clamp(const T &x,const T &min,const T &max);
	static const T floor(const T &x);
	static const T ceil(const T &x);
	static const bool isZero(const T &x,const T &epsilon=TMath<T>::EPSILON);
	static const bool isZero(const T *t_ptr,const unsigned int n,const T &epsilon=TMath<T>::EPSILON);
	static const bool isNan(const T &x);
	static void clean(T &x,const T &epsilon=TMath<T>::EPSILON);
	static void clean(T *t_ptr,const unsigned int n,const T &epsilon=TMath<T>::EPSILON);
	static const T abs(const T &x);
	static const T reciprocal(const T &x);
	static const T reciprocalF(const T &x); //fast version, does not check for division by zero.
	static const T sqrt(const T &x); //DO UNIT TEST
	static const T pow(const T &x,const T &n); //nth power of x
	static const T exp(const T &x);
	//trigonometric functions
	static const T sin(const T &x);
	static const T aSin(const T &x);
	static const T cos(const T &x);
	static const T aCos(const T &x);
	static void sinCos(const T &x,T &s,T &c);
	static const T tan(const T &x);
	static const T aTan(const T &x);
	static const T aTan2(const T &y,const T &x);
	//special functions
	//random number generation
	static const T random(const T &max);
	static const T randomBinomial(const T &max);
	static const int randomSign();
	//interpolation
	static const T lerp(const T &t,const T &a,const T &b);
	static const int/*QuadraticRoots*/ solveQuadratic(const T &a,const T &b,const T &c,T *t0,T *t1); //DO UNIT TEST
	//perlin noise
	static const T perlinNoise(const T &x,const T &y,const T &z);
	//static variables
	static const T ZERO;
	static const T ONE;
	static const T ONE_OVER_TWO;
	static const T TWO;
	static const T ONE_OVER_THREE;
	static const T ONE_OVER_FOUR;
	static const T MIN;
	static const T MAX;
	static const T EPSILON;
	static const T RAY_EPSILON;
	static const T PI;
	static const T HALF_PI;
	static const T TWO_PI;
	static const T SQRT_2;
	static const T DEG_TO_RAD;
	static const T RAD_TO_DEG;
	static const T e; //base of natural log
	static const T GAUSS_SEIDEL_EPSILON;
private:
	//static methods
	static const T perlinNoiseGradient(int x,int y,int z,const T &dx,const T &dy,const T &dz);
	static const T perlinNoiseWeight(const T &t);
protected:
	//
}; //class TMath

//////////// Basic functions ////////////

template<typename T> const bool TMath<T>::equal(const T &a,const T &b,const T &epsilon) { return TMath<T>::isZero(a-b,epsilon); }

template<typename T> const bool TMath<T>::inRange(const T &x,const T &a,const T &b)
{
	if(a > b) throw Exception(Exception::LOGIC_ERROR,"range is reversed, a is greater than b");
	if(x < a || x > b) return false;
	return true;
}

template<typename T> const T TMath<T>::minimum(const T &a,const T &b) { return a < b ? a : b; }
template<typename T> const T TMath<T>::maximum(const T &a,const T &b) { return a < b ? b : a; }

template<typename T> const T TMath<T>::clamp(const T &x,const T &min,const T &max)
{
	if(min > max) throw Exception(Exception::LOGIC_ERROR,"min greater than max");
	if(x < min) return min;
	if(x > max) return max;
	return x;
}

template<typename T> const bool TMath<T>::isZero(const T &x,const T &epsilon) { return TMath<T>::abs(x) < epsilon; }

template<typename T> const bool TMath<T>::isZero(const T *t_ptr,const unsigned int n,const T &epsilon)
{
	bool zero = true;
	for(unsigned int i=0; i<n; i++)
	{
		if(!(TMath<T>::abs(*t_ptr) < epsilon))
		{
			zero = false;
			break;
		}
	}
	return zero;
}

template<typename T> const bool TMath<T>::isNan(const T &x)
{
	return ::_isnan(x);
}

template<typename T> void TMath<T>::clean(T &x,const T &epsilon)
{
	if(isZero(x,epsilon))
	{
		x = TMath::ZERO;
	}
}

template<typename T> void TMath<T>::clean(T *t_ptr,const unsigned int n,const T &epsilon)
{
	for(unsigned int i=0; i<n; i++)
	{
		if(isZero(*t_ptr,epsilon))
		{
			*t_ptr = TMath<T>::ZERO;
		}
		t_ptr++;
	}
}

//////////// Special functions ////////////

template<typename T> const T TMath<T>::lerp(const T &t,const T &a,const T &b)
{
	return (TMath::ONE - t)*a + t*b;
}

template<typename T> const int/*TMath<T>::QuadraticRoots*/ TMath<T>::solveQuadratic(const T &a,const T &b,const T &c,T *t0,T *t1)
{
	//calculate the discriminant
	T discrim = b*b - (4 * a * c);

	if(TMath::isZero(discrim))
	{
		*t0 = *t1 = -b / (2 * a);
		return TMath::EQUAL_ROOTS;
	}
	if(discrim > TMath::EPSILON)
	{
		//two roots
		T sqRootOfDiscrim = TMath::sqrt(discrim);
		*t0 = (-b + sqRootOfDiscrim) / (2 * a);
		*t1 = (-b - sqRootOfDiscrim) / (2 * a);
		if(*t0 > *t1)
		{
			Util::swap<T>(*t0,*t1);
		}
		return TMath::NON_EQUAL_ROOTS;
	}
	else
	{
		//complex roots
		return TMath::NO_ROOTS;
	}
}

//perlin noise
template<typename T> const T TMath<T>::perlinNoise(const T &x,const T &y,const T &z)
{
	//compute noise cell coordinates and offsets
	int ix = (int)TMath::floor(x);
	int iy = (int)TMath::floor(y);
	int iz = (int)TMath::floor(z);
	T dx = x - ix, dy = y - iy, dz = z - iz;
	//compute gradient weights
	ix &= (perlin_noise::permutationSize-1);
	iy &= (perlin_noise::permutationSize-1);
	iz &= (perlin_noise::permutationSize-1);
	T w000 = TMath::perlinNoiseGradient(ix,   iy,   iz,   dx,   dy,   dz);
	T w100 = TMath::perlinNoiseGradient(ix+1, iy,   iz,   dx-1, dy,   dz);
	T w010 = TMath::perlinNoiseGradient(ix,   iy+1, iz,   dx,   dy-1, dz);
	T w110 = TMath::perlinNoiseGradient(ix+1, iy+1, iz,   dx-1, dy-1, dz);
	T w001 = TMath::perlinNoiseGradient(ix,   iy,   iz+1, dx,   dy,   dz-1);
	T w101 = TMath::perlinNoiseGradient(ix+1, iy,   iz+1, dx-1, dy,   dz-1);
	T w011 = TMath::perlinNoiseGradient(ix,   iy+1, iz+1, dx,   dy-1, dz-1);
	T w111 = TMath::perlinNoiseGradient(ix+1, iy+1, iz+1, dx-1, dy-1, dz-1);
	//compute trilinear interpolation of weights
	T wx = TMath::perlinNoiseWeight(dx);
	T wy = TMath::perlinNoiseWeight(dy);
	T wz = TMath::perlinNoiseWeight(dz);
	T x00 = TMath::lerp(wx, w000, w100);
	T x10 = TMath::lerp(wx, w010, w110);
	T x01 = TMath::lerp(wx, w001, w101);
	T x11 = TMath::lerp(wx, w011, w111);
	T y0 = TMath::lerp(wy, x00, x10);
	T y1 = TMath::lerp(wy, x01, x11);
	return TMath::lerp(wz, y0, y1);
}

//private methods

//perlin noise
template<typename T> const T TMath<T>::perlinNoiseGradient(int x,int y,int z,const T &dx,const T &dy,const T &dz)
{
	int h = perlin_noise::permutations[perlin_noise::permutations[perlin_noise::permutations[x]+y]+z];
	h &= 15;
	T u = h<8 || h==12 || h==13 ? dx : dy;
	T v = h<4 || h==12 || h==13 ? dy : dz;
	return ((h&1) ? -u : u) + ((h&2) ? -v : v);
}

template<typename T> const T TMath<T>::perlinNoiseWeight(const T &t)
{
	T t3 = t*t*t;
	T t4 = t3*t;
	return 6*t4*t - 15*t4 + 10*t3;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MATH_HPP__
