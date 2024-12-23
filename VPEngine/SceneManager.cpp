#include "pch.h"
#include "SceneManager.h"
#include "Components.h"
#include <fstream>
#include <random>
#include <queue>
#include "TransformSystem.h"
#include "../VPGraphics/Log.h"

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
	EventManager::GetInstance().Subscribe("OnRemoveComponent", CreateSubscriber(&SceneManager::OnRemoveComponent), EventType::ADD_DELETE);
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
	// 컴포넌트 풀에 컴포넌트를 추가합니다.
	auto& pool = m_CurrentScene->m_ComponentPool[comp->GetHandle()->type().id()];
	pool.push_back(comp);
}
#pragma region Delete Entity - Editior
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

		EventManager::GetInstance().ScheduleEvent("OnDeleteEntity", DeleteEntityID);
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

	// entityMap에서 해당 entityID를 가진 엔티티를 찾고 제거합니다.
	if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
	{
		auto ownedComponents = entityIter->second->GetOwnedComponents();

		// componentMap에서 해당 entityID와 연관된 컴포넌트를 찾아 제거합니다.
		for (auto& sharedComp : ownedComponents)
		{
			EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
		}

		// m_ComponentCache에서 entityID와 연관된 항목들을 모두 제거합니다.
		std::erase_if(m_ComponentCache, [entityID](const auto& pair) {
			return pair.first.first == entityID;
			});

		// 마지막으로, entityMap에서 엔티티를 제거합니다.
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

	// entityMap에서 해당 entityID를 가진 엔티티를 찾고 제거합니다.
	if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
	{
		// 엔티티가 소유한 모든 컴포넌트를 가져옵니다.
		auto ownedComponents = entity->GetOwnedComponents();

		// 각 컴포넌트에 대해 이벤트를 발생시킵니다.
		for (auto& sharedComp : ownedComponents)
		{
			EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
		}

		// componentMap에서 해당 엔티티와 연관된 컴포넌트들을 제거합니다.
		auto& componentMap = m_CurrentScene->m_ComponentPool;
		componentMap.erase(entityID);

		// componentCache에서 해당 엔티티와 연관된 항목들을 모두 제거합니다.
		auto& componentCache = m_ComponentCache;
		std::erase_if(componentCache, [entityID](const auto& pair) {
			return pair.first.first == entityID;
			});

		// 마지막으로, entityMap에서 엔티티를 제거합니다.
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

	Log::GetClientLogger()->info("Open " + m_CurrentScene->SceneName + " scene.");
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

	///TODO: 씬 시작시 설정할 Initialize
	//EventManager::GetInstance().ImmediateEvent("OnInitialize");
	///TODO: 씬 시작시 설정할 Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystems");
}

void SceneManager::OnEndScene(std::any data)
{
	///TODO:씬 끝났을 때 처리할 Finalize?
	//EventManager::GetInstance().ImmediateEvent("OnFinalize");
	///TODO: 씬 끝났을 때 설정할 Initializesystem.
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystems");

}

void SceneManager::SpawnPrefab(std::string prefabname, VPMath::Vector3 pos, VPMath::Vector3 rotation, VPMath::Vector3 scele)
{
	// Create scale matrix
	PrefabData temp = { CreateRandomEntityID(),prefabname ,pos,rotation,scele };
	std::any data = temp;
	EventManager::GetInstance().ScheduleEvent("OnSpawnPrefab", data);
	return;
}

std::shared_ptr<Entity> SceneManager::SpawnEditablePrefab(std::string prefabname, VPMath::Vector3 pos, VPMath::Quaternion Quater, VPMath::Vector3 scele)
{
	VPMath::Vector3 rot = Quater.ToEuler() * 180 / VPMath::XM_PI;
	return SpawnEditablePrefab(prefabname, pos, rot, scele);
}
std::shared_ptr<Entity> SceneManager::SpawnEditablePrefab(std::string prefabname, VPMath::Vector3 pos, VPMath::Vector3 rotation, VPMath::Vector3 scale)
{
	PrefabData prefabData = { CreateRandomEntityID(), prefabname, pos, rotation, scale };

	// 캐시 확인
	if (m_PrefabCache.find(prefabname) != m_PrefabCache.end())
	{
		// 캐시에 있으면 복제해서 사용
		return ClonePrefab(prefabname, prefabData);
	}

	std::vector<std::shared_ptr<Entity>> createdEntities;
	std::ifstream inputFile(prefabData.prefabname);
	std::vector<std::pair<uint32_t, uint32_t>> entityResettingPair;
	uint32_t mainprefabID;

	if (!inputFile.is_open())
		return nullptr;

	nlohmann::json prefabJson;
	inputFile >> prefabJson;

	for (const auto& entityJson : prefabJson)
	{
		const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
		uint32_t renewEntityID = findOrCreatePair(entityResettingPair, oldEntityID).second;
		std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
		tempEntity->SetEntityID(renewEntityID);
		SetEntityMap(renewEntityID, tempEntity);

		bool IsMainEntity = true;
		for (const nlohmann::json& compJson : entityJson["Component"])
		{
			entt::id_type comp_id = (entt::id_type)compJson["ComponentID"];
			auto metaType = entt::resolve(comp_id);
			if (metaType)
			{
				auto instance = metaType.construct();
				auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
				if (myFunctionMeta)
				{
					entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity.get(), true, false);
					if (auto compPPtr = result.try_cast<std::shared_ptr<Component>>())
					{
						auto compPtr = *compPPtr;
						if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Children>())
							for (auto& childID : static_cast<Children*>(compPtr.get())->ChildrenID)
								childID = findOrCreatePair(entityResettingPair, childID).second;
						else if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
						{
							IsMainEntity = false;
							Parent* parentComponent = static_cast<Parent*>(compPtr.get());
							parentComponent->ParentID = findOrCreatePair(entityResettingPair, parentComponent->ParentID).second;
						}
					}
				}
				else
					VP_ASSERT(false, "Reflection 함수 실패!");
			}
		}
		EventManager::GetInstance().ScheduleEvent("OnAddEntityComponentsToScene", renewEntityID);

		if (IsMainEntity)
			mainprefabID = renewEntityID;

		createdEntities.push_back(tempEntity);
	}

	// 캐시에 복사본으로 저장
	std::vector<std::shared_ptr<Entity>> independentCopies;
	for (const auto& entity : createdEntities) {
		std::shared_ptr<Entity> cloneEntity = entity->Clone(entity->GetEntityID());
		independentCopies.push_back(cloneEntity);
	}
	m_PrefabCache[prefabname] = independentCopies;

	auto Transform = GetEntity(mainprefabID)->GetComponent<TransformComponent>();
	Transform->SetLocalLocation(prefabData.pos);
	if (prefabData.scale.x > 0)
		Transform->SetLocalScale(prefabData.scale);
	Transform->SetLocalRotation(prefabData.rotation);
	for (auto [old, newID] : entityResettingPair)
		EventManager::GetInstance().ScheduleEvent("OnStart", newID);

	return GetEntity(mainprefabID);
}


//std::shared_ptr<Entity> SceneManager::SpawnSoundEntity(std::string soundName, float volume, bool isloop, VPMath::Vector3 pos)
//{
//	auto entity = CreateEntity(soundName);
//	auto soundcomp = entity->AddComponent<SoundComponent>();
//	soundcomp->SoundPath = soundName;
//	soundcomp->Volume = volume;
//	soundcomp->Loop= isloop;
//	entity->GetComponent<TransformComponent>()->Local_Location = pos;
//	EventManager::GetInstance().ScheduleEvent("OnStart", entity->GetEntityID());
//	return std::shared_ptr<Entity>();
//}

std::shared_ptr<Entity> SceneManager::SpawnSoundEntity(std::string soundName, int volume, bool Is2D, bool isloop, VPMath::Vector3 pos)
{
	auto entity = CreateEntity(soundName);
	auto soundcomp = entity->AddComponent<SoundComponent>();
	auto identity = entity->AddComponent<IdentityComponent>();
	identity->UUID = soundName;
	soundcomp->SoundPath = soundName;
	soundcomp->Is2D = Is2D;
	soundcomp->Volume = volume;
	soundcomp->Loop = isloop;
	entity->GetComponent<TransformComponent>()->Local_Location = pos;
	EventManager::GetInstance().ScheduleEvent("OnStart", entity->GetEntityID());
	return entity;
}




void SceneManager::SerializePrefab(uint32_t entityID, bool immidiate)
{
	if (immidiate)
	{
		OnSerializePrefab(entityID);
	}
	else
	{
	EventManager::GetInstance().ScheduleEvent("OnSerializePrefab", entityID);

	}
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

	// 만약 prefabname이 m_PrefabCache에 있다면 삭제
	if (m_PrefabCache.find(filePath) != m_PrefabCache.end())
	{
		m_PrefabCache.erase(filePath);
	}


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
	std::vector<std::pair<uint32_t, uint32_t>> entityResettingPair{};

	if (inputFile.is_open())
	{
		nlohmann::json prefabJson;
		inputFile >> prefabJson;
		// 테스트코드
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
						VP_ASSERT(false, "Reflection 함수 실패!");
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
			// 메타 타입으로부터 인스턴스를 생성합니다.
			auto instance = metaType.construct();
			// 특정 함수를 찾고 호출합니다.
			auto myFunctionMeta = metaType.func("DeserializeComponent"_hs);
			if (myFunctionMeta)
				myFunctionMeta.invoke(instance, (nlohmann::json&)componentjson, tempEntity.get(), Immidate, false);
			else
				VP_ASSERT(false, "Reflection 함수 실패!");
		}

	}
		EventManager::GetInstance().ImmediateEvent("OnAddEntityComponentsToScene", entityID);

	return tempEntity;
}
std::shared_ptr<Entity> SceneManager::ClonePrefab(const std::string& prefabname, PrefabData& prefabData)
{
	const auto& entitiesToClone = m_PrefabCache[prefabname];
	std::vector<std::pair<uint32_t, uint32_t>> entityResettingPair; // ID 매핑 저장
	uint32_t mainEntityID = 0;
	std::unordered_map<uint32_t, std::shared_ptr<Entity>> clonedEntities;

	// 1. 엔티티 복제 및 ID 매핑 생성
	for (const auto& originalEntity : entitiesToClone)
	{
		uint32_t newEntityID = findOrCreatePair(entityResettingPair, originalEntity->GetEntityID()).second;
		std::shared_ptr<Entity> clonedEntity = originalEntity->Clone(newEntityID);
		SetEntityMap(newEntityID, clonedEntity);
		clonedEntities[newEntityID] = clonedEntity;

		if (mainEntityID == 0)
			mainEntityID = newEntityID;
	}

	// 2. 관계 설정을 위한 ID 매핑 적용
	for (auto& [newEntityID, clonedEntity] : clonedEntities)
	{
		// Children 관계 설정
		if (clonedEntity->HasComponent<Children>())
		{
			auto childrenComp = clonedEntity->GetComponent<Children>();
			for (auto& childID : childrenComp->ChildrenID)
			{
				childID = findOrCreatePair(entityResettingPair, childID).second; // 새 ID로 매핑
				int a = childID;
			}
		}

		// Parent 관계 설정
		if (clonedEntity->HasComponent<Parent>())
		{
			auto parentComp = clonedEntity->GetComponent<Parent>();
			parentComp->ParentID = findOrCreatePair(entityResettingPair, parentComp->ParentID).second; // 새 ID로 매핑
		}

		EventManager::GetInstance().ScheduleEvent("OnAddEntityComponentsToScene", newEntityID);
	}

	// 3. 메인 엔티티의 트랜스폼 설정
	auto mainTransform = GetEntity(mainEntityID)->GetComponent<TransformComponent>();
	mainTransform->SetLocalLocation(prefabData.pos);
	if (prefabData.scale.x > 0)
		mainTransform->SetLocalScale(prefabData.scale);
	mainTransform->SetLocalRotation(prefabData.rotation);

	EventManager::GetInstance().ScheduleEvent("OnUpdate");
	for (const auto& [oldID, newID] : entityResettingPair)
		EventManager::GetInstance().ScheduleEvent("OnStart", newID);

	return GetEntity(mainEntityID);
}


std::shared_ptr<Entity> SceneManager::CreateEntity()
{
	return CreateEntity("Entity");
}
std::shared_ptr<Entity> SceneManager::CreateEntity(std::string entityName)
{
	// 난수 생성기를 사용하여 엔티티 ID를 생성합니다.
	uint32_t id = VPMath::Random_uint(0, UINT32_MAX); 


	// 생성된 ID가 이미 존재하는지 확인하고, 존재하면 새로운 ID를 생성합니다.
	while (HasEntity(id))
	{
		id = VPMath::Random_uint(0, UINT32_MAX);
	}

	// 새로운 엔티티를 생성하고 ID를 설정합니다.
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
	tempEntity->SetEntityID(id);
	SetEntityMap(id, tempEntity);

	// IDComponent와 TransformComponent를 추가합니다.
	std::shared_ptr<IDComponent> IDComp = tempEntity->AddComponent<IDComponent>();
	std::shared_ptr<TransformComponent> TransformComp = tempEntity->AddComponent<TransformComponent>();

	IDComp->Name = entityName;
	// IDComponent의 이름을 설정합니다.
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
	std::random_device rd;  // 난수 생성기
	std::mt19937 gen(rd()); // Mersenne Twister 난수 엔진
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX); // 0부터 UINT32_Max 까지의 난수 범위
	uint32_t id = VPMath::Random_uint(0, UINT32_MAX);
	while (HasEntity(id))
		id = VPMath::Random_uint(0, UINT32_MAX);
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

	// 컴포넌트를 엔티티에서 삭제
	comp->GetEntity()->removeComponent(comp);

	// 캐시에서 컴포넌트를 삭제
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
	if (!inputFile.is_open())
		return;
	nlohmann::json prefabJson;
	inputFile >> prefabJson;

	for (const auto& entityJson : prefabJson)
	{
		const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
		uint32_t renewEntityID = findOrCreatePair(entityResettingPair, oldEntityID).second;
		std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
		tempEntity->SetEntityID(renewEntityID);
		SetEntityMap(renewEntityID, tempEntity);
		///메인 Entity인가?
		bool IsMainEntity = true;
		///컴포넌트 배치.
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
					entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity.get(), true, false);
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
					VP_ASSERT(false, "Reflection 함수 실패!");
			}
		}
		EventManager::GetInstance().ImmediateEvent("OnAddEntityComponentsToScene", renewEntityID);

		if (IsMainEntity)
			mainprefabID = renewEntityID;
	}
	auto Transform = GetEntity(mainprefabID)->GetComponent<TransformComponent>();
	Transform->SetLocalLocation(prefabData.pos);
	if (prefabData.scale.x > 0)
		Transform->SetLocalScale(prefabData.scale);
	Transform->SetLocalRotation(prefabData.rotation);
	EventManager::GetInstance().ImmediateEvent("OnUpdate");

	for (auto [old, newID] : entityResettingPair)
		EventManager::GetInstance().ImmediateEvent("OnStart", newID);
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

		// GetEntity 호출을 최소화
		auto currentEntity = GetEntity(childID);
		auto idComponent = currentEntity->GetComponent<IDComponent>();
		if ( idComponent->Name == name)
		{
			return currentEntity;
		}

		// 자식 엔티티의 ID를 큐에 추가
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

std::shared_ptr<Entity> SceneManager::GetRelationEntityByName(uint32_t entityID, std::string name)
{
	uint32_t mainID = entityID;
	std::queue<uint32_t> ChildrenIDs;
	if (!HasEntity(mainID))
		return nullptr;
	
	while (GetEntity(mainID)->HasComponent<Parent>())
	{
		mainID = GetEntity(mainID)->GetComponent<Parent>()->ParentID;
	}
	ChildrenIDs.push(mainID);

	while (!ChildrenIDs.empty())
	{
		uint32_t childID = ChildrenIDs.front();
		ChildrenIDs.pop();

		// GetEntity 호출을 최소화
		auto currentEntity = GetEntity(childID);
		auto idComponent = currentEntity->GetComponent<IDComponent>();
		if (idComponent->Name == name)
		{
			return currentEntity;
		}

		// 자식 엔티티의 ID를 큐에 추가
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

std::shared_ptr<Entity> SceneManager::GetEntityByIdentityName( std::string name) 
{
	// IdentityComponent의 컴포넌트 풀을 가져옵니다.
	auto comppool = GetComponentPool<IdentityComponent>();
	// 컴포넌트 풀이 비어 있는지 확인합니다.
	if (comppool.empty()) 
		return nullptr;

	// 컴포넌트 풀을 순회합니다.
	for (auto& identity : comppool) 
	{
		// UUID가 주어진 이름과 일치하는지 확인합니다.
		if (identity.get().UUID == name) 
		{
			// entityID로 엔티티를 검색합니다.
			auto entity = GetEntity(identity.get().GetEntityID());

			// 엔티티가 Scene의 EntityMap에 존재하는지 확인합니다.
			if (entity) 
				return entity;
			else
				return nullptr;
		}
	}
	// 일치하는 IdentityComponent가 없는 경우 처리
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
	auto temp = GetEntity(mainID);
	if (!temp)
		return;

	std::list<uint32_t> DeleteEntityIDs;
	DeleteEntityIDs.push_back(mainID);
	std::vector<uint32_t> DeleteEntity;

	// Remove parent-child relationships at once
	if (HasComponent<Parent>(mainID))
	{
		auto parentEntity = GetEntity(GetComponent<Parent>(mainID)->ParentID);
		if (parentEntity && parentEntity->HasComponent<Children>())
		{
			auto parentsChildren = parentEntity->GetComponent<Children>();
			parentsChildren->ChildrenID.remove(mainID);
		}
		RemoveParent(mainID, true);
	}

	while (!DeleteEntityIDs.empty())
	{
		uint32_t DeleteEntityID = DeleteEntityIDs.front();
		DeleteEntity.push_back(DeleteEntityID);
		auto entity = GetEntity(DeleteEntityID);
		DeleteEntityIDs.pop_front();

		if (entity && entity->HasComponent<Children>())
		{
			auto children = entity->GetComponent<Children>();
			DeleteEntityIDs.insert(DeleteEntityIDs.end(), children->ChildrenID.begin(), children->ChildrenID.end());
		}
	}

	for (auto entityID : DeleteEntity)
	{
		if (auto entityIter = entityMap.find(entityID); entityIter != entityMap.end())
		{
			EventManager::GetInstance().ImmediateEvent("OnFinish", entityID);

			auto componentMap = entityIter->second->GetOwnedComponents();
			for (auto& sharedComp : componentMap)
			{
				EventManager::GetInstance().ImmediateEvent("OnReleasedComponent", sharedComp.get());
			}

			// Clear all related items in ComponentCache
			std::erase_if(m_ComponentCache, [entityID](const auto& pair) {
				return pair.first.first == entityID;
				});

			entityMap.erase(entityIter);  // Remove entity from EntityMap
		}
	}
}



#pragma region Trash 

#pragma endregion