#ifndef __ARKHE_T_SPHERE_HPP__
#define __ARKHE_T_SPHERE_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TSphere
{
public:
	//constructors
	TSphere() {}
	TSphere(const TVector3<T> &c,const T &r) : center(c),radius(r) {}
	TSphere(const TSphere &copy);
	//destructor
	~TSphere() {}
	//operator overloads
	TSphere &operator=(const TSphere &copy);
	//debugging
#ifdef ARKHE_DEBUG
	void render(unsigned slices=16,unsigned stacks=16); //defer to another source file
#endif //ARKHE_DEBUG
	//variables
	TVector3<T> center;
	T radius;
private:
	//
protected:
	//
}; //class TSphere

template<typename T> TSphere<T>::TSphere(const TSphere<T> &copy) : center(copy.center),radius(copy.radius) {}

template<typename T> TSphere<T> &TSphere<T>::operator=(const TSphere<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	center = copy.center;
	radius = copy.radius;
	return *this;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_SPHERE_HPP__
