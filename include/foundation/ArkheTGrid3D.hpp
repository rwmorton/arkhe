#ifndef __ARKHE_T_GRID_3D_HPP__
#define __ARKHE_T_GRID_3D_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTVector3.hpp"
#include "ArkheTAABB.hpp"
#include "ArkheTVoxel.hpp"

namespace arkhe
{

namespace foundation
{

//When parameters such as resolution/bounds are changed but not actually
//changed to different values then the method still sets the build pending
//status to true. This is strictly incorrect, but I choose to ignore this
//as it's a logical error anyway...
template<typename T> class TGrid3D
{
public:
	//constructor
	TGrid3D() : m_bounds(TAABB<T>::ZERO),m_resX(0),m_resY(0),m_resZ(0),m_voxels(0),m_build(false) {}
	TGrid3D(const TAABB<T> &bounds,unsigned int resX,unsigned int resY,unsigned int resZ) : m_bounds(bounds),m_resX(resX),m_resY(resY),m_resZ(resZ),m_voxels(0),m_build(true) {}
	//destructor
	~TGrid3D() { if(m_voxels) delete[] m_voxels; }
	//operators
	const TVoxel<T> operator()(unsigned int i,unsigned int j,unsigned int k) const;
	//methods
	void setBounds(const TAABB<T> &bounds);
	void setResolution(unsigned int resX,unsigned int resY,unsigned int resZ);
	const bool buildPending() const { return m_build; }
	const unsigned int numVoxels() const { return m_resX*m_resY*m_resZ; }
	const TVoxel<T> *getVoxels() const { return m_voxels; }
	const std::vector<TVoxel<T> *> getNeighborsTo(unsigned int i,unsigned int j,unsigned int k) const;
	const std::vector<TVoxel<T> *> getNeighborsTo(unsigned int id) { return m_neighbors[id]; }
	void cacheNeighbors();
	const bool build();
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//methods
	unsigned int index(unsigned int i,unsigned int j,unsigned int k) const;
	//variables
	TAABB<T> m_bounds;
	unsigned int m_resX,m_resY,m_resZ;
	TVoxel<T> *m_voxels;
	std::map<unsigned int,std::vector<TVoxel<T> *>> m_neighbors;
	bool m_build;	//must the grid be built/rebuilt?
protected:
	//
}; //class TGrid3D

template<typename T> const TVoxel<T> TGrid3D<T>::operator()(unsigned int i,unsigned int j,unsigned int k) const
{
	return m_voxels[index(i,j,k)];
}

template<typename T> void TGrid3D<T>::setBounds(const TAABB<T> &bounds)
{
	m_bounds = bounds;
	m_build = true;
}

template<typename T> void TGrid3D<T>::setResolution(unsigned int resX,unsigned int resY,unsigned int resZ)
{
	resX != 0 ? m_resX = resX : throw Exception(Exception::LOGIC_ERROR,"no dimension of the 3D grid can be zero");
	resY != 0 ? m_resY = resY : throw Exception(Exception::LOGIC_ERROR,"no dimension of the 3D grid can be zero");
	resZ != 0 ? m_resZ = resZ : throw Exception(Exception::LOGIC_ERROR,"no dimension of the 3D grid can be zero");
	m_build = true;
}

template<typename T> const std::vector<TVoxel<T> *> TGrid3D<T>::getNeighborsTo(unsigned int i,unsigned int j,unsigned int k) const
{
	//check i,j,k within legal range
	if(i >= m_resX || j >= m_resY || k >= m_resZ) throw Exception(Exception::OUT_OF_RANGE);
	std::vector<TVoxel<T> *> voxels;
	//get neighbors on x-axis
	if((int)i-1 >= 0) voxels.push_back(&m_voxels[index(i-1,j,k)]);
	if(i+1 < m_resX) voxels.push_back(&m_voxels[index(i+1,j,k)]);
	//get neighbors on y-axis
	if((int)j-1 >= 0) voxels.push_back(&m_voxels[index(i,j-1,k)]);
	if(j+1 < m_resY) voxels.push_back(&m_voxels[index(i,j+1,k)]);
	//get neighbors on z-axis
	if((int)k-1 >= 0) voxels.push_back(&m_voxels[index(i,j,k-1)]);
	if(k+1 < m_resZ) voxels.push_back(&m_voxels[index(i,j,k+1)]);
	//get neighbors on diagonals
	//xy-plane
	if((int)i-1 >= 0 && (int)j-1 >= 0) voxels.push_back(&m_voxels[index(i-1,j-1,k)]);
	if(i+1 < m_resX && (int)j-1 >= 0) voxels.push_back(&m_voxels[index(i+1,j-1,k)]);
	if((int)i-1 >= 0 && j+1 < m_resY) voxels.push_back(&m_voxels[index(i-1,j+1,k)]);
	if(i+1 < m_resX && j+1 < m_resY) voxels.push_back(&m_voxels[index(i+1,j+1,k)]);
	//xz-plane
	if((int)i-1 >= 0 && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i-1,j,k-1)]);
	if(i+1 < m_resX && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i+1,j,k-1)]);
	if((int)i-1 >= 0 && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i-1,j,k+1)]);
	if(i+1 < m_resX && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i+1,j,k+1)]);
	//yz-plane
	if((int)j-1 >= 0 && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i,j-1,k-1)]);
	if(j+1 < m_resY && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i,j+1,k-1)]);
	if((int)j-1 >= 0 && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i,j-1,k+1)]);
	if(j+1 < m_resY && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i,j+1,k+1)]);
	//bottom corners
	if((int)i-1 >= 0 && (int)j-1 >= 0 && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i-1,j-1,k-1)]);
	if(i+1 < m_resX && (int)j-1 >= 0 && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i+1,j-1,k-1)]);
	if(i+1 < m_resX && (int)j-1 >= 0 && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i+1,j-1,k+1)]);
	if((int)i-1 >= 0 && (int)j-1 >= 0 && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i-1,j-1,k+1)]);
	//top corners
	if((int)i-1 >= 0 && j+1 < m_resY && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i-1,j+1,k-1)]);
	if(i+1 < m_resX && j+1 < m_resY && (int)k-1 >= 0) voxels.push_back(&m_voxels[index(i+1,j+1,k-1)]);
	if(i+1 < m_resX && j+1 < m_resY && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i+1,j+1,k+1)]);
	if((int)i-1 >= 0 && j+1 < m_resY && k+1 < m_resZ) voxels.push_back(&m_voxels[index(i-1,j+1,k+1)]);
	return voxels;
}

template<typename T> void TGrid3D<T>::cacheNeighbors()
{
	m_neighbors.clear(); //don't worry about the heap, only references

	unsigned int id = 0;
	for(int i=0; i<m_resX; i++)
	{
		for(int j=0; j<m_resY; j++)
		{
			for(int k=0; k<m_resZ; k++)
			{
				m_neighbors[id] = getNeighborsTo(i,j,k);
				id++;
			}
		}
	}
}

template<typename T> const bool TGrid3D<T>::build()
{
	if(!m_build) return false; //no need to build

	unsigned int numVoxels = m_resX*m_resY*m_resZ;
	if(numVoxels == 0) return false;

	m_voxels = new TVoxel<T>[numVoxels];	//the voxels

	T xSize = TMath<T>::abs(m_bounds.max.x-m_bounds.min.x)/(T)m_resX;
	T ySize = TMath<T>::abs(m_bounds.max.y-m_bounds.min.y)/(T)m_resY;
	T zSize = TMath<T>::abs(m_bounds.max.z-m_bounds.min.z)/(T)m_resZ;
	//prevent rounding errors
	xSize += TMath<T>::EPSILON;
	ySize += TMath<T>::EPSILON;
	zSize += TMath<T>::EPSILON;
	TVector3<T> p;
	TVoxel<T> *voxelPtr = &m_voxels[0];
	unsigned int id = 0;
	for(T x=m_bounds.min.x; x<m_bounds.max.x; x+=xSize)
	{
		for(T y=m_bounds.min.y; y<m_bounds.max.y; y+=ySize)
		{
			for(T z=m_bounds.min.z; z<m_bounds.max.z; z+=zSize)
			{
				p.x = x;
				p.y = y;
				p.z = z;
				//set voxel id
				voxelPtr->id = id;
				//generate voxel vertices
				voxelPtr->vertices[0] = Vector3(p.x,p.y,p.z); //the current vertex (x,y,z)
				voxelPtr->vertices[1] = Vector3(p.x+xSize,p.y,p.z);
				voxelPtr->vertices[2] = Vector3(p.x+xSize,p.y,p.z+zSize);
				voxelPtr->vertices[3] = Vector3(p.x,p.y,p.z+zSize);
				voxelPtr->vertices[4] = Vector3(p.x,p.y+ySize,p.z);
				voxelPtr->vertices[5] = Vector3(p.x+xSize,p.y+ySize,p.z);
				voxelPtr->vertices[6] = Vector3(p.x+xSize,p.y+ySize,p.z+zSize);
				voxelPtr->vertices[7] = Vector3(p.x,p.y+ySize,p.z+zSize);
				voxelPtr++;
				id++;
			}
		}
	}

	cacheNeighbors();

	return true;
}

template<typename T> unsigned int TGrid3D<T>::index(unsigned int i,unsigned int j,unsigned int k) const
{
	if(i >= m_resX || j >= m_resY || k >= m_resZ) throw Exception(Exception::OUT_OF_RANGE);
	return i*m_resY*m_resZ + j*m_resZ + k;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_GRID_3D_HPP__
