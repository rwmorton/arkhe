//arkhe local includes (foundation)
#include "ArkheException.h"
#include "ArkheUtil.h"

//std includes
#include <sstream>

namespace arkhe
{

namespace foundation
{

//static variable definitions
const std::string Exception::s_descriptions[] =
{
	"Division-by-zero",
	"Out of range",
	"File I/O error",
	"Null pointer",
	"Unit test failed",
	"Switch case undefined",
	"No implementation",
	"Undefined"
};

Exception::Exception(const TYPE &type,const std::string &strExtra)
{
	m_str = s_descriptions[type];
	m_str += ", ";
	m_str += __FILE__;
	m_str += ", ";
	m_str += Util::toString<int>(__LINE__);
	if(!strExtra.empty())
	{
		m_str += ", ";
		m_str += strExtra;
	}
}

} //namespace foundation

} //namespace arkhe
