#ifndef __ARKHE_FOUNDATION_H__
#define __ARKHE_FOUNDATION_H__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheException.h"
#include "ArkheUtil.h"
#include "ArkheTMath.hpp"
#include "ArkheTVector3.hpp"
#include "ArkheTQuaternion.hpp"
#include "ArkheTRay.hpp"
#include "ArkheTLine.hpp"
#include "ArkheTLineReuse.hpp"
#include "ArkheTLineSegment.hpp"
#include "ArkheTLineSegmentReuse.hpp"
#include "ArkheTMatrix33.hpp"
#include "ArkheTMatrix44.hpp"
#include "ArkheTMatrixNN.hpp"
#include "ArkheTPlane.hpp"
#include "ArkheTAABB.hpp"
#include "ArkheTAABBReuse.hpp"
#include "ArkheTSphere.hpp"
#include "ArkheTTriangle.hpp"
#include "ArkheTTriangleReuse.hpp"
#include "ArkheTTetrahedron.hpp"
#include "ArkheTIntersection.hpp"
#include "ArkheTVoxel.hpp"
#include "ArkheTGrid3D.hpp"
#include "ArkheTMesh.hpp"
#include "ArkheTMarchingCubesMesh.hpp"
#include "ArkheTImplicit.hpp"
#include "ArkheTMarchingCubes.hpp"
#include "ArkheTMarchingCubesGrid.hpp"
#include "ArkheTMetaballs.hpp"

//THIS MUST BE INCLUDED LAST!
//HAS SOMETHING TO DO WITH THE
//ORDER OF GLUT DEPENDENCIES
#include "ArkheTimer.h"

namespace arkhe
{

namespace foundation
{

//typedefs
typedef TMath<Real> Math;
typedef TVector3<Real> Vector3;
typedef TQuaternion<Real> Quaternion;
typedef TRay<Real> Ray;
typedef TLine<Real> Line;
typedef TLineSegment<Real> LineSegment;
typedef TMatrix33<Real> Matrix33;
typedef TMatrix44<Real> Matrix44;
typedef TPlane<Real> Plane;
typedef TAABB<Real> AABB;
typedef TSphere<Real> Sphere;
typedef TTriangle<Real> Triangle;
typedef TTetrahedron<Real> Tetrahedron;
typedef TIntersection<Real> Intersection;
typedef TVoxel<Real> Voxel;
typedef TGrid3D<Real> Grid3D;
typedef TMesh<Real> Mesh;
typedef TTriangleMesh<Real> TriangleMesh;
typedef TMarchingCubesMesh<Real> MarchingCubesMesh;
typedef TMarchingCubes<Real> MarchingCubes;
typedef TMarchingCubesGrid<Real> MarchingCubesGrid;
typedef TMetaballs<Real> Metaballs;

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_FOUNDATION_H__
