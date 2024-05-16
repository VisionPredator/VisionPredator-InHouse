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
/// ID������ ��ƼƼ�� �������� ��Ȯ�� �𸣰ڴ�.
/// </summary>
/// <param name="id"></param>
/// <returns></returns>


Entity* EntityManager::CreateEntity()
{
	std::random_device rd;  // ���� ������
	std::mt19937 gen(rd()); // Mersenne Twister ���� ����
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0���� UINT32_Max ������ ���� ����
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
	// EntityMap���� �ش� Entity�� ã���ϴ�.
	uint32_t entityID = std::any_cast <int>(data);

	auto& entityMap = m_SceneManager->GetEntityMap();

	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		Entity* entityToRemove = entityIter->second;
		// m_ComponentPool���� �ش� Entity�� �ش��ϴ� ������Ʈ���� �����մϴ�.
		for (auto& compPair : m_SceneManager->GetScene().m_ComponentPool)
		{
			auto& components = compPair.second;
			// �ش� Entity�� ������Ʈ�� ã�� �����մϴ�.
			auto it = std::remove_if(components.begin(), components.end(),
				[entityID](Component* comp) { return comp->GetEntityID() == entityID; });
			components.erase(it, components.end());
			// ���ŵ� ������Ʈ���� �����մϴ�.
			for (auto eraseIt = it; eraseIt != components.end(); ++eraseIt)
			{
				delete* eraseIt;
			}
		}
		// EntityMap���� �ش� Entity�� �����մϴ�.
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

	nlohmann::ordered_json SceneJson;			///ordered_json �ϰ� json�� ���� �˾ƺ���!
	///Json�� ������ ���� ��ġ!
	std::ofstream ofsPrefabPath(filePath);
	if (!ofsPrefabPath)
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");

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
		VP_ASSERT(false, "Json �Է� �� ������ �����ϴ�.");
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
			// ��Ÿ Ÿ�����κ��� �ν��Ͻ��� �����մϴ�.
			auto instance = metaType.construct();
			// Ư�� �Լ��� ã�� ȣ���մϴ�.
			auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
			if (myFunctionMeta)
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, (EntityManager*)this, (uint32_t)entityID);
			else
			{
				VP_ASSERT(false, "Reflection �Լ� ����!");
			}
		}

	}

	return tempEntity;
}
