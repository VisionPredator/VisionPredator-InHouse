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



    // ITriggerable��(��) ���� ��ӵ�
    void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;

    void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
    void AddPickup(AutoPickComponent* autopick, GunComponent* pickuped);
    void ReleasePickup(AutoPickComponent* autopick, GunComponent* pickuped);
    void OnAutoPickup(std::any pcomp);
    void AutoPickUp(AutoPickComponent* autopick);
    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
    void RemoveInvalidEntities(AutoPickComponent* autopick); 
    bool IsValidPickupEntity(uint32_t entityID);
	uint32_t FindClosestValidEntity(AutoPickComponent* autopick);
    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;

    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override {};
    void PhysicsLateUpdate(float deltaTime) override {};
};

