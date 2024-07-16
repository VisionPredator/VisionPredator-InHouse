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
		EventManager::GetInstance().Subscribe("OnAddTransformSystem",CreateSubscriber(&SystemManager::OnAddTransformSystem));
		EventManager::GetInstance().Subscribe("OnAddPhysicsSystem",CreateSubscriber(&SystemManager::OnAddPhysicsSystem));
		EventManager::GetInstance().Subscribe("OnAddCameraSystem",CreateSubscriber(&SystemManager::OnAddCameraSystem));
	}
	SystemManager::~SystemManager()
	{

	}


	void SystemManager::Update(float deltatime)
	{
		for (auto updateable : m_Updatables)
			updateable->Update(deltatime);

	}

	void SystemManager::Initialize(SceneManager* entitymanager, Graphics::Interface* GraphicsInterface, Physic::IPhysx* physicInterface)
	{
		m_SceneManager = entitymanager;
		m_Graphics = GraphicsInterface;
		m_PhysicEngine = physicInterface;
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
		AddSystem<TransformSystem>();
	}

	void SystemManager::OnAddPhysicsSystem(std::any)
	{
		AddSystem<PhysicSystem>();
	}
		void SystemManager::OnAddCameraSystem(std::any)
	{
		AddSystem<CameraSystem>();
	}




