#include "pch.h"
#include "SystemManager.h"
#include "EventManager.h"
#include "TransformSystem.h"

	SystemManager::SystemManager()
	{
		EventManager::GetInstance().Subscribe("OnInitializeSystems", CreateSubscriber(&SystemManager::OnInitializeSystems));
		EventManager::GetInstance().Subscribe("OnFinalizeSystems",CreateSubscriber(&SystemManager::OnFinalizeSystems));
		EventManager::GetInstance().Subscribe("OnAddTransformSystem",CreateSubscriber(&SystemManager::OnAddTransformSystem));
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
	void SystemManager::InitializeSystems()
	{
		for (auto startable:m_Startables)
			startable->Initialize();
	}

	void SystemManager::FinalizeSystems()
	{
		for (auto startable : m_Startables)
			startable->Finalize();
	}


	void SystemManager::RenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
			renderable->RenderUpdate(deltatime);
	}

	void SystemManager::OnAddTransformSystem(std::any)
	{
		if (IsSystemAdded<TransformSystem>())
			return;
		m_Systems.push_back(std::make_unique<TransformSystem>(m_SceneManager));

		if constexpr (std::is_base_of_v<IUpdatable, TransformSystem>)
		{
			m_Updatables.push_back(static_cast<TransformSystem*>(m_Systems.back().get()));
		}
	}




