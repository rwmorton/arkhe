#ifndef __ARKHE_T_AABB_HPP__
#define __ARKHE_T_AABB_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTAABBReuse.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TAABB
{
public:
	//constructors
	TAABB() {}
	TAABB(const TVector3<T> &_min,const TVector3<T> &_max) : min(_min),max(_max) {}
	TAABB(const TAABB &copy) : min(copy.min),max(copy.max) {}
	//destructor
	~TAABB() {}
	//operator overloads
	TAABB &operator=(const TAABB &copy);
	//methods
	const TVector3<T> closestPoint(const TVector3<T> &p) const { return aabb::closestPoint(min,max,p); }
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
	//static variables
	static const TAABB<T> ZERO;
	static const TAABB<T> UNIT;	//bounding box of unit length
	//variables
	TVector3<T> min,max;
private:
	//
protected:
	//
}; //class TAABB

template<typename T> TAABB<T> &TAABB<T>::operator=(const TAABB<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	min = copy.min;
	max = copy.max;
	return *this;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_AABB_HPP__
