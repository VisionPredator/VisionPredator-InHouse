#pragma once
#include "System.h"
class ControllerMovementSystem :
    public System,public IPhysicable
{
public:

    ControllerMovementSystem(std::shared_ptr<SceneManager> sceneManager);
        ~ControllerMovementSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void ControllerMoveCalculate( float deltatime ,ControllerComponent& comp);
    //void ControllerUpdate( float deltatime ,ControllerComponent& comp);
    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;
    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsLateUpdate(float deltaTime) override;
};

