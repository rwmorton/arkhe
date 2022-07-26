#ifndef __ARKHE_T_LINE_HPP__
#define __ARKHE_T_LINE_HPP__

//arkhe local includes (foundation)
#include "ArkheTLineReuse.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TLine
{
public:
	//constructors
	TLine() {}
	TLine(const TVector3<T> &o,const TVector3<T> &d) : origin(o),direction(d) {}
	TLine(const TLine &copy) : origin(copy.origin),direction(copy.direction) {}
	//destructor
	~TLine() {}
	//operator overloads
	TLine &operator=(const TLine &copy);
	const TVector3<T> operator()(const T &t) const;
	//methods
	const TVector3<T> closestPoint(const TVector3<T> &p) const { return line::closestPoint(origin,direction,p); }
	const bool closestPoints(const TLine &line,TVector3<T> &p,TVector3<T> &q) const { return line::closestPoints(origin,direction,line.origin,line.directino,p,q); }
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
	//variables
	TVector3<T> origin,direction;
private:
	//
protected:
	//
}; //class TLine

template<typename T> TLine<T> &TLine<T>::operator=(const TLine<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	origin = copy.origin;
	direction = copy.direction;
	return *this;
}

//Get a point on the line at parameter t
template<typename T> const TVector3<T> TLine<T>::operator()(const T &t) const
{
	return origin + direction * t;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_LINE_HPP__
