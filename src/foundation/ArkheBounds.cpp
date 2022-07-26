//arkhe local includes (foundation)
#include "ArkheTAABB.hpp"

namespace arkhe
{

namespace foundation
{

//static variable definitions
//TAABB
const TAABB<float> TAABB<float>::ZERO(TVector3<float>::ZERO,TVector3<float>::ZERO);
const TAABB<double> TAABB<double>::ZERO(TVector3<double>::ZERO,TVector3<double>::ZERO);

const TAABB<float> TAABB<float>::UNIT
(
	-TVector3<float>(TMath<float>::ONE_OVER_TWO),
	TVector3<float>(TMath<float>::ONE_OVER_TWO)
);
const TAABB<double> TAABB<double>::UNIT
(
	-TVector3<double>(TMath<double>::ONE_OVER_TWO),
	TVector3<double>(TMath<double>::ONE_OVER_TWO)
);

} //namespace foundation

} //namespace arkhe
