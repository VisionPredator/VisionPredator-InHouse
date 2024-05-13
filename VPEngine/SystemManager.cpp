#include "pch.h"
#include "SystemManager.h"

namespace VisPred
{
	SystemManager::SystemManager()
	{
	}
	void SystemManager::Update(float deltatime)
	{

		for (auto updateable : m_Updatables)
			updateable->Update(deltatime);

	}
	void SystemManager::Initialize(EntityManager* entitymanager)
	{
		m_EntityManager = entitymanager;
	}

	void SystemManager::FixedUpdate(float deltatime)
	{
		m_ProgressedTime += deltatime;
		///FixedUpdate 발동 조건 : 1/60 넘을 때

		while (m_ProgressedTime > 1.f / 60.f)
		{
			for (auto fixedUpdateable : m_FixedUpdatables)
				fixedUpdateable->FixedUpdate(1.f / 60.f);

			m_ProgressedTime -= 1.f / 60.f;
		}

	}

	void SystemManager::Render()
	{
		//for (auto renderable : m_Renderables)
		//	renderable->Render();

	}



}