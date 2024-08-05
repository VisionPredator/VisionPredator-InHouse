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
	///씬 생성
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
	auto nextParent = GetComponent<Parent>(child);

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

	if (auto parentofchild = GetComponent<Parent>(child))
	{
		std::pair<uint32_t, uint32_t> newdata{ parentofchild->ParentID, child };
		OnRemoveChild(newdata);
	}

	GetEntity(child)->AddComponent<Parent>(true)->ParentID = parent;

	if (auto children = GetComponent<Children>(parent))
		children->ChildrenID.push_back(child);
	else
		GetEntity(parent)->AddComponent<Children>(true)->ChildrenID.push_back(child);

	EventManager::GetInstance().ImmediateEvent("OnSetParentAndChild", data);
}




void SceneManager::RemoveParent(uint32_t childID, bool Immediate)
{
	if (auto parent = GetComponent<Parent>(childID))
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
	EventManager::GetInstance().ScheduleEvent("OnSerializeScene", FilePath);
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

	if (auto parentOfChild = GetComponent<Parent>(child))
	{
		if (parentOfChild->ParentID == parent)
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
			OnRemoveComponent(GetEntity(child)->GetComponent(Reflection::GetTypeID<Parent>()));
			EventManager::GetInstance().ImmediateEvent("OnRelaseParentAndChild", child);
		}
	}
}







void SceneManager::AddCompToPool(std::shared_ptr<Component> comp)
{
	// 컴포넌트 풀에 컴포넌트를 추가합니다.
	auto& pool = m_CurrentScene->m_ComponentPool[comp->GetHandle()->type().id()];
	pool.push_back(comp);
}

void SceneManager::DeleteEntity(uint32_t entityID)
{
	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(entityID);

	// 엔티티가 부모 컴포넌트를 가지고 있는 경우, 부모와 자식 관계를 제거합니다.
	if (HasComponent<Parent>(entityID))
	{
		auto parentEntity = GetEntity(GetComponent<Parent>(entityID)->ParentID);
		auto parentsChildren = parentEntity->GetComponent<Children>();
		RemoveParent(entityID);
	}

	while (!DeleteEntityIDs.empty())
	{
		uint32_t DeleteEntityID = DeleteEntityIDs.front();

		EventManager::GetInstance().ScheduleEvent("OnDestroyEntity", DeleteEntityID);
		DeleteEntityIDs.pop_front();
		auto entity = GetEntity(DeleteEntityID);

		// 엔티티가 자식 컴포넌트를 가지고 있는 경우, 자식 엔티티도 삭제 리스트에 추가합니다.
		if (entity->HasComponent<Children>())
		{
			auto children = entity->GetComponent<Children>();
			DeleteEntityIDs.insert(DeleteEntityIDs.end(), children->ChildrenID.begin(), children->ChildrenID.end());
		}
	}
}


void SceneManager::OnDestroyEntity(std::any data)
{
	// EntityMap에서 해당 엔티티를 찾습니다.
	uint32_t entityID = std::any_cast<uint32_t>(data);

	auto& entityMap = GetEntityMap();
	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		std::shared_ptr<Entity> entityToRemove = entityIter->second;

		// m_ComponentPool에서 해당 엔티티에 해당하는 컴포넌트들을 제거합니다.
		for (auto& compPair : m_CurrentScene->m_ComponentPool)
		{
			auto& components = compPair.second;
			components.erase(std::remove_if(components.begin(), components.end(),
				[&entityID](std::weak_ptr<Component>& weakComp)
				{
					if (auto sharedComp = weakComp.lock())
					{
						if (sharedComp->GetEntityID() == entityID)
						{
							EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
							return true;
						}
					}
					return false;
				}), components.end());
		}

		// EntityMap에서 해당 엔티티를 제거합니다.
		entityMap.erase(entityIter);
	}
}

void SceneManager::RemoveEntity(std::shared_ptr<Entity> entity)
{
	if (!entity)
		return;

	// 엔티티 ID를 가져옵니다.
	uint32_t entityID = entity->GetEntityID();

	auto& entityMap = GetEntityMap();
	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		// 해당 엔티티에 연결된 컴포넌트들을 제거합니다.
		for (auto& compPair : m_CurrentScene->m_ComponentPool)
		{
			auto& components = compPair.second;
			components.erase(std::remove_if(components.begin(), components.end(),
				[&entityID](std::weak_ptr<Component>& weakComp)
				{
					if (auto sharedComp = weakComp.lock())
					{
						if (sharedComp->GetEntityID() == entityID)
						{
							EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
							return true;
						}
					}
					return false;
				}), components.end());
		}

		// 엔티티 맵에서 해당 엔티티를 제거합니다.
		entityMap.erase(entityIter);
	}
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
	for (auto& entity : entitiesToRemove) {
		RemoveEntity(entity);
	}
	ClearCache();
}
void SceneManager::OnNewScene(std::any data)
{
	EventManager::GetInstance().ImmediateEvent("OnClearAllEntity");
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
	m_CurrentScene = new Scene;
}
void SceneManager::OnOpenNewScene(std::any null)
{
	//씬 삭제 후 새로운 씬 생성

	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	EventManager::GetInstance().ImmediateEvent("OnSetPhysicInfo", GetScenePhysic());

}

//씬 체인지 이벤트 인게임 도중 씬 체인지 작동용도!
void SceneManager::OnChangeScene(std::any data)
{
	//씬 끝나는 이벤트 즉시 실행
	EventManager::GetInstance().ImmediateEvent("OnEndScene");
	//씬 삭제 이벤트 즉시 실행
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	//씬 시리얼라이즈 이벤트 즉시 실행
	EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
	//씬 시작 이벤트 즉시 실행
	EventManager::GetInstance().ImmediateEvent("OnStartScene");
}
//파일 열기 사용시에 사용!
void SceneManager::OnOpenScene(std::any data)
{
	//씬 초기화 이벤트 즉시 실행.
	EventManager::GetInstance().ImmediateEvent("OnNewScene");
	//씬 디시리얼라이즈 이벤트 즉시 실행
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
	EventManager::GetInstance().ImmediateEvent("OnSetPhysicInfo", GetScenePhysic());

	///TODO: 씬 시작시 설정할 Initialize
	EventManager::GetInstance().ImmediateEvent("OnInitialize");
	///TODO: 씬 시작시 설정할 Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystems");
}

void SceneManager::OnEndScene(std::any data)
{
	///TODO:씬 끝났을 때 처리할 Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	///TODO: 씬 끝났을 때 설정할 Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystems");

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

	// 파일 경로 설정
	std::string folderName = "../Data/Prefab/";
	std::string entityName = GetEntity(entityID)->GetComponent<IDComponent>()->Name;
	std::string fileExtension = ".prefab";
	std::string filePath = folderName + entityName + fileExtension;

	// 파일을 생성하기 전에 디렉토리가 존재하는지 확인
	std::filesystem::create_directories(folderName);

	// JSON 객체 생성 (ordered_json 사용)
	nlohmann::ordered_json SceneJson;
	std::ofstream ofsPrefabPath(filePath);
	if (!ofsPrefabPath)
	{
		VP_ASSERT(false, "파일을 여는데 실패하였습니다!");
		return;
	}

	while (!serializeIDs.empty())
	{
		uint32_t serializeID = serializeIDs.front();
		serializeIDs.pop_front();
		auto serializeEntity = GetEntity(serializeID);
		nlohmann::ordered_json entityJson;
		entityJson["EntityID"] = serializeEntity->GetEntityID();

		// 엔티티의 모든 컴포넌트를 순회하며 직렬화
		for (const auto& comp : serializeEntity->GetOwnedComponents())
		{
			if (entityID == serializeID)
			{
				if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
					continue;
				if (comp->GetHandle()->type().id() == Reflection::GetTypeID<TransformComponent>())
				{
					auto temp = std::dynamic_pointer_cast<TransformComponent>(comp);
					TransformComponent Temp = *temp;
					Temp.Local_Location = Temp.World_Location;
					Temp.Local_Quaternion = Temp.World_Quaternion;
					Temp.Local_Scale = Temp.World_Scale;
					nlohmann::json componentEntity;
					Temp.SerializeComponent(componentEntity);
					componentEntity["ComponentID"] = comp->GetHandle()->type().id();
					entityJson["Component"].push_back(componentEntity);
					continue;
				}
			}
			nlohmann::json componentEntity;
			comp->SerializeComponent(componentEntity);
			componentEntity["ComponentID"] = comp->GetHandle()->type().id();
			entityJson["Component"].push_back(componentEntity);
		}

		SceneJson.push_back(entityJson);
		if (!serializeEntity->HasComponent<Children>())
			continue;
		// 자식 엔티티를 직렬화 리스트에 추가
		auto childrenComp = serializeEntity->GetComponent<Children>();
		serializeIDs.insert(serializeIDs.end(), childrenComp->ChildrenID.begin(), childrenComp->ChildrenID.end());
	}

	// JSON 데이터를 파일에 저장
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
		//테스트코드
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

					// 메타 타입으로부터 인스턴스를 생성합니다.
					auto instance = metaType.construct();
					// 특정 함수를 찾고 호출합니다.
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
						VP_ASSERT(false, "Reflection 함수 실패!");
				}
			}
		}
	}
}

std::shared_ptr<Entity> SceneManager::DeSerializeEntity(const nlohmann::json entityjson)
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
			// 메타 타입으로부터 인스턴스를 생성합니다.
			auto instance = metaType.construct();
			// 특정 함수를 찾고 호출합니다.
			auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
			if (myFunctionMeta)
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, tempEntity);
			else
				VP_ASSERT(false, "Reflection 함수 실패!");
		}
	}

	return tempEntity;
}

void SceneManager::OnDeSerializeEntity(std::any data)
{
	const nlohmann::json entityjson = std::any_cast<const nlohmann::json>(data);
	uint32_t entityID = entityjson["EntityID"].get<uint32_t>();
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();

	tempEntity->SetEntityID(entityID);
	SetEntityMap(entityID, tempEntity);

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
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, (SceneManager*)this, tempEntity);
			else
				VP_ASSERT(false, "Reflection 함수 실패!");
		}
	}
}
std::shared_ptr<Entity> SceneManager::CreateEntity()
{
	// 난수 생성기를 사용하여 엔티티 ID를 생성합니다.
	std::random_device rd;  // 난수 생성기
	std::mt19937 gen(rd()); // Mersenne Twister 난수 엔진
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0부터 UINT32_MAX까지의 난수 범위
	uint32_t id = dis(gen);

	// 생성된 ID가 이미 존재하는지 확인하고, 존재하면 새로운 ID를 생성합니다.
	while (HasEntity(id))
	{
		id = dis(gen);
	}

	// 새로운 엔티티를 생성하고 ID를 설정합니다.
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
	tempEntity->SetEntityID(id);
	SetEntityMap(id, tempEntity);

	// IDComponent와 TransformComponent를 추가합니다.
	std::shared_ptr<IDComponent> IDComp = tempEntity->AddComponent<IDComponent>();
	std::shared_ptr<TransformComponent> TransformComp = tempEntity->AddComponent<TransformComponent>();

	// IDComponent의 이름을 설정합니다.
	if (IDComp->Name == "Entity")
	{
		static int a = 0;
		IDComp->Name = IDComp->Name + std::to_string(a);
		a++;
	}

	return tempEntity;
}
//
//std::shared_ptr<Entity> SceneManager::CreateEntity(uint32_t id)
//{
//	// 주어진 ID가 이미 존재하는지 확인합니다.
//	if (HasEntity(id))
//	{
//		VP_ASSERT(false, "이미 존재하는 EntityID 입니다.");
//		return nullptr;
//	}
//
//	// 새로운 엔티티를 생성하고 ID를 설정합니다.
//	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
//	tempEntity->SetEntityID(id);
//	SetEntityMap(id, tempEntity);
//
//	// IDComponent와 TransformComponent를 추가합니다.
//	std::shared_ptr<IDComponent> IDComp = tempEntity->AddComponent<IDComponent>();
//	std::shared_ptr<TransformComponent> TransformComp = tempEntity->AddComponent<TransformComponent>();
//
//	// IDComponent의 이름을 설정합니다.
//	if (IDComp->Name == "Entity")
//	{
//		static int a = 0;
//		IDComp->Name = IDComp->Name + std::to_string(a);
//		a++;
//	}
//
//	return tempEntity;
//}
uint32_t SceneManager::CreateRandomEntityID()
{
	std::random_device rd;  // 난수 생성기
	std::mt19937 gen(rd()); // Mersenne Twister 난수 엔진
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0부터 UINT32_Max 까지의 난수 범위
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
	// std::any에서 std::shared_ptr<Component>를 추출합니다.
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
		VP_ASSERT(false, "해당 타입의 컴포넌트가 존재하지 않습니다.");
		return;
	}
	EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", comp);

	comp->GetEntity()->ReleaseComponent(comp);
	ReleaseCompFromPool(CompID, comp);
}

