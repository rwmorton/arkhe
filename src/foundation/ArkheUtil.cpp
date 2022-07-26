//arkhe local includes (foundation)
#include "ArkheUtil.h"
#include "ArkheException.h"

//std includes
#include <fstream>

namespace arkhe
{

namespace foundation
{

const std::string Util::readFile(const std::string &filename)
{
	std::ifstream in(filename,std::ios::in|std::ios::binary|std::ios::ate);
	std::ifstream::pos_type size;
	char *memblock = 0;
	if(!in.is_open())
	{
		throw Exception(Exception::FILE_IO_ERROR);
	}
	else
	{
		size = in.tellg();
		memblock = new char[size];
		in.seekg(0,std::ios::beg);
		in.read(memblock,size);
		in.close();
	}
	std::string data(memblock,size);
	delete[] memblock;
	return data;
}

void Util::writeFile(const std::string &data,const std::string &filename)
{
	std::ofstream out(filename,std::ios::out|std::ios::binary);
	if(!out.is_open())
	{
		throw Exception(Exception::FILE_IO_ERROR);
	}
	else
	{
		const char *data_ptr = data.c_str();
		for(int i=0; i<data.size(); i++)
		{
			out << *data_ptr++;
		}
	}
	out.close();
}

//unit test
#ifdef ARKHE_ENABLE_FOUNDATION_TESTS
void Util::UNIT_TEST()
{
	//
}
#endif //ARKHE_ENABLE_FOUNDATION_TESTS

} //namespace foundation

} //namespace arkhe
