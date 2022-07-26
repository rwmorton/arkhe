#ifndef __ARKHE_FOUNDATION_CFG_H__
#define __ARKHE_FOUNDATION_CFG_H__

#include <iostream>
#include <vector>
#include <map>

//Preprocessor stuff

//#define MATHEMATICA_STREAM_OUTPUT //output to Mathematica friendly format
//#define RAW_STREAM_OUTPUT //no formatting for stream output

#define ARKHE_DEBUG

#define UNIT_TEST_ERROR(x) throw Exception(Exception::UNIT_TEST_FAILED,x);

#define ARKHE_MATH_STRICT_CHECK //enable strict math checks - ie. reciprocal function
//#define ARKHE_DOUBLE_PRECISION

namespace arkhe
{

namespace foundation
{

#ifdef ARKHE_DOUBLE_PRECISION
typedef double Real;
#else //floating point
typedef float Real;
#endif //ARKHE_DOUBLE_PRECISION

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_FOUNDATION_CFG_H__
