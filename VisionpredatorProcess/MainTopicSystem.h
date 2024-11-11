#pragma once
#include <System.h>
#include "EventSubscriber.h"
class MainTopicSystem :
    public System,public EventSubscriber,public IStartable,public IFixedUpdatable
{
public:
    MainTopicSystem(std::shared_ptr<SceneManager> scenemanager);

    void OnChangeTopic(std::any mode);
    std::weak_ptr<Entity> m_MainTopicEntity;

    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
};

