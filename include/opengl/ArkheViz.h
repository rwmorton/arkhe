#ifndef __ARKHE_VIZ_H__
#define __ARKHE_VIZ_H__

//arkhe includes
#include <foundation/ArkheFoundation.h>
#include <physics/ArkhePhysics.h>
using namespace arkhe::physics;

namespace arkhe
{

namespace opengl
{

class Viz
{
public:
	//static methods
	static void render(const Vector3 &v,const Vector3 &pos=Vector3::ZERO);
	static void render2DVectorField(int extentsFromOrigin,Real scale,Vector3 (*vectorFunction)(Real,Real),bool realScale=false);
	static void render3DVectorField(int extentsFromOrigin,Real scale,Vector3 (*vectorFunction)(Real,Real,Real),bool realScale=false);
	static void render3DVectorField(const Vector3 &min,const Vector3 &max,Real scale,Vector3 (*vectorFunction)(Real,Real,Real),bool realScale=false);
	static void renderSphere(const Vector3 &p,Real radius);
	static void renderCube(const Vector3 *vertices);
	static void render(const Ray &ray);
	static void render(const Particle &p,Real r=1,Real g=0,Real b=0);
	static void render(const Spring &s,Real r=0,Real g=1,Real b=0);
	static void render(const MA_Cube &c,bool full=true);
	static void render(const MA_String &s,bool full=true);
	static void render(const MA_Sphere &s,bool full=true);
	//FEM
	static void render(const Assemblage &a);
private:
	//non-copyable
	Viz(const Viz &);
	Viz &operator=(const Viz &);
protected:
	//
}; //class Viz

} //namespace opengl

} //namespace arkhe

#endif //__ARKHE_VIZ_H__
