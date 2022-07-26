#ifndef __ARKHE_T_MARCHING_CUBES_HPP__
#define __ARKHE_T_MARCHING_CUBES_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTVector3.hpp"
#include "ArkheTAABB.hpp"
#include "ArkheTMesh.hpp"
#include "ArkheMarchingCubesTables.h"
#include "ArkheTImplicit.hpp"

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
template<typename T> class TMarchingCubes
{
public:
	//static methods
	static TTriangleMesh<T> *build
	(
		const TAABB<T> &bounds,unsigned int resolution,
		const T (*implicitFunction)(const TVector3<T> &p),
		const TVector3<T> (*gradientFunction)(const TVector3<T> &p)=0,
		const T &isoLevel=TMath<T>::ZERO
	);
	static TTriangleMesh<T> *build(const TAABB<T> &bounds,unsigned int resolution,TImplicit<T> *implicit,const T &isoLevel=TMath<T>::ZERO);
private:
	//data
	struct Data
	{
		TVector3<T> vertices[8];
		T isoValues[8];
		int voxelIndex;
		TVector3<T> vertList[12];
	}; //struct Data
	//static methods
	static const TVector3<T> vertexInterpolate(const T &isoLevel,const TVector3<T> &v1,const TVector3<T> &v2,const T &isoV1,const T &isoV2);
	static void generateVoxel(const TVector3<T> &p,Data *data,const T &xSize,const T &ySize,const T &zSize);
	static void generateVertexList(Data *data,const T &isoLevel);
	static void addTriangles(Data *data,TTriangleMesh<T> *mesh);
	static void generateNormals(const TVector3<T> (*gradientFunction)(const TVector3<T> &p),TTriangleMesh<T> *mesh);
	static void generateNormals(TImplicit<T> *implicit,TTriangleMesh<T> *mesh);
protected:
	//
}; //class TMarchingCubes

//Build an approximation to the implicit function on a grid
//within the bounds given and with a resolution voxelSize
template<typename T>
TTriangleMesh<T> *TMarchingCubes<T>::build
(
	const TAABB<T> &bounds,unsigned int resolution,
	const T (*implicitFunction)(const TVector3<T> &p),
	const TVector3<T> (*gradientFunction)(const TVector3<T> &p),
	const T &isoLevel
)
{
	if(!implicitFunction) throw Exception(Exception::LOGIC_ERROR,"must pass a pointer to an implementation of an implicit function");

	TTriangleMesh<T> *mesh = new TTriangleMesh<T>();

	//On the innermost part of the loop the point (x,y,z) is the corner
	//of the voxel we are processing. From here on we process the remaining
	//7 corners of the voxel to compute the isosurface.
	Data data;
	//for each voxel
	T xSize = TMath<T>::abs(bounds.max.x-bounds.min.x)/(T)resolution;
	T ySize = TMath<T>::abs(bounds.max.y-bounds.min.y)/(T)resolution;
	T zSize = TMath<T>::abs(bounds.max.z-bounds.min.z)/(T)resolution;
	xSize += TMath<T>::EPSILON; //prevent rounding errors
	ySize += TMath<T>::EPSILON;
	zSize += TMath<T>::EPSILON;
	for(T x=bounds.min.x; x<bounds.max.x; x+=xSize)
	{
		for(T y=bounds.min.y; y<bounds.max.y; y+=ySize)
		{
			for(T z=bounds.min.z; z<bounds.max.z; z+=zSize)
			{
				generateVoxel(TVector3<T>(x,y,z),&data,xSize,ySize,zSize);

				//for each vertex
				//determine the index into the edge table which
				//tells us which vertices are inside the surface
				data.voxelIndex = 0; //reset
				for(int i=0; i<8; i++)
				{
					//compute and save the isovalue at this vertex
					data.isoValues[i] = implicitFunction(data.vertices[i]);

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

				generateVertexList(&data,isoLevel);
				addTriangles(&data,mesh);
			}
		}
	}

	//generate mesh normals from function gradient
	if(gradientFunction) generateNormals(gradientFunction,mesh);

	return mesh;
}

template<typename T> TTriangleMesh<T> *TMarchingCubes<T>::build(const TAABB<T> &bounds,unsigned int resolution,TImplicit<T> *implicit,const T &isoLevel)
{
	if(!implicit) throw Exception(Exception::LOGIC_ERROR,"must pass a pointer to an implementation of an implicit function");

	TTriangleMesh<T> *mesh = new TTriangleMesh<T>();

	//On the innermost part of the loop the point (x,y,z) is the corner
	//of the voxel we are processing. From here on we process the remaining
	//7 corners of the voxel to compute the isosurface.
	Data data;
	//for each voxel
	T xSize = TMath<T>::abs(bounds.max.x-bounds.min.x)/(T)resolution;
	T ySize = TMath<T>::abs(bounds.max.y-bounds.min.y)/(T)resolution;
	T zSize = TMath<T>::abs(bounds.max.z-bounds.min.z)/(T)resolution;
	xSize += TMath<T>::EPSILON; //prevent rounding errors
	ySize += TMath<T>::EPSILON;
	zSize += TMath<T>::EPSILON;
	for(T x=bounds.min.x; x<bounds.max.x; x+=xSize)
	{
		for(T y=bounds.min.y; y<bounds.max.y; y+=ySize)
		{
			for(T z=bounds.min.z; z<bounds.max.z; z+=zSize)
			{
				generateVoxel(TVector3<T>(x,y,z),&data,xSize,ySize,zSize);

				//for each vertex
				//determine the index into the edge table which
				//tells us which vertices are inside the surface
				data.voxelIndex = 0; //reset
				for(int i=0; i<8; i++)
				{
					//compute and save the isovalue at this vertex
					data.isoValues[i] = implicit->evaluate(data.vertices[i]);

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

				generateVertexList(&data,isoLevel);
				addTriangles(&data,mesh); //gradient function not used
			}
		}
	}

	//generate mesh normals from function gradient
	if(implicit->gradientDefined()) generateNormals(implicit,mesh);

	return mesh;
}

template<typename T> const TVector3<T> TMarchingCubes<T>::vertexInterpolate(const T &isoLevel,const TVector3<T> &v1,const TVector3<T> &v2,const T &isoV1,const T &isoV2)
{
	return v1 + ((v2 - v1) * (isoLevel - isoV1)) / (isoV2 - isoV1);
}

template<typename T> void TMarchingCubes<T>::generateVoxel(const TVector3<T> &p,Data *data,const T &xSize,const T &ySize,const T &zSize)
{
	//generate voxel vertices
	data->vertices[0] = Vector3(p.x,p.y,p.z); //the current vertex (x,y,z)
	data->vertices[1] = Vector3(p.x+xSize,p.y,p.z);
	data->vertices[2] = Vector3(p.x+xSize,p.y,p.z+zSize);
	data->vertices[3] = Vector3(p.x,p.y,p.z+zSize);
	data->vertices[4] = Vector3(p.x,p.y+ySize,p.z);
	data->vertices[5] = Vector3(p.x+xSize,p.y+ySize,p.z);
	data->vertices[6] = Vector3(p.x+xSize,p.y+ySize,p.z+zSize);
	data->vertices[7] = Vector3(p.x,p.y+ySize,p.z+zSize);
}

template<typename T> void TMarchingCubes<T>::generateVertexList(Data *data,const T &isoLevel)
{
	//find the vertices where the surface intersects the voxel
	for(int i=0; i<12; i++)
	{
		if(marching_cubes::edgeTable[data->voxelIndex] & (int)(TMath<T>::pow(2,i)))
		{
			data->vertList[i] = vertexInterpolate
			(
				isoLevel,
				data->vertices[marching_cubes::interpTable[i][0]],
				data->vertices[marching_cubes::interpTable[i][1]],
				data->isoValues[marching_cubes::interpTable[i][0]],
				data->isoValues[marching_cubes::interpTable[i][1]]
			);
		}
	}
}

template<typename T> void TMarchingCubes<T>::addTriangles(Data *data,TTriangleMesh<T> *mesh)
{
	//create the triangles
	TTriangleMesh<T>::Tri *tri;
	for(int v=0; marching_cubes::triTable[data->voxelIndex][v]!=-1; v+=3)
	{
		//generate vertices
		tri = new TTriangleMesh<T>::Tri;
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
		//save the triangle
		mesh->triangles.push_back(tri);
	}
}

template<typename T> void TMarchingCubes<T>::generateNormals(const TVector3<T> (*gradientFunction)(const TVector3<T> &p),TTriangleMesh<T> *mesh)
{
	//generate normals from function gradient
	std::vector<TTriangleMesh<T>::Tri *>::iterator i = mesh->triangles.begin();
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

template<typename T> void TMarchingCubes<T>::generateNormals(TImplicit<T> *implicit,TTriangleMesh<T> *mesh)
{
	//generate normals from function gradient
	std::vector<TTriangleMesh<T>::Tri *>::iterator i = mesh->triangles.begin();
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

#endif //__ARKHE_T_MARCHING_CUBES_HPP__
