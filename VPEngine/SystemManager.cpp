#include "pch.h"
#include "SystemManager.h"
#include "EventManager.h"

	SystemManager::SystemManager()
	{
		EventManager::GetInstance().Subscribe("OnInitialize", CreateSubscriber(&SystemManager::OnInitialize));
		EventManager::GetInstance().Subscribe("OnFinalize",CreateSubscriber(&SystemManager::OnFinalize));
	}
	void SystemManager::Initialize(SceneManager* entitymanager, Graphics::Interface* Interfaces)
	{
		m_SceneManager = entitymanager;
		m_Graphics = Interfaces;
	}

	void SystemManager::Update(float deltatime)
	{
		for (auto updateable : m_Updatables)
			updateable->Update(deltatime);

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
	void SystemManager::Initialize()
	{
		for (auto startable:m_Startables)
			startable->Initialize();
	}

	void SystemManager::Finalize()
	{
		for (auto startable : m_Startables)
			startable->Finalize();
	}


	void SystemManager::RenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
			renderable->RenderUpdate(deltatime);
	}




