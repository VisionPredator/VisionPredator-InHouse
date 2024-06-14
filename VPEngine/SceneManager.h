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
	// ��ƼƼ�� �����Ѵ�.
	void DeleteEntity(uint32_t entityID);


	void SerializePrefab(uint32_t entityID);
	void DeSerializePrefab(std::string filePath);
	/// �ش� json�� Deserialize �Ѵ�.
	// ��ƼƼ�� CreateEvnet�� ȣ�� �ϰ�, Entity�� ��ȯ�ϴ� �Լ�.
	Entity* CreateEntity();
	Entity* CreateEntity(uint32_t id);
	uint32_t CreateRandomEntityID();



	std::vector<Component*> GetOwnedComponent(uint32_t EntityID) { return GetEntity(EntityID)->GetOwnedComponent(); }


	template<typename T>
	bool HasComponent(uint32_t EntityID) { return GetEntity(EntityID)->HasComponent<T>(); }
	bool HasComponent(uint32_t EntityID, entt::id_type compid) { return GetEntity(EntityID)->HasComponent(compid); }
	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }


	template<typename T>
	T* GetComponent(uint32_t EntityID) { return GetEntity(EntityID)->GetComponent<T>(); }
	Component* GetComponent(uint32_t EntityID, entt::id_type compId) { return GetEntity(EntityID)->GetComponent(compId); }
	Entity* GetEntity(uint32_t entityID) { return m_CurrentScene->EntityMap[entityID]; }
	std::unordered_map<uint32_t, Entity*>& GetEntityMap() { return m_CurrentScene->EntityMap; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }

	void SetSceneName(std::string sceneName) { m_CurrentScene->SceneName = sceneName; }
	
	template<typename T>
	inline std::vector<T*> GetComponentPool();

	void AddChild(uint32_t Parent, uint32_t Child);
	void RemoveParent(uint32_t child);
protected:
	friend class CompIter;
private:
	std::pair<uint32_t, uint32_t>& findOrCreatePair(std::vector<std::pair<uint32_t, uint32_t>>& vec, uint32_t key);


	bool CheckParent(uint32_t parent, uint32_t child);
	void OnAddChild(std::any data);
	void OnRemoveChild(std::any data);

	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }

	void AddCompToPool(Component* comp); 

	// ���� �ٲ۴�.
	void OnChangeScene(std::any data);
	// �� ���� �̺�Ʈ�� ȣ���Ѵ�.
	void OnStartScene(std::any data);
	// ���� ���� �� �����Ѵ�.
	void OnResetScene(std::any data);
	// �� ������ �̺�Ʈ�� ȣ���Ѵ�.
	void OnEndScene(std::any data);
	// �ش� ���� ����.
	void OnOpenScene(std::any data);
	// ��� Entity�� �����.
	void OnAddCompToScene(std::any data);
	// Entity�� �����Ѵ�.
	void OnDestroyEntity(std::any entityID);
	// ��� Entity�� �����.
	void OnClearAllEntity(std::any data);
	// �ش� Component �����Ѵ�.
	void OnRemoveComponent(std::any data);
	// �ش� Prefab�� serialize�Ѵ�.
	void OnSerializePrefab(std::any entityID);
	void OnDeSerializePrefab(std::any data);
	// �ش� Prefab�� Deserialize�Ѵ�.
	void OnDeSerializeEntity(std::any data);

	template<typename T>
	void ReleaseCompFromPool(T* comp)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		auto it = std::find(pool.begin(), pool.end(), comp);
		if (it != pool.end())
		{
			std::iter_swap(it, pool.end() - 1); // �ش� ������Ʈ�� ������ ������Ʈ�� ��ȯ
			pool.pop_back(); // ������ ������Ʈ�� Ǯ���� ����
		}
	}

	void ReleaseCompFromPool(entt::id_type compID, Component* comp)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[compID];
		auto it = std::find(pool.begin(), pool.end(), comp);
		if (it != pool.end())
		{
			delete* it; // �����Ͱ� ����Ű�� ���� ��ü�� ����
			std::iter_swap(it, pool.end() - 1); // �ش� ������Ʈ�� ������ ������Ʈ�� ��ȯ
			pool.pop_back(); // ������ ������Ʈ�� Ǯ���� ����
		}
	}
	friend class SceneSerializer;
	Entity* DeSerializeEntity(const nlohmann::json entityjson);

	Scene* m_CurrentScene = nullptr;

};



template<typename T>
inline std::vector<T*> SceneManager::GetComponentPool()
{
	auto& baseVec = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
	std::vector<T*> result;
	result.reserve(baseVec.size()); // ���� �޸� �Ҵ�
	for (Component* comp : baseVec)
		result.push_back(static_cast<T*>(comp));
	return result;
}