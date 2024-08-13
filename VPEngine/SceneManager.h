#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "EventSubscriber.h"
struct PrefabData
{
	std::string prefabname;
	VPMath::Vector3 pos;
	VPMath::Vector3 direction;
	VPMath::Vector3 scale;
};


class SceneManager :public EventSubscriber
{
public:
	SceneManager();
	~SceneManager();
	void Initialize();
	void Finalize();
	// ��ƼƼ�� �����Ѵ�.
	void DeleteEntity(uint32_t entityID);

	void SpawnPrefab(std::string prefabname, VPMath::Vector3 pos = { 0,0,0 }, VPMath::Vector3 direction = {0,0,1}, VPMath::Vector3 scele = { 1,1,1 });
	void SerializePrefab(uint32_t entityID);
	void DeSerializePrefab(std::string filePath);
	
	/// �ش� json�� Deserialize �Ѵ�.
	// ��ƼƼ�� CreateEvnet�� ȣ�� �ϰ�, Entity�� ��ȯ�ϴ� �Լ�.
	std::shared_ptr<Entity> CreateEntity();
	//std::shared_ptr<Entity> CreateEntity(uint32_t id);
	uint32_t CreateRandomEntityID();

	void SetScenePhysic(VPPhysics::PhysicsInfo physicInfo);
	VPPhysics::PhysicsInfo GetScenePhysic();

	//std::vector<std::shared_ptr<Component>> GetOwnedComponents(uint32_t EntityID) { return GetEntity(EntityID)->GetOwnedComponents(); }


	template<typename T>
	bool HasComponent(uint32_t entityID)
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
	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }	///�̰� üũ

	template<typename T>
	T* GetComponent(uint32_t EntityID)
	{
		//return GetEntity(EntityID)->GetComponent<T>();

		// ĳ�õ� ������Ʈ ID�� �����Ͽ� �ݺ� ȣ���� ����
		static const auto componentTypeID = Reflection::GetTypeID<T>();

		// ĳ�ø� ���� Ȯ��
		auto cachedComponent = m_ComponentCache.find({ EntityID, componentTypeID });
		if (cachedComponent != m_ComponentCache.end())
		{
			return static_cast<T*>(cachedComponent->second);
		}

		// ��ƼƼ�� ������
		auto entity = GetEntity(EntityID);
		// ��ƼƼ���� ������Ʈ�� ������
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

	const std::string& GetSceneName() { return m_CurrentScene->SceneName; }



	void SetSceneName(const std::string& sceneName) { m_CurrentScene->SceneName = sceneName; }

	template<typename T>
	inline std::vector<std::reference_wrapper<T>> GetComponentPool();

	void AddChild(uint32_t Parent, uint32_t Child);
	void RemoveParent(uint32_t child, bool Immediate = false);
	bool CheckParent(uint32_t parent, uint32_t child);

protected:
	friend class CompIter;
private:
	std::unordered_map<uint32_t, std::shared_ptr<Entity>>& GetEntityMap()
	{
		return m_CurrentScene->EntityMap;
	}
	void OpenNewScene();
	void SceneSerialize(std::string FilePath);
	void SceneDeSerialize(std::string FilePath);
	void OnAddChild(std::any data);
	void OnRemoveChild(std::any data);
	void OnSpawnPrefab(std::any pair);

	void SetEntityMap(uint32_t entityID, std::shared_ptr<Entity> entity) { m_CurrentScene->EntityMap[entityID] = entity; }
	void AddCompToPool(std::shared_ptr<Component> comp);
	// ���ο� ���� ����.
	void OnNewScene(std::any data);
	// ���ο� ���� ����.
	void OnOpenNewScene(std::any null);

	// ���� �ٲ۴�.
	void OnChangeScene(std::any data);
	// �� ���� �̺�Ʈ�� ȣ���Ѵ�.
	void OnStartScene(std::any data);

	// �� ������ �̺�Ʈ�� ȣ���Ѵ�.
	void OnEndScene(std::any data);
	// �ش� ���� ����.
	void OnOpenScene(std::any data);
	//������� Temp�� ������ ������.
	void OnOverwriteTempToCurrent(std::any data);
	void OnSaveCurrentToTemp(std::any data);
	// ��� Entity�� �����.
	void OnAddCompToScene(std::any data);
	// Entity�� �����Ѵ�.
	void OnDestroyEntity(std::any entityID);

	void RemoveEntity(std::shared_ptr<Entity> entity);
	// ��� Entity�� �����.
	void OnClearAllEntity(std::any data);
	// �ش� Component �����Ѵ�.
	void OnRemoveComponent(std::any data);
	// �ش� Prefab�� serialize�Ѵ�.
	void OnSerializePrefab(std::any entityID);
	// �ش� Prefab�� Deserialize�Ѵ�.
	void OnDeSerializePrefab(std::any data);
	//Entity�� Deserialize�Ѵ� : Map ����.
	std::shared_ptr<Entity> DeSerializeEntity(const nlohmann::json entityjson);
	//void OnDeSerializeEntity(std::any data);

	template<typename T>
	void ReleaseCompFromPool(std::shared_ptr<T> comp);
	inline void ReleaseCompFromPool(entt::id_type compID, std::shared_ptr<Component> comp);
	inline void ReleaseCompFromPool(entt::id_type compID, Component* comp);

	friend class SceneSerializer;

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
};


template<typename T>
inline std::vector<std::reference_wrapper<T>> SceneManager::GetComponentPool()
{
	std::vector<std::reference_wrapper<T>> result;

	auto it = m_CurrentScene->m_ComponentPool.find(Reflection::GetTypeID<T>());
	if (it != m_CurrentScene->m_ComponentPool.end())
	{
		result.reserve(it->second.size()); // �޸� �Ҵ� ����ȭ

		for (auto& weakComp : it->second)
		{
			if (auto sharedComp = weakComp.lock())
			{
				result.push_back(*std::dynamic_pointer_cast<T>(sharedComp));
			}
		}
	}

	return result; // RVO (Return Value Optimization) �Ǵ� std::move�� ����� �� ����
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