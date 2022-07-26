#ifndef __ARKHE_T_MARCHING_CUBES_MESH_HPP__
#define __ARKHE_T_MARCHING_CUBES_MESH_HPP__

//arkhe local includes (foundation)
#include "ArkheTMesh.hpp"
#include "ArkheTVoxel.hpp"

namespace arkhe
{

namespace foundation
{

//This class contains relationship information between the
//generated triangles and voxels. Used for optimization.
//All this extra information can be removed to convert
//the mesh into a standard triangle mesh.
template<typename T> class TMarchingCubesMesh
{
public:
	//friends
	template<typename T> friend class TMarchingCubes;
	template<typename T> friend class TMarchingCubesGrid;
public:
	//data
	struct Edge
	{
		Edge() {}
		Edge(const TVector3<T> &_v0,const TVector3<T> &_v1)
		{
			v0 = TMath<TVector3<T>>::minimum(_v0,_v1);
			v1 = TMath<TVector3<T>>::maximum(_v0,_v1);
		}
		const bool operator<(const Edge &e)
		{
			if(v0 != e.v0) return v0 < e.v0;
			else return v1 < e.v1;
		}
		const bool operator==(const Edge &e)
		{
			return v0==e.v0 && v1==e.v1;
		}
		TVector3<T> v0,v1;	//edge vertices
	}; //struct Edge
	struct Tri
	{
		TVector3<T> v0,v1,v2;		//vertices
		TVector3<T> n0,n1,n2;		//vertex normals
		Edge e0,e1,e2;				//edges - e0=v1-v0,e1=v2-v1,e2=v0-v2
		Tri *en0,*en1,*en2;			//edge neighbors
	}; //struct Tri
	//destructor
	~TMarchingCubesMesh();
	//methods
	const unsigned int numTriangles() const { return triangles.size(); }
	void computeEdgeNeighbors(TGrid3D<T> &grid);
	void render(bool shade=true); //defer to another source file
//private: //TMP - TODO
	//data
	std::vector<Tri *> triangles;	//ownership of the heap memory assumed here
	std::map<unsigned int,std::vector<Tri *>> voxelTriangles;
protected:
	//
}; //class TMarchingCubesMesh

template<typename T> TMarchingCubesMesh<T>::~TMarchingCubesMesh()
{
	std::vector<Tri *>::iterator i = triangles.begin();
	while(i != triangles.end())
	{
		delete *i;
		i++;
	}
	triangles.clear();
}

//TODO - broken!!!
template<typename T> void TMarchingCubesMesh<T>::computeEdgeNeighbors(TGrid3D<T> &grid)
{
	//run through the voxel-triangles map
	//if voxel is empty ignore neighboring voxels
	//otherwise run through the neighbors
	//to find shared edges
	std::map<unsigned int,std::vector<Tri *>>::iterator i = voxelTriangles.begin();
	while(i != voxelTriangles.end())
	{
		if(i->second.size() > 0)
		{
			//current voxel is not empty
			//loop through it's triangles
			std::vector<Tri *>::iterator j = i->second.begin();
			while(j != i->second.end())
			{
				//check triangle against all neighbors to this voxel
				std::vector<TVoxel<T> *> neighbors = grid.getNeighborsTo(i->first);
				std::vector<TVoxel<T> *>::iterator k = neighbors.begin();
				while(k != neighbors.end())
				{
					//get all triangles for this neighboring voxel
					std::vector<Tri *>::iterator t = voxelTriangles[(*k)->id].begin();
					//and loop over them
					while(t != voxelTriangles[(*k)->id].end())
					{
						//e0 test
						if((*j)->e0 == (*t)->e0) (*j)->en0 = *t;
						if((*j)->e0 == (*t)->e1) (*j)->en0 = *t;
						if((*j)->e0 == (*t)->e2) (*j)->en0 = *t;
						//e1 test
						if((*j)->e1 == (*t)->e0) (*j)->en1 = *t;
						if((*j)->e1 == (*t)->e1) (*j)->en1 = *t;
						if((*j)->e1 == (*t)->e2) (*j)->en1 = *t;
						//e2 test
						if((*j)->e2 == (*t)->e0) (*j)->en2 = *t;
						if((*j)->e2 == (*t)->e1) (*j)->en2 = *t;
						if((*j)->e2 == (*t)->e2) (*j)->en2 = *t;
						t++;
					}
					k++;
				}
				j++;
			}
		}
		i++;
	}
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MARCHING_CUBES_MESH_HPP__
