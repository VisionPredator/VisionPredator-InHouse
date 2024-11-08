#pragma once
#include <System.h>
#include "EventSubscriber.h"
class EntityRemoverSystem :
    public System,public ITriggerable,public IStartable, public IPhysicable
{
public : 
    EntityRemoverSystem(std::shared_ptr<SceneManager> scenemanager);
    

    // ITriggerable을(를) 통해 상속됨
    void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;

    void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
    void AddRemoveEntity(std::shared_ptr<Entity> remover, std::shared_ptr<Entity> removing);


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;


    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override {};

    void PhysicsLateUpdate(float deltaTime) override {};

};

