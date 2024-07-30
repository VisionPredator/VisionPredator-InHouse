#include "pch.h"
#include "SystemManager.h"
#include "EventManager.h"
#include "TransformSystem.h"
#include "PhysicSystem.h"
#include "CameraSystem.h"

	SystemManager::SystemManager()
	{
		EventManager::GetInstance().Subscribe("OnInitializeSystems", CreateSubscriber(&SystemManager::OnInitializeSystems));
		EventManager::GetInstance().Subscribe("OnFinalizeSystems",CreateSubscriber(&SystemManager::OnFinalizeSystems));
		m_FixedDeltatime = 1.f / m_FixedFrame;
		m_PhysicDeltatime= 1.f / m_PhysicsFrame;

	}
	SystemManager::~SystemManager()
	{
		m_PhysicEngine = nullptr;
		m_Graphics = nullptr;
		m_FixedUpdatables.clear();
		m_Updatables.clear();
		m_Renderables.clear();
		m_Startables.clear();
		m_LateUpdatable.clear();

	}
	void SystemManager::PhysicUpdatable(float deltatime)
	{

		m_PhysicProgressedTime += deltatime;
		while (m_PhysicProgressedTime > m_PhysicDeltatime)
		{
			for (auto physicsUpdatable : m_PhysicUpdatable)
			{
				physicsUpdatable->PhysicsUpdate(m_PhysicDeltatime);
				EventManager::GetInstance().ImmediateEvent("OnUpdateTransfomData");
			}
			m_PhysicProgressedTime -= m_PhysicDeltatime;
		}

	}
	void SystemManager::FixedUpdate(float deltatime)
	{
		m_ProgressedTime += deltatime;

		///FixedUpdate 발동 조건 : 1/60 넘을 때
		while (m_ProgressedTime > m_FixedDeltatime)
		{
			for (auto fixedUpdateable : m_FixedUpdatables)
				fixedUpdateable->FixedUpdate(m_FixedDeltatime);
			m_ProgressedTime -= m_FixedDeltatime;

		}
	}
	void SystemManager::Update(float deltatime)
	{
		for (auto updateable : m_Updatables)
		{
			updateable->Update(deltatime);
		}

	}
	void SystemManager::LateUpdate(float deltatime)
	{
		for (auto lateUpdatable : m_LateUpdatable)
		{
			lateUpdatable->LateUpdate(deltatime);
		}
	}
	void SystemManager::RenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
		{
			renderable->RenderUpdate(deltatime);
		}
	}




	void SystemManager::Initialize(std::shared_ptr<SceneManager> sceneManger, Graphics::Interface* GraphicsInterface, Physic::IPhysx* physicInterface)
	{
		m_SceneManager = sceneManger;
		m_Graphics = GraphicsInterface;
		m_PhysicEngine = physicInterface;
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






