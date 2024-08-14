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
	///엔티티들을 담을 공간!
	nlohmann::ordered_json SceneJson;			///ordered_json 하고 json의 차이 알아보기!
	///Json을 저장할 파일 위치!
	std::ofstream ofsmapFilePath(FilePath);
	if (!ofsmapFilePath) {
		VP_ASSERT(false, "파일을 여는데 실패하였습니다!");
	}
	///TODO 씬이름으로 json 파일 만들어서 넣기!!
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
		VP_ASSERT(false, "Json 입력 중 에러가 났습니다.");
	}

	ofsmapFilePath.close();
}





void SceneSerializer::OnDeSerializeScene(std::any data)
{
	if (data.type() != typeid(std::string))
	{
		// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
		VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
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


	///씬 데이터 가져오기 + 복사.
	std::ifstream inputFile(FilePath);
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
					VP_ASSERT(false, "EntityID: " + std::to_string(entityID) + " - JSON 처리 중 오류가 발생했습니다: " + std::string(e.what()));
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
		VP_ASSERT(false, "전체 JSON 처리 중 오류가 발생했습니다: " + std::string(e.what()));
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
		// 테스트코드
		auto count = prefabJson.size();
		for (const auto& entityJson : prefabJson)
		{
			const uint32_t oldEntityID = entityJson["EntityID"].get<uint32_t>();
			uint32_t renewEntityID = GetSceneManager()->findOrCreatePair(entityResettingPair, oldEntityID).second;
			std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
			tempEntity->SetEntityID(renewEntityID);
			GetSceneManager()->SetEntityMap(renewEntityID, tempEntity);
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
						VP_ASSERT(false, "Reflection 함수 실패!");
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