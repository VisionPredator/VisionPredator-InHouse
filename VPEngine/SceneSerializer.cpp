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
	std::string FilePath = std::any_cast<std::string>(data);
	///Set FilePath
	std::string folderName = "../Data/Scene";
	//std::string sceneName = m_SceneManager->GetSceneName();
	//std::string fileExtension = ".json";
	//std::string filePath = folderName + sceneName + fileExtension;
	// Ensure directory exists before creating the file
	std::filesystem::create_directories(folderName);
	///��ƼƼ���� ���� ����!
	nlohmann::ordered_json SceneJson;			///ordered_json �ϰ� json�� ���� �˾ƺ���!
	///Json�� ������ ���� ��ġ!
	std::ofstream ofsmapFilePath(FilePath);
	if (!ofsmapFilePath) {
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");
	}
	///TODO ���̸����� json ���� ���� �ֱ�!!
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
		VP_ASSERT(false, "Json �Է� �� ������ �����ϴ�.");
	}
	ofsmapFilePath.close();
}





void SceneSerializer::OnDeSerializeScene(std::any data)
{
	if (data.type() != typeid(std::string))
	{
		// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
		VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
		return;
	}

	///Set FilePath
	//std::string floderName = "../Data/Scene";
	//std::string sceneName = std::any_cast<std::string>(data);
	//std::string fileExtension = ".json";
	//std::string filePath = floderName + sceneName + fileExtension;

	std::string FilePath = std::any_cast<std::string>(data);

	std::string path = FilePath;
	std::replace(path.begin(), path.end(), '/', '\\');
	// Find the position of the last backslash (\)
	size_t lastBackslash = path.find_last_of('\\');
	// Find the position of the dot (.) after the last backslash
	size_t dotPosition = path.find('.', lastBackslash);
	// Extract the substring between the last backslash and the dot
	std::string SceneName = path.substr(lastBackslash + 1, dotPosition - lastBackslash - 1);


	///�� ������ �������� + ����.
	std::ifstream inputFile(FilePath);
	if (!inputFile.is_open())
		VP_ASSERT(false, "������ �������� �ʾ� �� �� �����ϴ�.");
	try {
		// JSON ������ ��Ʈ�� ó��
		nlohmann::json sceneJson = nlohmann::json::parse(inputFile, nullptr, true, true);
		// ��ƼƼ ������ ó��
		if (sceneJson.contains("Entitys"))
		{
			for (auto& entityJson : sceneJson["Entitys"])
			{
				EventManager::GetInstance().ImmediateEvent("OnDeSerializeEntity", entityJson);
				m_SceneManager->DeSerializeEntity(entityJson);
			}
		}
		m_SceneManager->SetSceneName(SceneName);
	}
	catch (const std::exception&)
	{
		VP_ASSERT(false, "JSON ó�� �� ������ �߻��߽��ϴ�: ");
	}
	inputFile.close();
}

