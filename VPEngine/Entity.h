#pragma once
#include "pch.h"
	class EntityManager;
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
			if (HasComponent<T>())
			{
				T* TtypeComponent = FindComponent<T>();
				return TtypeComponent;
			}
			else 
			{
				VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
				return nullptr;
			}
		}

		template<typename T>
		bool HasComponent() requires std::derived_from<T, Component>
		{
			return m_OwnedComp.count(Reflection::GetTypeID<T>()) > 0;
		}
		const uint32_t GetEntityID() { return m_EntityID; }
	private:
		/// <summary>
		/// 이걸로만 추가되지 않으니 주의
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="comp"></param>
		template<typename T> requires std::derived_from<T, Component>
		void AddComponent(T* comp)
		{
			/// 메타를 사용하여 T를 타입추론해서 ID값에 해당하는 컴포넌트를 추가할수 있을듯함.
			entt::id_type idNum = Reflection::GetTypeID<T>();
			VP_ASSERT(!HasComponent<T>(), "컴포넌트가 존재합니다");
			m_OwnedComp[idNum] = comp;
		}
		void SetEntityID(uint32_t entityid) { m_EntityID = entityid; }




		template<typename T>
		inline T* FindComponent()
		{
			return (T*)m_OwnedComp[Reflection::GetTypeID<T>()];
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
		friend class EntityManager;
		friend class SceneSerializer;
	};
