#ifndef __ARKHE_T_MESH_HPP__
#define __ARKHE_T_MESH_HPP__

//arkhe local includes (foundation)
#include "ArkheTVector3.hpp"
#include "ArkheTTriangle.hpp"

namespace arkhe
{

namespace foundation
{

//Base class for meshes.
template<typename T> class TMesh
{
public:
	//methods
	virtual void render(bool shade=true) = 0; //defer to another source file
private:
	//
protected:
	//
}; //class TMesh

//Triangle mesh
template<typename T> class TTriangleMesh : public TMesh<T>
{
public:
	//friends
	template<typename T> friend class TMarchingCubes;
public:
	//data
	struct Tri
	{
		TVector3<T> v0,v1,v2;		//vertices
		TVector3<T> n0,n1,n2;		//vertex normals
	}; //struct Tri
	//methods
	const unsigned int numTriangles() const { return triangles.size(); }
	void render(bool shade=true); //defer to another source file
private:
	//variables
	std::vector<Tri *> triangles;
protected:
	//
}; //class TMesh

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MESH_HPP__
