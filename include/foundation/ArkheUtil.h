#ifndef __ARKHE_UTIL_H__
#define __ARKHE_UTIL_H__

//std includes
#include <sstream>

namespace arkhe
{

namespace foundation
{

class Util
{
public:
	//static methods
	static const std::string readFile(const std::string &filename);
	static void writeFile(const std::string &data,const std::string &filename);
	template<typename T> static void swap(T &a,T &b);
	template<typename T> static const std::string toString(const T &t);
	template<typename T> static const std::string toString(const T *t_ptr,unsigned int n);
private:
	//
protected:
	//
}; //class Util

//////////// Global utility functions ////////////

template<typename T> void Util::swap(T &a,T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T> const std::string Util::toString(const T &t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

//streams each element of the array and separates with whitespace.
template<typename T> const std::string Util::toString(const T *t_ptr,unsigned int n)
{
	std::stringstream ss;
	for(unsigned int i=0; i<n; i++)
	{
		ss << *t_ptr;
		if(i < n-1) ss << ' ';
		t_ptr++;
	}
	return ss.str();
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_UTIL_H__
