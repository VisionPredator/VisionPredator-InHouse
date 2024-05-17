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
	void OnDestroyEntity(std::any entityID);
	// ��� Entity�� �����.
	void OnClearEntity(std::any data);
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
	void OnAddComponent(std::any data);
	// �ش� Component �����Ѵ�.
	void OnRemoveComponent(std::any data);
	// �ش� Entity�� �ʿ� �߰��Ѵ�.
	void OnSetEntityMap(std::any data);
	// �ش� Prefab�� serialize�Ѵ�.
	void OnSerializePrefab(std::any entityID);
	// �ش� Prefab�� serialize�Ѵ�.
	void SerializePrefab(uint32_t entityID);
	/// �ش� json�� Deserialize �Ѵ�.
	Entity* DeSerializeEntity(const nlohmann::json entityjson);

	// ��ƼƼ�� CreateEvnet�� ȣ�� �ϰ�, Entity�� ��ȯ�ϴ� �Լ�.
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
		result.reserve(baseVec.size()); // ���� �޸� �Ҵ�
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
			std::iter_swap(it, pool.end() - 1); // �ش� ������Ʈ�� ������ ������Ʈ�� ��ȯ
			pool.pop_back(); // ������ ������Ʈ�� Ǯ���� ����
		}
	}

	void ReleaseCompFromPool(entt::id_type compID,Component* comp)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[compID];
		auto it = std::find(pool.begin(), pool.end(), comp);
		if (it != pool.end())
		{
			std::iter_swap(it, pool.end() - 1); // �ش� ������Ʈ�� ������ ������Ʈ�� ��ȯ
			pool.pop_back(); // ������ ������Ʈ�� Ǯ���� ����
		}
	}




	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }


	Entity* GetEntity(uint32_t entityID) { return m_CurrentScene->EntityMap[entityID]; }
	std::unordered_map<uint32_t, Entity*>& GetEntityMap() { return m_CurrentScene->EntityMap; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }

	void SetSceneName(std::string sceneName) { m_CurrentScene->SceneName = sceneName; }
	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }

#pragma region ������ �Լ�

	//template<typename T>
	//void RemoveComponent(uint32_t EntityID);

#pragma endregion

#pragma endregion
	template<typename T>
	inline T* AddComponent(uint32_t entityID)
	{

		Entity* ParentEntity = GetEntity(entityID);///GetEntity
		VP_ASSERT(!HasComponent<T>(entityID), "���� Ÿ���� ������Ʈ�� �����մϴ�.");
		T* comp = new T;

		///�Ʒ��� �������� �ٷ� �̺�Ʈ ȣ�� �ϴ°͵� ���� ��!!!!
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

#pragma region ������ �Լ�

//template<typename T>
//inline void SceneManager::RemoveComponent(uint32_t EntityID)
//{
//	VP_ASSERT(HasComponent<T>(EntityID), "�ش� Ÿ���� ������Ʈ�� �������� �ʽ��ϴ�.");
//	Entity& ParentEntity = GetEntity(EntityID);///GetEntity
//	T* Component = ParentEntity.GetComponent<T>();
//	ReleaseCompFromPool<T>(Component);
//	ParentEntity.RemoveComponent<T>();
//}

#pragma endregion