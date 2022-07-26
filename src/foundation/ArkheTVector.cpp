//arkhe local includes (foundation)
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

//static variable definitions
//TVector3
const TVector3<float> TVector3<float>::ZERO(0.0f,0.0f,0.0f);
const TVector3<double> TVector3<double>::ZERO(0.0,0.0,0.0);
const TVector3<float> TVector3<float>::UNIT_X(1.0f,0.0f,0.0f);
const TVector3<double> TVector3<double>::UNIT_X(1.0,0.0,0.0);
const TVector3<float> TVector3<float>::UNIT_Y(0.0f,1.0f,0.0f);
const TVector3<double> TVector3<double>::UNIT_Y(0.0,1.0,0.0);
const TVector3<float> TVector3<float>::UNIT_Z(0.0f,0.0f,1.0f);
const TVector3<double> TVector3<double>::UNIT_Z(0.0,0.0,1.0);

//static function definitions
//TVector3
#define ARKHE_TVECTOR3_ORTHOGONALIZE(TYPE,V0,V1,V2,W0,W1,W2,NORMALIZE)	\
	_orthogonalize(V0,V1,V2,W0,W1,W2,NORMALIZE);						\

void TVector3<float>::orthogonalize
(
	const TVector3<float> &v0,const TVector3<float> &v1,const TVector3<float> &v2,
	TVector3<float> &w0,TVector3<float> &w1,TVector3<float> &w2,
	const bool normalize
) { TVector3<float> tmp; tmp._orthogonalize(v0,v1,v2,w0,w1,w2,normalize); }

void TVector3<double>::orthogonalize
(
	const TVector3<double> &v0,const TVector3<double> &v1,const TVector3<double> &v2,
	TVector3<double> &w0,TVector3<double> &w1,TVector3<double> &w2,
	const bool normalize
) { TVector3<double> tmp; tmp._orthogonalize(v0,v1,v2,w0,w1,w2,normalize); }

#define ARKHE_TVECTOR3_RANDOM(TYPE,V)		\
	return TVector3<TYPE>					\
	(										\
		TMath<TYPE>::randomBinomial(V),		\
		TMath<TYPE>::randomBinomial(V),		\
		TMath<TYPE>::randomBinomial(V)		\
	);										\

const TVector3<float> TVector3<float>::random(const float &t) { ARKHE_TVECTOR3_RANDOM(float,t) }
const TVector3<double> TVector3<double>::random(const double &t) { ARKHE_TVECTOR3_RANDOM(double,t) }

#define ARKHE_TVECTOR3_RANDOM_XYZ(TYPE,V0,V1,V2)	\
	return TVector3<TYPE>							\
	(												\
		TMath<TYPE>::randomBinomial(V0),			\
		TMath<TYPE>::randomBinomial(V1),			\
		TMath<TYPE>::randomBinomial(V2)				\
	);												\

const TVector3<float> TVector3<float>::random(const float &x,const float &y,const float &z) { ARKHE_TVECTOR3_RANDOM_XYZ(float,x,y,z) }
const TVector3<double> TVector3<double>::random(const double &x,const double &y,const double &z) { ARKHE_TVECTOR3_RANDOM_XYZ(double,x,y,z) }

//unit tests
//TVector3
#ifdef ARKHE_ENABLE_FOUNDATION_TESTS
template<typename T> void TVector3<T>::UNIT_TEST()
{
	//
}
#endif //ARKHE_ENABLE_FOUNDATION_TESTS

} //namespace foundation

} //namespace arkhe
