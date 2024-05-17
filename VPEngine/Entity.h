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
			/// ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
			entt::id_type idNum = Reflection::GetTypeID<T>();
			VP_ASSERT(!HasComponent<T>(), "������Ʈ�� �����մϴ�");
			m_OwnedComp[idNum] = comp;
		}

		void AddComponent(entt::id_type compID,Component* comp)
		{
			/// ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
			VP_ASSERT(!HasComponent(compID), "������Ʈ�� �����մϴ�");
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
				VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
			}
		}
		void RemoveComponent(entt::id_type compID)
		{
			auto it = m_OwnedComp.find(compID);
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
		friend class SceneSerializer;
		friend class SceneManager;
	};
