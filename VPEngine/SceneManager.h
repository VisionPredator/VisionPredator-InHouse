#pragma once
#include "Scene.h"
#include "EventSubscriber.h"
class SceneManager :public EventSubscriber
{
public:
	SceneManager();
	~SceneManager();
	void Initialize();


	void OnChangeScene(std::any data);
	void OnStartScene(std::any data);
	void OnResetScene(std::any data);
	void OnEndScene(std::any data);
	void OnOpenScene(std::any data);

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
	void AddComponenttoPool(T* component)
	{
		m_CurrentScene->m_ComponentPool[Reflection::GetTypeID<T>()].push_back(component);
	}

	bool HasEntity(uint32_t entityID);

	void SetEntityMap(uint32_t entityID, Entity* entity) { m_CurrentScene->EntityMap[entityID] = entity; }

	std::unordered_map<uint32_t, Entity*> GetEntityMap() { return m_CurrentScene->EntityMap; }
	const Scene& GetScene() const { return *m_CurrentScene; }
	const std::string GetSceneName() { return m_CurrentScene->SceneName; }
	void SetSceneName(std::string scenename) { m_CurrentScene->SceneName = scenename; }
	Entity* GetEntity(uint32_t entityID);
#pragma endregion
private:
	Scene* m_CurrentScene = nullptr;

};


