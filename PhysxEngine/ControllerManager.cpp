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

bool ControllerManager::Update(float deltatime)
{
    if (m_CharectorMap.empty())
        return true;
    for (auto [entityID, controller] : m_CharectorMap)
    {
        if (controller ->GetTypeID() == Reflection::GetTypeID<CapsuleController>())
        {
            CapsuleController* capsuleController = static_cast<CapsuleController*>(controller);

            physx::PxControllerCollisionFlags collisionFlags = capsuleController->m_Controller->move(capsuleController->m_Velocity, 0.001f, deltatime, *capsuleController->GetFilters());

            if (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
                capsuleController->SetIsFall(false);
            else
                capsuleController->SetIsFall(true);
        }
    }
    return true;

}

bool ControllerManager::RemoveController(const unsigned int& id)
{
    auto controller = m_CharectorMap.find(id);
    if (controller != m_CharectorMap.end())
    {
        delete controller->second;
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
	if (!HasController(entityID))
    return nullptr;
    return m_CharectorMap[entityID];
}
