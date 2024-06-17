#include "pch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Macro\VisPredMacro.h"

SceneSerializer::SceneSerializer(SceneManager* sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSerializeScene", CreateSubscriber(&SceneSerializer::OnSerializeScene));
	EventManager::GetInstance().Subscribe("OnDeSerializeScene", CreateSubscriber(&SceneSerializer::OnDeSerializeScene));
}
SceneSerializer::~SceneSerializer()
{
}
void SceneSerializer::Initialize(SceneManager* sceneManager)
{
	m_SceneManager = sceneManager;
}


void SceneSerializer::OnSerializeScene(std::any data)
{
	///Set FilePath
	std::string folderName = "../Resource/Map/";
	std::string sceneName = m_SceneManager->GetSceneName();
	std::string fileExtension = ".json";
	std::string filePath = folderName + sceneName + fileExtension;

	///엔티티들을 담을 공간!
	nlohmann::ordered_json SceneJson;			///ordered_json 하고 json의 차이 알아보기!
	///Json을 저장할 파일 위치!
	std::ofstream ofsmapFilePath(filePath);
	if (!ofsmapFilePath) {
		VP_ASSERT(false, "파일을 여는데 실패하였습니다!");
	}
	///TODO 씬이름으로 json 파일 만들어서 넣기!!
	try {
		for (const auto& entityPair : m_SceneManager->GetEntityMap())
		{
			nlohmann::ordered_json entityJson;
			entityJson["EntityID"] = entityPair.first;
			for (const auto& [id, comp] : entityPair.second->m_OwnedComp)
			{
				nlohmann::json compnentEntity;
				comp->SerializeComponent(compnentEntity);
				compnentEntity["ComponentID"] = id;
				entityJson["Component"].push_back(compnentEntity);
			}
			SceneJson["Entitys"].push_back(entityJson);
		}
		ofsmapFilePath << SceneJson.dump(4);
	}
	catch (const std::exception&)
	{
		VP_ASSERT(false, "Json 입력 중 에러가 났습니다.");
	}
	ofsmapFilePath.close();
}




/// <summary>
/// 함수를 쓰기전에 Scene초기화 해주기!!
/// </summary>
/// <param name="data"></param>
void SceneSerializer::OnDeSerializeScene(std::any data)
{
	if (data.type() != typeid(std::string))
	{
		// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
		VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
		return;
	}

	///Set FilePath
	std::string floderName = "..\\Resource\\Map\\";
	std::string sceneName = std::any_cast<std::string>(data);
	std::string fileExtension = ".json";
	std::string filePath = floderName + sceneName + fileExtension;

	///씬 데이터 가져오기 + 복사.
	std::ifstream inputFile(filePath);
	if (!inputFile.is_open())
		VP_ASSERT(false, "파일이 존재하지 않아 열 수 없습니다.");
	try {
		// JSON 데이터 스트림 처리
		nlohmann::json sceneJson = nlohmann::json::parse(inputFile, nullptr, true, true);
		// 엔티티 데이터 처리
		if (sceneJson.contains("Entitys"))
		{
			for (auto& entityJson : sceneJson["Entitys"])
			{
				m_SceneManager->DeSerializeEntity(entityJson);
			}
		}
		m_SceneManager->SetSceneName(sceneName);
	}
	catch (const std::exception&)
	{
		VP_ASSERT(false, "JSON 처리 중 오류가 발생했습니다: ");
	}
	inputFile.close();
}

