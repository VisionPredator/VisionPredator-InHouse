#include "pch.h"
#include "SceneManager.h"
#include "Components.h"
#include <fstream>
#include <random>

SceneManager::SceneManager()
{
	EventManager::GetInstance().Subscribe("OnChangeScene", CreateSubscriber(&SceneManager::OnChangeScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnStartScene", CreateSubscriber(&SceneManager::OnStartScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnNewScene", CreateSubscriber(&SceneManager::OnNewScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnEndScene", CreateSubscriber(&SceneManager::OnEndScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOpenScene", CreateSubscriber(&SceneManager::OnOpenScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOpenNewScene", CreateSubscriber(&SceneManager::OnOpenNewScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnSaveCurrentToTemp", CreateSubscriber(&SceneManager::OnSaveCurrentToTemp), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOverwriteTempToCurrent", CreateSubscriber(&SceneManager::OnOverwriteTempToCurrent), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&SceneManager::OnDestroyEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnClearAllEntity", CreateSubscriber(&SceneManager::OnClearAllEntity), EventType::ADD_DELETE);

	EventManager::GetInstance().Subscribe("OnRemoveComp_Scene", CreateSubscriber(&SceneManager::OnRemoveComponent), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnAddChild", CreateSubscriber(&SceneManager::OnAddChild), EventType::ADD_DELETE);

	//EventManager::GetInstance().Subscribe("OnAddEntity", CreateSubscriber(&SceneManager::OnAddEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnAddCompToScene", CreateSubscriber(&SceneManager::OnAddCompToScene), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnRemoveChild", CreateSubscriber(&SceneManager::OnRemoveChild), EventType::ADD_DELETE);

	EventManager::GetInstance().Subscribe("OnSerializePrefab", CreateSubscriber(&SceneManager::OnSerializePrefab), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDeSerializeEntity", CreateSubscriber(&SceneManager::OnDeSerializeEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDeSerializePrefab", CreateSubscriber(&SceneManager::OnDeSerializePrefab), EventType::ADD_DELETE);

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
}
void SceneManager::Finalize()
{

}

std::pair<uint32_t, uint32_t>& SceneManager::findOrCreatePair(std::vector<std::pair<uint32_t, uint32_t>>& vec, uint32_t key)
{
	// Search for the pair
	for (auto& pair : vec) 
	{
		if (pair.first == key) 
		{
			return pair;  // Return the found pair
		}
	}
	// If not found, create a new pair
	vec.emplace_back(key, CreateRandomEntityID());  // Default value for second element is 0, can be modified as needed
	return vec.back();  // Return the newly created pair
}
bool SceneManager::CheckParent(uint32_t parent, uint32_t child)
{
	Parent* nextParent = GetComponent<Parent>(child);

	while (nextParent != nullptr)
	{
		if (!GetComponent<Children>(nextParent->ParentID))
		{
			nextParent = GetComponent<Parent>(nextParent->ParentID);
			continue;
		}

		if (nextParent->ParentID == parent)
		{
			return true;
		}
		nextParent = GetComponent<Parent>(nextParent->ParentID);
	}
	return false;
}

void SceneManager::AddChild(uint32_t parent, uint32_t child)
{
	std::pair<uint32_t, uint32_t> data{ parent ,child };
	EventManager::GetInstance().ScheduleEvent("OnAddChild", data);
}

void SceneManager::OnAddChild(std::any data)
{
	auto [parent, child] = std::any_cast<std::pair<uint32_t, uint32_t>>(data);
	if (parent == child || CheckParent(parent, child))
		return;

	if (Parent* parentofchild = GetComponent<Parent>(child); parentofchild)
	{
		std::pair<uint32_t, uint32_t> newdata{ parentofchild->ParentID, child };
		OnRemoveChild(newdata);
	}

	GetEntity(child)->AddComponent<Parent>(true)->ParentID= parent;

	if (Children* children = GetComponent<Children>(parent); children)
		children->ChildrenID.push_back(child);
	else
		GetEntity(parent)->AddComponent<Children>(true)->ChildrenID.push_back(child);
	EventManager::GetInstance().ImmediateEvent("OnSetParentAndChild", data);
}





void SceneManager::RemoveParent(uint32_t childID, bool Immediate)
{
	if (Parent* parent = GetComponent<Parent>(childID); parent)
	{
		std::pair<uint32_t, uint32_t> data{ parent->ParentID, childID };
		if (Immediate)
		{
			EventManager::GetInstance().ImmediateEvent("OnRemoveChild", data);

			return;
		}

		EventManager::GetInstance().ScheduleEvent("OnRemoveChild", data);

	}
}

void SceneManager::OpenNewScene()
{

	EventManager::GetInstance().ScheduleEvent("OnOpenNewScene");
}

void SceneManager::SceneSerialize(std::string FilePath)
{
	EventManager::GetInstance().ScheduleEvent("OnSerializeScene",FilePath);
}

void SceneManager::SceneDeSerialize(std::string FilePath)
{
	EventManager::GetInstance().ScheduleEvent("OnDeSerializeScene", FilePath);
}



void SceneManager::OnRemoveChild(std::any data)
{

	auto [parent, child] = std::any_cast<std::pair<uint32_t, uint32_t>>(data);
	if (!CheckParent(parent, child))
		return;

	if (Parent* parentOfChild = GetComponent<Parent>(child); parentOfChild)
	{

		if (parentOfChild->ParentID == parent)
		{
			Children* children =GetComponent<Children>(parent);
			children->ChildrenID.remove_if(
				[child](uint32_t id)
				{
					return id == child;
				}
			);

			if (children->ChildrenID.empty())
			{
				OnRemoveComponent(static_cast<Component*>(children));
			}
			OnRemoveComponent(static_cast<Component*>(parentOfChild));

			EventManager::GetInstance().ImmediateEvent("OnRelaseParentAndChild", child);

		}
	}

}







void SceneManager::AddCompToPool(Component* comp)
{
	m_CurrentScene->m_ComponentPool[comp->GetHandle()->type().id()].push_back(comp);
}
void SceneManager::DeleteEntity(uint32_t entityID)
{
	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(entityID);

	if (HasComponent<Parent>(entityID))
	{
		Entity* parentEntity = GetEntity(GetComponent<Parent>(entityID)->ParentID);
		auto parentsChildren = parentEntity->GetComponent<Children>();
		RemoveParent(entityID);
	}

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
				EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", comp);
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

void SceneManager::OnNewScene(std::any data)
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
	m_CurrentScene = new Scene;
}
void SceneManager::OnOpenNewScene(std::any null)
{
	//�� ���� �� ���ο� �� ����
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
}

//�� ü���� �̺�Ʈ �ΰ��� ���� �� ü���� �۵��뵵!
void SceneManager::OnChangeScene(std::any data)
{
	//�� ������ �̺�Ʈ ��� ����
	EventManager::GetInstance().ImmediateEvent("OnEndScene");
	//�� ���� �̺�Ʈ ��� ����
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	//�� �ø�������� �̺�Ʈ ��� ����
	EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
	//�� ���� �̺�Ʈ ��� ����
	EventManager::GetInstance().ImmediateEvent("OnStartScene");
}
//���� ���� ���ÿ� ���!
void SceneManager::OnOpenScene(std::any data)
{
	//�� �ʱ�ȭ �̺�Ʈ ��� ����.
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	//�� ��ø�������� �̺�Ʈ ��� ����
	EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
}

void SceneManager::OnOverwriteTempToCurrent(std::any Null)
{
	std::string curName = GetSceneName();
	std::string tempMapPath = "../Data/Temp/Temp.scene";
	EventManager::GetInstance().ImmediateEvent("OnOpenScene", tempMapPath);
	SetSceneName(curName);

}
void SceneManager::OnSaveCurrentToTemp(std::any Null)
{
	std::string tempMapPath = "../Data/Temp/Temp.scene";
	EventManager::GetInstance().ImmediateEvent("OnSerializeScene", tempMapPath);
}




void SceneManager::OnStartScene(std::any data)
{
	///TODO: �� ���۽� ������ Initialize
	EventManager::GetInstance().ImmediateEvent("OnInitialize");
	///TODO: �� ���۽� ������ Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystems");
}

void SceneManager::OnEndScene(std::any data)
{
	///TODO:�� ������ �� ó���� Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	///TODO: �� ������ �� ������ Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystems");

}



void SceneManager::SerializePrefab(uint32_t entityID)
{
	EventManager::GetInstance().ScheduleEvent("OnSerializePrefab", entityID);
}

void SceneManager::OnSerializePrefab(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);
	std::list<uint32_t>  serializeIDs;
	serializeIDs.push_back(entityID);
	
	///Set FilePath
	std::string folderName = "../Data/Prefab/";
	std::string entityName = GetEntity(entityID)->GetComponent<IDComponent>()->Name;
	std::string fileExtension = ".prefab";
	std::string filePath = folderName + entityName + fileExtension;

	// Ensure directory exists before creating the file
	std::filesystem::create_directories(folderName);

	nlohmann::ordered_json SceneJson;			///ordered_json �ϰ� json�� ���� �˾ƺ���!
	///Json�� ������ ���� ��ġ!
	std::ofstream ofsPrefabPath(filePath);
	if (!ofsPrefabPath)
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");

	while (!serializeIDs.empty())
	{
		uint32_t serializeID = serializeIDs.front();
		serializeIDs.pop_front();
		Entity* serializeEntity = GetEntity(serializeID);
		nlohmann::ordered_json entityJson;
		entityJson["EntityID"] = serializeEntity->GetEntityID();
		for (const auto& [id, comp] : serializeEntity->m_OwnedComp)
		{
			if (entityID == serializeID)
			{
				if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
					continue;
				if (comp->GetHandle()->type().id() == Reflection::GetTypeID<TransformComponent>())
				{
					auto temp = static_cast<TransformComponent*>(comp);
					TransformComponent Temp = *temp;
					Temp.Local_Location= Temp.World_Location;
					Temp.Local_Quaternion = Temp.World_Quaternion;
					Temp.Local_Scale = Temp.World_Scale;
					nlohmann::json compnentEntity;
					Temp.SerializeComponent(compnentEntity);
					compnentEntity["ComponentID"] = id;
					entityJson["Component"].push_back(compnentEntity);
					continue;
				}
			}



			nlohmann::json compnentEntity;
			comp->SerializeComponent(compnentEntity);
			compnentEntity["ComponentID"] = id;
			entityJson["Component"].push_back(compnentEntity);
		}

		SceneJson.push_back(entityJson);
		if (!serializeEntity->HasComponent<Children>())
			continue;
		Children* childrenComp = serializeEntity->GetComponent<Children>();
		serializeIDs.insert(serializeIDs.end(), childrenComp->ChildrenID.begin(), childrenComp->ChildrenID.end());
	}

	ofsPrefabPath << SceneJson.dump(4);

	ofsPrefabPath.close();
}

void SceneManager::DeSerializePrefab(std::string filePath)
{
	EventManager::GetInstance().ScheduleEvent("OnDeSerializePrefab", filePath);
}

void SceneManager::OnDeSerializePrefab(std::any data)
{
	std::string filePath = std::any_cast<std::string>(data);


	std::ifstream inputFile(filePath);
	std::vector<std::pair<uint32_t, uint32_t> > entityResettingPair{};

	if (inputFile.is_open())
	{
		nlohmann::json prefabJson;
		inputFile >> prefabJson;
		//�׽�Ʈ�ڵ�
		auto count = prefabJson.size();
		for (const auto& entityJson : prefabJson)
		{
			const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
			uint32_t renewEntityID = findOrCreatePair(entityResettingPair, oldEntityID).second;
			Entity* tempEntity = new Entity;
			tempEntity->SetEntityID(renewEntityID);
			SetEntityMap(renewEntityID, tempEntity);
			for (const nlohmann::json compJson : entityJson["Component"])
			{
				entt::id_type comp_id = (entt::id_type)compJson["ComponentID"];
				auto metaType = entt::resolve(comp_id);
				if (metaType)
				{

					// ��Ÿ Ÿ�����κ��� �ν��Ͻ��� �����մϴ�.
					auto instance = metaType.construct();
					// Ư�� �Լ��� ã�� ȣ���մϴ�.
					auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
					if (myFunctionMeta)
					{
						entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity);
						if (auto compPPtr = result.try_cast<Component*>())
						{
							auto compPtr = *compPPtr;
							if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Children>())
								for (auto& childID : static_cast<Children*> (compPtr)->ChildrenID)
									childID = findOrCreatePair(entityResettingPair, childID).second;
							else if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
							{
								Parent* parentComponet = static_cast<Parent*> (compPtr);
								parentComponet->ParentID = findOrCreatePair(entityResettingPair, parentComponet->ParentID).second;
							}
						}
					}
					else
						VP_ASSERT(false, "Reflection �Լ� ����!");
				}
			}
		}
	}
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
	uint32_t entityID = entityjson["EntityID"].get<uint32_t>();

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
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, (SceneManager*)this, tempEntity);
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

Entity* SceneManager::CreateEntity(uint32_t id)
{
	if (HasEntity(id))
	{
		VP_ASSERT(false, "�̹� �����ϴ� EntityID �Դϴ�.");
		return nullptr;
	}

	Entity* tempEntity = new Entity;
	tempEntity->SetEntityID(id);
	SetEntityMap(id, tempEntity);

	Component* IDComp = tempEntity->AddComponent(Reflection::GetTypeID<IDComponent>());
	Component* TransformComp = tempEntity->AddComponent(Reflection::GetTypeID<TransformComponent>());
	IDComponent* temp = static_cast<IDComponent*>(IDComp);
	static int a = 0;
	if (temp->Name == "Entity")
	{
		temp->Name = temp->Name + std::to_string(a);
		a++;
	}

	return tempEntity;
}

uint32_t SceneManager::CreateRandomEntityID()
{
	std::random_device rd;  // ���� ������
	std::mt19937 gen(rd()); // Mersenne Twister ���� ����
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0���� UINT32_Max ������ ���� ����
	uint32_t id = dis(gen);
	while (HasEntity(id))
	{
		id = dis(gen);
	}
	return id;
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

