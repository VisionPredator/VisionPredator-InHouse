#pragma once
#include "SystemInterface.h"
#include "System.h"
#include "EventSubscriber.h"

	class SystemManager:public EventSubscriber
	{
		float m_ProgressedTime = 0.f;

	public:
		SystemManager();
		~SystemManager() = default;
		void Update(float deltatime);
		void Initialize(SceneManager* entitymanager,Graphics::Interface* Interfaces);

		void FixedUpdate(float deltatime);
		void RenderUpdate(float deltatime);
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
			m_Systems.push_back(std::make_unique<T>(m_SceneManager));

			if constexpr (std::is_base_of_v<IUpdatable, T>)
			{
				m_Updatables.push_back(static_cast<T*>(m_Systems.back().get()));
			}

			if constexpr (std::is_base_of_v<IFixedUpdatable, T>)
			{
				m_FixedUpdatables.push_back(static_cast<T*>(m_Systems.back().get()));
			}

			if constexpr (std::is_base_of_v<IRenderable, T>)
			{
				m_Renderables.push_back(static_cast<T*>(m_Systems.back().get()));
				static_cast<T*>(m_Systems.back().get())->SetGraphics(m_Graphics);


			}

			if constexpr (std::is_base_of_v<IStartable, T>)
			{
				m_Startables.push_back(static_cast<T*>(m_Systems.back().get()));
			}

			return static_cast<T*>(m_Systems.back().get());
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

			if constexpr (std::is_base_of_v<IStartable, T>)
			{
				auto it = std::find_if(m_Startables.begin(), m_Startables.end(), [](auto* ptr) { return dynamic_cast<T*>(ptr) != nullptr; });
				if (it != m_Startables.end())
					m_Startables.erase(it);
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

		void Finish(uint32_t gameObjectId)
		{
			for (auto& startable : m_Startables)
			{
				startable->Finish(gameObjectId);
			}
		}

	private:
		void Initialize();
		void Finalize();

		void OnInitialize(std::any data)
		{
			Initialize();
		}

		void OnFinalize(std::any data)
		{
			Finalize();
		}



		SceneManager* m_SceneManager = nullptr;
		Graphics::Interface* m_Graphics = nullptr;

		std::vector<std::unique_ptr<System>> m_Systems;
		std::vector<IFixedUpdatable*> m_FixedUpdatables;
		std::vector<IUpdatable*>m_Updatables;
		std::vector<IRenderable*>m_Renderables;
		std::vector<IStartable*>m_Startables;
	};

