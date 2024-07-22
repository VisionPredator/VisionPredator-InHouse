#include "pch.h"
#include "ControllerManager.h"
#include "VPPhysicsStructs.h"
#include "CapsuleController.h"
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
    CapsuleController* capsuleController = new CapsuleController;
    //CollisionData* collisionData = new CollisionData;
    capsuleController->Initialize(capsuleinfo,m_PxControllerManager, m_Material, physicsinfo);
    m_CharectorMap.insert(std::make_pair(capsuleController->GetEntityID(), capsuleController));
    return true;

}

bool ControllerManager::RemoveController(const unsigned int& id)
{


    return false;
}

bool ControllerManager::HasController(uint32_t entityID)
{
    return m_CharectorMap.count(entityID) > 0;
}

Controller* ControllerManager::GetController(uint32_t entityID)
{
	if (!HasController(entityID))
    return nullptr;
    return m_CharectorMap[entityID];
}
