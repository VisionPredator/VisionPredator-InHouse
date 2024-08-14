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
			PlayerLocation(TransformComp);
			PlayerRotation(TransformComp);
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

void PlayerSystem::PlayerLocation(TransformComponent* comp)
{
	ControllerComponent* controllercomp = comp->GetComponent<ControllerComponent>();
	controllercomp->InputDir = {};
	if (INPUTKEY(KEYBOARDKEY::W))
	{
		controllercomp->InputDir += comp->FrontVector;
	}
	if (INPUTKEY(KEYBOARDKEY::S))
	{
		controllercomp->InputDir -= comp->FrontVector;

	}
	if (INPUTKEY(KEYBOARDKEY::A))
	{
		controllercomp->InputDir -= comp->RightVector;
	}
	if (INPUTKEY(KEYBOARDKEY::D))
	{
		controllercomp->InputDir += comp->RightVector;
	}
}

void PlayerSystem::PlayerRotation(TransformComponent* comp)
{
	auto player = comp->GetComponent<PlayerComponent>();
	if (InputManager::GetInstance().GetKey(MOUSEKEY::RBUTTON))
	{
		int deltaCurposx = InputManager::GetInstance().GetMouseDeltaX();
		int deltaCurposy = InputManager::GetInstance().GetMouseDeltaY();
		float yaw = deltaCurposx * player->Sencitive;
		float pitch = deltaCurposy * player->Sencitive;
		VPMath::Vector3 rotation = comp->World_Rotation;

		rotation.x -= pitch;
		rotation.y -= yaw;

		rotation.x = std::clamp(rotation.x, -89.9f, 89.9f);
		comp->SetWorldRotation(rotation);
	}

}
