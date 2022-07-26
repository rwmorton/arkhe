#ifndef __CAMERA_H__
#define __CAMERA_H__

//flux includes
#include "Base.h"
#include "Film.h"

class Camera
{
public:
	//constructor
	Camera(const Matrix &c2w,Film *film,double near,double far) : m_cameraToWorld(c2w),m_film(film),m_nearClip(near),m_farClip(far) { m_worldToCamera = c2w.inverse(); }
	//destructor
	virtual ~Camera() {}
	//methods
	virtual const Ray genRay(int x,int y) const = 0;
private:
	//
protected:
	//methods
	void initProjection(const Matrix &c2w,const Matrix &proj,Film *film,double near,double far);
	//data
	Matrix m_cameraToWorld,m_worldToCamera;
	Matrix m_cameraToScreen,m_worldToScreen;
	Matrix m_rasterToCamera;
	Matrix m_screenToRaster,m_rasterToScreen;
	double m_nearClip,m_farClip;
	Film *m_film;
}; //class Camera

class PerspectiveCamera : public Camera
{
public:
	//constructor
	PerspectiveCamera(const Matrix &c2w,Film *film,double fov,double near,double far);
	//methods
	const Ray genRay(int x,int y) const;
private:
	//
protected:
	//
}; //class PerspectiveCamera

#endif //__CAMERA_H__
