#include "pch.h"
#include "SceneManager.h"
#include "Components.h"
#include <fstream>
#include <random>
#include <queue>
#include "TransformSystem.h"

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
	EventManager::GetInstance().Subscribe("OnAddEntityComponentsToScene", CreateSubscriber(&SceneManager::OnAddEntityComponentsToScene), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDeleteEntity", CreateSubscriber(&SceneManager::OnDeleteEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&SceneManager::OnDestroyEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnClearAllEntity", CreateSubscriber(&SceneManager::OnClearAllEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnRemoveComp_Scene", CreateSubscriber(&SceneManager::OnRemoveComponent), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnAddChild", CreateSubscriber(&SceneManager::OnAddChild), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnAddCompToScene", CreateSubscriber(&SceneManager::OnAddCompToScene), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnRemoveChild", CreateSubscriber(&SceneManager::OnRemoveChild), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnSerializePrefab", CreateSubscriber(&SceneManager::OnSerializePrefab), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnDeSerializePrefab", CreateSubscriber(&SceneManager::OnDeSerializePrefab), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnSpawnPrefab", CreateSubscriber(&SceneManager::OnSpawnPrefab), EventType::ADD_DELETE);
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
	if (!HasComponent<Parent>(child))
		return false;
	auto nextParent = GetComponent<Parent>(child);

	while (nextParent != nullptr)
	{
		if (!HasComponent<Children>(nextParent->ParentID))
		{
			if (HasComponent<Parent>(nextParent->ParentID))
				nextParent = GetComponent<Parent>(nextParent->ParentID);
			else
				nextParent = nullptr;

			continue;
		}

		if (nextParent->ParentID == parent)
		{
			return true;
		}

		if (HasComponent<Parent>(nextParent->ParentID))
			nextParent = GetComponent<Parent>(nextParent->ParentID);
		else
			nextParent = nullptr;
	}
	return false;
}

void SceneManager::AddChild(uint32_t parent, uint32_t child, bool Immidiate)
{
	std::pair<uint32_t, uint32_t> data{ parent ,child };
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnAddChild", data);
	else
		OnAddChild(data);
}
void SceneManager::OnAddChild(std::any data)
{
	auto [parent, child] = std::any_cast<std::pair<uint32_t, uint32_t>>(data);
	if (parent == child || CheckParent(parent, child))
		return;

	if (HasComponent<Parent>(child))
	{
		auto parentofchild = GetComponent<Parent>(child);
		std::pair<uint32_t, uint32_t> newdata{ parentofchild->ParentID, child };
		OnRemoveChild(newdata);
	}

	GetEntity(child)->AddComponent<Parent>(true)->ParentID = parent;

	if (HasComponent<Children>(parent))
	{
		auto children = GetComponent<Children>(parent);
		children->ChildrenID.push_back(child);
	}
	else
	{
		auto entity = GetEntity(parent);
		auto comp= entity->AddComponent<Children>(true);
		comp->ChildrenID.push_back(child);
	}

	EventManager::GetInstance().ImmediateEvent("OnSetParentAndChild", data);
}




void SceneManager::RemoveParent(uint32_t childID, bool Immediate)
{
	if (HasComponent<Parent>(childID))
	{
		auto parent = GetComponent<Parent>(childID);
		std::pair<uint32_t, uint32_t> data{ parent->ParentID, childID };
		if (!Immediate)
			EventManager::GetInstance().ScheduleEvent("OnRemoveChild", data);
		else
			OnRemoveChild(data);
	}
}
void SceneManager::OpenNewScene(bool Immidiate)
{
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnOpenNewScene");
	else
		EventManager::GetInstance().ImmediateEvent("OnOpenNewScene");

}

void SceneManager::SceneSerialize(std::string FilePath, bool Immidiate)
{
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnSerializeScene", FilePath);
	else
		EventManager::GetInstance().ImmediateEvent("OnSerializeScene", FilePath);
}
void SceneManager::SceneDeSerialize(std::string FilePath, bool Immidiate)
{
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnDeSerializeScene", FilePath);
	else
		EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", FilePath);
}



void SceneManager::OnRemoveChild(std::any data)
{
	auto [parent, child] = std::any_cast<std::pair<uint32_t, uint32_t>>(data);
	if (!CheckParent(parent, child))
		return;

	if (HasComponent<Parent>(child))
	{
		auto parentOfChild = GetComponent<Parent>(child);
		if (parentOfChild->ParentID == parent)
		{
			if (HasComponent<Children>(parent))
			{
				auto children = GetComponent<Children>(parent);
				if (children)
				{
					children->ChildrenID.remove(child);

					if (children->ChildrenID.empty())
					{
						OnRemoveComponent(GetEntity(parent)->GetComponent(Reflection::GetTypeID<Children>()));
					}
				}
			}
			OnRemoveComponent(GetEntity(child)->GetComponent(Reflection::GetTypeID<Parent>()));
			EventManager::GetInstance().ImmediateEvent("OnRelaseParentAndChild", child);
		}
	}
}

void SceneManager::AddCompToPool(std::shared_ptr<Component> comp)
{
	// ������Ʈ Ǯ�� ������Ʈ�� �߰��մϴ�.
	auto& pool = m_CurrentScene->m_ComponentPool[comp->GetHandle()->type().id()];
	pool.push_back(comp);
}
#pragma region Delete Entity - Editior
void SceneManager::DeleteEntity(uint32_t entityID)
{
	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(entityID);

	// ��ƼƼ�� �θ� ������Ʈ�� ������ �ִ� ���, �θ�� �ڽ� ���踦 �����մϴ�.
	if (HasComponent<Parent>(entityID))
	{
		auto parentEntity = GetEntity(GetComponent<Parent>(entityID)->ParentID);
		auto parentsChildren = parentEntity->GetComponent<Children>();
		RemoveParent(entityID);
	}

	while (!DeleteEntityIDs.empty())
	{
		uint32_t DeleteEntityID = DeleteEntityIDs.front();

		EventManager::GetInstance().ScheduleEvent("OnDeleteEntity", DeleteEntityID);
		DeleteEntityIDs.pop_front();
		auto entity = GetEntity(DeleteEntityID);

		// ��ƼƼ�� �ڽ� ������Ʈ�� ������ �ִ� ���, �ڽ� ��ƼƼ�� ���� ����Ʈ�� �߰��մϴ�.
		if (entity->HasComponent<Children>())
		{
			auto children = entity->GetComponent<Children>();
			DeleteEntityIDs.insert(DeleteEntityIDs.end(), children->ChildrenID.begin(), children->ChildrenID.end());
		}
	}
}
void SceneManager::ChangeScene(std::string FilePath,bool Immidiate)
{
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnChangeScene", FilePath);
	else
		OnChangeScene(FilePath);

}
void SceneManager::OnDeleteEntity(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);
	auto& entityMap = GetEntityMap();

	// entityMap���� �ش� entityID�� ���� ��ƼƼ�� ã�� �����մϴ�.
	if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
	{
		auto ownedComponents = entityIter->second->GetOwnedComponents();

		// componentMap���� �ش� entityID�� ������ ������Ʈ�� ã�� �����մϴ�.
		for (auto& sharedComp : ownedComponents)
		{
			EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
		}

		// m_ComponentCache���� entityID�� ������ �׸���� ��� �����մϴ�.
		std::erase_if(m_ComponentCache, [entityID](const auto& pair) {
			return pair.first.first == entityID;
			});

		// ����������, entityMap���� ��ƼƼ�� �����մϴ�.
		entityMap.erase(entityIter);
	}
}


#pragma endregion
#pragma region Clear Entity
void SceneManager::ClearAllEntity(bool Immidiate)
{
	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnClearAllEntity");
	else
		OnClearAllEntity(0);
}
void SceneManager::OnClearAllEntity(std::any data)
{
	auto& entityMap = GetEntityMap();
	std::vector<std::shared_ptr<Entity>> entitiesToRemove;

	// Collect all entities to remove
	for (auto& pair : entityMap) {
		entitiesToRemove.push_back(pair.second);
	}

	// Remove each entity using RemoveEntity
	for (auto& entity : entitiesToRemove) 
	{
		ClearEntity(entity);
	}
	ClearCache();
}
void SceneManager::ClearEntity(std::shared_ptr<Entity> entity)
{
	if (!entity)
		return;

	uint32_t entityID = entity->GetEntityID();
	auto& entityMap = GetEntityMap();

	// entityMap���� �ش� entityID�� ���� ��ƼƼ�� ã�� �����մϴ�.
	if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
	{
		// ��ƼƼ�� ������ ��� ������Ʈ�� �����ɴϴ�.
		auto ownedComponents = entity->GetOwnedComponents();

		// �� ������Ʈ�� ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
		for (auto& sharedComp : ownedComponents)
		{
			EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
		}

		// componentMap���� �ش� ��ƼƼ�� ������ ������Ʈ���� �����մϴ�.
		auto& componentMap = m_CurrentScene->m_ComponentPool;
		componentMap.erase(entityID);

		// componentCache���� �ش� ��ƼƼ�� ������ �׸���� ��� �����մϴ�.
		auto& componentCache = m_ComponentCache;
		std::erase_if(componentCache, [entityID](const auto& pair) {
			return pair.first.first == entityID;
			});

		// ����������, entityMap���� ��ƼƼ�� �����մϴ�.
		entityMap.erase(entityIter);
	}
}
#pragma endregion

void SceneManager::OnNewScene(std::any data)
{
	ClearAllEntity(true);
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
	m_CurrentScene = new Scene;
}
void SceneManager::OnOpenNewScene(std::any null)
{
	//�� ���� �� ���ο� �� ����
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	EventManager::GetInstance().ImmediateEvent("OnSetPhysicInfo", GetScenePhysic());
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
void SceneManager::OnAddEntityComponentsToScene(std::any entityID)
{
	uint32_t entityid = std::any_cast<uint32_t>(entityID);
	auto entity = GetEntity(entityid);
	for (auto component :entity->GetOwnedComponents())
	{
		OnAddCompToScene(component);
	}
}
void SceneManager::OnSaveCurrentToTemp(std::any Null)
{
	std::string tempMapPath = "../Data/Temp/Temp.scene";
	EventManager::GetInstance().ImmediateEvent("OnSerializeScene", tempMapPath);
}




void SceneManager::OnStartScene(std::any data)
{
	EventManager::GetInstance().ImmediateEvent("OnSetPhysicInfo", GetScenePhysic());

	///TODO: �� ���۽� ������ Initialize
	//EventManager::GetInstance().ImmediateEvent("OnInitialize");
	///TODO: �� ���۽� ������ Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystems");
}

void SceneManager::OnEndScene(std::any data)
{
	///TODO:�� ������ �� ó���� Finalize?
	//EventManager::GetInstance().ImmediateEvent("OnFinalize");
	///TODO: �� ������ �� ������ Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystems");

}

void SceneManager::SpawnPrefab(std::string prefabname, VPMath::Vector3 pos, VPMath::Vector3 rotation, VPMath::Vector3 scele)
{
	// Create scale matrix
	PrefabData temp = { CreateRandomEntityID(),prefabname ,pos,rotation,scele };
	std::any data = temp;
	EventManager::GetInstance().ScheduleEvent("OnSpawnPrefab", data);
}


void SceneManager::SerializePrefab(uint32_t entityID)
{
	EventManager::GetInstance().ScheduleEvent("OnSerializePrefab", entityID);
}
void SceneManager::OnSerializePrefab(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);
	std::list<uint32_t> serializeIDs;
	serializeIDs.push_back(entityID);

	// ���� ��� ����
	std::string folderName = "../Data/Prefab/";
	std::string entityName = GetEntity(entityID)->GetComponent<IDComponent>()->Name;
	std::string fileExtension = ".prefab";
	std::string filePath = folderName + entityName + fileExtension;

	// ������ �����ϱ� ���� ���丮�� �����ϴ��� Ȯ��
	std::filesystem::create_directories(folderName);

	// JSON ��ü ���� (ordered_json ���)
	nlohmann::ordered_json SceneJson;
	std::ofstream ofsPrefabPath(filePath);
	if (!ofsPrefabPath)
	{
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");
		return;
	}

	while (!serializeIDs.empty())
	{
		uint32_t serializeID = serializeIDs.front();
		serializeIDs.pop_front();
		auto serializeEntity = GetEntity(serializeID);
		nlohmann::ordered_json entityJson;
		entityJson["EntityID"] = serializeEntity->GetEntityID();

		// ��ƼƼ�� ��� ������Ʈ�� ��ȸ�ϸ� ����ȭ
		for (const auto& comp : serializeEntity->GetOwnedComponents())
		{
			auto compid = comp->GetHandle()->type().id();
			if (entityID == serializeID)
			{
				if (Reflection::IsSameType<Parent>(compid))
					continue;
		
				if (Reflection::IsSameType<TransformComponent>(compid))
				{
					auto temp = std::dynamic_pointer_cast<TransformComponent>(comp);
					TransformComponent Temp = *temp;
					Temp.Local_Location = Temp.World_Location;
					Temp.Local_Quaternion = Temp.World_Quaternion;
					Temp.Local_Scale = Temp.World_Scale;
					nlohmann::json componentEntity;
					Temp.SerializeComponent(componentEntity);
					componentEntity["ComponentID"] = compid;
					entityJson["Component"].push_back(componentEntity);
					continue;
				}
			}
			nlohmann::json componentEntity;
			comp->SerializeComponent(componentEntity);
			componentEntity["ComponentID"] = compid;
			entityJson["Component"].push_back(componentEntity);
		}

		SceneJson.push_back(entityJson);
		if (!serializeEntity->HasComponent<Children>())
			continue;
		// �ڽ� ��ƼƼ�� ����ȭ ����Ʈ�� �߰�
		auto childrenComp = serializeEntity->GetComponent<Children>();
		serializeIDs.insert(serializeIDs.end(), childrenComp->ChildrenID.begin(), childrenComp->ChildrenID.end());
	}

	// JSON �����͸� ���Ͽ� ����
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
	std::vector<std::pair<uint32_t, uint32_t>> entityResettingPair{};

	if (inputFile.is_open())
	{
		nlohmann::json prefabJson;
		inputFile >> prefabJson;
		// �׽�Ʈ�ڵ�
		auto count = prefabJson.size();
		for (const auto& entityJson : prefabJson)
		{
			const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
			uint32_t renewEntityID = findOrCreatePair(entityResettingPair, oldEntityID).second;
			std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
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
						entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity.get(), false, false);
						if (auto compPPtr = result.try_cast<std::shared_ptr<Component>>())
						{
							auto compPtr = *compPPtr;
							auto compid = compPtr->GetHandle()->type().id();
							if (Reflection::IsSameType<Children>(compid))
							{
								for (auto& childID : static_cast<Children*>(compPtr.get())->ChildrenID)
									childID = findOrCreatePair(entityResettingPair, childID).second;
							}
							else if (Reflection::IsSameType<Parent>(compid))
							{
								Parent* parentComponet = static_cast<Parent*>(compPtr.get());
								parentComponet->ParentID = findOrCreatePair(entityResettingPair, parentComponet->ParentID).second;
							}
						}
					}
					else
						VP_ASSERT(false, "Reflection �Լ� ����!");
				}
			}
			EventManager::GetInstance().ImmediateEvent("OnAddEntityComponentsToScene", renewEntityID);
		}
	}
}
std::shared_ptr<Entity> SceneManager::DeSerializeEntity(const nlohmann::json entityjson, bool Immidate )
{
	uint32_t entityID = entityjson["EntityID"];
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
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
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, tempEntity.get(), Immidate, false);
			else
				VP_ASSERT(false, "Reflection �Լ� ����!");
		}

	}
		EventManager::GetInstance().ImmediateEvent("OnAddEntityComponentsToScene", entityID);

	return tempEntity;
}
std::shared_ptr<Entity> SceneManager::CreateEntity()
{
	// ���� �����⸦ ����Ͽ� ��ƼƼ ID�� �����մϴ�.
	std::random_device rd;  // ���� ������
	std::mt19937 gen(rd()); // Mersenne Twister ���� ����
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0���� UINT32_MAX������ ���� ����
	uint32_t id = dis(gen);

	// ������ ID�� �̹� �����ϴ��� Ȯ���ϰ�, �����ϸ� ���ο� ID�� �����մϴ�.
	while (HasEntity(id))
	{
		id = dis(gen);
	}

	// ���ο� ��ƼƼ�� �����ϰ� ID�� �����մϴ�.
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
	tempEntity->SetEntityID(id);
	SetEntityMap(id, tempEntity);

	// IDComponent�� TransformComponent�� �߰��մϴ�.
	std::shared_ptr<IDComponent> IDComp = tempEntity->AddComponent<IDComponent>();
	std::shared_ptr<TransformComponent> TransformComp = tempEntity->AddComponent<TransformComponent>();

	// IDComponent�� �̸��� �����մϴ�.
	if (IDComp->Name == "Entity")
	{
		static int a = 0;
		IDComp->Name = IDComp->Name + std::to_string(a);
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
		id = dis(gen);
	return id;
}
void SceneManager::SetScenePhysic(VPPhysics::PhysicsInfo physicInfo)
{
	m_CurrentScene->ScenePhysicInfo = physicInfo;
}

VPPhysics::PhysicsInfo SceneManager::GetScenePhysic()
{
	return m_CurrentScene->ScenePhysicInfo;
}

void SceneManager::OnAddCompToScene(std::any data)
{
	// std::any���� std::shared_ptr<Component>�� �����մϴ�.
	std::shared_ptr<Component> comp = std::any_cast<std::shared_ptr<Component>>(data);
	AddCompToPool(comp);
	EventManager::GetInstance().ImmediateEvent("OnAddedComponent", comp.get());
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

	// ������Ʈ�� ��ƼƼ���� ����
	comp->GetEntity()->ReleaseComponent(comp);

	// ĳ�ÿ��� ������Ʈ�� ����
	auto it = m_ComponentCache.find({ EntityID, CompID });
	if (it != m_ComponentCache.end())
	{
		m_ComponentCache.erase(it);
	}

	ReleaseCompFromPool(CompID, comp);
}

void SceneManager::OnSpawnPrefab(std::any prefabdata)
{
	auto prefabData = std::any_cast<PrefabData> (prefabdata);
	std::ifstream inputFile(prefabData.prefabname);
	std::vector<std::pair<uint32_t, uint32_t>> entityResettingPair{};
	uint32_t mainprefabID{};
	if (inputFile.is_open())
	{
		nlohmann::json prefabJson;
		inputFile >> prefabJson;
		// �׽�Ʈ�ڵ�
		auto count = prefabJson.size();
		for (const auto& entityJson : prefabJson)
		{
			const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
			uint32_t renewEntityID = findOrCreatePair(entityResettingPair, oldEntityID).second;
			std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
			tempEntity->SetEntityID(renewEntityID);
			SetEntityMap(renewEntityID, tempEntity);
			///���� Entity�ΰ�?
			bool IsMainEntity = true;
			///������Ʈ ��ġ.
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
						entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity.get(),true, false);
						if (auto compPPtr = result.try_cast<std::shared_ptr<Component>>())
						{
							auto compPtr = *compPPtr;
							if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Children>()) 
								for (auto& childID : static_cast<Children*>(compPtr.get())->ChildrenID)
									childID = findOrCreatePair(entityResettingPair, childID).second;
							else if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
							{
								IsMainEntity = false;
								Parent* parentComponet = static_cast<Parent*>(compPtr.get());
								parentComponet->ParentID = findOrCreatePair(entityResettingPair, parentComponet->ParentID).second;
							}
						}
					}
					else
						VP_ASSERT(false, "Reflection �Լ� ����!");
				}
			}
			EventManager::GetInstance().ImmediateEvent("OnAddEntityComponentsToScene", renewEntityID);

			if (IsMainEntity)
			{
				mainprefabID = renewEntityID;
			}
			EventManager::GetInstance().ImmediateEvent("OnStart", renewEntityID);

		}
		auto Transform = GetEntity(mainprefabID)->GetComponent<TransformComponent>();
		Transform->SetLocalLocation(prefabData.pos);
		Transform->SetLocalScale(prefabData.scale);
		//prefabData.direction = -prefabData.direction;
		Transform->SetLocalRotation(prefabData.rotation);
		//VPMath::Matrix rotationMatrix = VPMath::Matrix::CreateLookAt_LH(VPMath::Vector3::Zero, prefabData.direction, VPMath::Vector3::Up);
		//VPMath::Quaternion newRotation = VPMath::Quaternion::CreateFromRotationMatrix(rotationMatrix);
		//newRotation.y = -newRotation.y;
		//newRotation.x = -newRotation.x;
		//Transform->SetLocalQuaternion(newRotation);

		//Transform->Local_Quaternion= Transform->Local_Quaternion.CreateFromRotationMatrixLH(rotationMatrix);
	}
}
std::shared_ptr<Entity> SceneManager::GetChildEntityByName(uint32_t entityID, std::string name)
{
	uint32_t mainID = entityID;
	std::queue<uint32_t> ChildrenIDs;
	ChildrenIDs.push(mainID);

	while (!ChildrenIDs.empty())
	{
		uint32_t childID = ChildrenIDs.front();
		ChildrenIDs.pop();

		// GetEntity ȣ���� �ּ�ȭ
		auto currentEntity = GetEntity(childID);
		auto idComponent = currentEntity->GetComponent<IDComponent>();
		if ( idComponent->Name == name)
		{
			return currentEntity;
		}

		// �ڽ� ��ƼƼ�� ID�� ť�� �߰�
		if (currentEntity->HasComponent<Children>())
		{
			auto children = currentEntity->GetComponent<Children>();
			for (const auto& child : children->ChildrenID)
			{
				ChildrenIDs.push(child);
			}
		}
	}
	return nullptr;
}

void SceneManager::DestroyEntity(uint32_t entityID, bool Immidiate)
{
	if (!HasEntity(entityID))
		return;

	if (!Immidiate)
		EventManager::GetInstance().ScheduleEvent("OnDestroyEntity", entityID);
	else
		OnDestroyEntity(entityID);
}
void SceneManager::OnDestroyEntity(std::any data)
{
	uint32_t mainID = std::any_cast<uint32_t>(data);
	auto& entityMap = GetEntityMap();
	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(mainID);

	std::vector <uint32_t> DeleteEntity;
	// ��ƼƼ�� �θ� ������Ʈ�� ������ �ִ� ���, �θ�� �ڽ� ���踦 �����մϴ�.
	if (HasComponent<Parent>(mainID))
	{
		auto parentEntity = GetEntity(GetComponent<Parent>(mainID)->ParentID);
		auto parentsChildren = parentEntity->GetComponent<Children>();
		RemoveParent(mainID);
	}
	while (!DeleteEntityIDs.empty())
	{
		uint32_t DeleteEntityID = DeleteEntityIDs.front();
		DeleteEntity.push_back(DeleteEntityID);
		auto entity = GetEntity(DeleteEntityID);
		
		DeleteEntityIDs.pop_front();
		// ��ƼƼ�� �ڽ� ������Ʈ�� ������ �ִ� ���, �ڽ� ��ƼƼ�� ���� ����Ʈ�� �߰��մϴ�.
		if (entity)
			if (entity->HasComponent<Children>())
			{
				auto children = entity->GetComponent<Children>();
				DeleteEntityIDs.insert(DeleteEntityIDs.end(), children->ChildrenID.begin(), children->ChildrenID.end());
			}
	}

	for (auto entityID : DeleteEntity)
	{

		// entityMap���� �ش� entityID�� ���� ��ƼƼ�� ã�� �����մϴ�.
		if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
		{
			EventManager::GetInstance().ImmediateEvent("OnFinish", entityID);
			auto componentMap = entityIter->second->GetOwnedComponents();

			// componentMap���� �ش� entityID�� ������ ������Ʈ�� ã�� �����մϴ�.
			for (auto& sharedComp : componentMap)
			{
				EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
			}

			// m_ComponentCache���� entityID�� ������ �׸���� ��� �����մϴ�.
			std::erase_if(m_ComponentCache, [entityID](const auto& pair) {
				return pair.first.first == entityID;
				});

			// ����������, entityMap���� ��ƼƼ�� �����մϴ�.
			entityMap.erase(entityIter);

		}

	}


}



#pragma region Trash 

#pragma endregion