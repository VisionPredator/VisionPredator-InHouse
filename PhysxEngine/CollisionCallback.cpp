#include "pch.h"
#include <iostream>
#include "CollisionCallback.h"

using namespace physx;
CollisionCallback::CollisionCallback()
{
}
CollisionCallback::~CollisionCallback()
{
}
void CollisionCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CollisionCallback::onWake(PxActor** actors, PxU32 count)
{
	std::cout << "onWake" << std::endl;

}

void CollisionCallback::onSleep(PxActor** actors, PxU32 count)
{
	std::cout << "onSleep" << std::endl;

}

void CollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	//std::cout << "onContact" << std::endl;

}

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	std::cout << "onTrigger" << std::endl;

}

void CollisionCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
