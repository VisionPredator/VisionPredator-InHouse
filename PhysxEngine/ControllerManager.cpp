#include "pch.h"
#include "ControllerManager.h"
#include "VPPhysicsStructs.h"
#include "CapsuleController.h"
#include "Controller.h"
#include "..\VPEngine\InputManager.h"
ControllerManager::ControllerManager()
{
}

ControllerManager::~ControllerManager()
{
    m_CharectorMap.clear();
    
    PX_RELEASE(m_PxControllerManager);
}

bool ControllerManager::Initialize(physx::PxScene* scene, physx::PxPhysics* physics, CollisionManager* collisionManager)
{
    m_PxPhysics = physics;
    m_Material = m_PxPhysics->createMaterial(1.f, 1.f, 0.f);
    m_Scene = scene;
    m_CollisionManager = collisionManager;
    m_PxControllerManager = PxCreateControllerManager(*scene);
    return true;
}

bool ControllerManager::CreatController()
{
    return true;
}

bool ControllerManager::CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo, VPPhysics::PhysicsInfo physicsinfo)
{
    
    auto capsuleController = std::make_shared<CapsuleController>();
    //CollisionData* collisionData = new CollisionData;
    capsuleController->Initialize(capsuleinfo,m_PxControllerManager, m_Material, physicsinfo);
    m_CharectorMap.insert(std::make_pair(capsuleController->GetEntityID(), capsuleController));
    return true;

}

bool ControllerManager::Update(float deltatime)
{
    if (m_CharectorMap.empty())
        return true;
    for (auto [entityID, controller] : m_CharectorMap)
    {
        if (controller ->GetTypeID() == Reflection::GetTypeID<CapsuleController>())
        {
            CapsuleController* capsuleController = static_cast<CapsuleController*>(controller.get());

            physx::PxControllerCollisionFlags collisionFlags = capsuleController->m_Controller->move(capsuleController->m_Velocity*deltatime , 0.001f, deltatime, *capsuleController->GetFilters());

            if (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
                capsuleController->SetIsFall(false);
            else
                capsuleController->SetIsFall(true);
        }
    }
    return true;

}

uint32_t ControllerManager::RaycastToHitActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
	auto temp = GetController(entityID);
	if (!temp)
		return 0;
	auto tempController = temp->GetPxController();
	auto tempActor = temp->GetPxController()->getActor();
	physx::PxVec3 tempDir = { dir.x,dir.y,dir.z };
	tempDir.normalize();
#pragma region Controller ver.
	{
		PxF32 max = (PxF32)distance;
		const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
		PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
		PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here
		if (tempDir.isZero())
			return 0;
		auto temppose = tempController->getPosition();
		PxVec3 controllerpose = { static_cast<float>(temppose.x), static_cast<float>(temppose.y), static_cast<float>(temppose.z) };
		bool find = m_Scene->raycast(
			controllerpose,
			tempDir,
			max,
			buf);
		if (!find)
			return 0;

		for (PxU32 i = 0; i < buf.nbTouches; i++)
		{

			PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
			PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
			PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴
			auto tempID = GetIDFromActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음
			if (tempID == entityID)
				continue;
			if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
				return tempID;
		}
		return 0;
	}
#pragma endregion
}

uint32_t ControllerManager::RaycastToHitActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
    return 0;
}

uint32_t ControllerManager::RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
    return 0;
}

bool ControllerManager::RemoveController(const unsigned int& id)
{
    auto controller = m_CharectorMap.find(id);
    if (controller != m_CharectorMap.end())
    {
        m_CharectorMap.erase(controller);
        return true;
    }

    return false;
}

bool ControllerManager::HasController(uint32_t entityID)
{
    return m_CharectorMap.count(entityID) > 0;
}

Controller* ControllerManager::GetController(uint32_t entityID)
{
    // 먼저 entityID가 m_CharectorMap에 존재하는지 확인
    auto it = m_CharectorMap.find(entityID);
    if (it == m_CharectorMap.end())
        return nullptr;
    auto controller = it->second;  // 검색한 결과의 값을 가져옴

    return it->second.get();
    if (auto capsuleController = std::dynamic_pointer_cast<CapsuleController>(controller))
        return capsuleController.get();  // raw 포인터로 반환
    // 키가 존재하면 해당 shared_ptr의 raw 포인터를 반환
}

uint32_t ControllerManager::GetIDFromActor(physx::PxRigidActor* Actor)
{
    auto key = static_cast<uint32_t*>(Actor->userData);
    return *key;
}
//#pragma region Actor ver.
//{
//PxF32 max = (PxF32)distance;
//	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
//	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
//	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here
//	if (tempDir.isZero())
//		return 0;
//	bool find = m_Scene->raycast(
//		tempActor->getGlobalPose().p,
//		tempDir,
//		max,
//		buf);
//	if (!find)
//		return 0;
//	for (PxU32 i = 0; i < buf.nbTouches; i++)
//	{
//
//		PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
//		PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
//		PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴
//		auto tempID = GetIDFromActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음
//		if (tempID == entityID)
//			continue;
//		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
//			return tempID;
//	}
//	return 0;
//}
//#pragma endregion