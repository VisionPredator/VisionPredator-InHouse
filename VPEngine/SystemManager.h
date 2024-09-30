#pragma once
#include "SystemInterface.h"
#include "System.h"
#include "EventSubscriber.h"
#include <thread>

	class SystemManager:public EventSubscriber
	{
		float m_ProgressedTime = 0.f;
		float m_PhysicProgressedTime = 0.f;
		uint32_t m_PhysicsFrame = 60;
		int m_FixedFrame = 60;
		float m_PhysicDeltatime = 1.f / 60.f;
		float m_FixedDeltatime = 1.f / 60.f;
		float m_RenderFPS = 60.f;
		float m_RenderDeltatime{};
		float m_EditorRenderDeltatime{};
		float m_RenderProgressTime{};
		float m_EditorRenderProgressTime{};
	public:
		SystemManager();
		~SystemManager();
		void Update(float deltatime);
		void Initialize(std::shared_ptr<SceneManager> sceneManger, Graphics::Interface* GraphicsInterface, Physic::IPhysx* physicInterface, Sound::ISound* soundengine);

		void OnCollisionEnter(std::any pair);
		void OnCollisionExit(std::any pair);
		void OnAddedComponent(std::any data);
		void OnReleasedComponent(std::any data);
		void FixedUpdate(float deltatime);
		void RenderSystemUpdate(float deltatime);
		void BeginRender();
		bool ImguiBeginRender();
		void ImguiEndRender();
		void EndRender();

		void EditorRenderSystemUpdate(float deltatime);
		void BeginRenderUpdate(float deltatime);
		void RenderUpdate(float deltatime);
		void LateRenderUpdate(float deltatime);
		void EditorRenderUpdate(float deltatime);
		void LateUpdate(float deltatime);
		void PhysicUpdate(float deltatime);
		void SoundUpdate(float deltatime);
		template <typename T>
		bool IsSystemAdded() const
		{
			for (const auto& system : m_Systems)
			{
				if (dynamic_cast<T*>(system.get()) != nullptr)
					return true;
			}
			return false;
		}


		template <typename T> requires std::is_base_of_v<System, T>
		T* AddSystem()
		{
			if (IsSystemAdded<T>())
			{
				return nullptr;
			}
			m_Systems.push_back(std::make_unique<T>(m_SceneManager.lock()));
			auto* system = static_cast<T*>(m_Systems.back().get());
			system->SetThread1(&m_JThread1);
			system->SetThread2(&m_JThread2);
			if constexpr (std::is_base_of_v<IUpdatable, T>)
			{
				m_Updatables.push_back(system);
			}		
			if constexpr (std::is_base_of_v<IFixedUpdatable, T>)
			{
				m_FixedUpdatables.push_back(system);
			}
			if constexpr (std::is_base_of_v<IRenderable, T>)
			{
				m_Renderables.push_back(system);
				system->SetGraphics(m_Graphics);
			}
			if constexpr (std::is_base_of_v<IPhysicable, T>)
			{
				m_PhysicUpdatable.push_back(system);
				system->SetPhysicEngine(m_PhysicEngine);
			}

			if constexpr (std::is_base_of_v<ILateUpdatable, T>)
			{
				m_LateUpdatable.push_back(system);
			}

			if constexpr (std::is_base_of_v<IStartable, T>)
			{
				m_Startables.push_back(system);
			}

			if constexpr (std::is_base_of_v<IContactable, T>)
			{
				m_Contactable.push_back(system);
			}
			if constexpr (std::is_base_of_v<ISoundable, T>)
			{
				m_Soundable.push_back(system);
				system->SetSoundEngine(m_SoundEngine);
			}
			if constexpr (std::is_base_of_v<ICompAddable, T>)
			{
				m_CompAddable.push_back(system);
			}

			return static_cast<T*>(system);
		}
		template <typename T>
		void ReleaseSystem()
		{
			// Optional: Remove system from update/render lists
			if constexpr (std::is_base_of_v<IUpdatable, T>)
			{
				auto it = std::find_if(m_Updatables.begin(), m_Updatables.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Updatables.end())
					m_Updatables.erase(it);
			}

			if constexpr (std::is_base_of_v<IFixedUpdatable, T>)
			{
				auto it = std::find_if(m_FixedUpdatables.begin(), m_FixedUpdatables.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_FixedUpdatables.end())
					m_FixedUpdatables.erase(it);
			}

			if constexpr (std::is_base_of_v<IRenderable, T>)
			{
				auto it = std::find_if(m_Renderables.begin(), m_Renderables.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Renderables.end())
					m_Renderables.erase(it);
			}
			if constexpr (std::is_base_of_v<ILateUpdatable, T>)
			{
				auto it = std::find_if(m_LateUpdatable.begin(), m_LateUpdatable.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_LateUpdatable.end())
					m_LateUpdatable.erase(it);
			}

			if constexpr (std::is_base_of_v<IStartable, T>)
			{
				auto it = std::find_if(m_Startables.begin(), m_Startables.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Startables.end())
					m_Startables.erase(it);
			}
			if constexpr (std::is_base_of_v<IPhysicable, T>)
			{
				auto it = std::find_if(m_PhysicUpdatable.begin(), m_PhysicUpdatable.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_PhysicUpdatable.end())
					m_PhysicUpdatable.erase(it);
			}

			if constexpr (std::is_base_of_v<IContactable, T>)
			{
				auto it = std::find_if(m_Contactable.begin(), m_Contactable.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Contactable.end())
					m_Contactable.erase(it);
			}
			if constexpr (std::is_base_of_v<ISoundable, T>)
			{
				auto it = std::find_if(m_Soundable.begin(), m_Soundable.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Soundable.end())
					m_Soundable.erase(it);
			}
			if constexpr (std::is_base_of_v<ICompAddable, T>)
			{
				auto it = std::find_if(m_CompAddable.begin(), m_CompAddable.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_CompAddable.end())
					m_CompAddable.erase(it);
			}


			// Optional: Delete system object
			auto it = std::find_if(m_Systems.begin(), m_Systems.end(), [](const auto& ptr) { return dynamic_cast<T*>(ptr.get()) != nullptr; });
			if (it != m_Systems.end())
				m_Systems.erase(it);
		}

		void Start(uint32_t gameObjectId)
		{
			for (auto& startable : m_Startables)
			{
				startable->Start(gameObjectId);
			}
		}
		void Start_Parent(uint32_t entityid);
		void Finish_Parent(uint32_t entityid);

		void Finish(uint32_t gameObjectId)
		{
			for (auto& startable : m_Startables)
			{
				startable->Finish(gameObjectId);
			}
		}

	private:
		void InitializeSystems();
		void FinalizeSystems();
		void OnStart_Parent(std::any entityID);
		void OnStart(std::any entityID);
		void OnFinish(std::any entityID);
		void OnFinish_Parent(std::any data);
		void OnSetPhysicUpdateRate(std::any rate);
		void OnInitializeSystems(std::any data)
		{
			InitializeSystems();
		}

		void OnFinalizeSystems(std::any data)
		{
			FinalizeSystems();
		}

		std::jthread m_JThread1;
		std::jthread m_JThread2;
		std::weak_ptr<SceneManager>m_SceneManager;
		Physic::IPhysx* m_PhysicEngine=nullptr;
		Graphics::Interface* m_Graphics = nullptr;
		Sound::ISound* m_SoundEngine = nullptr;

		std::vector<std::unique_ptr<System>> m_Systems;
		std::vector<IPhysicable*> m_PhysicUpdatable;
		std::vector<IFixedUpdatable*> m_FixedUpdatables;
		std::vector<IUpdatable*> m_Updatables;
		std::vector<ILateUpdatable*> m_LateUpdatable;
		std::vector<IRenderable*> m_Renderables;
		std::vector<IStartable*> m_Startables;
		std::vector<IContactable*> m_Contactable;
		std::vector<ICompAddable*> m_CompAddable;
		std::vector<ISoundable*> m_Soundable;
	};

