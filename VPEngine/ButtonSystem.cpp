#include "pch.h"
#include "ButtonSystem.h"
#include "InputManager.h"

ButtonSystem::ButtonSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
	//EventManager::GetInstance().Subscribe("OnChangeAnimation", CreateSubscriber(&AnimationSystem::OnChangeAnimation));

}

void ButtonSystem::Update(float deltaTime)
{

	int curMousePosX = InputManager::GetInstance().GetMouseX();
	int curMousePosY = InputManager::GetInstance().GetMouseY();

	for (ButtonComponent& comp : COMPITER(ButtonComponent))
	{
		if (comp.HasComponent<ImageComponent>())
		{
			auto imageComp = comp.GetComponent<ImageComponent>();

		}
	}


}
