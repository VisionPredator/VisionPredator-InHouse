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
	void OnInitalizeEntity(std::any data);
	void OnDestroyEntity(std::any entityID);
	void OnClearEntity(std::any data);
	void OnChangeScene(std::any data);
	void OnStartScene(std::any data);
	void OnResetScene(std::any data);
	void OnEndScene(std::any data);
	void OnOpenScene(std::any data);

	void SerializePrefab(uint32_t entityID);
	void OnSerializePrefab(std::any entityID);


	//�� �ø�������� �뵵!
	Entity* DeSerializeEntity(const nlohmann::json entityjson);
	Entity* CreateEntity();

	template<typename T, typename ...Args>
	inline T* AddComponent(uint32_t EntityID, Args && ...args)
	{
		Entity* ParentEntity = GetEntity(EntityID);///GetEntity
		VP_ASSERT(!HasComponent<T>(EntityID), "���� Ÿ���� ������Ʈ�� �����մϴ�.");
		T* component = new T;
		component->OwnedEntity = ParentEntity;
		ParentEntity->AddComponent<T>(component);
		AddCompToPool<T>(component);
		return component;
	}


#pragma region Finished Fuctions


	template<typename T>
	T* GetComponent(uint32_t EntityID) { return GetEntity(EntityID)->GetComponent<T>(); }
		template<typename T>
		bool HasComponent(uint32_t EntityID) { return GetEntity(EntityID)->HasComponent<T>(); }
	template<typename T>
	std::vector<T*> GetComponentPool()
	{
		auto& baseVec = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		std::vector<T*> result;
		result.reserve(baseVec.size()); // ���� �޸� �Ҵ�
		for (Component* comp : baseVec) {
			result.push_back(static_cast<T*>(comp));
		}
		return result;
	}

	template<typename T>
	void AddCompToPool(T* component)
	{
		m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()].push_back(component);
	}
	template<typename T>
	void ReleaseCompFromPool(T* component)
	{
		auto& pool = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		auto it = std::find(pool.begin(), pool.end(), component);
		if (it != pool.end())
		{
			std::iter_swap(it, pool.end() - 1); // �ش� ������Ʈ�� ������ ������Ʈ�� ��ȯ
			pool.pop_back(); // ������ ������Ʈ�� Ǯ���� ����
		}
	}

	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }


	 Scene& GetScene() const { return *m_CurrentScene; }
	Entity* GetEntity(uint32_t entityID) { return m_CurrentScene->EntityMap[entityID]; }
	std::unordered_map<uint32_t, Entity*>& GetEntityMap() { return m_CurrentScene->EntityMap; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }

	void SetSceneName(std::string scenename) { m_CurrentScene->SceneName = scenename; }
	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }



	template<typename T>
	void RemoveComponent(uint32_t EntityID);


#pragma endregion
private:
	Scene* m_CurrentScene = nullptr;

};


/// TODO : �̿�
template<typename T>
inline void SceneManager::RemoveComponent(uint32_t EntityID)
{
	VP_ASSERT(HasComponent<T>(EntityID), "�ش� Ÿ���� ������Ʈ�� �������� �ʽ��ϴ�.");
	Entity& ParentEntity = GetEntity(EntityID);///GetEntity
	T* Component = ParentEntity.GetComponent<T>();
	ReleaseCompFromPool<T>(Component);
	ParentEntity.RemoveComponent<T>();
}

