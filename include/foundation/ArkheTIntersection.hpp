#ifndef __ARKHE_T_INTERSECTION_HPP__
#define __ARKHE_T_INTERSECTION_HPP__

//arkhe local includes (foundation)
#include "ArkheTRay.hpp"
#include "ArkheTLine.hpp"
#include "ArkheTLineSegment.hpp"
#include "ArkheTPlane.hpp"
#include "ArkheTAABB.hpp"
#include "ArkheTTriangle.hpp"
#include "ArkheTTetrahedron.hpp"

namespace arkhe
{

namespace foundation
{

template <typename T> class TIntersection
{
public:
	//data structures
	struct BarycentricCoordinates { T u,v,w; }; //struct BarycentricCoordinates
	//static methods
	static const bool Ray_Plane(const TRay<T> &ray,const TPlane<T> &plane,T &tmin);
	static const bool Ray_Sphere(const TRay<T> &ray,const TSphere<T> &sphere,T &tmin);
	static const bool Ray_AABB(const TRay<T> &ray,const TAABB<T> &box,T &tmin);
	static const bool Ray_Triangle(const TRay<T> &ray,const TTriangle<T> &triangle,T &tmin,BarycentricCoordinates *barycentricCoordinates=0);
	static const bool Triangle_Triangle(const TTriangle<T> &A,const TTriangle<T> &B); //TODO
	static const bool Tetrahedron_Tetrahedron(const TTetrahedron<T> &A,const TTetrahedron<T> &B); //TODO
	static const bool AABB_AABB(const TAABB<T> &A,const TAABB<T> &B);
	static const bool Sphere_Sphere(const TSphere<T> &A,const TSphere<T> &B);
	static const bool Sphere_Plane(const TSphere<T> &s,const TPlane<T> &p);
	static const bool Sphere_AABB(const TSphere<T> &s,const TAABB<T> &b,TVector3<T> *closestPoint=0);
	static const bool Sphere_Triangle(const TSphere<T> &s,const TTriangle<T> &t,TVector3<T> *closestPoint=0);
private:
	//
protected:
	//
}; //class TIntersection

//Test ray with plane
template<typename T> const bool TIntersection<T>::Ray_Plane(const TRay<T> &ray,const TPlane<T> &plane,T &tmin)
{
	T denom = plane.normal.dot(ray.direction);
	if(TMath<T>::isZero(denom)) throw Exception(Exception::DIVISION_BY_ZERO);

	tmin = plane.distance - plane.normal.dot(ray.origin);
	tmin /= denom;

	if(tmin <= TMath<T>::RAY_EPSILON) return false;

	return true;
}

//Test ray with sphere
//Quadratic is in form t^2 + 2bt + c = 0 and it's solutions
//are t = -b +- (b^2 - c)^1/2
template<typename T> const bool TIntersection<T>::Ray_Sphere(const TRay<T> &ray,const TSphere<T> &sphere,T &tmin)
{
	TVector3<T> m = ray.origin - sphere.center;
	T b = m.dot(ray.direction);
	T c = m.dot(m) - sphere.radius*sphere.radius;
	if(c > 0 && b > 0) return false;
	T discrim = b*b - c;
	if(discrim < 0) return false;
	tmin = -b - TMath<T>::sqrt(discrim);
	if(tmin < 0) tmin = 0;
	return true;
}

//Test ray with AABB
template<typename T> const bool TIntersection<T>::Ray_AABB(const TRay<T> &ray,const TAABB<T> &box,T &tmin)
{
	tmin = 0;
	T tmax = TMath<T>::MAX;
	for(int i=0; i<3; i++)
	{
		if(TMath<T>::abs(ray.direction[i]) < TMath<T>::RAY_EPSILON)
		{
			if(ray.origin[i] < box.min[i] || ray.origin[i] > box.max[i]) return false;
		}
		else
		{
			T ood = TMath<T>::reciprocal(ray.direction[i]);
			T t1 = (box.min[i] - ray.origin[i]) * ood;
			T t2 = (box.max[i] - ray.origin[i]) * ood;
			if(t1 > t2) Util::swap<T>(t1,t2);
			tmin = TMath<T>::max(tmin,t1);
			tmax = TMath<T>::min(tmax,t2);
			if(tmin > tmax) return false;
		}
	}
	return true;
}

//Test ray with triangle
template<typename T> const bool TIntersection<T>::Ray_Triangle(const TRay<T> &ray,const TTriangle<T> &triangle,T &tmin,BarycentricCoordinates *barycentricCoordinates)
{
	TVector3<T> ab = triangle.p1 - triangle.p0;
	TVector3<T> ac = triangle.p2 - triangle.p0;

	TVector3<T> n = ab.cross(ac);
	T d = -ray.direction.dot(n);

	if(d <= 0) return false;

	TVector3<T> ap = ray.origin - triangle.p0;
	tmin = ap.dot(n);

	if(tmin < 0) return false;

	TVector3<T> e = -ray.direction.cross(ap);
	T v = ac.dot(e);
	if(v < 0 || v > d) return false;

	T w = -ab.dot(e);
	if(w < 0 || v+w > d) return false;

	T recipD = TMath<T>::reciprocal(d);
	tmin *= recipD;

	if(barycentricCoordinates)
	{
		barycentricCoordinates->v = v * recipD;
		barycentricCoordinates->w = w * recipD;
		barycentricCoordinates->u = 1 - v - w;
	}

	return true;
}

//Test triangle with triangle
template<typename T> const bool TIntersection<T>::Triangle_Triangle(const TTriangle<T> &A,const TTriangle<T> &B)
{
	throw Exception(Exception::NO_IMPLEMENTATION);
	return false;
}

//Test tetrahedron with tetrahedron
template<typename T> const bool TIntersection<T>::Tetrahedron_Tetrahedron(const TTetrahedron<T> &A,const TTetrahedron<T> &B)
{
	throw Exception(Exception::NO_IMPLEMENTATION);
	return false;
}

//Test AABB with AABB
template<typename T> const bool TIntersection<T>::AABB_AABB(const TAABB<T> &A,const TAABB<T> &B)
{
	if(A.min.x > B.max.x || B.min.x > A.max.x)
		return false;
	if(A.min.y > B.max.y || B.min.y > A.max.y)
		return false;
	if(A.min.z > B.max.z || B.min.z > A.max.z)
		return false;
	return true;
}

//Test sphere with sphere
template<typename T> const bool TIntersection<T>::Sphere_Sphere(const TSphere<T> &A,const TSphere<T> &B)
{
	T distSq = (A.center-B.center).lengthSq();
	T radSum = A.radius + B.radius;
	if(distSq <= radSum*radSum) return true;
	return false;
}

//Test sphere with plane
template<typename T> const bool TIntersection<T>::Sphere_Plane(const TSphere<T> &s,const TPlane<T> &p)
{
	T d = s.center.dot(p.normal) - p.distance;
	return TMath<T>::abs(d) <= s.radius;
}

//Test sphere with AABB
template<typename T> const bool TIntersection<T>::Sphere_AABB(const TSphere<T> &s,const TAABB<T> &b,TVector3<T> *closestPoint)
{
	if(closestPoint) *closestPoint = b.closestPoint(s.center);

	TVector3<T> v = b.closestPoint(s.center) - s.center;
	return v.lengthSq() <= s.radius*s.radius;
}

//Test sphere with triangle
template<typename T> const bool TIntersection<T>::Sphere_Triangle(const TSphere<T> &s,const TTriangle<T> &t,TVector3<T> *closestPoint)
{
	if(closestPoint) *closestPoint = t.closestPoint(s.center);

	TVector3<T> v = t.closestPoint(s.center) - s.center;
	return v.lengthSq() <= s.radius*s.radius;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_INTERSECTION_HPP__
