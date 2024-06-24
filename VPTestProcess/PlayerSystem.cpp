#include "pch.h"
#include "Components.h"
#include "PlayerSystem.h"
#include "InputManager.h"

void PlayerSystem::Update(float deltaTime)
{

	for (PlayerComponent& player :COMPITER(PlayerComponent))
	{
		auto comp = player.GetComponent<TransformComponent>();

		comp->World_Location = {};
		//IDComp.Name == "Player";
		//IDComp.GetComponent<TransformComponent>();

		//InputManager::GetInstance().GetKeyDown(KEY::A);
	}

	
}
