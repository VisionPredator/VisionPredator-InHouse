#include "pch.h"
#include "SceneSerializer.h"
#include "EntityManager.h"
#include "Components.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Macro\VisPredMacro.h"

SceneSerializer::SceneSerializer(EntityManager* entityManager) :System(entityManager)
{
	EventManager::GetInstance().Subscribe("OnSerializeScene", CreateSubscriber(&SceneSerializer::OnSerializeScene));
	EventManager::GetInstance().Subscribe("OnSerializePrefab", CreateSubscriber(&SceneSerializer::OnSerializePrefab));

	EventManager::GetInstance().Subscribe("OnDeSerializeScene", CreateSubscriber(&SceneSerializer::OnDeSerializeScene));
	EventManager::GetInstance().Subscribe("OnDeSerializePrefab", CreateSubscriber(&SceneSerializer::OnDeSerializePrefab));
}
SceneSerializer::~SceneSerializer()
{
}
void SceneSerializer::Initialize(EntityManager* entityManager)
{
	m_EntityManager = entityManager;
}


void SceneSerializer::OnSerializeScene(std::any data)
{
	///Set FilePath
	std::string folderName = "../Resource/Map/";
	std::string sceneName = m_EntityManager->GetSceneName();
	std::string fileExtension = ".json";
	std::string filePath = folderName + sceneName + fileExtension;

	///��ƼƼ���� ���� ����!
	nlohmann::ordered_json SceneJson;			///ordered_json �ϰ� json�� ���� �˾ƺ���!
	///Json�� ������ ���� ��ġ!
	std::ofstream ofsmapFilePath(filePath);
	if (!ofsmapFilePath) {
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");
	}
	///TODO ���̸����� json ���� ���� �ֱ�!!
	try {
		for (const auto& entityPair : m_EntityManager->GetEntityMap())
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




/// <summary>
/// �Լ��� �������� Scene�ʱ�ȭ ���ֱ�!!
/// </summary>
/// <param name="data"></param>
void SceneSerializer::OnDeSerializeScene(std::any data)
{
	if (data.type() != typeid(std::string))
	{
		// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
		VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
		return;
	}

	///Set FilePath
	std::string floderName = "..\\Resource\\Map\\";
	std::string sceneName = std::any_cast<std::string>(data);
	std::string fileExtension = ".json";
	std::string filePath = floderName + sceneName + fileExtension;

	///�� ������ �������� + ����.
	std::ifstream inputFile(filePath);
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
				m_EntityManager->DeSerializeEntity(entityJson);
			}
		}
		m_EntityManager->SetSceneName(sceneName);
	}
	catch (const std::exception&)
	{
		VP_ASSERT(false, "JSON ó�� �� ������ �߻��߽��ϴ�: ");
	}
	inputFile.close();
}

void SceneSerializer::OnSerializePrefab(std::any data)
{
	try
	{
		uint32_t entityID = std::any_cast<uint32_t>(data);
		m_EntityManager->SerializePrefab(entityID);
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}
}

void SceneSerializer::OnDeSerializePrefab(std::any data)
{
}
