#include "pch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Macro\VisPredMacro.h"

SceneSerializer::SceneSerializer(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSerializeScene", CreateSubscriber(&SceneSerializer::OnSerializeScene));
	EventManager::GetInstance().Subscribe("OnDeSerializeScene", CreateSubscriber(&SceneSerializer::OnDeSerializeScene));
}
SceneSerializer::~SceneSerializer()
{
}
void SceneSerializer::Initialize(std::shared_ptr<SceneManager> sceneManager)
{
	m_SceneManager = sceneManager;
}


void SceneSerializer::OnSerializeScene(std::any data)
{
	std::string FilePath = std::any_cast<std::string>(data);
	///Set FilePath
	std::string folderName = "../Data/Scene";
	std::string folderName2 = "../Data/Temp";
	std::filesystem::create_directories(folderName);
	std::filesystem::create_directories(folderName2);
	///��ƼƼ���� ���� ����!
	nlohmann::ordered_json SceneJson;			///ordered_json �ϰ� json�� ���� �˾ƺ���!
	///Json�� ������ ���� ��ġ!
	std::ofstream ofsmapFilePath(FilePath);
	if (!ofsmapFilePath) {
		VP_ASSERT(false, "������ ���µ� �����Ͽ����ϴ�!");
	}
	///TODO ���̸����� json ���� ���� �ֱ�!!
	try {
		for (const auto& entityPair : GetSceneManager()->GetEntityMap())
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
		        // Serialize scene physics data
        VPPhysics::PhysicsInfo ScenePhysicInfo = GetSceneManager()->GetScenePhysic();

		nlohmann::json physicsJson = ScenePhysicInfo;
        SceneJson["PhysicsInfo"] = physicsJson;

        // Write the serialized JSON data to the file
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
				// Initialize entityID to an invalid state or default value
				uint32_t entityID = static_cast<uint32_t>(-1);
				if (entityJson.contains("EntityID")) {
					entityID = entityJson["EntityID"].get<uint32_t>();
				}

				try {
					// Attempt to deserialize the entity
					GetSceneManager()->DeSerializeEntity(entityJson);
				}
				catch (const std::exception& e) 
				{
					VP_ASSERT(false, "EntityID: " + std::to_string(entityID) + " - JSON ó�� �� ������ �߻��߽��ϴ�: " + std::string(e.what()));
					throw;
				}
			}
			EventManager::GetInstance().ScheduleEvent("OnUpdateTransfomData");;
		}
		GetSceneManager()->SetSceneName(SceneName);

		// Deserialize and set physics information
		if (sceneJson.contains("PhysicsInfo"))
		{
			VPPhysics::PhysicsInfo ScenePhysicInfo;
			sceneJson.at("PhysicsInfo").get_to(ScenePhysicInfo);
			GetSceneManager()->SetScenePhysic(ScenePhysicInfo);
		}
	}
	catch (const std::exception& e)
	{
		VP_ASSERT(false, "��ü JSON ó�� �� ������ �߻��߽��ϴ�: " + std::string(e.what()));
	}

	}

void SceneSerializer::OnSerializePrefab(std::any data)
{

}

void SceneSerializer::OnSpawnPrefab(std::any prefabdata)
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
			uint32_t renewEntityID = GetSceneManager()->findOrCreatePair(entityResettingPair, oldEntityID).second;
			std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
			tempEntity->SetEntityID(renewEntityID);
			GetSceneManager()->SetEntityMap(renewEntityID, tempEntity);
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
						entt::meta_any result = myFunctionMeta.invoke(instance, compJson, tempEntity.get());
						if (auto compPPtr = result.try_cast<std::shared_ptr<Component>>())
						{
							auto compPtr = *compPPtr;
							if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Children>())
								for (auto& childID : static_cast<Children*>(compPtr.get())->ChildrenID)
									childID = GetSceneManager()->findOrCreatePair(entityResettingPair, childID).second;
							else if (compPtr->GetHandle()->type().id() == Reflection::GetTypeID<Parent>())
							{
								IsMainEntity = false;
								Parent* parentComponet = static_cast<Parent*>(compPtr.get());
								parentComponet->ParentID = GetSceneManager()->findOrCreatePair(entityResettingPair, parentComponet->ParentID).second;
							}
						}
					}
					else
						VP_ASSERT(false, "Reflection �Լ� ����!");
				}
			}

			if (IsMainEntity)
			{
				mainprefabID = renewEntityID;
			}
		}

		auto Transform = GetSceneManager()->GetEntity(mainprefabID)->GetComponent<TransformComponent>();
		Transform->Local_Location = prefabData.pos;
		Transform->Local_Scale = prefabData.scale;
		VPMath::Matrix rotationMatrix = VPMath::Matrix::CreateLookAt_LH(VPMath::Vector3::Zero, prefabData.direction, VPMath::Vector3::Up);
		Transform->Local_Quaternion = Transform->Local_Quaternion.CreateFromRotationMatrix(rotationMatrix);

	}
}