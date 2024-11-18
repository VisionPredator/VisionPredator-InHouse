#pragma once
#include <System.h>
#include "EventSubscriber.h"
struct AutoPickComponent;
struct GunComponent;
class AutoPickSystem :
    public System,public ITriggerable,public IFixedUpdatable,public IUpdatable, public IStartable,public EventSubscriber,public IPhysicable
{
public: 
    AutoPickSystem(std::shared_ptr<SceneManager> scenemnager);



    // ITriggerable을(를) 통해 상속됨
    void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;

    void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
    void AddPickup(AutoPickComponent* autopick, GunComponent* pickuped);
    void ReleasePickup(AutoPickComponent* autopick, GunComponent* pickuped);
    void OnAutoPickup(std::any pcomp);
    void AutoPickUp(AutoPickComponent* autopick);
    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
    void RemoveInvalidEntities(AutoPickComponent* autopick); 
    bool IsValidPickupEntity(uint32_t entityID);
	uint32_t FindClosestValidEntity(AutoPickComponent* autopick);
    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;

    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override {};
    void PhysicsLateUpdate(float deltaTime) override {};
};

