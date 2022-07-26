#ifndef __ARKHE_T_AABB_REUSE_HPP__
#define __ARKHE_T_AABB_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

namespace aabb
{

//Compute closest point to AABB from given point
template<typename T> const TVector3<T> closestPoint(const TVector3<T> &min,const TVector3<T> &max,const TVector3<T> &p)
{
	TVector3<T> q;
	T v;
	for(int i=0; i<3; i++)
	{
		v = p[i];
		if(v < min[i]) v = min[i];
		if(v > max[i]) v = max[i];
		q[i] = v;
	}
	return q;
}

} //namespace aabb

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_AABB_REUSE_HPP__
