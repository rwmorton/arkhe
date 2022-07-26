//arkhe local includes (foundation)
#include "ArkheTMatrix33.hpp"
#include "ArkheTMatrix44.hpp"
#include "ArkheTQuaternion.hpp"

namespace arkhe
{

namespace foundation
{

//static variable definitions
//TMatrix33
const unsigned int TMatrix33<float>::s_dimension = 3;
const unsigned int TMatrix33<double>::s_dimension = 3;
const unsigned int TMatrix33<float>::s_numElements = 9;
const unsigned int TMatrix33<double>::s_numElements = 9;
const TMatrix33<float> TMatrix33<float>::ZERO
(
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
);
const TMatrix33<double> TMatrix33<double>::ZERO
(
	0.0,0.0,0.0,
	0.0,0.0,0.0,
	0.0,0.0,0.0
);
const TMatrix33<float> TMatrix33<float>::IDENTITY
(
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f
);
const TMatrix33<double> TMatrix33<double>::IDENTITY
(
	1.0,0.0,0.0,
	0.0,1.0,0.0,
	0.0,0.0,1.0
);

//TMatrix44
const unsigned int TMatrix44<float>::s_dimension = 4;
const unsigned int TMatrix44<double>::s_dimension = 4;
const unsigned int TMatrix44<float>::s_numElements = 16;
const unsigned int TMatrix44<double>::s_numElements = 16;
const TMatrix44<float> TMatrix44<float>::ZERO
(
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f
);
const TMatrix44<double> TMatrix44<double>::ZERO
(
	0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0
);
const TMatrix44<float> TMatrix44<float>::IDENTITY
(
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f
);
const TMatrix44<double> TMatrix44<double>::IDENTITY
(
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//static method definitions
//TMatrix33
//rotation
#define ARKHE_MATRIX33_ROTATE_X(TYPE,VALUE)						\
	TYPE s,c;													\
	TMath<TYPE>::sinCos(VALUE,s,c);								\
	return TMatrix33<TYPE>										\
	(															\
		TMath<TYPE>::ONE,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,	\
		TMath<TYPE>::ZERO,c,-s,									\
		TMath<TYPE>::ZERO,s,c									\
	);															\

const TMatrix33<float> TMatrix33<float>::rotateX(const float &angle) { ARKHE_MATRIX33_ROTATE_X(float,angle) }
const TMatrix33<double> TMatrix33<double>::rotateX(const double &angle) { ARKHE_MATRIX33_ROTATE_X(double,angle) }

#define ARKHE_MATRIX33_ROTATE_Y(TYPE,VALUE)						\
	TYPE s,c;													\
	TMath<TYPE>::sinCos(VALUE,s,c);								\
	return TMatrix33<TYPE>										\
	(															\
		c,TMath<TYPE>::ZERO,s,									\
		TMath<TYPE>::ZERO,TMath<TYPE>::ONE,TMath<TYPE>::ZERO,	\
		-s,TMath<TYPE>::ZERO,c									\
	);															\

const TMatrix33<float> TMatrix33<float>::rotateY(const float &angle) { ARKHE_MATRIX33_ROTATE_Y(float,angle) }
const TMatrix33<double> TMatrix33<double>::rotateY(const double &angle) { ARKHE_MATRIX33_ROTATE_Y(double,angle) }

#define ARKHE_MATRIX33_ROTATE_Z(TYPE,VALUE)						\
	TYPE s,c;													\
	TMath<TYPE>::sinCos(VALUE,s,c);								\
	return TMatrix33<TYPE>										\
	(															\
		c,-s,TMath<TYPE>::ZERO,									\
		s,c,TMath<TYPE>::ZERO,									\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ONE	\
	);															\

const TMatrix33<float> TMatrix33<float>::rotateZ(const float &angle) { ARKHE_MATRIX33_ROTATE_Z(float,angle) }
const TMatrix33<double> TMatrix33<double>::rotateZ(const double &angle) { ARKHE_MATRIX33_ROTATE_Z(double,angle) }

#define ARKHE_MATRIX33_ROTATE_XYZ(TYPE,VALUE1,VALUE2,VALUE3)		\
	TYPE Cx,Cy,Cz,Sx,Sy,Sz;											\
	TMath<TYPE>::sinCos(VALUE1,Sx,Cx);								\
	TMath<TYPE>::sinCos(VALUE2,Sy,Cy);								\
	TMath<TYPE>::sinCos(VALUE3,Sz,Cz);								\
	return TMatrix33<TYPE>											\
	(																\
		Cy*Cz,-Cy*Sz,Sy,											\
		Cx*Sz + Sx*Sy*Cz,Cx*Cz-Sx*Sy*Sz,-Sx*Cy,						\
		Sx*Sz-Cx*Cz*Sy,Sx*Cz+Cx*Sy*Sz,Cx*Cy							\
	);																\

const TMatrix33<float> TMatrix33<float>::rotate(const float &x,const float &y,const float &z) { ARKHE_MATRIX33_ROTATE_XYZ(float,x,y,z) }
const TMatrix33<double> TMatrix33<double>::rotate(const double &x,const double &y,const double &z) { ARKHE_MATRIX33_ROTATE_XYZ(double,x,y,z) }

#define ARKHE_MATRIX33_ROTATE_AXIS_ANGLE(TYPE,AXIS,VALUE)								\
	if(TMath<TYPE>::isZero(AXIS.lengthSq()))											\
		throw Exception(Exception::DIVISION_BY_ZERO,"axis is the zero vector");			\
	TVector3<T> A = AXIS;																\
	A.normalize();																		\
	TYPE s,c;																			\
	TMath<TYPE>::sinCos(VALUE,s,c);														\
	TYPE oneMinusC = TMath<TYPE>::ONE - c;												\
	return TMatrix33<TYPE>																\
	(																					\
		c + oneMinusC*A.x*A.x,oneMinusC*A.x*A.y - s*A.z,oneMinusC*A.x*A.z + s*A.y,		\
		oneMinusC*A.x*A.y + s*A.z,c + oneMinusC*A.y*A.y,oneMinusC*A.y*A.z - s*A.x,		\
		oneMinusC*A.x*A.z - s*A.y,oneMinusC*A.y*A.z + s*A.x,c + oneMinusC*A.z*A.z		\
	);																					\

const TMatrix33<float> TMatrix33<float>::rotate(const TVector3<float> &axis,const float &angle) { ARKHE_MATRIX33_ROTATE_AXIS_ANGLE(float,axis,angle); }
const TMatrix33<double> TMatrix33<double>::rotate(const TVector3<double> &axis,const double &angle) { ARKHE_MATRIX33_ROTATE_AXIS_ANGLE(double,axis,angle); }

#define ARKHE_MATRIX33_ROTATE_QUATERNION(TYPE,QUAT)													\
	TMatrix33<TYPE> m																				\
	(																								\
		TMath<TYPE>::ONE - TMath<TYPE>::TWO*QUAT.y*QUAT.y - TMath<TYPE>::TWO*QUAT.z*QUAT.z,			\
		TMath<TYPE>::TWO*QUAT.x*QUAT.y - TMath<TYPE>::TWO*QUAT.w*QUAT.z,							\
		TMath<TYPE>::TWO*QUAT.x*QUAT.z + TMath<TYPE>::TWO*QUAT.w*QUAT.y,							\
		TMath<TYPE>::TWO*QUAT.x*QUAT.y + TMath<TYPE>::TWO*QUAT.w*QUAT.z,							\
		TMath<TYPE>::ONE - TMath<TYPE>::TWO*QUAT.x*QUAT.x - TMath<TYPE>::TWO*QUAT.z*QUAT.z,			\
		TMath<TYPE>::TWO*QUAT.y*QUAT.z - TMath<TYPE>::TWO*QUAT.w*QUAT.x,							\
		TMath<TYPE>::TWO*QUAT.x*QUAT.z - TMath<TYPE>::TWO*QUAT.w*QUAT.y,							\
		TMath<TYPE>::TWO*QUAT.y*QUAT.z + TMath<TYPE>::TWO*QUAT.w*QUAT.x,							\
		TMath<TYPE>::ONE - TMath<TYPE>::TWO*QUAT.x*QUAT.x - TMath<TYPE>::TWO*QUAT.y*QUAT.y			\
	);																								\
	if(!QUAT.isUnit())																				\
	{																								\
		T norm = QUAT.norm();																		\
		if(TMath<TYPE>::isZero(norm)) return TMatrix33<TYPE>::ZERO;									\
		else																						\
		{																							\
			m *= TMath<TYPE>::reciprocal(norm);														\
		}																							\
	}																								\
	return m;																						\

const TMatrix33<float> TMatrix33<float>::rotate(const TQuaternion<float> &q) { ARKHE_MATRIX33_ROTATE_QUATERNION(float,q) }
const TMatrix33<double> TMatrix33<double>::rotate(const TQuaternion<double> &q) { ARKHE_MATRIX33_ROTATE_QUATERNION(double,q) }

//scale
#define ARKHE_MATRIX33_SCALE(TYPE,VALUE1,VALUE2,VALUE3)		\
	return TMatrix33<TYPE>									\
	(														\
		VALUE1,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,			\
		TMath<TYPE>::ZERO,VALUE2,TMath<TYPE>::ZERO,			\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,VALUE3			\
	);														\

const TMatrix33<float> TMatrix33<float>::scale(const float &factor) { ARKHE_MATRIX33_SCALE(float,factor,factor,factor) }
const TMatrix33<double> TMatrix33<double>::scale(const double &factor) { ARKHE_MATRIX33_SCALE(double,factor,factor,factor) }
const TMatrix33<float> TMatrix33<float>::scale(const float &x,const float &y,const float &z) { ARKHE_MATRIX33_SCALE(float,x,y,z) }
const TMatrix33<double> TMatrix33<double>::scale(const double &x,const double &y,const double &z) { ARKHE_MATRIX33_SCALE(double,x,y,z) }

/////////////////////////////////////////////////////////////////////////////////////////////////////
//TMatrix44
//rotation
//rotate X
const TMatrix44<float> TMatrix44<float>::rotateX(const float &angle) { return TMatrix44<float>(TMatrix33<float>::rotateX(angle)); }
const TMatrix44<double> TMatrix44<double>::rotateX(const double &angle) { return TMatrix44<double>(TMatrix33<double>::rotateX(angle)); }
//rotate Y
const TMatrix44<float> TMatrix44<float>::rotateY(const float &angle) { return TMatrix44<float>(TMatrix33<float>::rotateY(angle)); }
const TMatrix44<double> TMatrix44<double>::rotateY(const double &angle) { return TMatrix44<double>(TMatrix33<double>::rotateY(angle)); }
//rotate Z
const TMatrix44<float> TMatrix44<float>::rotateZ(const float &angle) { return TMatrix44<float>(TMatrix33<float>::rotateZ(angle)); }
const TMatrix44<double> TMatrix44<double>::rotateZ(const double &angle) { return TMatrix44<double>(TMatrix33<double>::rotateZ(angle)); }
//rotate XYZ
const TMatrix44<float> TMatrix44<float>::rotate(const float &x,const float &y,const float &z) { return TMatrix44<float>(TMatrix33<float>::rotate(x,y,z)); }
const TMatrix44<double> TMatrix44<double>::rotate(const double &x,const double &y,const double &z) { return TMatrix44<double>(TMatrix33<double>::rotate(x,y,z)); }
//rotate axis-angle
const TMatrix44<float> TMatrix44<float>::rotate(const TVector3<float> &axis,const float &angle) { return TMatrix44<float>(TMatrix33<float>::rotate(axis,angle)); }
const TMatrix44<double> TMatrix44<double>::rotate(const TVector3<double> &axis,const double &angle) { return TMatrix44<double>(TMatrix33<double>::rotate(axis,angle)); }
//rotate quaternion
const TMatrix44<float> TMatrix44<float>::rotate(const TQuaternion<float> &q) { return TMatrix44<float>(TMatrix33<float>::rotate(q)); }
const TMatrix44<double> TMatrix44<double>::rotate(const TQuaternion<double> &q) { return TMatrix44<double>(TMatrix33<double>::rotate(q)); }
//uniform scale
const TMatrix44<float> TMatrix44<float>::scale(const float &factor) { return TMatrix44<float>(TMatrix33<float>::scale(factor)); }
const TMatrix44<double> TMatrix44<double>::scale(const double &factor) { return TMatrix44<double>(TMatrix33<double>::scale(factor)); }
//non-uniform scale
const TMatrix44<float> TMatrix44<float>::scale(const float &x,const float &y,const float &z) { return TMatrix44<float>(TMatrix33<float>::scale(x,y,z)); }
const TMatrix44<double> TMatrix44<double>::scale(const double &x,const double &y,const double &z) { return TMatrix44<double>(TMatrix33<double>::scale(x,y,z)); }

//translate
#define ARKHE_MATRIX44_TRANSLATE(TYPE,VALUE1,VALUE2,VALUE3)							\
	return TMatrix44<TYPE>															\
	(																				\
		TMath<TYPE>::ONE,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,VALUE1,				\
		TMath<TYPE>::ZERO,TMath<TYPE>::ONE,TMath<TYPE>::ZERO,VALUE2,				\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ONE,VALUE3,				\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ONE		\
	);																				\

const TMatrix44<float> TMatrix44<float>::translate(const float &x,const float &y,const float &z) { ARKHE_MATRIX44_TRANSLATE(float,x,y,z) }
const TMatrix44<double> TMatrix44<double>::translate(const double &x,const double &y,const double &z) { ARKHE_MATRIX44_TRANSLATE(double,x,y,z) }
const TMatrix44<float> TMatrix44<float>::translate(const TVector3<float> &t) { ARKHE_MATRIX44_TRANSLATE(float,t.x,t.y,t.z); }
const TMatrix44<double> TMatrix44<double>::translate(const TVector3<double> &t) { ARKHE_MATRIX44_TRANSLATE(double,t.x,t.y,t.z); }

//look at transform.
//construct view-to-world transform
//and return it's inverse (world-to-view).
//upper 3x3 is transposed and the position is transformed
//by the upper 3x3 transpose and negated.
#define ARKHE_MATRIX44_LOOKAT(TYPE,EYE,LOOKAT,UP)												\
	TVector3<TYPE> dir = -(LOOKAT - EYE);														\
	dir.normalize();																			\
	TVector3<TYPE> upPrime = up.perpendicularTo(dir);											\
	upPrime.normalize();																		\
	TVector3<TYPE> side = upPrime.cross(dir);													\
	return TMatrix44<TYPE>																		\
	(																							\
		side.x,side.y,side.z,-(side.x*EYE.x + side.y*EYE.y + side.z*EYE.z),						\
		upPrime.x,upPrime.y,upPrime.z,-(upPrime.x*EYE.x + upPrime.y*EYE.y + upPrime.z*EYE.z),	\
		dir.x,dir.y,dir.z,-(dir.x*EYE.x + dir.y*EYE.y + dir.z*EYE.z),							\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ONE					\
	);																							\

const TMatrix44<float> TMatrix44<float>::lookAt(const TVector3<float> &eye,const TVector3<float> &lookAt,const TVector3<float> &up) { ARKHE_MATRIX44_LOOKAT(float,eye,lookAt,up) }
const TMatrix44<double> TMatrix44<double>::lookAt(const TVector3<double> &eye,const TVector3<double> &lookAt,const TVector3<double> &up) { ARKHE_MATRIX44_LOOKAT(double,eye,lookAt,up) }

//orthographic transform
#define ARKHE_MATRIX44_ORTHOGRAPHIC(TYPE,L,R,T,B,N,F)								\
	return TMatrix44<TYPE>															\
	(																				\
		TMath<TYPE>::TWO/(R-L),TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,-(R+L)/(R-L),	\
		TMath<TYPE>::ZERO,TMath<TYPE>::TWO/(T-B),TMath<TYPE>::ZERO,-(T+B)/(T-B),	\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::TWO/(F-N),(F+N)/(F-N),		\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ONE		\
	);																				\

const TMatrix44<float> TMatrix44<float>::orthographic(const float &l,const float &r,const float &t,const float &b,const float &n,const float &f) { ARKHE_MATRIX44_ORTHOGRAPHIC(float,l,r,t,b,n,f) }
const TMatrix44<double> TMatrix44<double>::orthographic(const double &l,const double &r,const double &t,const double &b,const double &n,const double &f) { ARKHE_MATRIX44_ORTHOGRAPHIC(double,l,r,t,b,n,f) }

//perspective transform.
#define ARKHE_MATRIX44_PERSPECTIVE(TYPE,FOV,ASPECT,NEAR,FAR)														\
	if(TMath<T>::isZero(ASPECT)) throw Exception(Exception::DIVISION_BY_ZERO,"aspect ratio zero");					\
	if(TMath<T>::equal(near,far)) throw Exception(Exception::LOGIC_ERROR,"near clip equals far clip");				\
	if(NEAR > FAR) throw Exception(Exception::LOGIC_ERROR,"near clip greater than far clip");						\
	if(NEAR < TMath<T>::EPSILON) throw Exception(Exception::LOGIC_ERROR,"clip plane(s) negative or zero");			\
	TYPE d = TMath<TYPE>::reciprocal(TMath<TYPE>::tan((FOV*TMath<TYPE>::ONE_OVER_TWO)*TMath<TYPE>::DEG_TO_RAD));	\
	TYPE A = (NEAR + FAR) / (NEAR - FAR);																			\
	TYPE B = (TMath<TYPE>::TWO * NEAR * FAR) / (NEAR - FAR);														\
	return TMatrix44<TYPE>																							\
	(																												\
		d/ASPECT,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,												\
		TMath<TYPE>::ZERO,d,TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,													\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,A,B,																	\
		TMath<TYPE>::ZERO,TMath<TYPE>::ZERO,-TMath<TYPE>::ONE,TMath<TYPE>::ZERO										\
	);																												\

const TMatrix44<float> TMatrix44<float>::perspective(const float &fov,const float &aspect,const float &near,const float &far) { ARKHE_MATRIX44_PERSPECTIVE(float,fov,aspect,near,far) }
const TMatrix44<double> TMatrix44<double>::perspective(const double &fov,const double &aspect,const double &near,const double &far) { ARKHE_MATRIX44_PERSPECTIVE(double,fov,aspect,near,far) }

} //namespace foundation

} //namespace arkhe
