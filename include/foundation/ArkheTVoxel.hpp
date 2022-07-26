#ifndef __ARKHE_T_VOXEL_HPP__
#define __ARKHE_T_VOXEL_HPP__

namespace arkhe
{

namespace foundation
{

template<typename T> class TVoxel
{
public:
	//variables
	unsigned int id;
	TVector3<T> vertices[8];
private:
	//
protected:
	//
}; //class TVoxel<T>

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_VOXEL_HPP__
