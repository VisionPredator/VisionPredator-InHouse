#include "pch.h"
#include "SpawnerSystem.h"
#include "EventManager.h"
#include "VisPredComponents.h"
SpawnerSystem::SpawnerSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&SpawnerSystem::OnInterected));
}

void SpawnerSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);

	if (!interected->HasComponent<SpawnerComponent>())
		return;
	SpawnEntitys(interected->GetComponent<SpawnerComponent>());
	auto temp = interected->GetComponent<InterectiveComponent>();
	GetSceneManager()->SpawnSoundEntity(temp->Soundkey, temp->Volume,false, false, interected->GetComponent<TransformComponent>()->World_Location);

}

void SpawnerSystem::SpawnEntitys(SpawnerComponent* comp) 
{
    if (comp->SpawnPrefab.empty())
        return;
    const size_t transformSize = comp->SpawnTransform.size();
    const VPMath::Vector3 defaultPos{ 0.0f, 0.0f, 0.0f };
	const VPMath::Vector3 defaultRotation{ 0.0f, 0.0f, 0.0f };
	const VPMath::Vector3 defaultScale{ 1.0f, 1.0f, 1.0f };


	for (size_t i = 0; i < comp->SpawnPrefab.size(); ++i)
	{
		VPMath::Vector3 pos = defaultPos;
		VPMath::Vector3 rotation = defaultRotation;
		VPMath::Vector3 scale = defaultScale;

		if (i < transformSize)
			std::tie(pos, rotation, scale) = comp->SpawnTransform[i];

		GetSceneManager()->SpawnPrefab(comp->SpawnPrefab[i], pos, rotation, scale);
	}
}

void SpawnerSystem::Initialize()
{
	COMPLOOP(SpawnChildComponent, comp)
	{
		Start(comp.GetEntityID());
	}
}

void SpawnerSystem::Start(uint32_t gameObjectId)
{
	auto startentity = GetSceneManager()->GetEntity(gameObjectId);
	if (startentity->HasComponent<SpawnChildComponent>())
	{
		auto comp = startentity->GetComponent<SpawnChildComponent>();

		SpawnPrefabToChild(comp, comp->Prefab1, comp->Prefab1_Pose);
		SpawnPrefabToChild(comp, comp->Prefab2, comp->Prefab2_Pose);
		SpawnPrefabToChild(comp, comp->Prefab3, comp->Prefab3_Pose);
		SpawnPrefabToChild(comp, comp->Prefab4, comp->Prefab4_Pose);
		SpawnPrefabToChild(comp, comp->Prefab5, comp->Prefab5_Pose);
	}
}

void SpawnerSystem::Finish(uint32_t gameObjectId)
{
}

void SpawnerSystem::Finalize()
{
}

void SpawnerSystem::SpawnPrefabToChild(SpawnChildComponent* spawncomp, const std::string& prefabName, const std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>>& poses)
{
	// ��ġ, ȸ��, �����Ͽ� ���� �⺻ �� ����
	const VPMath::Vector3 defaultPos{ 0.0f, 0.0f, 0.0f };
	const VPMath::Vector3 defaultRotation{ 0.0f, 0.0f, 0.0f };
	const VPMath::Vector3 defaultScale{ 0.2f, 0.2f, 0.2f };

	// �� pose�� ���� �ݺ�
	for (const auto& pose : poses)
	{
		// ������ ��ƼƼ�� ����
		auto spawnentity = GetSceneManager()->SpawnEditablePrefab(prefabName, {}, VPMath::Vector3{}, {});
		if (!spawnentity)
			continue;

		VPMath::Vector3 pos = defaultPos;
		VPMath::Vector3 rotation = defaultRotation;
		VPMath::Vector3 scale = defaultScale;
	auto a = 	spawnentity->GetComponent<IDComponent>()->Name;
		// pose Ʃ�ÿ��� ��ġ, ȸ��, �������� ����
		std::tie(pos, rotation, scale) = pose;
		// ������ ��ƼƼ�� start ��ƼƼ�� �ڽ����� �߰�
		GetSceneManager()->AddChild(spawncomp->GetEntityID(), spawnentity->GetEntityID(), true);
		// ������ ��ƼƼ�� TransformComponent ��������
		auto spawnentitytrans = spawnentity->GetComponent<TransformComponent>();
		spawnentitytrans->SetLocalLocation(pos);
		spawnentitytrans->SetLocalRotation(rotation);
		spawnentitytrans->SetLocalScale(scale);
	}
}

