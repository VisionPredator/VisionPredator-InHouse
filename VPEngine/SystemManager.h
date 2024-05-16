#pragma once
#include "SystemInterface.h"
#include "System.h"

	class SystemManager
	{
		float m_ProgressedTime = 0.f;

	public:
		SystemManager();
		~SystemManager() = default;
		void Update(float deltatime);
		void Initialize(SceneManager* entitymanager);
		void FixedUpdate(float deltatime);
		void Render();

		template <typename T> requires std::is_base_of_v<System, T>
		T* AddSystem()
		{
			m_Systems.push_back(std::make_unique<T>(m_SceneManager));

			if constexpr (std::is_base_of_v<IUpdatable, T>)
			{
				m_Updatables.push_back(static_cast<T*>(m_Systems.back().get()));
			}

			if constexpr (std::is_base_of_v<IFixedUpdatable, T>)
			{
				m_FixedUpdatables.push_back(static_cast<T*>(m_Systems.back().get()));
			}

			//if constexpr (std::is_base_of_v<IRenderable, T>)
			//{
			//	m_Renderables.push_back(static_cast<T*>(m_Systems.back().get()));
			//}

			//if constexpr (std::is_base_of_v<IStartable, T>)
			//{
			//	m_Startables.push_back(static_cast<T*>(m_Systems.back().get()));
			//}

			return static_cast<T*>(m_Systems.back().get());
		}

	private:
		SceneManager* m_SceneManager = nullptr;

		std::vector<std::unique_ptr<System>> m_Systems;
		std::vector<IFixedUpdatable*> m_FixedUpdatables;
		std::vector<IUpdatable*>m_Updatables;
		//std::vector<IRenderable*> m_Renderables;
	};

