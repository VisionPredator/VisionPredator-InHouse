#include "pch.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Components.h"
#include <fstream>

SceneManager::SceneManager()
{
	EventManager::GetInstance().Subscribe("OnChangeScene", CreateSubscriber(&SceneManager::OnChangeScene),EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnEndScene", CreateSubscriber(&SceneManager::OnEndScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnResetScene", CreateSubscriber(&SceneManager::OnResetScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOpenScene", CreateSubscriber(&SceneManager::OnOpenScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnStartScene", CreateSubscriber(&SceneManager::OnStartScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&SceneManager::OnDestroyEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnClearEntity", CreateSubscriber(&SceneManager::OnClearEntity), EventType::ADD_DELETE);

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
	m_CurrentScene->SceneName = "TempScene";
	EventManager::GetInstance().ScheduleEvent("OnStartScene");
}
void SceneManager::Finalize()
{
}
void SceneManager::OnDestroyEntity(std::any data)
{
	// EntityMap에서 해당 Entity를 찾습니다.
	uint32_t entityID = std::any_cast <int>(data);

	auto& entityMap = GetEntityMap();

	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		Entity* entityToRemove = entityIter->second;
		// m_ComponentPool에서 해당 Entity에 해당하는 컴포넌트들을 제거합니다.
		for (auto& compPair :GetScene().m_ComponentPool)
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

void SceneManager::OnClearEntity(std::any data)
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
//씬 체인지 이벤트 :: SchduleEvent로 호출 권장!
void SceneManager::OnChangeScene(std::any data)
{

	try
	{
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}

		//씬 끝나는 이벤트 즉시 실행
		EventManager::GetInstance().GetInstance().ImmediateEvent("OnEndScene");
		//씬 열기 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnOpenScene", data);

	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
	}
}
void SceneManager::OnStartScene(std::any data)
{
	///TODO: 씬 시작시 설정할 init??
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
	///TODO:씬 끝났을 때 처리할 Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystem");

}

void SceneManager::OnOpenScene(std::any data)
{
	try
	{
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}
		//씬 초기화 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnResetScene");
		//씬 시리얼라이즈 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
		//씬 시작 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnStartScene");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
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
