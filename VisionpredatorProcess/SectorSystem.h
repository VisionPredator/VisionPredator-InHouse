#pragma once
#include <System.h>
#include "EventSubscriber.h"
#include "VisPredComponents.h"
class SectorSystem :
    public System, public EventSubscriber
{
public:
    SectorSystem(std::shared_ptr<SceneManager> scenemanager);
    void OnUpdateSector(std::any enemyid);
    void CheckSectorClear(SectorClearComponent* sectorcomp);
};

