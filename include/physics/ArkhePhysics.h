#ifndef __ARKHE_PHYSICS_H__
#define __ARKHE_PHYSICS_H__

//arkhe local includes (physics)
#include "ArkheParticle.h"
#include "ArkheSpring.h"
#include "ArkheMassAggregates.h"

#include "ArkheRigidBody.h"

#include "ArkheFEM.h"

#include "ArkheForce.h"

static const Vector3 G(0,(Real)-9.8,0);
static const Real INFINITE_MASS = Math::MAX;

#endif //__ARKHE_PHYSICS_H__
