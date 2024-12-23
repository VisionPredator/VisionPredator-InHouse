#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "EventSubscriber.h"
#include "NavMeshData.h"
struct PrefabData
{
	uint32_t MainEntityID{};
	std::string prefabname{};
	VPMath::Vector3 pos{};
	VPMath::Vector3 rotation{};
	VPMath::Vector3 scale{};
};


class SceneManager :public EventSubscriber
{
public:
	SceneManager();
	~SceneManager();
	void Initialize();
	void Finalize();
	// 엔티티를 삭제한다.
	void DeleteEntity(uint32_t entityID);
	void ChangeScene(std::string FilePath, bool Immidiate=false);
	void SpawnPrefab(std::string prefabname, VPMath::Vector3 pos = { 0,0,0 }, VPMath::Vector3 rotation = {0,0,0}, VPMath::Vector3 scele = { -1,-1,-1 });
	std::shared_ptr<Entity> SpawnEditablePrefab(std::string prefabname, VPMath::Vector3 pos = { 0,0,0 }, VPMath::Vector3 rotation = { 0,0,0 }, VPMath::Vector3 scele = { -1,-1,-1 });
	//std::shared_ptr<Entity> SpawnSoundEntity(std::string soundName,float volume,bool isloop=false, VPMath::Vector3 pos = { 0,0,0 });
	std::shared_ptr<Entity> SpawnSoundEntity(std::string soundName,int volume,bool Is2D, bool isloop=false, VPMath::Vector3 pos = { 0,0,0 });
	std::shared_ptr<Entity> SpawnEditablePrefab(std::string prefabname, VPMath::Vector3 pos = { 0,0,0 }, VPMath::Quaternion Quater = VPMath::Quaternion::Identity, VPMath::Vector3 scele = { -1,-1,-1 });

	void SerializePrefab(uint32_t entityID, bool immidiate = false);
	void DeSerializePrefab(std::string filePath);
	void SetScenePhysic(VPPhysics::PhysicsInfo physicInfo);
	VPPhysics::PhysicsInfo GetScenePhysic();

	//std::vector<std::shared_ptr<Component>> GetOwnedComponents(uint32_t EntityID) { return GetEntity(EntityID)->GetOwnedComponents(); }


	template<typename T>
	bool HasComponent(uint32_t entityID) requires std::derived_from<T, Component>
	{
		auto entity = GetEntity(entityID);
		return entity ? entity->HasComponent<T>() : false;
	}
	bool HasComponent(uint32_t EntityID, entt::id_type compid) 
	{
		if (!HasEntity(EntityID))
			return false;
		return GetEntity(EntityID)->HasComponent(compid);
	}
	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }	///이거 체크

	template<typename T> 
	T* GetComponent(uint32_t EntityID)requires std::derived_from<T, Component>
	{
		//return GetEntity(EntityID)->GetComponent<T>();

		// 캐시된 컴포넌트 ID를 저장하여 반복 호출을 줄임
		static const auto componentTypeID = Reflection::GetTypeID<T>();

		// 캐시를 먼저 확인
		auto cachedComponent = m_ComponentCache.find({ EntityID, componentTypeID });
		if (cachedComponent != m_ComponentCache.end())
		{
			return static_cast<T*>(cachedComponent->second);
		}

		// 엔티티를 가져옴
		auto entity = GetEntity(EntityID);
		// 엔티티에서 컴포넌트를 가져옴
		T* component = entity->GetComponent<T>();
		m_ComponentCache[{EntityID, componentTypeID}] = component;
		return component;
	}

	Component* GetComponent(uint32_t entityID, entt::id_type compId)
	{
		return GetEntity(entityID)->GetComponent(compId);
	}

	std::shared_ptr<Entity> GetEntity(uint32_t entityID)
	{
		auto it = m_CurrentScene->EntityMap.find(entityID);
		return it != m_CurrentScene->EntityMap.end() ? it->second : nullptr;
	}

	std::shared_ptr<Entity> GetChildEntityByName(uint32_t entityID,std::string name);
	std::shared_ptr<Entity> GetRelationEntityByName(uint32_t entityID,std::string name);
	std::shared_ptr<Entity> GetEntityByIdentityName(std::string name);
	template<typename T>
	T* GetParentEntityComp_HasComp(uint32_t entityID) requires std::derived_from<T, Component>;
	template<typename T>
	std::vector<T*>  GetChildEntityComps_HasComp(uint32_t entityID) requires std::derived_from<T, Component>;
	template<typename T>
	T*  GetChildEntityComp_HasComp(uint32_t entityID) requires std::derived_from<T, Component>;

	const std::string& GetSceneName() { return m_CurrentScene->SceneName; }
	const BuildSettings& GetSceneBuildSettrings() { return m_CurrentScene->NavBuildSetting; }


	void SetSceneName(const std::string& sceneName) { m_CurrentScene->SceneName = sceneName; }
	void SetSceneBuildSettrings(BuildSettings navbuildsetting) { m_CurrentScene->NavBuildSetting = navbuildsetting; }

	template<typename T>
	inline std::vector<std::reference_wrapper<T>> GetComponentPool();
	void DestroyEntity(uint32_t entityID, bool Immidiate = false);

	void AddChild(uint32_t Parent, uint32_t Child, bool Immidiate=false);
	void RemoveParent(uint32_t child, bool Immediate = false);
	bool CheckParent(uint32_t parent, uint32_t child);

protected:
	friend class CompIter;
private:
	std::shared_ptr<Entity> ClonePrefab(const std::string& prefabname, PrefabData& prefabData);
	std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> m_PrefabCache;
	/// 해당 json를 Deserialize 한다.
// 엔티티를 CreateEvnet를 호출 하고, Entity를 반환하는 함수.
	std::shared_ptr<Entity> CreateEntity();
	std::shared_ptr<Entity> CreateEntity(std::string entityName);
	uint32_t CreateRandomEntityID();
	std::shared_ptr<NavMeshData> GetSceneNavMeshData() { return m_CurrentScene->SceneNavData; }
	void SetSceneNavMeshData(std::shared_ptr<NavMeshData> navMeshdata) { m_CurrentScene->SceneNavData = navMeshdata; }
	void ResetNavMeshData() { m_CurrentScene->SceneNavData.reset(); }
	std::unordered_map<uint32_t, std::shared_ptr<Entity>>& GetEntityMap()
	{
		return m_CurrentScene->EntityMap;
	}
	void OpenNewScene(bool Immidiate = false);
	void SceneSerialize(std::string FilePath, bool Immidiate = false);
	void SceneDeSerialize(std::string FilePath, bool Immidiate = false);
	void OnAddChild(std::any data);
	void OnRemoveChild(std::any data);
	void OnSpawnPrefab(std::any pair);

	void SetEntityMap(uint32_t entityID, std::shared_ptr<Entity> entity) { m_CurrentScene->EntityMap[entityID] = entity; }
	void AddCompToPool(std::shared_ptr<Component> comp);
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
	void OnAddEntityComponentsToScene(std::any entityID);
	void OnSaveCurrentToTemp(std::any data);
	// 모든 Entity를 지운다.
	void OnAddCompToScene(std::any data);

	// Entity를 삭제한다.
	void OnDeleteEntity(std::any entityID);
	void OnDestroyEntity(std::any entityID);

	void ClearEntity(std::shared_ptr<Entity> entity);
	// 모든 Entity를 지운다.
	void ClearAllEntity(bool Immidiate = false);

	void OnClearAllEntity(std::any data);
	// 해당 Component 삭제한다.
	void OnRemoveComponent(std::any data);
	// 해당 Prefab을 serialize한다.
	void OnSerializePrefab(std::any entityID);
	// 해당 Prefab을 Deserialize한다.
	void OnDeSerializePrefab(std::any data);
	//Entity를 Deserialize한다 : Map 전용.
	std::shared_ptr<Entity> DeSerializeEntity(const nlohmann::json entityjson,bool Immidate=false);
	//void OnDeSerializeEntity(std::any data);

	template<typename T>
	void ReleaseCompFromPool(std::shared_ptr<T> comp);
	inline void ReleaseCompFromPool(entt::id_type compID, std::shared_ptr<Component> comp);
	inline void ReleaseCompFromPool(entt::id_type compID, Component* comp);

	friend class SceneSerializer;
	friend class NavAgentSystem;

	std::pair<uint32_t, uint32_t>& findOrCreatePair(std::vector<std::pair<uint32_t, uint32_t>>& vec, uint32_t key);

	Scene* m_CurrentScene = nullptr;
	void ClearCache()
	{
		m_ComponentCache.clear();
	}
	struct pair_hash
	{
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& pair) const
		{
			auto hash1 = std::hash<T1>{}(pair.first);
			auto hash2 = std::hash<T2>{}(pair.second);
			return hash1 ^ hash2;
		}
	};
	std::unordered_map<std::pair<uint32_t, entt::id_type>, Component*, pair_hash> m_ComponentCache;

	friend class Toolbar;
	friend class SceneSerializer;
	friend class NavMeshSystem;
	friend class Hierarchy;
	friend class HierarchySystem;
};


template<typename T>
inline T* SceneManager::GetParentEntityComp_HasComp(uint32_t entityID) requires std::derived_from<T, Component>
{
	auto mainentity = GetEntity(entityID);
	while (mainentity)
	{
		if (mainentity->HasComponent<T>())
			return mainentity->GetComponent<T>();
		else if (mainentity->HasComponent<Parent>())
			mainentity = GetEntity(mainentity->GetComponent<Parent>()->ParentID);
		else
			return nullptr;
	}
	return nullptr;
}
template<typename T>
inline std::vector<T*>  SceneManager::GetChildEntityComps_HasComp(uint32_t entityID) requires std::derived_from<T, Component>
{
	std::vector<T*> components;

	// Retrieve the main entity
	auto mainEntity = GetEntity(entityID);

	if (!mainEntity)
		return components;  // No children to process

	if (mainEntity->HasComponent<T>())
		components.push_back(mainEntity->GetComponent<T>());

	if (!mainEntity->HasComponent<Children>())
		return components;
	// Function to recursively collect components from child entities
	std::function<void(uint32_t)> collectComponents = [&](uint32_t currentEntityID) 
		{
		auto currentEntity = GetEntity(currentEntityID);
		if (!currentEntity)
			return;

		// If the current entity has the component T, add it to the list
		if (currentEntity->HasComponent<T>())
			components.push_back(currentEntity->GetComponent<T>());

		// If the current entity has children, continue recursively
		if (currentEntity->HasComponent<Children>()) 
		{
			for (auto childID : currentEntity->GetComponent<Children>()->ChildrenID)
			{
				collectComponents(childID);
			}
		}
		};

	// Start collecting from the main entity's children
	for (auto childID : mainEntity->GetComponent<Children>()->ChildrenID)
	{
		collectComponents(childID);
	}

	return components;
}

template<typename T>
inline T* SceneManager::GetChildEntityComp_HasComp(uint32_t entityID) requires std::derived_from<T, Component> {
	// 메인 엔티티를 가져옵니다
	auto mainEntity = GetEntity(entityID);
	if (!mainEntity)
		return nullptr;

	// 메인 엔티티 자체에 컴포넌트 T가 있는지 확인합니다
	if (mainEntity->HasComponent<T>())
		return mainEntity->GetComponent<T>();

	// 엔티티에 자식이 있는지 확인합니다
	if (!mainEntity->HasComponent<Children>())
		return nullptr;

	// 자식 엔티티에서 컴포넌트를 재귀적으로 수집하는 함수
	std::function<T* (uint32_t)> collectComponents = [&](uint32_t currentEntityID) -> T* {
		auto currentEntity = GetEntity(currentEntityID);
		if (!currentEntity)
			return nullptr;

		// 현재 엔티티에 컴포넌트 T가 있으면 반환합니다
		if (currentEntity->HasComponent<T>())
			return currentEntity->GetComponent<T>();

		// 현재 엔티티에 자식이 있으면 재귀적으로 탐색을 계속합니다
		if (currentEntity->HasComponent<Children>()) {
			for (auto childID : currentEntity->GetComponent<Children>()->ChildrenID) {
				T* result = collectComponents(childID);
				if (result)
					return result; // 찾으면 탐색을 중지하고 반환
			}
		}
		return nullptr;
		};

	// 메인 엔티티의 자식들부터 수집을 시작합니다
	for (auto childID : mainEntity->GetComponent<Children>()->ChildrenID) {
		T* result = collectComponents(childID);
		if (result)
			return result;
	}

	return nullptr;
}


template<typename T>
inline std::vector<std::reference_wrapper<T>> SceneManager::GetComponentPool()
{
	std::vector<std::reference_wrapper<T>> result;

	auto it = m_CurrentScene->m_ComponentPool.find(Reflection::GetTypeID<T>());
	if (it != m_CurrentScene->m_ComponentPool.end())
	{
		result.reserve(it->second.size()); // 메모리 할당 최적화

		for (auto& weakComp : it->second)
		{
			if (auto sharedComp = weakComp.lock())
			{
				result.push_back(*std::dynamic_pointer_cast<T>(sharedComp));
			}
		}
	}

	return result; // RVO (Return Value Optimization) 또는 std::move가 적용될 수 있음
}

template<typename T>
inline void SceneManager::ReleaseCompFromPool(std::shared_ptr<T> comp)
{
	auto& pool = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
	pool.erase(std::remove_if(pool.begin(), pool.end(), [&comp](const std::weak_ptr<Component>& weakComp)
		{
			if (auto sharedComp = weakComp.lock())
			{
				return sharedComp == comp;
			}
			return false;
		}), pool.end());
}





inline void SceneManager::ReleaseCompFromPool(entt::id_type compID, std::shared_ptr<Component> comp)
{
	auto& pool = m_CurrentScene->m_ComponentPool[compID];
	pool.erase(std::remove_if(pool.begin(), pool.end(), [&comp](const std::weak_ptr<Component>& weakComp)
		{
			if (auto sharedComp = weakComp.lock())
			{
				return sharedComp == comp;
			}
			return false;
		}), pool.end());
}

inline void SceneManager::ReleaseCompFromPool(entt::id_type compID, Component* comp)
{
	auto& pool = m_CurrentScene->m_ComponentPool[compID];
	pool.erase(std::remove_if(pool.begin(), pool.end(), [comp](const std::weak_ptr<Component>& weakComp)
		{
			if (auto sharedComp = weakComp.lock())
			{
				return sharedComp.get() == comp;
			}
			return false;
		}), pool.end());
}