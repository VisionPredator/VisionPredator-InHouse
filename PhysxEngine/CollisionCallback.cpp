#include "pch.h"
#include <iostream>
#include "CollisionCallback.h"
#include "EventManager.h"
#include "VPPhysicsStructs.h"

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
	for (PxU32 i = 0; i < nbPairs; i++)
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
			//std::cout << "OnContact" << std::endl;
		}
	}

} 

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
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
	auto userdata1 = static_cast<VPPhysics::USERDATA*>(pairs.shapes[0]->userData);
	auto userdata2 = static_cast<VPPhysics::USERDATA*>(pairs.shapes[1]->userData);

	std::pair<uint32_t, uint32_t> entitypair;
	if (userdata1->entityID > userdata2->entityID)
	{
		entitypair = { userdata2->entityID,userdata1->entityID };
	}
	else
	{
		entitypair = { userdata1->entityID,  userdata2->entityID };
	}
	return entitypair;
}

std::pair<uint32_t, uint32_t> CollisionCallback::SortEntityPair(const PxTriggerPair& pairs)
{
	auto userdata1 = static_cast<VPPhysics::USERDATA*>(pairs.triggerShape->userData);
	auto userdata2 = static_cast<VPPhysics::USERDATA*>(pairs.otherShape->userData);

	std::pair<uint32_t, uint32_t> entitypair;
	if (userdata1->entityID > userdata2->entityID)
	{
		entitypair = { userdata2->entityID, userdata1->entityID };
	}
	else
	{
		entitypair = { userdata1->entityID, userdata2->entityID };
	}
	return entitypair;
}