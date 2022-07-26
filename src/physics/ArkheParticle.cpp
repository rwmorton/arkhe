//local includes
#include "ArkheParticle.h"
#include "ArkheForce.h"

namespace arkhe
{

namespace physics
{

const Vector3 Particle::getAcceleration() const
{
	return globalAccel + (forceAccum / m);
}

void Particle::integrate(Real t)
{
	Vector3 a = globalAccel + (forceAccum / m);
	v += a * t;
	p += v * t + a * Math::ONE_OVER_TWO * t * t;
	forceAccum = Vector3::ZERO; //reset external forces
}

//first order euler integration
void Particle::integrateEuler1(Real t)
{
	updateForces();
	Vector3 a = globalAccel + (forceAccum / m);
	v += a * t;
	p += v * t;
	forceAccum = Vector3::ZERO; //reset external forces
}

//second order euler integration
void Particle::integrateEuler2(Real t)
{
	updateForces();
	Vector3 a = globalAccel + (forceAccum / m);
	v += a * t;
	p += v * t + a * Math::ONE_OVER_TWO * t * t;
	forceAccum = Vector3::ZERO; //reset external forces
}

//midpoint integration
void Particle::integrateMidpoint(Real t)
{
	Vector3 pOld = p;
	integrateEuler1(t);
	Vector3 deltaP = p - pOld;
	deltaP *= Math::ONE_OVER_TWO;
	p = pOld + deltaP;
	integrateEuler1(t * Math::ONE_OVER_TWO);
}

void Particle::integrateVerlet(Real t)
{
	verletStack.push_back(p);

	updateForces();

	Vector3 a = globalAccel + (forceAccum / m);

	p *= Math::TWO;
	p -= verletStack.positions[1];
	p += a * t * t;

	forceAccum = Vector3::ZERO;
}

void Particle::updateForces()
{
	force_iterator = forces.begin();
	while(force_iterator != forces.end())
	{
		(*force_iterator)->update();
		force_iterator++;
	}
}

} //namespace physics

} //namespaec arkhe
