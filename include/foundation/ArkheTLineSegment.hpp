#ifndef __ARKHE_T_LINE_SEGMENT_HPP__
#define __ARKHE_T_LINE_SEGMENT_HPP__

//arkhe local includes (foundation)
#include "ArkheTLineSegmentReuse.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TLineSegment
{
public:
	//constructors
	TLineSegment() {}
	TLineSegment(const TVector3<T> &s,const TVector3<T> &e) : start(s),end(e) {}
	TLineSegment(const TLineSegment &copy) : start(copy.start),end(copy.end) {}
	//destructor
	~TLineSegment() {}
	//operator overloads
	TLineSegment &operator=(const TLineSegment &copy);
	const TVector3<T> operator()(const T &t) const;
	//methods
	const TVector3<T> closestPoint(const TVector3<T> &p) const { return line_segment::closestPoint<T>(start,end,p); }
	const T closestPoints(const TLineSegment &line,TVector3<T> &p,TVector3<T> &q) const { return line_segment::closestPoints<T>(start,end,line.start,line.end,p,q); }
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
	//variables
	TVector3<T> start,end;
private:
	//
protected:
	//
}; //class TLine

template<typename T> TLineSegment<T> &TLineSegment<T>::operator=(const TLineSegment<T> &copy)
{
	if(this == &copy)
	{
		return *this;
	}
	start = copy.start;
	end = copy.end;
	return *this;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_LINE_SEGMENT_HPP__
