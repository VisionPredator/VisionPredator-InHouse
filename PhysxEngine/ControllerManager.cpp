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
        // Sort hits by distance


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
		std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
			return a.distance < b.distance;
			});
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
    auto temp = GetController(entityID);
    if (!temp)
        return 0;

    auto tempController = temp->GetPxController();
    physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
    tempDir.normalize();

    PxF32 max = (PxF32)distance;
    const PxU32 bufferSize = 32;
    PxRaycastHit hitBuffer[bufferSize];
    PxRaycastBuffer buf(hitBuffer, bufferSize);

    if (tempDir.isZero())
        return 0;

    // Controller 위치에 offset을 적용
    auto temppose = tempController->getPosition();
    PxVec3 controllerpose = { static_cast<float>(temppose.x + offset.x),
                              static_cast<float>(temppose.y + offset.y),
                              static_cast<float>(temppose.z + offset.z) };

    // Offset된 위치에서 레이캐스트 수행
    bool find = m_Scene->raycast(controllerpose, tempDir, max, buf);

    if (!find)
        return 0;
    std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
        return a.distance < b.distance;
        });
    for (PxU32 i = 0; i < buf.nbTouches; i++)
    {
        auto tempID = GetIDFromActor(buf.touches[i].actor);
        if (tempID == entityID)
            continue;

        if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
            return tempID;
    }

    return 0;
}

uint32_t ControllerManager::RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
    physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
    tempDir.normalize();

    PxF32 max = (PxF32)distance;
    const PxU32 bufferSize = 32;
    PxRaycastHit hitBuffer[bufferSize];
    PxRaycastBuffer buf(hitBuffer, bufferSize);

    if (tempDir.isZero())
        return 0;

    // 주어진 location에서 레이캐스트 수행
    PxVec3 rayOrigin = { static_cast<float>(location.x),
                         static_cast<float>(location.y),
                         static_cast<float>(location.z) };

    bool find = m_Scene->raycast(rayOrigin, tempDir, max, buf);

    if (!find)
        return 0;
    std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
        return a.distance < b.distance;
        });
    for (PxU32 i = 0; i < buf.nbTouches; i++)
    {
        auto tempID = GetIDFromActor(buf.touches[i].actor);
        if (tempID == 0)
            continue;

        if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
            return tempID;
    }

    return 0;
}

void ControllerManager::ResizeCapsuleControllerSize(uint32_t entityID, float radius, float height)
{
    if (!HasController(entityID))
        return;
    auto controller = GetController(entityID);
    if (Reflection::IsSameType<CapsuleController>(controller->GetTypeID()))
    {
        auto Capsulecontroller = dynamic_cast<CapsuleController*>(controller);
        Capsulecontroller->ResizeCapsuleControllerSize(radius, height);
    }
}

//void ControllerManager::UpdateCapsuleSize(uint32_t entityID, const VPPhysics::CapsuleControllerInfo& info)
//{
//    if (!HasController(entityID))
//        return;
//    auto controller = GetController(entityID);
//    if (Reflection::IsSameType<CapsuleController>(controller->GetTypeID()))
//    {
//        auto Capsulecontroller = dynamic_cast<CapsuleController*>(controller); 
//        Capsulecontroller->UpdateCapsuleSize(info);
//    }
//}

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
}

physx::PxController* ControllerManager::GetPxController(uint32_t entityID)
{
    // 먼저 entityID가 m_CharectorMap에 존재하는지 확인
    auto it = m_CharectorMap.find(entityID);
    if (it == m_CharectorMap.end())
        return nullptr;
    auto controller = it->second;  // 검색한 결과의 값을 가져옴
    return controller->GetPxController();
}

uint32_t ControllerManager::GetIDFromActor(physx::PxRigidActor* Actor)
{
    auto key = static_cast<VPPhysics::USERDATA*>(Actor->userData);
    return key->entityID;
}
