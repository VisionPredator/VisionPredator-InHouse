#pragma once
#include <System.h>
#include "EventSubscriber.h"
#include "SpawnerComponent.h"
class SpawnerSystem :
    public System,public EventSubscriber
{
public:
    SpawnerSystem(std::shared_ptr<SceneManager> scenemnager);
    void OnInterected(std::any interective_interector);
    void SpawnEntitys(SpawnerComponent* comp);
};

