#ifndef __ARKHE_PARTICLE_H__
#define __ARKHE_PARTICLE_H__

//arkhe includes (foundation)
#include <foundation/ArkheFoundation.h>
using namespace arkhe::foundation;

//std includes
#include <vector>

namespace arkhe
{

namespace physics
{

//forward declarations
class Force;

struct PositionStack
{
	PositionStack() { positions[0] = positions[1] = Vector3::ZERO; }
	void push_back(const Vector3 &p)
	{
		positions[1] = positions[0];
		positions[0] = p;
	}
	Vector3 positions[2];
};

//Does not take ownership of forces!
class Particle
{
public:
	//constructors
	Particle() : m(1.0),p(0,0,0),v(0,0,0),globalAccel(0,0,0),forceAccum(0,0,0),infiniteMass(false) {}
	Particle(Real mass,const Vector3 &pos,const Vector3 &vel) : m(mass),infiniteMass(false),p(pos),v(vel),globalAccel(0,0,0),forceAccum(0,0,0) {}
	//methods
	void integrate(Real t);
	void integrateEuler1(Real t);
	void integrateEuler2(Real t);
	void integrateMidpoint(Real t);
	void integrateVerlet(Real t);
	const Vector3 getPosition() const { return p; }
	void setPosition(const Vector3 &pos) { p = pos; }
	const Vector3 getVelocity() const { return v; }
	void setVelocity(const Vector3 &vel) { v = vel; }
	const Vector3 getAcceleration() const;
	void setInfiniteMass() { infiniteMass = true; }
	const bool isInfiniteMass() { return infiniteMass; }
	Real getMass() { return m; }
	void setMass(Real mass) { m = mass; }
	void setGlobalAcceleration(const Vector3 &a) { globalAccel = a; }
	void addForce(const Vector3 &f) { forceAccum += f; }
	void addForce(Force *f) { forces.push_back(f); }
	void updateForces();
private:
	//variables
	Vector3 p,v;
	Vector3 globalAccel;
	Vector3 forceAccum;
	Real m;
	bool infiniteMass;
	std::vector<Force *> forces;
	std::vector<Force *>::iterator force_iterator;
	//verlet
	PositionStack verletStack;
protected:
	//
}; //class Particle

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_PARTICLE_H__
