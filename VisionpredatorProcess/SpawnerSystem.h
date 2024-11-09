#pragma once
#include <System.h>
#include "EventSubscriber.h"
#include "SpawnerComponent.h"
#include "SpawnChildComponent.h"
#include"SpawnSoundComponent.h"
class SpawnerSystem :
    public System,public EventSubscriber,public IStartable
{
public:
    SpawnerSystem(std::shared_ptr<SceneManager> scenemnager);
    void OnInterected(std::any interective_interector);
    void SpawnEntitys(SpawnerComponent* comp);
    void SpawnSounds(SpawnSoundComponent* comp);

    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
    void SpawnPrefabToChild(SpawnChildComponent* startentity, const std::string& prefabName, const std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>>& poses);

};

