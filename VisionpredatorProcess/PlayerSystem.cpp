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

		}

		if (INPUTKEYDOWN(KEYBOARDKEY::SPACE))
		{
			auto temppos =TransformComp->World_Location + TransformComp->FrontVector * 2;
			auto tempdir =TransformComp->FrontVector;
			VPMath::Vector3 tempscale = {1,1,1};

			m_SceneManager.lock()->SpawnPrefab("../Data/Prefab/cube.prefab", temppos, tempdir, tempscale);
		}

	}


}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{
}
