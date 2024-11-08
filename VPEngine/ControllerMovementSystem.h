#pragma once
#include "System.h"
class ControllerMovementSystem :
    public System,public IPhysicable
{
public:

    ControllerMovementSystem(std::shared_ptr<SceneManager> sceneManager);
        ~ControllerMovementSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void ControllerMoveCalculate( float deltatime ,ControllerComponent& comp);
    //void ControllerUpdate( float deltatime ,ControllerComponent& comp);
    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;
    // IPhysicable을(를) 통해 상속됨
    void PhysicsLateUpdate(float deltaTime) override;
};

