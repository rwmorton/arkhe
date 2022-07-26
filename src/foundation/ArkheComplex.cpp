//arkhe local includes (foundation)
#include "ArkheTQuaternion.hpp"

namespace arkhe
{

namespace foundation
{

//static variable definitions
//TQuaternion
const TQuaternion<float> TQuaternion<float>::ZERO(0.0f,0.0f,0.0f,0.0f);
const TQuaternion<double> TQuaternion<double>::ZERO(0.0,0.0,0.0,0.0);
const TQuaternion<float> TQuaternion<float>::IDENTITY(1.0f,0.0f,0.0f,0.0f);
const TQuaternion<double> TQuaternion<double>::IDENTITY(1.0f,0.0f,0.0f,0.0f);

//static method definitions
//TQuaternion
//interpolation

//lerp
#define ARKHE_QUATERNION_LERP(START,END,PARAM,TYPE)				\
	throw Exception(Exception::NO_IMPLEMENTATION);				\

const TQuaternion<float> TQuaternion<float>::lerp(const TQuaternion<float> &start,const TQuaternion<float> &end,const float &t) { ARKHE_QUATERNION_LERP(start,end,t,float) }
const TQuaternion<double> TQuaternion<double>::lerp(const TQuaternion<double> &start,const TQuaternion<double> &end,const double &t) { ARKHE_QUATERNION_LERP(start,end,t,double) }

//slerp
#define ARKHE_QUATERNION_SLERP(START,END,PARAM,TYPE)				\
	throw Exception(Exception::NO_IMPLEMENTATION);					\

const TQuaternion<float> TQuaternion<float>::slerp(const TQuaternion<float> &start,const TQuaternion<float> &end,const float &t) { ARKHE_QUATERNION_SLERP(start,end,t,float) }
const TQuaternion<double> TQuaternion<double>::slerp(const TQuaternion<double> &start,const TQuaternion<double> &end,const double &t) { ARKHE_QUATERNION_SLERP(start,end,t,double) }

//approx slerp
#define ARKHE_QUATERNION_APPROX_SLERP(START,END,PARAM,TYPE)				\
	throw Exception(Exception::NO_IMPLEMENTATION);						\

const TQuaternion<float> TQuaternion<float>::approxSlerp(const TQuaternion<float> &start,const TQuaternion<float> &end,const float &t) { ARKHE_QUATERNION_APPROX_SLERP(start,end,t,float) }
const TQuaternion<double> TQuaternion<double>::approxSlerp(const TQuaternion<double> &start,const TQuaternion<double> &end,const double &t) { ARKHE_QUATERNION_APPROX_SLERP(start,end,t,double) }

} //namespace foundation

} //namespace arkhe
