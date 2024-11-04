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