#pragma once
#include "System.h"
#include "EventSubscriber.h"
#include "VisPredComponents.h"
class GunSystem :
    public System, public IUpdatable,public IContactable,public EventSubscriber,public IPhysicable
{
public:
    GunSystem(std::shared_ptr<SceneManager> scenemanager);
    ~GunSystem() = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IContactable을(를) 통해 상속됨
    void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;
    void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override;
    void ApplyDamage(Entity& gun, Entity& Other);
    void OnShoot(std::any entityid);
    //EventManager::GetInstance().ImmediateEvent("OnBulletHit", std::make_tuple(attackedEntityID, guncomp.Damage1, hitlocation));
    void OnInterected(std::any interective_interector);
    void Interected_Gun(GunComponent& guncomp, PlayerComponent& playercomp);

    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override {};
    void PhysicsLateUpdate(float deltaTime) override {};
};

