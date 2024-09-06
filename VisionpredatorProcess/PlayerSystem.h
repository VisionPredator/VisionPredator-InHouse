#pragma once
#include <System.h>
#include "VisPredComponents.h"
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable
{

public:
    PlayerSystem(std::shared_ptr<SceneManager> sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;


    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;
    void PlayerLocation(TransformComponent* comp);
    void PlayerRotation(TransformComponent* comp);
    void PlayerShoot(PlayerComponent& comp);
};

