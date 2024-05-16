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











#pragma region Finished Fuctions
	template<typename T>
	std::vector<T*> GetComponentpool() {
		auto& baseVec = m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()];
		std::vector<T*> result;
		result.reserve(baseVec.size()); // 예비 메모리 할당
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
			std::iter_swap(it, pool.end() - 1); // 해당 컴포넌트를 마지막 컴포넌트와 교환
			pool.pop_back(); // 마지막 컴포넌트를 풀에서 제거
		}
	}

	bool HasEntity(uint32_t entityID) { return m_CurrentScene->EntityMap.count(entityID) > 0; }


	 Scene& GetScene() const { return *m_CurrentScene; }
	Entity* GetEntity(uint32_t entityID) { return m_CurrentScene->EntityMap[entityID]; }
	std::unordered_map<uint32_t, Entity*>& GetEntityMap() { return m_CurrentScene->EntityMap; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }

	void SetSceneName(std::string scenename) { m_CurrentScene->SceneName = scenename; }
	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }





#pragma endregion
private:
	Scene* m_CurrentScene = nullptr;

};


