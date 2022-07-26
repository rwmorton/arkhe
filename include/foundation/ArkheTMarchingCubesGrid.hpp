#ifndef __ARKHE_T_MARCHING_CUBES_GRID_HPP__
#define __ARKHE_T_MARCHING_CUBES_GRID_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTVector3.hpp"
#include "ArkheTAABB.hpp"
#include "ArkheTMesh.hpp"
#include "ArkheMarchingCubesTables.h"
#include "ArkheTImplicit.hpp"
#include "ArkheTGrid3D.hpp"
#include "ArkheTMarchingCubesMesh.hpp"

namespace arkhe
{

namespace foundation
{

//
//Build a mesh off an implicit function. There are two ways to
//pass an implicit function, one is to pass a pointer to function(s)
//of the form:
//
//const T implicitFunction(const TVector3<T> &p);
//const TVector3<T> gradientFunction(const TVector3<T> &p);
//
//and the other way is to pass a class that derives from TImplicit
//
//template<typename T> class MyImplicit : public TImplicit<T> { ... };
//
//and implementing the evaluate(...), gradientDefined() and gradient(...) methods as required.
//
template<typename T> class TMarchingCubesGrid
{
public:
	//static methods
	static TMarchingCubesMesh<T> *build
	(
		const TGrid3D<T> &grid,
		const T (*implicitFunction)(const TVector3<T> &p),
		const TVector3<T> (*gradientFunction)(const TVector3<T> &p)=0,
		const T &isoLevel=TMath<T>::ZERO
	);
	static TMarchingCubesMesh<T> *build(const TGrid3D<T> &grid,TImplicit<T> *implicit,const T &isoLevel=TMath<T>::ZERO);
private:
	//data
	struct Data
	{
		T isoValues[8];
		int voxelIndex;
		TVector3<T> vertList[12];
	}; //struct Data
	//static methods
	static const TVector3<T> vertexInterpolate(const T &isoLevel,const TVector3<T> &v1,const TVector3<T> &v2,const T &isoV1,const T &isoV2);
	static void generateVertexList(const TVoxel<T> *voxel,Data *data,const T &isoLevel);
	static void addTriangles(const TVoxel<T> *voxel,Data *data,TMarchingCubesMesh<T> *mesh);
	static void generateNormals(const TVector3<T> (*gradientFunction)(const TVector3<T> &p),TMarchingCubesMesh<T> *mesh);
	static void generateNormals(TImplicit<T> *implicit,TMarchingCubesMesh<T> *mesh);
	//
	//TODO
	//
	//1. return list of voxels that are neighbors to the voxel under consideration
	//will be a big optimization on the edge lookup at the mesh processing stage
	//make this an option during the computation of the grid.
	//process: for current voxel generate voxels surrounding it and remove those
	//that fall out of bounds
	//
	//2. add custom volume to define voxels in. this serves as an optimization.
	//ie. a polygonizing a sphere in a cube volume is a waste of resources, a
	//spherical volume will work better. look into minkowski sum/diff as a method
	//for combinations of convex volumes (see notes - #11 08/03/14)
	//
protected:
	//
}; //class TMarchingCubesGrid

//Build an approximation to the implicit function on a grid
//within the bounds given and with a resolution voxelSize
template<typename T>
TMarchingCubesMesh<T> *TMarchingCubesGrid<T>::build
(
	const TGrid3D<T> &grid,
	const T (*implicitFunction)(const TVector3<T> &p),
	const TVector3<T> (*gradientFunction)(const TVector3<T> &p),
	const T &isoLevel
)
{
	if(!implicitFunction) throw Exception(Exception::LOGIC_ERROR,"must pass a pointer to an implementation of an implicit function");

	TMarchingCubesMesh<T> *mesh = new TMarchingCubesMesh<T>();

	//On the innermost part of the loop the point (x,y,z) is the corner
	//of the voxel we are processing. From here on we process the remaining
	//7 corners of the voxel to compute the isosurface.
	Data data;
	//for each voxel
	const TVoxel<T> *voxel = grid.getVoxels();
	for(int i=0; i<grid.numVoxels(); i++,voxel++)
	{
		//for each vertex
		//determine the index into the edge table which
		//tells us which vertices are inside the surface
		data.voxelIndex = 0; //reset
		for(int i=0; i<8; i++)
		{
			//compute and save the isovalue at this vertex
			data.isoValues[i] = implicitFunction(voxel->vertices[i]);

			if(data.isoValues[i] < isoLevel)
			{
				data.voxelIndex |= (int)TMath<T>::pow(2,i);
			}
		}

		//voxel is entirely in or out of the surface
		if(marching_cubes::edgeTable[data.voxelIndex] == 0)
		{
			continue; //process next voxel
		}

		generateVertexList(voxel,&data,isoLevel);
		addTriangles(voxel,&data,mesh);
	}

	//generate mesh normals from function gradient
	if(gradientFunction) generateNormals(gradientFunction,mesh);

	return mesh;
}

template<typename T> TMarchingCubesMesh<T> *TMarchingCubesGrid<T>::build(const TGrid3D<T> &grid,TImplicit<T> *implicit,const T &isoLevel)
{
	if(!implicit) throw Exception(Exception::LOGIC_ERROR,"must pass a pointer to an implementation of an implicit function");

	TMarchingCubesMesh<T> *mesh = new TMarchingCubesMesh<T>();

	//On the innermost part of the loop the point (x,y,z) is the corner
	//of the voxel we are processing. From here on we process the remaining
	//7 corners of the voxel to compute the isosurface.
	Data data;
	//for each voxel
	const TVoxel<T> *voxel = grid.getVoxels();
	for(int i=0; i<grid.numVoxels(); i++,voxel++)
	{
		//for each vertex
		//determine the index into the edge table which
		//tells us which vertices are inside the surface
		data.voxelIndex = 0; //reset
		for(int i=0; i<8; i++)
		{
			//compute and save the isovalue at this vertex
			data.isoValues[i] = implicitFunction(voxel->vertices[i]);

			if(data.isoValues[i] < isoLevel)
			{
				data.voxelIndex |= (int)TMath<T>::pow(2,i);
			}
		}

		//voxel is entirely in or out of the surface
		if(marching_cubes::edgeTable[data.voxelIndex] == 0)
		{
			continue; //process next voxel
		}

		generateVertexList(voxel,&data,isoLevel);
		addTriangles(voxel,&data,mesh);
	}

	//generate mesh normals from function gradient
	if(implicit->gradientDefined()) generateNormals(implicit,mesh);

	return mesh;
}

template<typename T> const TVector3<T> TMarchingCubesGrid<T>::vertexInterpolate(const T &isoLevel,const TVector3<T> &v1,const TVector3<T> &v2,const T &isoV1,const T &isoV2)
{
	return v1 + ((v2 - v1) * (isoLevel - isoV1)) / (isoV2 - isoV1);
}

template<typename T> void TMarchingCubesGrid<T>::generateVertexList(const TVoxel<T> *voxel,Data *data,const T &isoLevel)
{
	//find the vertices where the surface intersects the voxel
	for(int i=0; i<12; i++)
	{
		if(marching_cubes::edgeTable[data->voxelIndex] & (int)(TMath<T>::pow(2,i)))
		{
			data->vertList[i] = vertexInterpolate
			(
				isoLevel,
				voxel->vertices[marching_cubes::interpTable[i][0]],
				voxel->vertices[marching_cubes::interpTable[i][1]],
				data->isoValues[marching_cubes::interpTable[i][0]],
				data->isoValues[marching_cubes::interpTable[i][1]]
			);
		}
	}
}

template<typename T> void TMarchingCubesGrid<T>::addTriangles(const TVoxel<T> *voxel,Data *data,TMarchingCubesMesh<T> *mesh)
{
	if(!voxel) throw Exception(Exception::NULL_POINTER);
	//create the triangles
	TMarchingCubesMesh<T>::Tri *tri;
	std::vector<TMarchingCubesMesh<T>::Tri *> voxelTriangles;
	for(int v=0; marching_cubes::triTable[data->voxelIndex][v]!=-1; v+=3)
	{
		//generate vertices
		tri = new TMarchingCubesMesh<T>::Tri;
		tri->v0 = 
		TVector3<T>
		(
			data->vertList[marching_cubes::triTable[data->voxelIndex][v]].x,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v]].y,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v]].z
		);
		tri->v1 =
		TVector3<T>
		(
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+1]].x,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+1]].y,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+1]].z
		);
		tri->v2 =
		TVector3<T>
		(
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+2]].x,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+2]].y,
			data->vertList[marching_cubes::triTable[data->voxelIndex][v+2]].z
		);
		//set triangle edges
		tri->e0 = TMarchingCubesMesh<T>::Edge(tri->v0,tri->v1);
		tri->e1 = TMarchingCubesMesh<T>::Edge(tri->v1,tri->v2);
		tri->e2 = TMarchingCubesMesh<T>::Edge(tri->v0,tri->v2);

		//save the triangle
		mesh->triangles.push_back(tri);
		voxelTriangles.push_back(tri);
	}
	mesh->voxelTriangles[voxel->id] = voxelTriangles;
}

template<typename T> void TMarchingCubesGrid<T>::generateNormals(const TVector3<T> (*gradientFunction)(const TVector3<T> &p),TMarchingCubesMesh<T> *mesh)
{
	//generate normals from function gradient
	std::vector<TMarchingCubesMesh<T>::Tri *>::iterator i = mesh->triangles.begin();
	TVector3<T> n;
	while(i != mesh->triangles.end())
	{
		//n0
		n = gradientFunction((*i)->v0);
		if(!n.isZero()) n.normalize();
		(*i)->n0 = n;
		//n1
		n = gradientFunction((*i)->v1);
		if(!n.isZero()) n.normalize();
		(*i)->n1 = n;
		//n2
		n = gradientFunction((*i)->v2);
		if(!n.isZero()) n.normalize();
		(*i)->n2 = n;
		//next triangle
		i++;
	}
}

template<typename T> void TMarchingCubesGrid<T>::generateNormals(TImplicit<T> *implicit,TMarchingCubesMesh<T> *mesh)
{
	//generate normals from function gradient
	std::vector<TMarchingCubesMesh<T>::Tri *>::iterator i = mesh->triangles.begin();
	TVector3<T> n;
	while(i != mesh->triangles.end())
	{
		//n0
		n = implicit->gradient((*i)->v0);
		if(!n.isZero()) n.normalize();
		(*i)->n0 = n;
		//n1
		n = implicit->gradient((*i)->v1);
		if(!n.isZero()) n.normalize();
		(*i)->n1 = n;
		//n2
		n = implicit->gradient((*i)->v2);
		if(!n.isZero()) n.normalize();
		(*i)->n2 = n;
		//next triangle
		i++;
	}
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_MARCHING_CUBES_GRID_HPP__
