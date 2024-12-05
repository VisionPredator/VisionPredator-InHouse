#include "pch.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "TransformSystem.h"
#include "PhysicSystem.h"
#include "CameraSystem.h"
#include"Components.h"
#include <iostream>
	SystemManager::SystemManager()
	{
		EventManager::GetInstance().Subscribe("OnInitializeSystems", CreateSubscriber(&SystemManager::OnInitializeSystems));
		EventManager::GetInstance().Subscribe("OnFinalizeSystems",CreateSubscriber(&SystemManager::OnFinalizeSystems));
		EventManager::GetInstance().Subscribe("OnSetPhysicUpdateRate", CreateSubscriber(&SystemManager::OnSetPhysicUpdateRate));
		EventManager::GetInstance().Subscribe("OnStart", CreateSubscriber(&SystemManager::OnStart));
		EventManager::GetInstance().Subscribe("OnCollisionEnter", CreateSubscriber(&SystemManager::OnCollisionEnter));
		EventManager::GetInstance().Subscribe("OnCollisionExit", CreateSubscriber(&SystemManager::OnCollisionExit));
		EventManager::GetInstance().Subscribe("OnTriggerEnter", CreateSubscriber(&SystemManager::OnTriggerEnter));
		EventManager::GetInstance().Subscribe("OnTriggerExit", CreateSubscriber(&SystemManager::OnTriggerExit));
		EventManager::GetInstance().Subscribe("OnStart_Parent", CreateSubscriber(&SystemManager::OnStart_Parent));
		EventManager::GetInstance().Subscribe("OnFinish", CreateSubscriber(&SystemManager::OnFinish));
		EventManager::GetInstance().Subscribe("OnFinish_Parent", CreateSubscriber(&SystemManager::OnFinish_Parent));
		EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&SystemManager::OnAddedComponent));
		EventManager::GetInstance().Subscribe("OnReleasedComponent", CreateSubscriber(&SystemManager::OnReleasedComponent));

		m_FixedDeltatime = 1.f / m_FixedFrame;
		m_PhysicDeltatime= 1.f / m_PhysicsFrame;
		m_RenderDeltatime= 1.f / m_RenderFPS;
		m_EditorRenderDeltatime = 1.f / m_RenderFPS;

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
	void SystemManager::PhysicUpdate(float deltatime)
	{
		m_PhysicProgressedTime += deltatime;
		while (m_PhysicProgressedTime > m_PhysicDeltatime)
		{
			for (auto physicsUpdatable : m_PhysicUpdatable)
			{
				physicsUpdatable->PhysicsUpdate(m_PhysicDeltatime);
			}
			for (auto physicsUpdatable : m_PhysicUpdatable)
			{
				physicsUpdatable->PhysicsLateUpdate(m_PhysicDeltatime);
			}
			m_PhysicProgressedTime -= m_PhysicDeltatime;
		}

	}

	void SystemManager::SoundUpdate(float deltatime)
	{
		for (auto soundable : m_Soundable)
		{
			soundable->SoundUpdate(deltatime);
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
	void SystemManager::RenderSystemUpdate(float deltatime)
	{
		m_RenderProgressTime += deltatime;

		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			BeginRenderUpdate(m_RenderProgressTime);
			RenderUpdate(m_RenderProgressTime);
			LateRenderUpdate(m_RenderProgressTime);
		}
	}
	void SystemManager::BeginRender()
	{
		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			m_Graphics->BeginRender();
			m_Graphics->Render(m_RenderDeltatime);
		}
	}
	bool SystemManager::ImguiBeginRender()
	{
		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			m_Graphics->ImguiBeginRender();
			return true;
		}
		else
		{
		return false;
		}
	}
	void SystemManager::ImguiEndRender()
	{
		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			m_Graphics->ImguiEndRender();
		}
	}
	void SystemManager::EndRender()
	{
		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			m_Graphics->EndRender();
			m_RenderProgressTime = 0;
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
	void SystemManager::EditorRenderSystemUpdate(float deltatime)
	{
		m_RenderProgressTime += deltatime;

		if (m_RenderProgressTime > m_RenderDeltatime)
		{
			EditorRenderUpdate(m_RenderProgressTime);
		}
	}
	void SystemManager::BeginRenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
		{
			renderable->BeginRenderUpdate(deltatime);
		}
	}
	void SystemManager::RenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
		{
			renderable->RenderUpdate(deltatime);
		}
	}
	void SystemManager::LateRenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
		{
			renderable->LateRenderUpdate(deltatime);
		}
	}

	void SystemManager::EditorRenderUpdate(float deltatime)
	{
		for (auto renderable : m_Renderables)
		{
			renderable->EditorRenderUpdate(deltatime);
		}
	}




	void SystemManager::Initialize(std::shared_ptr<SceneManager> sceneManger, Graphics::Interface* GraphicsInterface, Physic::IPhysx* physicInterface, Sound::ISound* soundengine)
	{
		m_SceneManager = sceneManger;
		m_Graphics = GraphicsInterface;
		m_PhysicEngine = physicInterface;
		m_SoundEngine = soundengine;
	}

	void SystemManager::OnCollisionEnter(std::any pair)
	{
		auto entitypair = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
		for (auto contactable:m_Contactable)
		{
			contactable->EnterCollision(entitypair);
		}
	}

	void SystemManager::OnCollisionExit(std::any pair)
	{
		auto entitypair = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
		for (auto contactable : m_Contactable)
		{
			contactable->ExitCollision(entitypair);
		}
	}

	void SystemManager::OnTriggerEnter(std::any pair)
	{
		auto entitypair = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);

		auto entity_first = m_SceneManager.lock()->GetEntity(entitypair.first);
		auto entity_second = m_SceneManager.lock()->GetEntity(entitypair.second);
		if (!entity_first || !entity_second)
			return;
		for (auto contactable : m_Triggerable)
		{
			contactable->EnterTrigger(entity_first, entity_second);
		}
	}

	void SystemManager::OnTriggerExit(std::any pair)
	{
		auto entitypair = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
		auto entity_first = m_SceneManager.lock()->GetEntity(entitypair.first);
		auto entity_second = m_SceneManager.lock()->GetEntity(entitypair.second);
		if (!entity_first || !entity_second)
			return;
		for (auto contactable : m_Triggerable)
		{
			contactable->ExitTrigger(entity_first, entity_second);
		}
	}

	void SystemManager::OnAddedComponent(std::any data)
	{
		auto comp = std::any_cast<Component*>(data);
		for (auto compaddable : m_CompAddable)
		{
			compaddable->ComponentAdded(comp);
		}

	}

	void SystemManager::OnReleasedComponent(std::any data)
	{
		auto comp = std::any_cast<Component*>(data);
		for (auto compaddable : m_CompAddable)
		{
			compaddable->ComponentReleased(comp);
		}
	}


	void SystemManager::Start_Parent(uint32_t entityid)
	{
		Start(entityid);
		if (m_SceneManager.lock()->HasComponent<Children>(entityid))
		{
			auto children = m_SceneManager.lock()->GetComponent<Children>(entityid);
			for (auto childID : children->ChildrenID)
			{
				Start_Parent(childID);
			}
		}
	}

	void SystemManager::Finish_Parent(uint32_t entityid)
	{
		Finish(entityid);
		if (m_SceneManager.lock()->HasComponent<Children>(entityid))
		{
			auto children = m_SceneManager.lock()->GetComponent<Children>(entityid);
			for (auto childID : children->ChildrenID)
			{
				Finish_Parent(childID);
			}
		}
	}

	void SystemManager::InitializeSystems()
	{
		EventManager::GetInstance().ImmediateEvent("OnUpdate");
		for (auto startable:m_Startables)
			startable->Initialize();

	}

	void SystemManager::FinalizeSystems()
	{
		for (auto startable : m_Startables)
			startable->Finalize();
	}

	void SystemManager::OnStart_Parent(std::any data)
	{
		uint32_t entityID = std::any_cast<uint32_t>(data);
		Start_Parent(entityID);
	}

	void SystemManager::OnStart(std::any data)
	{

		uint32_t entityID = std::any_cast<uint32_t>(data);
		Start(entityID);

	}

	void SystemManager::OnFinish(std::any data)
	{
		uint32_t entityID = std::any_cast<uint32_t>(data);
		Finish(entityID);
	}

	void SystemManager::OnFinish_Parent(std::any data)
	{
		uint32_t entityID = std::any_cast<uint32_t>(data);
		Finish_Parent(entityID);
	}

	void SystemManager::OnSetPhysicUpdateRate(std::any rate)
	{
		m_PhysicsFrame = std::any_cast<uint32_t>(rate);
		m_PhysicDeltatime = 1.f / m_PhysicsFrame;
	}






