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
				VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
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
		/// �̰ɷθ� �߰����� ������ ����
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="comp"></param>
		template<typename T> requires std::derived_from<T, Component>
		void AddComponent(T* comp)
		{
			/// ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
			entt::id_type idNum = Reflection::GetTypeID<T>();
			VP_ASSERT(!HasComponent<T>(), "������Ʈ�� �����մϴ�");
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
				VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
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
