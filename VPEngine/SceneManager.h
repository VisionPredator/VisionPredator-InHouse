#pragma once
#include "Scene.h"
#include "Entity.h"

#include "EventSubscriber.h"
class SceneManager :public EventSubscriber
{
public:
	SceneManager();
	~SceneManager();
	void Initialize();
	void Finalize();
	// 엔티티를 삭제한다.
	void DeleteEntity(uint32_t entityID);

	void OnDestroyEntity(std::any entityID);
	// 모든 Entity를 지운다.
	void OnClearEntity(std::any data);
	// 씬을 바꾼다.
	void OnChangeScene(std::any data);
	// 씬 시작 이벤트를 호출한다.
	void OnStartScene(std::any data);
	// 씬을 삭제 후 생성한다.
	void OnResetScene(std::any data);
	// 씬 끝나는 이벤트를 호출한다.
	void OnEndScene(std::any data);
	// 해당 씬을 연다.
	void OnOpenScene(std::any data);
	// 모든 Entity를 지운다.
	void OnAddComponent(std::any data);
	// 해당 Component 삭제한다.
	void OnRemoveComponent(std::any data);
	// 해당 Entity를 맵에 추가한다.
	void OnSetEntityMap(std::any data);
	// 해당 Prefab을 serialize한다.
	void OnSerializePrefab(std::any entityID);
	// 해당 Prefab을 serialize한다.
	void SerializePrefab(uint32_t entityID);
	/// 해당 json를 Deserialize 한다.
	Entity* DeSerializeEntity(const nlohmann::json entityjson);

	// 엔티티를 CreateEvnet를 호출 하고, Entity를 반환하는 함수.
	Entity* CreateEntity();




#pragma region Finished Fuctions


	template<typename T>
	T* GetComponent(uint32_t EntityID) { return GetEntity(EntityID)->GetComponent<T>(); }
	Component* GetComponent(uint32_t EntityID, entt::id_type compId) { return GetEntity(EntityID)->GetComponent(compId); }

	template<typename T>
	bool HasComponent(uint32_t EntityID) { return GetEntity(EntityID)->HasComponent<T>(); }
	bool HasComponent(uint32_t EntityID, entt::id_type compid) { return GetEntity(EntityID)->HasComponent(compid); }

	template<typename T>
	std::vector<T*> GetComponentPool()
	{
		auto& baseVec = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		std::vector<T*> result;
		result.reserve(baseVec.size()); // 예비 메모리 할당
		for (Component* comp : baseVec)
			result.push_back(static_cast<T*>(comp));
		return result;
	}

	template<typename T>
	void AddCompToPool(Component* comp) { m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()].push_back(comp); }
	void AddCompToPool(entt::id_type compID, Component* comp) { m_CurrentScene->m_ComponentPool[compID].push_back(comp); }


	template<typename T>
	void ReleaseCompFromPool(T* comp)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		auto it = std::find(pool.begin(), pool.end(), comp);
		if (it != pool.end())
		{
			std::iter_swap(it, pool.end() - 1); // 해당 컴포넌트를 마지막 컴포넌트와 교환
			pool.pop_back(); // 마지막 컴포넌트를 풀에서 제거
		}
	}

	void ReleaseCompFromPool(entt::id_type compID,Component* comp)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[compID];
		auto it = std::find(pool.begin(), pool.end(), comp);
		if (it != pool.end())
		{
			std::iter_swap(it, pool.end() - 1); // 해당 컴포넌트를 마지막 컴포넌트와 교환
			pool.pop_back(); // 마지막 컴포넌트를 풀에서 제거
		}
	}




	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }


	Entity* GetEntity(uint32_t entityID) { return m_CurrentScene->EntityMap[entityID]; }
	std::unordered_map<uint32_t, Entity*>& GetEntityMap() { return m_CurrentScene->EntityMap; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }

	void SetSceneName(std::string sceneName) { m_CurrentScene->SceneName = sceneName; }
	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }

#pragma region 삭제된 함수

	//template<typename T>
	//void RemoveComponent(uint32_t EntityID);

#pragma endregion

#pragma endregion
	template<typename T>
	inline T* AddComponent(uint32_t entityID)
	{

		Entity* ParentEntity = GetEntity(entityID);///GetEntity
		VP_ASSERT(!HasComponent<T>(entityID), "같은 타입의 컴포넌트가 존재합니다.");
		T* comp = new T;

		///아래껄 하지말고 바로 이벤트 호출 하는것도 좋을 듯!!!!
		comp->OwnedEntity = ParentEntity;
		ParentEntity->AddComponent<T>(comp);
		AddCompToPool<T>(comp);
		std::pair<uint32_t, entt::id_type> data = { ParentEntity ->GetEntityID(),comp->GetTypeID()};
		EventManager::GetInstance().ImmediateEvent("OnAddedComponent", data);
		return comp;
	}
protected:
	private:
	Scene* m_CurrentScene = nullptr;

};

#pragma region 삭제된 함수

//template<typename T>
//inline void SceneManager::RemoveComponent(uint32_t EntityID)
//{
//	VP_ASSERT(HasComponent<T>(EntityID), "해당 타입의 컴포넌트가 존재하지 않습니다.");
//	Entity& ParentEntity = GetEntity(EntityID);///GetEntity
//	T* Component = ParentEntity.GetComponent<T>();
//	ReleaseCompFromPool<T>(Component);
//	ParentEntity.RemoveComponent<T>();
//}

#pragma endregion