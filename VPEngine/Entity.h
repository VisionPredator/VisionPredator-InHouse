#pragma once
#include "pch.h"
#include "EventManager.h"
	struct Component;
	class Entity
	{
	public:
		Entity();
		Entity(uint32_t entityID);
		Entity(const Entity& other) = default;
		~Entity();


		template<typename T>
		T* GetComponent() requires std::derived_from<T, Component>
		{
				return FindComponent<T>();
		}
		Component* GetComponent(entt::id_type compID)
		{
			return FindComponent(compID);
		}

		template<typename T>
		bool HasComponent() requires std::derived_from<T, Component>
		{
			return m_OwnedComp.count(Reflection::GetTypeID<T>()) > 0;
		}
		bool HasComponent(entt::id_type compid) { return m_OwnedComp.count(compid) > 0; }

		const uint32_t GetEntityID() { return m_EntityID; }

		void AddComponentToMap(Component* comp);

		template<typename T>
		T* AddComponent(bool Immediately = false)
		{
			if (HasComponent<T>())
			{
				VP_ASSERT(false, "이미 있는 Component 입니다.");
					return nullptr;
			}
			T* newcomp = new T;
			AddComponentToMap(newcomp);
			newcomp->SetEntity(this);
			if (!Immediately)
			EventManager::GetInstance().ScheduleEvent("OnAddCompToScene", static_cast<Component*>(newcomp));
			else
				EventManager::GetInstance().ImmediateEvent("OnAddCompToScene", static_cast<Component*>(newcomp));
			return newcomp;
		}
		Component* AddComponent(entt::id_type compID);
		template<typename T>
		void RemoveComponent()
		{
			EventManager::GetInstance().ScheduleEvent("OnRemoveComp_Scene", GetComponent(Reflection::GetTypeID<T>()));

		}

		void RemoveComponent(entt::id_type compID)
		{
			EventManager::GetInstance().ScheduleEvent("OnRemoveComp_Scene", GetComponent(compID));
		}

	private:
		std::vector<Component*> GetOwnedComponent()
		{
			std::vector<Component*> tempComponentPool;
			for (auto& component : m_OwnedComp)
			{
				tempComponentPool.push_back(component.second);
			}
			return tempComponentPool;
		}
		void SetEntityID(uint32_t entityid) { m_EntityID = entityid; }

		template<typename T>
		inline T* FindComponent()
		{
			if (!HasComponent<T>())
			{
				return nullptr;
			}
			auto it = m_OwnedComp.find(Reflection::GetTypeID<T>());
			return static_cast<T*>(it->second);
		}
		Component* FindComponent(entt::id_type compID)
		{
			if (!HasComponent(compID))
			{
				VP_ASSERT(false,"Component not found in m_OwnedComp");
				return nullptr;
			}

			auto it = m_OwnedComp.find(compID);
			return it->second;
		}

	
		void ReleaseComponent(Component* comp);



		operator bool() const { return m_EntityID; }
		operator uint32_t() const { return m_EntityID; }
		bool operator==(const Entity& other) const
		{
			return m_EntityID == other.m_EntityID;/* && m_Scene == other.m_Scene;*/
		}
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}



		uint32_t m_EntityID{};
		std::unordered_map<entt::id_type, Component*> m_OwnedComp;
		friend class SceneSerializer;
		friend class SceneManager;
	};
