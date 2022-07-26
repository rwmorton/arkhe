#ifndef __ARKHE_EXCEPTION_H__
#define __ARKHE_EXCEPTION_H__

//std includes
#include <exception>
#include <string>
#include <map>

namespace arkhe
{

namespace foundation
{

class Exception : public std::exception
{
public:
	//enums
	enum TYPE
	{
		DIVISION_BY_ZERO = 0,
		OUT_OF_RANGE,
		FILE_IO_ERROR,
		NULL_POINTER,
		UNIT_TEST_FAILED,
		SWITCH_CASE_UNDEFINED,
		NO_IMPLEMENTATION,
		LOGIC_ERROR,
		UNDEFINED
	}; //enum TYPE
	//constructors
	Exception(const std::string &str) : m_str(str) {};
	Exception(const TYPE &type,const std::string &strExtra="");
	//destructor
	~Exception() throw() {}
	//methods
	virtual const char *what() const throw() { return m_str.c_str(); }
private:
	//variables
	static const std::string s_descriptions[];
	std::string m_str;
protected:
	//
}; //class Exception

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_EXCEPTION_H__
