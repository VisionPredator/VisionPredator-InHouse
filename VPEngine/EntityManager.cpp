#include "pch.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Components.h"
#include <fstream>

#include <random>


EntityManager::EntityManager()
{
	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&EntityManager::OnDestroyEntity), true);
	EventManager::GetInstance().Subscribe("OnClearEntity", CreateSubscriber(&EntityManager::OnClearEntity), true);
}
EntityManager::~EntityManager()
{
	Finalize();
}

void EntityManager::Initalize()
{
	m_SceneManager = new SceneManager;
	m_SceneManager->Initialize();

}

void EntityManager::Finalize()
{
	delete m_SceneManager;
}

/// <summary>
/// ID값으로 엔티티를 만들지만 정확히 모르겠다.
/// </summary>
/// <param name="id"></param>
/// <returns></returns>


Entity* EntityManager::CreateEntity()
{
	std::random_device rd;  // 난수 생성기
	std::mt19937 gen(rd()); // Mersenne Twister 난수 엔진
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0부터 UINT32_Max 까지의 난수 범위
	uint32_t id = dis(gen);
	while (m_SceneManager->HasEntity(id))
	{
		id = dis(gen);
	}

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(id);
	m_SceneManager->SetEntityMap(id, tempEntity);
	AddComponent<IDComponent>(id);
	AddComponent<TransformComponent>(id);
	return tempEntity;
}

void EntityManager::OnDestroyEntity(std::any data)
{
	// EntityMap에서 해당 Entity를 찾습니다.
	uint32_t entityID = std::any_cast <int>(data);

	auto& entityMap = m_SceneManager->GetEntityMap();

	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		Entity* entityToRemove = entityIter->second;
		// m_ComponentPool에서 해당 Entity에 해당하는 컴포넌트들을 제거합니다.
		for (auto& compPair : m_SceneManager->GetScene().m_ComponentPool)
		{
			auto& components = compPair.second;
			// 해당 Entity의 컴포넌트를 찾아 제거합니다.
			auto it = std::remove_if(components.begin(), components.end(),
				[entityID](Component* comp) { return comp->GetEntityID() == entityID; });
			components.erase(it, components.end());
			// 제거된 컴포넌트들을 삭제합니다.
			for (auto eraseIt = it; eraseIt != components.end(); ++eraseIt)
			{
				delete* eraseIt;
			}
		}
		// EntityMap에서 해당 Entity를 제거합니다.
		delete entityToRemove;
		entityMap.erase(entityIter);
	}
}

void EntityManager::OnClearEntity(std::any data)
{



}

void EntityManager::SerializePrefab(uint32_t entityID)
{
	Entity tempEntity = *m_SceneManager->GetEntity(entityID);
	tempEntity.GetComponent<IDComponent>();
	///Set FilePath
	std::string folderName = "../Resource/Prefab/";
	std::string entityName = tempEntity.GetComponent<IDComponent>()->Name;
	std::string fileExtension = ".json";
	std::string filePath = folderName + entityName + fileExtension;

	nlohmann::ordered_json SceneJson;			///ordered_json 하고 json의 차이 알아보기!
	///Json을 저장할 파일 위치!
	std::ofstream ofsPrefabPath(filePath);
	if (!ofsPrefabPath)
		VP_ASSERT(false, "파일을 여는데 실패하였습니다!");

	try {
		std::pair<uint32_t, Entity > entityPair = std::make_pair(entityID, tempEntity);
		nlohmann::ordered_json entityJson;
		entityJson["EntityID"] = entityPair.first;
		for (const auto& [id, comp] : entityPair.second.m_OwnedComp)
		{
			nlohmann::json compnentEntity;
			comp->SerializeComponent(compnentEntity);
			compnentEntity["ComponentID"] = id;
			entityJson["Component"].push_back(compnentEntity);
		}
		SceneJson["Entitys"].push_back(entityJson);
		ofsPrefabPath << SceneJson.dump(4);
	}
	catch (const std::exception&)
	{
		VP_ASSERT(false, "Json 입력 중 에러가 났습니다.");
	}
	ofsPrefabPath.close();
























}

Entity* EntityManager::DeSerializeEntity(const nlohmann::json entityjson)
{
	uint32_t entityID = entityjson["EntityID"];

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(entityID);
	m_SceneManager->SetEntityMap(entityID, tempEntity);

	for (auto& componentjson : entityjson["Component"])
	{
		entt::id_type comp_id = (entt::id_type)componentjson["ComponentID"];
		auto metaType = entt::resolve(comp_id);
		if (metaType)
		{
			// 메타 타입으로부터 인스턴스를 생성합니다.
			auto instance = metaType.construct();
			// 특정 함수를 찾고 호출합니다.
			auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
			if (myFunctionMeta)
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, (EntityManager*)this, (uint32_t)entityID);
			else
			{
				VP_ASSERT(false, "Reflection 함수 실패!");
			}
		}

	}

	return tempEntity;
}
