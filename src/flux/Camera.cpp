//local includes
#include "Camera.h"

void Camera::initProjection(const Matrix &c2w,const Matrix &proj,Film *film,double near,double far)
{
	m_cameraToScreen = proj;
	m_worldToScreen = m_cameraToScreen * m_worldToCamera;

	double aspect = film->getAspect();
	double screen[4]; //[0,3] -> left, right, bottom, top
	if(aspect > 1.0f)
	{
		screen[0] = -aspect;
		screen[1] = aspect;
		screen[2] = -1.0;
		screen[3] = 1.0;
	}
	else
	{
		screen[0] = -1.0;
		screen[1] = 1.0;
		screen[2] = -Math::reciprocal(aspect);
		screen[3] = Math::reciprocal(aspect);
	}
	Matrix T = Matrix::translate(-screen[0],-screen[2],0.0);
	Matrix NDC_S = Matrix::scale(Math::reciprocal(screen[1]-screen[0]),Math::reciprocal(screen[3]-screen[2]),1.0); //screen to NDC scale
	Matrix RS = Matrix::scale((double)film->getWidth(),(double)film->getHeight(),1.0); //raster scale
	Matrix s2r = RS * NDC_S * T;
	//hack?
	m_screenToRaster = s2r;
	m_rasterToScreen = m_screenToRaster.inverse();
	m_rasterToCamera = m_cameraToScreen.inverse() * m_rasterToScreen;
}

PerspectiveCamera::PerspectiveCamera(const Matrix &c2w,Film *film,double fov,double n,double f) : Camera(c2w,film,n,f)
{
	initProjection(c2w,Matrix::perspective(fov,film->getAspect(),n,f),film,n,f);
}

const Ray PerspectiveCamera::genRay(int x,int y) const
{
	Vector rasterP(x,y,0);
	Vector cameraP = m_rasterToCamera(rasterP,Matrix::TransformAs::POINT);
	Ray ray(cameraP,cameraP);
	ray.direction.normalize();
	ray.min = 0.0;
	ray.max = (m_farClip - m_nearClip) / ray.direction.z;
	ray = m_cameraToWorld(ray);
	return ray;
}
