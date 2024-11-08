#pragma once
#include <System.h>
#include "EventSubscriber.h"
class CabinetSystem :
    public System,public EventSubscriber,public IFixedUpdatable,public IStartable
{
public:
    CabinetSystem(std::shared_ptr<SceneManager>scenemanager);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    void OnInterected(std::any interective_interector);


    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;

};

