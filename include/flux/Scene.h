#ifndef __SCENE_H__
#define __SCENE_H__

// Contains camera, all shapes, all lights etc.

//flux includes
#include "Camera.h"
#include "Shape.h"

class Scene
{
public:
	//
private:
	//data
	Camera *m_camera;
	Shape *m_shapes;
protected:
	//
}; //class Scene

#endif //__SCENE_H__
