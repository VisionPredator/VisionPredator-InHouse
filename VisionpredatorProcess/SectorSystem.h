#pragma once
#include <System.h>
#include "EventSubscriber.h"
#include "VisPredComponents.h"
class SectorSystem :
    public System, public EventSubscriber,public IStartable
{
public:
    SectorSystem(std::shared_ptr<SceneManager> scenemanager);
    
    void OnEnemyKilled(std::any enemyid);
    void CheckSectorClear(SectorClearComponent* sectorcomp);
    //void CheckSectoStart(SectorClearComponent* sectorcomp);


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;

    //// IStartable을(를) 통해 상속됨
    //void Initialize() override;
    //void Start(uint32_t gameObjectId) override;
    //void Finish(uint32_t gameObjectId) override;
    //void Finalize() override;
};

