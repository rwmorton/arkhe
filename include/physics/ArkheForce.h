#ifndef __ARKHE_FORCE_H__
#define __ARKHE_FORCE_H__

//arkhe local includes (physics)
#include "ArkheParticle.h"

//std includes
#include <vector>

namespace arkhe
{

namespace physics
{

//Does not take ownership of particles!
class Force
{
public:
	//destructor
	virtual ~Force() {}
	//methods
	virtual void update() = 0;
	void addParticle(Particle *p)
	{
		particles.push_back(p);
		p->addForce(this);
	}
private:
	//
protected:
	//variables
	std::vector<Particle *> particles;	//all particles this force affects
	std::vector<Particle *>::iterator particle_iterator;
};

class ForceField : public Force
{
public:
	//methods
	void update()
	{
		particle_iterator = particles.begin();
		while(particle_iterator != particles.end())
		{
			//temp force field - add callback later... just a test for now...
			Real x = (*particle_iterator)->getPosition().x;
			Real y = (*particle_iterator)->getPosition().y;
			Real X = -y*y + 11 * Math::sin(x*y);
			Real Y = x*y*y*y - 9 * Math::cos(x);
			Vector3 f = Vector3(X,Y,0);
			(*particle_iterator)->addForce(f);
			*particle_iterator++;
		}
	}
private:
	//
protected:
	//
};

class SpringForce : public Force {};

	//etc.

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_FORCE_H__
