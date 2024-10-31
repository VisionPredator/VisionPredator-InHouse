#include "pch.h"
#include "ButtonSystem.h"
#include "InputManager.h"
#include "..\VPGraphics\Log.h"

ButtonSystem::ButtonSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSceneChange", CreateSubscriber(&ButtonSystem::OnSceneChange));
	//EventManager::GetInstance().Subscribe("OnSpawnPrefabButton", CreateSubscriber(&ButtonSystem::OnSpawnPrefab));
	EventManager::GetInstance().Subscribe("OnDestrorPrefab", CreateSubscriber(&ButtonSystem::OnDestrorPrefab));
	EventManager::GetInstance().Subscribe("OnOffUI", CreateSubscriber(&ButtonSystem::OnOffUI));
	EventManager::GetInstance().Subscribe("OnOffUISelf", CreateSubscriber(&ButtonSystem::OnOffUISelf));
	EventManager::GetInstance().Subscribe("OnDragLR", CreateSubscriber(&ButtonSystem::OnDragLR));

}

void ButtonSystem::Update(float deltaTime)
{
	activeLayer.clear();

	int curMousePosX = InputManager::GetInstance().GetMouseX();
	int curMousePosY = InputManager::GetInstance().GetMouseY();

	//ui layer 확인해서 최저 layer에 있는 button만 활성화를 위한 정렬
	for (ButtonComponent& comp : COMPITER(ButtonComponent))
	{
		if (comp.HasComponent<ImageComponent>())
		{
			auto imageComp = comp.GetComponent<ImageComponent>();

			if (imageComp->Color.w > 0)
			{
				comp.isActive = true;
				activeLayer.insert(imageComp->Layer);
			}
			else
			{
				comp.isActive = false;
			}
		}
	}

	if (!activeLayer.empty())
	{
		minUILayer = (*activeLayer.begin());
	}

	for (ButtonComponent& comp : COMPITER(ButtonComponent))
	{
		if (!comp.isActive)
			continue;

		if (comp.HasComponent<ImageComponent>())
		{
			auto imageComp = comp.GetComponent<ImageComponent>();

			if (imageComp->Layer > minUILayer)
			{
				continue;
			}

			RECT rect = m_Graphics->GetImageRect(imageComp->GetEntityID());

			if (rect.left < curMousePosX && curMousePosX < rect.right &&
				rect.top < curMousePosY && curMousePosY < rect.bottom)
			{
				imageComp->Scale = 1.3f;

				if (INPUTKEYUP(MOUSEKEY::LBUTTON))
				{
					comp.isClicked = !comp.isClicked;

					switch (comp.skill)
					{
						case VisPred::Engine::Skill::SCENECHANGE:
						{
							if (!comp.name.empty())
							{
								for (auto& name : comp.name)
								{
									std::string path = "../Data/Scene/";
									std::string ext = ".scene";
									path += name;
									path += ext;
									EventManager::GetInstance().ScheduleEvent("OnSceneChange", path);
								}
							}
						}
						break;

						case VisPred::Engine::Skill::SPAWNPREFAB:
							if (!comp.name.empty())
							{
								for (auto& name : comp.name)
								{
									std::string path = "../Data/Prefab/";
									std::string ext = ".prefab";
									path += name;
									path += ext;
									EventManager::GetInstance().ScheduleEvent("OnUI", path);
								}
							}
							break;
						case VisPred::Engine::Skill::DESTORYPREFAB:
							if (!comp.name.empty())
							{
								for (auto& name : comp.name)
								{
									EventManager::GetInstance().ScheduleEvent("OnDestrorPrefab", name);
								}

							}

							break;
						case VisPred::Engine::Skill::UION:
						{
							for (auto& childname : comp.name)
							{
								//눌렸으면 하위 ui on
								EventManager::GetInstance().ScheduleEvent("OnOffUI", std::pair<std::string, bool>(childname, true));
							}
						}
						break;

						case VisPred::Engine::Skill::UIOFFSELF:
						{
							imageComp->Color.w = false;
							for (auto& childname : comp.name)
							{
								//눌렸으면 자신을 포함한 하위 ui off
								EventManager::GetInstance().ScheduleEvent("OnOffUI", std::pair<std::string, bool>(childname, false));
							}
						}
						break;

						case VisPred::Engine::Skill::UIOFF:
						{
							for (auto& childname : comp.name)
							{
								//눌렸으면 자신을 포함한 하위 ui off
								EventManager::GetInstance().ScheduleEvent("OnOffUI", std::pair<std::string, bool>(childname, false));
							}
						}
						break;

						case VisPred::Engine::Skill::EXIT:
							PostQuitMessage(0);
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

			//디버그용
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


void ButtonSystem::OnSceneChange(std::any scenename)
{
	auto changescene = std::any_cast<std::string>(scenename);
	GetSceneManager()->ChangeScene(changescene);
}

void ButtonSystem::OnSpawnPrefab(std::any prefabname)
{
	auto name = std::any_cast<std::string>(prefabname);

	auto entity = GetSceneManager()->SpawnEditablePrefab(name, {}, VPMath::Vector3{});
}

void ButtonSystem::OnDestrorPrefab(std::any prefabname)
{
	auto name = std::any_cast<std::string>(prefabname);

	uint32_t id = GetSceneManager()->GetEntityByIdentityName(name)->GetEntityID();
	GetSceneManager()->DestroyEntity(id);
}

void ButtonSystem::OnOffUI(std::any entity)
{
	auto [name, useUI] = std::any_cast<std::pair<std::string, bool>>(entity);
	auto child = GetSceneManager()->GetEntityByIdentityName(name);

	if (child->HasComponent<ImageComponent>())
	{
		auto comp = child->GetComponent<ImageComponent>();
		comp->Color.w = useUI;
	}

	if (child->HasComponent<ButtonComponent>())
	{
		auto comp = child->GetComponent<ButtonComponent>();
	}
}

void ButtonSystem::OnDragLR(std::any entity)
{
	auto curEntity = std::any_cast<Entity*>(entity);

	int dx = InputManager::GetInstance().GetMouseDeltaX();


	if (curEntity->HasComponent<Children>())
	{
		auto& childrens = curEntity->GetComponent<Children>()->ChildrenID;

		for (auto child : childrens)
		{
			if (curEntity->HasComponent<ImageComponent>())
			{
				auto comp = curEntity->GetComponent<ImageComponent>();
				if (dx < 0)
				{
					comp->LeftPercent;
				}
				else
				{
					comp->RightPercent;
				}
			}
		}
	}
}

void ButtonSystem::OnOffUISelf(std::any entity)
{
}
