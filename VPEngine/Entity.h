#pragma once
#include "pch.h"
	struct Component;
	class Entity
	{
	public:
		Entity() = default;
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
	private:
		template<typename T> requires std::derived_from<T, Component>
		void AddComponent(Component* comp)
		{
			/// 메타를 사용하여 T를 타입추론해서 ID값에 해당하는 컴포넌트를 추가할수 있을듯함.
			entt::id_type idNum = Reflection::GetTypeID<T>();
			VP_ASSERT(!HasComponent<T>(), "컴포넌트가 존재합니다");
			m_OwnedComp[idNum] = comp;
		}

		void AddComponent(entt::id_type compID,Component* comp)
		{
			/// 메타를 사용하여 T를 타입추론해서 ID값에 해당하는 컴포넌트를 추가할수 있을듯함.
			VP_ASSERT(!HasComponent(compID), "컴포넌트가 존재합니다");
			m_OwnedComp[compID] = comp;
		}


		void SetEntityID(uint32_t entityid) { m_EntityID = entityid; }


		template<typename T>
		inline T* FindComponent()
		{
			return (T*)m_OwnedComp[Reflection::GetTypeID<T>()];
		}
		Component* FindComponent(entt::id_type compID)
		{
			return m_OwnedComp[compID];
		}

		template<typename T>
		void RemoveComponent()
		{
			auto it = m_OwnedComp.find(Reflection::GetTypeID<T>());
			if (it != m_OwnedComp.end())
				m_OwnedComp.erase(it);
			else
			{
				VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
			}
		}
		void RemoveComponent(entt::id_type compID)
		{
			auto it = m_OwnedComp.find(compID);
			if (it != m_OwnedComp.end())
				m_OwnedComp.erase(it);
			else
			{
				VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
			}
		}




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
