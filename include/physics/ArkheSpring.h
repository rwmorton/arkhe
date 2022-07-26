#ifndef __ARKHE_SPRING_H__
#define __ARKHE_SPRING_H__

//arkhe local includes (physics)
#include "ArkheParticle.h"

namespace arkhe
{

namespace physics
{

//Does not take ownership of spring pointers.
class Spring
{
public:
	//constructor
	Spring() {} //uninitialized!
	//methods
	void setEndpoints(Particle *a,Particle *b);
	void setK(Real k) { K = k; }
	void setL(Real l) { L = l; }
	void update();
	const Vector3 getAPosition() const;
	const Vector3 getBPosition() const;
private:
	//data
	Real L; //spring rest length
	Real K; //spring constant
	Particle *A,*B; //end points of the spring
protected:
	//
}; //class Spring

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_SPRING_H__
