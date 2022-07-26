//arkhe local includes (physics)
#include "ArkheRigidBody.h"

namespace arkhe
{

namespace physics
{

void RigidBody::calculateDerivedData()
{
	orientation.normalize();
	transformMatrix = Matrix44::translate(position) * Matrix44::rotate(orientation);

	//_transformInertiaTensor(inverseInertiaTensorWorld,inverseInertiaTensor,transformMatrix);
	Matrix33 m(transformMatrix); //get the rotation part only
	//TODO
	//OTHER ORDER? CHECK
	inverseInertiaTensorWorld = inverseInertiaTensor * m * inverseInertiaTensor.inverse();
}

void RigidBody::setInertiaTensor(const Matrix33 &inertiaTensor)
{
	inverseInertiaTensor = inertiaTensor.inverse();
}

//
//Add force to a given ponit on the rigid body. The direction of
//the force is given in world coordinates but point of application
//is in body space. Apparently useful for spring forces or other
//forces fixed in body space.
//
void RigidBody::addForceAtBodyPoint(const Vector3 &f,const Vector3 &p)
{
	Vector3 q = transformMatrix(p,Matrix44::TransformAs::POINT);
	addForceAtPoint(f,q);
}

void RigidBody::addForceAtPoint(const Vector3 &f,const Vector3 &p)
{
	Vector3 q = p;
	q -= position;

	forceAccum += f;
	torqueAccum += q.cross(f);
}

void RigidBody::clearAccumulators()
{
	forceAccum = Vector3::ZERO;
	torqueAccum = Vector3::ZERO;
}

void RigidBody::integrate(Real t)
{
	lastFrameAcceleration = acceleration;
	lastFrameAcceleration += forceAccum * inverseMass;

	Vector3 angularAcceleration = inverseInertiaTensorWorld(torqueAccum);

	velocity += lastFrameAcceleration * t;
	rotation += angularAcceleration * t;

	velocity *= Math::pow(linearDamping,t);
	rotation *= Math::pow(angularDamping,t);

	//euler 1
	position += velocity * t;
	orientation += rotation * t;

	//drag again?
	//velocity *= Math::pow(linearDamping,t);
	//rotation *= Math::pow(angularDamping,t);

	calculateDerivedData();

	clearAccumulators();
}

void RigidBody::initDefaults()
{
	inverseMass = 1;
	position = Vector3::ZERO;
	orientation = Quaternion::IDENTITY;
	velocity = Vector3::ZERO;
	rotation = Vector3::ZERO;
	transformMatrix = Matrix44::IDENTITY;
	inverseInertiaTensor = Matrix44::IDENTITY;
	inverseInertiaTensorWorld = Matrix44::IDENTITY;
	forceAccum = Vector3::ZERO;
	torqueAccum = Vector3::ZERO;
	acceleration = Vector3::ZERO;
	lastFrameAcceleration = Vector3::ZERO;
	linearDamping = 1;
	angularDamping = 1;
}

} //namespace physics

} //namespace arkhe
