	#include "pch.h"
#include "ButtonSystem.h"
#include "InputManager.h"
#include "..\VPGraphics\Log.h"

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
			RECT rect = m_Graphics->GetImageRect(imageComp->GetEntityID());

			if (rect.left < curMousePosX && curMousePosX < rect.right &&
				rect.top < curMousePosY && curMousePosY < rect.bottom)
			{
				imageComp->Scale = 1.3f;
				
				if (INPUTKEYUP(MOUSEKEY::LBUTTON))
				{

					switch (comp.skill)	
					{
						case VisPred::Engine::Skill::START:
							m_SceneManager.lock()->ChangeScene("../Data/Scene/level10.scene");
							break;
						case VisPred::Engine::Skill::OPTION:
							break;
						case VisPred::Engine::Skill::EXIT:
							break;

						default:
							break;
					}


				}

			}
			else
			{
				imageComp->Scale = 1;

			}

			if (comp.HasComponent<TextComponent>())
			{

				auto textComp = comp.GetComponent<TextComponent>();

				std::wstring x = std::to_wstring(curMousePosX);
				std::wstring y = std::to_wstring(curMousePosY);
				std::wstring mouse = L"Mouse : " + x + L", " + y + L"\n";

				std::wstring L = std::to_wstring(rect.left);
				std::wstring T = std::to_wstring(rect.top);
				std::wstring R = std::to_wstring(rect.right);
				std::wstring B = std::to_wstring(rect.bottom);
				std::wstring rectsize = L"LT : " + L + L", " + T + L" / RB : " + R + L", " + B;

				textComp->Text = mouse + rectsize;
			}
		}

	}


}

void ButtonSystem::BeginRenderUpdate(float deltaTime)
{
}

void ButtonSystem::RenderUpdate(float deltaTime)
{
}

void ButtonSystem::LateRenderUpdate(float deltaTime)
{
}

void ButtonSystem::EditorRenderUpdate(float deltaTime)
{
}
