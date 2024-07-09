#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"

PhysicSystem::PhysicSystem(SceneManager* sceneManager)
	:System(sceneManager)
{
}

void PhysicSystem::Update(float deltaTime)
{

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
	}
		m_PhysxEngine->Update(deltaTime);

}

void PhysicSystem::FixedUpdate(float deltaTime)
{

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
	}

}
void PhysicSystem::Initialize()
{
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		Start(rigidBodyComponent.GetEntityID());
	}
}

void PhysicSystem::Start(uint32_t gameObjectId)
{
	Entity* entity = m_SceneManager->GetEntity(gameObjectId);
	RigidBodyComponent* rigidComp = entity->GetComponent<RigidBodyComponent>();
	rigidComp->Test;
}

void PhysicSystem::Finish(uint32_t gameObjectId)
{
	Entity* entity = m_SceneManager->GetEntity(gameObjectId);

}

void PhysicSystem::Finalize()
{
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		Start(rigidBodyComponent.GetEntityID());
	}
}
