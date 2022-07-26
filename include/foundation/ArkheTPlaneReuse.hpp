#ifndef __ARKHE_T_PLANE_REUSE_HPP__
#define __ARKHE_T_PLANE_REUSE_HPP__

//arkhe local includes (foundation)
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

namespace plane
{

//get closest point to plane from point p
template<typename T> const TVector3<T> closestPoint(const TVector3<T> normal,const T &distance,const TVector3<T> &p)
{
	T t = (normal.dot(p) - distance) / normal.dot(normal);
	return p - normal * t;
}

} //namespace plane

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_PLANE_REUSE_HPP__
