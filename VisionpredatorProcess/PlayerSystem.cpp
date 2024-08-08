#include "pch.h"
#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
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
			if (INPUTKEY(KEYBOARDKEY::W))
			{
				controllercomp->InputDir += TransformComp->FrontVector;
			}
			if (INPUTKEY(KEYBOARDKEY::S))
			{
				controllercomp->InputDir -= TransformComp->FrontVector;

			}
			if (INPUTKEY(KEYBOARDKEY::A))
			{
				controllercomp->InputDir -= TransformComp->RightVector;
			}
			if (INPUTKEY(KEYBOARDKEY::D))
			{
				controllercomp->InputDir += TransformComp->RightVector;
			}

			return;
		}


		uint32_t entityID = comp.GetEntityID();
		if (INPUTKEY(KEYBOARDKEY::W))
		{
			m_PhysicsEngine->AddVelocity(entityID, TransformComp->FrontVector, comp.Accel);

		}		
		if (INPUTKEY(KEYBOARDKEY::S))
		{
			m_PhysicsEngine->AddVelocity(entityID, -TransformComp->FrontVector, comp.Accel);

		}
		if (INPUTKEY(KEYBOARDKEY::A))
		{
			m_PhysicsEngine->AddVelocity(entityID, -TransformComp->RightVector, comp.Accel);
		}
		if (INPUTKEY(KEYBOARDKEY::D))
		{
			m_PhysicsEngine->AddVelocity(entityID, TransformComp->RightVector, comp.Accel);
		}

	}


}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{
}
