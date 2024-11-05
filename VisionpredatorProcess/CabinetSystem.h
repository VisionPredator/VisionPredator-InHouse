#pragma once
#include <System.h>
#include "EventSubscriber.h"
class CabinetSystem :
    public System,public EventSubscriber,public IFixedUpdatable,public IStartable
{
public:
    CabinetSystem(std::shared_ptr<SceneManager>scenemanager);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
    void OnInterected(std::any interective_interector);


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;

};

