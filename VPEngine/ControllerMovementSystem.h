#pragma once
#include "System.h"
class ControllerMovementSystem :
    public System,public IUpdatable,public IPhysicable
{
public:

    ControllerMovementSystem(SceneManager* sceneManager);
        ~ControllerMovementSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;
};

