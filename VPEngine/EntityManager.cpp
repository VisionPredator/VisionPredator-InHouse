#include "pch.h"
#include "EntityManager.h"
#include "Components.h"
#include <random>


EntityManager::EntityManager()
{
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
