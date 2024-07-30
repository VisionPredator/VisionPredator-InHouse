#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
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


	void SerializePrefab(uint32_t entityID);
	void DeSerializePrefab(std::string filePath);
	/// 해당 json를 Deserialize 한다.
	// 엔티티를 CreateEvnet를 호출 하고, Entity를 반환하는 함수.
	Entity* CreateEntity();
	Entity* CreateEntity(uint32_t id);
	uint32_t CreateRandomEntityID();

	void SetScenePhysic(VPPhysics::PhysicsInfo physicInfo);
	VPPhysics::PhysicsInfo GetScenePhysic();

	std::vector<Component*> GetOwnedComponent(uint32_t EntityID) { return GetEntity(EntityID)->GetOwnedComponent(); }


	template<typename T>
	bool HasComponent(uint32_t EntityID)
	{
		if (!HasEntity(EntityID))
			return false;
		return GetEntity(EntityID)->HasComponent<T>();
	}
	bool HasComponent(uint32_t EntityID, entt::id_type compid)
	{
		if (!HasEntity(EntityID))
			return false;
		return GetEntity(EntityID)->HasComponent(compid);
	}
	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }

	template<typename T>
	T* GetComponent(uint32_t EntityID)
	{
		if (!HasEntity(EntityID))
			return nullptr;
		return GetEntity(EntityID)->GetComponent<T>();
	}
	Component* GetComponent(uint32_t EntityID, entt::id_type compId)
	{
		if (!HasEntity(EntityID))
			return nullptr;
		return GetEntity(EntityID)->GetComponent(compId);
	}
	Entity* GetEntity(uint32_t entityID)
	{
		if (!HasEntity(entityID))
			return nullptr;
		return m_CurrentScene->EntityMap[entityID];
	}
	std::unordered_map<uint32_t, Entity*>& GetEntityMap()
	{
		return m_CurrentScene->EntityMap;
	}
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }



	void SetSceneName(std::string sceneName) { m_CurrentScene->SceneName = sceneName; }

	template<typename T>
	inline std::vector<T*> GetComponentPool();

	void AddChild(uint32_t Parent, uint32_t Child);
	void RemoveParent(uint32_t child, bool Immediate = false);
protected:
	friend class CompIter;
private:
	void OpenNewScene();
	void SceneSerialize(std::string FilePath);
	void SceneDeSerialize(std::string FilePath);



	bool CheckParent(uint32_t parent, uint32_t child);
	void OnAddChild(std::any data);
	void OnRemoveChild(std::any data);

	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }

	void AddCompToPool(Component* comp);
	// 새로운 씬을 연다.
	void OnNewScene(std::any data);

	// 새로운 씬을 연다.
	void OnOpenNewScene(std::any null);

	// 씬을 바꾼다.
	void OnChangeScene(std::any data);
	// 씬 시작 이벤트를 호출한다.
	void OnStartScene(std::any data);

	// 씬 끝나는 이벤트를 호출한다.
	void OnEndScene(std::any data);
	// 해당 씬을 연다.
	void OnOpenScene(std::any data);
	//현재씬에 Temp씬 데이터 덮어씌우기.
	void OnOverwriteTempToCurrent(std::any data);
	void OnSaveCurrentToTemp(std::any data);
	// 모든 Entity를 지운다.
	void OnAddCompToScene(std::any data);
	// Entity를 삭제한다.
	void OnDestroyEntity(std::any entityID);

	void RemoveEntity(Entity* entity);
	// 모든 Entity를 지운다.
	void OnClearAllEntity(std::any data);
	// 해당 Component 삭제한다.
	void OnRemoveComponent(std::any data);
	// 해당 Prefab을 serialize한다.
	void OnSerializePrefab(std::any entityID);
	// 해당 Prefab을 Deserialize한다.
	void OnDeSerializePrefab(std::any data);
	//Entity를 Deserialize한다 : Map 전용.
	Entity* DeSerializeEntity(const nlohmann::json entityjson);
	void OnDeSerializeEntity(std::any data);

	template<typename T>
	void ReleaseCompFromPool(T* comp);
	inline void ReleaseCompFromPool(entt::id_type compID, Component* comp);

	friend class SceneSerializer;

	std::pair<uint32_t, uint32_t>& findOrCreatePair(std::vector<std::pair<uint32_t, uint32_t>>& vec, uint32_t key);

	Scene* m_CurrentScene = nullptr;

	friend class Toolbar;
};



template<typename T>
inline std::vector<T*> SceneManager::GetComponentPool()
{
	auto& baseVec = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
	std::vector<T*> result;
	result.reserve(baseVec.size()); // 예비 메모리 할당
	for (Component* comp : baseVec)
		result.push_back(static_cast<T*>(comp));
	return result;
}

template<typename T>
inline void SceneManager::ReleaseCompFromPool(T* comp)
{
	auto& pool = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
	auto it = std::find(pool.begin(), pool.end(), comp);
	if (it != pool.end())
	{
		std::iter_swap(it, pool.end() - 1); // 해당 컴포넌트를 마지막 컴포넌트와 교환
		pool.pop_back(); // 마지막 컴포넌트를 풀에서 제거
	}
}
inline void SceneManager::ReleaseCompFromPool(entt::id_type compID, Component* comp)
{
	auto& pool = m_CurrentScene->m_ComponentPool[compID];
	auto it = std::find(pool.begin(), pool.end(), comp);
	if (it != pool.end())
	{
		delete* it; // 포인터가 가리키는 실제 객체를 삭제
		std::iter_swap(it, pool.end() - 1); // 해당 컴포넌트를 마지막 컴포넌트와 교환
		pool.pop_back(); // 마지막 컴포넌트를 풀에서 제거
	}
}