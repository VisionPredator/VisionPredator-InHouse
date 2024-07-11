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
	for (int i = 0; i < nbPairs; i++)
	{
		if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "EnterContact" << std::endl;
		}

		/// END_COLLISION 충돌 이벤트 실행
		else if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "ExitContact" << std::endl;
		}

		/// ON_COLLSION 충돌 이벤트 실행
		else if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "OnContact" << std::endl;
		}
	}

} 

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	std::cout << "onTrigger" << std::endl;

}

void CollisionCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
