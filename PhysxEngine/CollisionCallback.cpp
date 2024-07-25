#include "pch.h"
#include <iostream>
#include "CollisionCallback.h"
#include "VPPhysicsStructs.h"
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

			CollisionDetection(pairs[i], "OnAddEnterData");
		}

		/// END_COLLISION 충돌 이벤트 실행
		else if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "ExitContact" << std::endl;
			CollisionDetection(pairs[i], "OnAddExitData");

		}

		/// ON_COLLSION 충돌 이벤트 실행
		else if (pairs[i].events & (physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			std::cout << "OnContact" << std::endl;
			CollisionDetection(pairs[i], "OnAddContactData");

		}
	}

} 

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	std::cout << "onTrigger" << std::endl;

	for (int i = 0; i < count; i++)
	{
		/// ENTER_OVERLAP 충돌 이벤트 실행
		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//(&pairs[i], ECollisionEventType::ENTER_OVERLAP);
		}

		/// END_OVERLAP 충돌 이벤트 실행
		if (pairs[i].status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//SettingTriggerData(&pairs[i], ECollisionEventType::END_OVERLAP);
		}
	}

}

void CollisionCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

void CollisionCallback::CollisionDetection(const PxContactPair pairs, std::string event)
{
	VPPhysics::CollisionData* entity1 = static_cast<VPPhysics::CollisionData*>(pairs.shapes[0]->userData);
	VPPhysics::CollisionData* entity2 = static_cast<VPPhysics::CollisionData*>(pairs.shapes[1]->userData);

	std::pair<uint32_t, uint32_t> entitypair;
	if (entity1->myId > entity2->myId)
	{
		entitypair = { entity2->myId, entity1->myId };
	}
	else
	{
		entitypair = { entity1->myId, entity2->myId };
	}

	EventManager::GetInstance().ScheduleEvent(event, entitypair);
}
