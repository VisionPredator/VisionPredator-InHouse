#include "pch.h"
#include "SceneManager.h"
#include "Components.h"
#include <fstream>
#include <random>

SceneManager::SceneManager()
{
	EventManager::GetInstance().Subscribe("OnChangeScene", CreateSubscriber(&SceneManager::OnChangeScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnStartScene", CreateSubscriber(&SceneManager::OnStartScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnResetScene", CreateSubscriber(&SceneManager::OnResetScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnEndScene", CreateSubscriber(&SceneManager::OnEndScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOpenScene", CreateSubscriber(&SceneManager::OnOpenScene), EventType::SCENE);

	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&SceneManager::OnDestroyEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnClearAllEntity", CreateSubscriber(&SceneManager::OnClearAllEntity), EventType::ADD_DELETE);

	EventManager::GetInstance().Subscribe("OnRemoveComp_Scene", CreateSubscriber(&SceneManager::OnRemoveComponent), EventType::ADD_DELETE);

	//EventManager::GetInstance().Subscribe("OnAddEntity", CreateSubscriber(&SceneManager::OnAddEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnAddCompToScene", CreateSubscriber(&SceneManager::OnAddCompToScene), EventType::ADD_DELETE);

	EventManager::GetInstance().Subscribe("OnSerializePrefab", CreateSubscriber(&SceneManager::OnSerializePrefab), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDeSerializeEntity", CreateSubscriber(&SceneManager::OnDeSerializeEntity), EventType::ADD_DELETE);

}
SceneManager::~SceneManager()
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
}
void SceneManager::Initialize()
{
	///�� ����
	m_CurrentScene = new Scene;
	m_CurrentScene->SceneName = "TempScene";
	EventManager::GetInstance().ScheduleEvent("OnStartScene");
}
void SceneManager::Finalize()
{

}

void SceneManager::AddCompToPool(Component* comp)
{
	m_CurrentScene->m_ComponentPool[comp->GetHandle()->type().id()].push_back(comp);
}
void SceneManager::DeleteEntity(uint32_t entityID)
{
	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(entityID);

	while (!DeleteEntityIDs.empty())
	{
		uint32_t DeleteEntityID = DeleteEntityIDs.front();

		EventManager::GetInstance().ScheduleEvent("OnDestroyEntity", DeleteEntityID);
		DeleteEntityIDs.pop_front();
		if (!GetEntity(DeleteEntityID)->HasComponent<Children>())
			continue;
		Children* children = GetEntity(DeleteEntityID)->GetComponent<Children>();
		DeleteEntityIDs.insert(DeleteEntityIDs.end(), children->ChildrenID.begin(), children->ChildrenID.end());
	}
}

void SceneManager::OnDestroyEntity(std::any data)
{
	// EntityMap���� �ش� Entity�� ã���ϴ�.
	uint32_t entityID = std::any_cast<uint32_t>(data);

	auto& entityMap = GetEntityMap();
	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		Entity* entityToRemove = entityIter->second;
		// m_ComponentPool���� �ش� Entity�� �ش��ϴ� ������Ʈ���� �����մϴ�.
		for (auto& compPair : m_CurrentScene->m_ComponentPool)
		{
			auto& components = compPair.second;
			std::vector<Component*> toDelete;

			// �ش� Entity�� ������Ʈ�� ã�� �����մϴ�.
			components.erase(std::remove_if(components.begin(), components.end(),
				[entityID, &toDelete](Component* comp) 
				{
					if (comp->GetEntityID() == entityID)
					{
						toDelete.push_back(comp);
						return true;
					}
					return false;
				}), components.end());

			// ���ŵ� ������Ʈ���� �����մϴ�.
			for (auto* comp : toDelete)
			{
				delete comp;
			}
		}
		// EntityMap���� �ش� Entity�� �����մϴ�.
		delete entityToRemove;
		entityMap.erase(entityIter);
	}
}

void SceneManager::OnClearAllEntity(std::any data)
{
	// Clear EntityMap and manage memory for Entity*
	for (auto& pair : m_CurrentScene->EntityMap) {
		delete pair.second;  // Assuming that you are responsible for deleting the Entity*
	}
	m_CurrentScene->EntityMap.clear();

	// Clear m_ComponentPool and manage memory for Component* objects
	for (auto& pair : m_CurrentScene->m_ComponentPool) {
		for (Component* comp : pair.second) {
			delete comp;  // Assuming that you are responsible for deleting the Component*
		}
		pair.second.clear();
	}
	m_CurrentScene->m_ComponentPool.clear();
}

//�� ü���� �̺�Ʈ :: SchduleEvent�� ȣ�� ����!
void SceneManager::OnChangeScene(std::any data)
{

	try
	{
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}

		//�� ������ �̺�Ʈ ��� ����
		EventManager::GetInstance().GetInstance().ImmediateEvent("OnEndScene");
		//�� ���� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnOpenScene", data);

	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}
}
void SceneManager::OnStartScene(std::any data)
{
	///TODO: �� ���۽� ������ init??
	EventManager::GetInstance().ImmediateEvent("OnInitialize");
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystem");
}

void SceneManager::OnResetScene(std::any data)
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
	m_CurrentScene = new Scene;
}
void SceneManager::OnEndScene(std::any data)
{
	///TODO:�� ������ �� ó���� Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystem");

}

void SceneManager::OnOpenScene(std::any data)
{
	try
	{
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}
		//�� �ʱ�ȭ �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnResetScene");
		//�� �ø�������� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
		//�� ���� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnStartScene");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}

}

void SceneManager::SerializePrefab(uint32_t entityID)
{
	EventManager::GetInstance().ScheduleEvent("OnSerializePrefab", entityID);
}

void SceneManager::OnSerializePrefab(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);

	Entity tempEntity = *GetEntity(entityID);
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

Entity* SceneManager::DeSerializeEntity(const nlohmann::json entityjson)
{
	uint32_t entityID = entityjson["EntityID"];

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(entityID);
	SetEntityMap(entityID, tempEntity);

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
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, tempEntity);
			else
				VP_ASSERT(false, "Reflection �Լ� ����!");
		}
	}

	return tempEntity;
}

void SceneManager::OnDeSerializeEntity(std::any data)
{
	const nlohmann::json entityjson = std::any_cast<const nlohmann::json>(data);
	uint32_t entityID = entityjson["EntityID"];

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(entityID);
	SetEntityMap(entityID, tempEntity);

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
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, (SceneManager*)this, (uint32_t)entityID);
			else
				VP_ASSERT(false, "Reflection �Լ� ����!");
		}
	}
}

Entity* SceneManager::CreateEntity()
{
	std::random_device rd;  // ���� ������
	std::mt19937 gen(rd()); // Mersenne Twister ���� ����
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0���� UINT32_Max ������ ���� ����
	uint32_t id = dis(gen);
	while (HasEntity(id))
	{
		id = dis(gen);
	}

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(id);
	SetEntityMap(id, tempEntity);

	Component* IDComp = tempEntity->AddComponent(Reflection::GetTypeID<IDComponent>());
	Component* TransformComp =tempEntity->AddComponent(Reflection::GetTypeID<TransformComponent>());
	IDComponent* temp = static_cast<IDComponent*>(IDComp);
	static int a = 0;
	if (temp->Name=="Entity")
	{
	temp->Name= temp->Name+ std::to_string(a);
	a++;
	}

	return tempEntity;
}

void SceneManager::OnAddCompToScene(std::any data)
{
	auto comp = std::any_cast< Component*>(data);
	AddCompToPool(comp);
	EventManager::GetInstance().ImmediateEvent("OnAddedComponent", comp);
}

void SceneManager::OnRemoveComponent(std::any data)
{
	Component* comp = std::any_cast<Component*>(data);
	uint32_t EntityID = comp->GetEntityID();
	entt::id_type CompID = comp->GetHandle()->type().id();
	if (!HasComponent(EntityID, CompID))
	{
		VP_ASSERT(false, "�ش� Ÿ���� ������Ʈ�� �������� �ʽ��ϴ�.");
		return;
	}
	EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", comp);

	comp->GetEntity()->ReleaseComponent(comp);
	ReleaseCompFromPool(CompID, comp);
}

//void SceneManager::OnAddEntity(std::any data)
//{
//	auto [entityID, entity] = std::any_cast <std::pair< uint32_t, Entity* >> (data);
//	m_CurrentScene->EntityMap[entityID] = entity;
//}