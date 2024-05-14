#include "pch.h"
#include "Components.h"
#include "PlayerSystem.h"
#include "InputManager.h"

void PlayerSystem::Update(float deltaTime)
{

	for (IDComponent& IDComp :COMPITER(IDComponent))
	{
		IDComp.Name == "Player";
		IDComp.GetComponent<TransformComponent>();

		InputManager::GetInstance().GetKeyDown(KEY::A);
	}
}
