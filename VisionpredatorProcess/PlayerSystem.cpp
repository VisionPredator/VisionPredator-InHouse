#include "pch.h"
#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(SceneManager* sceneManager) :System{ sceneManager }
{
}

void PlayerSystem::Update(float deltaTime)
{
	for (PlayerComponent& comp: COMPITER(PlayerComponent))
	{
		TransformComponent* TransformComp = comp.GetComponent<TransformComponent>();

		if (comp.HasComponent<ControllerComponent>())
		{
			ControllerComponent* controllercomp = comp.GetComponent<ControllerComponent>();
			controllercomp->InputDir = {};
			if (INPUTKEY(KEY::W))
			{
				controllercomp->InputDir += TransformComp->FrontVector;
			}
			if (INPUTKEY(KEY::S))
			{
				controllercomp->InputDir -= TransformComp->FrontVector;

			}
			if (INPUTKEY(KEY::A))
			{
				controllercomp->InputDir -= TransformComp->RightVector;
			}
			if (INPUTKEY(KEY::D))
			{
				controllercomp->InputDir += TransformComp->RightVector;
			}

			return;
		}


		uint32_t entityID = comp.GetEntityID();
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

	}


}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{
}
