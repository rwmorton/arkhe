#ifndef __ARKHE_RIGID_BODY_H__
#define __ARKHE_RIGID_BODY_H__

//arkhe foundation includes
#include <foundation/ArkheFoundation.h>

namespace arkhe
{

namespace physics
{

class RigidBody
{
public:
	//methods
	void calculateDerivedData();
	void setInertiaTensor(const Matrix33 &inertiaTensor);
	//const Vector3 getPointInLocalSpace(const Vector3 &p); //transformMatrix.inverse()(p,Matrix44::transformAs::POINT);
	//const Vector3 getPointInWorldSpace(const Vector3 &p); //transformMatrix(p,Matrix44::transformAs::POINT);
	void addForce(const Vector3 &f) { forceAccum += f; } //add force to center of mass
	void addTorque(const Vector3 &torque) { torqueAccum += torque; }
	void addForceAtBodyPoint(const Vector3 &f,const Vector3 &p);
	void addForceAtPoint(const Vector3 &f,const Vector3 &p);
	void clearAccumulators();
	void integrate(Real t);
	//variables
	Real inverseMass;
	Vector3 position; //position in world space
	Quaternion orientation; //orientation in world space
	Vector3 velocity; //linear velocity in world space
	Vector3 rotation; //angular velocity in world space
	Matrix44 transformMatrix;
	//Inverse inertia tensor given in local space
	Matrix33 inverseInertiaTensor;
	//Inverse inertia tensor given in world space
	Matrix33 inverseInertiaTensorWorld;
	//forces, torques etc.
	Vector3 forceAccum;
	Vector3 torqueAccum;
	Vector3 acceleration;
	Vector3 lastFrameAcceleration;
	//damping
	Real linearDamping;
	Real angularDamping;
private:
	//
protected:
	//methods
	void initDefaults();
}; //class RigidBody

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_RIGID_BODY_H__
