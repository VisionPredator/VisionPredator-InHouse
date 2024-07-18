#include "pch.h"
#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(SceneManager* sceneManager) :System{ sceneManager }
{
}

void PlayerSystem::Update(float deltaTime)
{
	for (PlayerComponent& comp: COMPITER(PlayerComponent))
	{
		uint32_t entityID = comp.GetEntityID();
		TransformComponent* TransformComp = comp.GetComponent<TransformComponent>();
		if (INPUTKEY(KEY::W))
		{
			m_PhysicsEngine->AddVelocity(entityID, TransformComp->FrontVector, comp.Accel);

		}		
		if (INPUTKEY(KEY::S))
		{
			m_PhysicsEngine->AddVelocity(entityID, -TransformComp->FrontVector, comp.Accel);

		}
		if (INPUTKEY(KEY::A))
		{
			m_PhysicsEngine->AddVelocity(entityID, -TransformComp->RightVector, comp.Accel);
		}
		if (INPUTKEY(KEY::D))
		{
			m_PhysicsEngine->AddVelocity(entityID, TransformComp->RightVector, comp.Accel);
		}

		if (true)
		{

		}

	}


}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{
}
