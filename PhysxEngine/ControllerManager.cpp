#include "pch.h"
#include "ControllerManager.h"
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
    m_CollisionManager = collisionManager;
    m_PxControllerManager = PxCreateControllerManager(*scene);
    return true;
}

bool ControllerManager::CreatController()
{

}

bool ControllerManager::CreatCapsuleController()
{
    CapsuleController* capsuleController = new CapsuleController;
    //CollisionData* collisionData = new CollisionData;
    capsuleController->Initialize();

}

bool ControllerManager::RemoveController(const unsigned int& id)
{
    return false;
}

bool ControllerManager::HasController(uint32_t entityID)
{
    return m_CharectorMap.count(entityID) > 0;
}

CharacterController* ControllerManager::GetController(uint32_t entityID)
{
	if (!HasController(entityID))
    return nullptr;
    return m_CharectorMap[entityID];
}
