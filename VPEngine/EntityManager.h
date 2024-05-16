#pragma once
#include "SceneManager.h"
#include "Entity.h"


	class Scene;
	class SceneManager;
	class EntityManager: public EventSubscriber
	{
	public:
		friend class Entity;
		EntityManager();
		~EntityManager();
		void Initalize();
		void Finalize();
		Entity* CreateEntity();

		/// 
		/// 
		/// 
		/// 
		void SerializePrefab(uint32_t entityID);
		Entity* DeSerializeEntity(const nlohmann::json entityjson);
		template<typename T, typename... Args>
		T* AddComponent(uint32_t EntityID, Args&&... args);
		template<typename T>
		void RemoveComponent(uint32_t EntityID);

		template<typename T>
		T* GetComponent(uint32_t EntityID) { return m_SceneManager->GetEntity(EntityID)->GetComponent<T>(); }
		const Scene& GetScene() const { return m_SceneManager->GetScene(); }
		const std::string GetSceneName() const { return m_SceneManager->GetSceneName(); }
		Entity* GetEntity(uint32_t EntityID) { return m_SceneManager->GetEntity(EntityID); }
		std::unordered_map<uint32_t, Entity*> GetEntityMap() { return m_SceneManager->GetEntityMap(); }
		template<typename T>
		std::vector<T*> GetComponentPool() { return m_SceneManager->GetComponentpool<T>(); }

		void SetSceneName(std::string scenename) const { m_SceneManager->SetSceneName(scenename); }
		template<typename T>
		bool HasComponent(uint32_t EntityID) { return m_SceneManager->GetEntity(EntityID)->HasComponent<T>(); }
	private:
		SceneManager* m_SceneManager;
	};



	template<typename T, typename ...Args>
	inline T* EntityManager::AddComponent(uint32_t EntityID, Args && ...args)
	{
		Entity* ParentEntity = m_SceneManager->GetEntity(EntityID);///GetEntity
		VP_ASSERT(!HasComponent<T>(EntityID), "같은 타입의 컴포넌트가 존재합니다.");
		T* component = new T;
		component->OwnedEntity = ParentEntity;
		ParentEntity->AddComponent<T>(component);
		m_SceneManager->AddCompToPool<T>(component);
		return component;
	}


	/// TODO : 미완
	template<typename T>
	inline void EntityManager::RemoveComponent(uint32_t EntityID)
	{
		VP_ASSERT(HasComponent<T>(EntityID), "해당 타입의 컴포넌트가 존재하지 않습니다.");
		Entity& ParentEntity = m_SceneManager->GetEntity(EntityID);///GetEntity
		T* Component = ParentEntity.GetComponent<T>();
		m_SceneManager->ReleaseCompFromPool<T>(Component);
		ParentEntity.RemoveComponent<T>();
	}

