#include "pch.h"
#include <iostream>
#include "CollisionCallback.h"
#include "../VPEngine/EventManager.h"

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
			auto entitypair = SortEntityPair(pairs[i]);
			EventManager::GetInstance().ImmediateEvent("OnAddEnter_Collision", entitypair);
		}

		/// END_COLLISION 충돌 이벤트 실행
		else if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "ExitContact" << std::endl;
			auto entitypair = SortEntityPair(pairs[i]);

			EventManager::GetInstance().ImmediateEvent("OnMoveContactToExit_Collision", entitypair);

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
	for (int i = 0; i < count; i++)
	{
		/// ENTER_OVERLAP 충돌 이벤트 실행
		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			auto entitypair = SortEntityPair(pairs[i]);
			EventManager::GetInstance().ImmediateEvent("OnAddEnter_Trigger", entitypair);
		}

		/// END_OVERLAP 충돌 이벤트 실행
		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			auto entitypair = SortEntityPair(pairs[i]);
			EventManager::GetInstance().ImmediateEvent("OnMoveContactToExit_Trigger", entitypair);
		}
	}

}

void CollisionCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
std::pair<uint32_t, uint32_t> CollisionCallback::SortEntityPair(const PxContactPair pairs)
{
	auto entityID1 = static_cast<uint32_t*>(pairs.shapes[0]->userData);
	auto entityID2 = static_cast<uint32_t*>(pairs.shapes[1]->userData);

	std::pair<uint32_t, uint32_t> entitypair;
	if (*entityID1 > *entityID2)
	{
		entitypair = { *entityID2,*entityID1 };
	}
	else
	{
		entitypair = { *entityID1,  *entityID2 };
	}
	return entitypair;
}

std::pair<uint32_t, uint32_t> CollisionCallback::SortEntityPair(const PxTriggerPair& pairs)
{
	auto entityID1 = static_cast<uint32_t*>(pairs.triggerShape->userData);
	auto entityID2 = static_cast<uint32_t*>(pairs.otherShape->userData);

	std::pair<uint32_t, uint32_t> entitypair;
	if (*entityID1 > *entityID2)
	{
		entitypair = { *entityID2, *entityID1 };
	}
	else
	{
		entitypair = { *entityID1, *entityID2 };
	}
	return entitypair;
}