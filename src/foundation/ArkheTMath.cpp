//arkhe local includes (foundation)
#include "ArkheTMath.hpp"
#include "ArkheException.h"

//std includes
#include <cfloat>
#include <cmath>

namespace arkhe
{

namespace foundation
{

//static variable definitions
const float TMath<float>::ZERO = 0.0f;
const double TMath<double>::ZERO = 0.0;
const float TMath<float>::ONE = 1.0f;
const double TMath<double>::ONE = 1.0;
const float TMath<float>::ONE_OVER_TWO = 0.5f;
const double TMath<double>::ONE_OVER_TWO = 0.5;
const float TMath<float>::TWO = 2.0f;
const double TMath<double>::TWO = 2.0;
const float TMath<float>::ONE_OVER_THREE = 0.33333333333333333333333333333333f;
const double TMath<double>::ONE_OVER_THREE = 0.33333333333333333333333333333333;
const float TMath<float>::ONE_OVER_FOUR = 0.25f;
const double TMath<double>::ONE_OVER_FOUR = 0.25;
const float TMath<float>::MIN = FLT_MIN;
const double TMath<double>::MIN = DBL_MIN;
const float TMath<float>::MAX = FLT_MAX;
const double TMath<double>::MAX = DBL_MAX;
const float TMath<float>::EPSILON = FLT_EPSILON;
const double TMath<double>::EPSILON = DBL_EPSILON;
const float TMath<float>::RAY_EPSILON = 1.0e-3f;
const double TMath<double>::RAY_EPSILON = 1.0e-3;
const float TMath<float>::PI = 3.1415926535897932384626433832795f;
const double TMath<double>::PI = 3.1415926535897932384626433832795;
const float TMath<float>::HALF_PI = TMath<float>::PI * 0.5f;
const double TMath<double>::HALF_PI = TMath<double>::PI * 0.5;
const float TMath<float>::TWO_PI = TMath<float>::PI * 2.0f;
const double TMath<double>::TWO_PI = TMath<double>::PI * 2.0;
const float TMath<float>::SQRT_2 = 1.4142135623730950488016887242097f;
const double TMath<double>::SQRT_2 = 1.4142135623730950488016887242097;
const float TMath<float>::DEG_TO_RAD = TMath<float>::PI / 180.0f;
const double TMath<double>::DEG_TO_RAD = TMath<double>::PI / 180.0;
const float TMath<float>::RAD_TO_DEG = 180.0f / TMath<float>::PI;
const double TMath<double>::RAD_TO_DEG = 180.0 / TMath<double>::PI;
const float TMath<float>::e = 2.7182818284590452353602874713527f;
const double TMath<double>::e = 2.7182818284590452353602874713527;
const float TMath<float>::GAUSS_SEIDEL_EPSILON = 0.000001f;				//specifically for iterative methods
const double TMath<double>::GAUSS_SEIDEL_EPSILON = 0.000000000000001;	//as round off errors cause problems... these values might have to be adjusted

//////////// Basic functions ////////////

const float TMath<float>::floor(const float &x) { return ::floorf(x); }
const double TMath<double>::floor(const double &x) { return ::floor(x); }

const float TMath<float>::ceil(const float &x) { return ::ceilf(x); }
const double TMath<double>::ceil(const double &x) { return ::ceil(x); }

const float TMath<float>::abs(const float &x) { return fabsf(x); }
const double TMath<double>::abs(const double &x) { return fabs(x); }

const float TMath<float>::reciprocal(const float &x)
{
#ifdef ARKHE_MATH_STRICT_CHECK
	if(TMath<float>::isZero(x))
		throw Exception(Exception::DIVISION_BY_ZERO);
#endif //ARKHE_MATH_STRICT_CHECK
	return 1.0f / x;
}
const double TMath<double>::reciprocal(const double &x)
{
#ifdef ARKHE_MATH_STRICT_CHECK
	if(TMath<double>::isZero(x))
		throw Exception(Exception::DIVISION_BY_ZERO);
#endif //ARKHE_MATH_STRICT_CHECK
	return 1.0 / x;
}

const float TMath<float>::reciprocalF(const float &x) { return 1.0f / x; }
const double TMath<double>::reciprocalF(const double &x) { return 1.0 / x; }

const float TMath<float>::sqrt(const float &x) { return sqrtf(x); }
const double TMath<double>::sqrt(const double &x) { return ::sqrt(x); }

const float TMath<float>::pow(const float &x,const float &n) { return powf(x,n); }
const double TMath<double>::pow(const double &x,const double &n) { return ::pow(x,n); }

const float TMath<float>::exp(const float &x) { return ::expf(x); }
const double TMath<double>::exp(const double &x) { return ::exp(x); }

//////////// Trigonometric functions ////////////

const float TMath<float>::sin(const float &x) { return sinf(x); }
const double TMath<double>::sin(const double &x) { return std::sin(x); }
const float TMath<float>::aSin(const float &x) { return asinf(x); }
const double TMath<double>::aSin(const double &x) { return asin(x); }
const float TMath<float>::cos(const float &x) { return cosf(x); }
const double TMath<double>::cos(const double &x) { return std::cos(x); }

void TMath<float>::sinCos(const float &x,float &s,float &c)
{
	s = sinf(x);
	c = cosf(x);
}
void TMath<double>::sinCos(const double &x,double &s,double &c)
{
	s = std::sin(x);
	c = std::cos(x);
}

const float TMath<float>::aCos(const float &x) { return acosf(x); }
const double TMath<double>::aCos(const double &x) { return acos(x); }
const float TMath<float>::tan(const float &x) { return tanf(x); }
const double TMath<double>::tan(const double &x) { return std::tan(x); }
const float TMath<float>::aTan(const float &x) { return atanf(x); }
const double TMath<double>::aTan(const double &x) { return atan(x); }
const float TMath<float>::aTan2(const float &y,const float &x) { return std::atan2f(y,x); }
const double TMath<double>::aTan2(const double &y,const double &x) { return std::atan2(y,x); }

//////////// Special functions ////////////

//////////// Random number generation ////////////

const float TMath<float>::random(const float &max) { return max * (float(::rand()) / RAND_MAX); }
const double TMath<double>::random(const double &max) { return max * (double(::rand()) / RAND_MAX); }
const float TMath<float>::randomBinomial(const float &max) { return random(max) - random(max); }
const double TMath<double>::randomBinomial(const double &max) { return random(max) - random(max); }

const int TMath<float>::randomSign() { return TMath<T>::random(TMath<T>::ONE) < TMath<T>::ONE_OVER_TWO ? -1 : 1; }
const int TMath<double>::randomSign() { return TMath<T>::random(TMath<T>::ONE) < TMath<T>::ONE_OVER_TWO ? -1 : 1; }

} //namespace foundation

} //namespace arkhe
