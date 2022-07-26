#ifndef __ARKHE_T_IMPLICIT_HPP__
#define __ARKHE_T_IMPLICIT_HPP__

//arkhe local includes (foundation)
#include "ArkheException.h"

namespace arkhe
{

namespace foundation
{

//Base class for implicit functions.
template<typename T> class TImplicit
{
public:
	//methods
	virtual const T evaluate(const TVector3<T> &p) = 0;
	virtual const bool gradientDefined() const { return false; }
	const TVector3<T> gradient(const TVector3<T> &p) { throw Exception(Exception::NO_IMPLEMENTATION,"no gradient method for implicit function implementation"); }
private:
	//
protected:
	//
}; //class TImplicit

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_IMPLICIT_HPP__
