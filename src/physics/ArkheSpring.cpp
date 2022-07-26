//local includes
#include "ArkheSpring.h"

namespace arkhe
{

namespace physics
{

void Spring::setEndpoints(Particle *a,Particle *b)
{
	if(!a || !b) throw Exception(Exception::NULL_POINTER);

	A = a;
	B = b;
	//set rest length from initial distance of two particles
	L = (A->getPosition() - B->getPosition()).length();
}

void Spring::update()
{
	if(!A || !B) throw Exception(Exception::NULL_POINTER);

	Vector3 d = A->getPosition() - B->getPosition();
	Real deltaL = d.length() - L;
	d.normalize();

	//set force on A and B
	A->addForce(d * -K * deltaL);
	B->addForce(-d * -K * deltaL);
}

const Vector3 Spring::getAPosition() const
{
	if(!A) throw Exception(Exception::NULL_POINTER);
	return A->getPosition();
}

const Vector3 Spring::getBPosition() const
{
	if(!B) throw Exception(Exception::NULL_POINTER);
	return B->getPosition();
}

} //namespace physics

} //namespace arkhe
