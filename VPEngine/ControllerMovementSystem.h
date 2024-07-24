#pragma once
#include "System.h"
class ControllerMovementSystem :
    public System,public IUpdatable,public IPhysicable
{
public:

    ControllerMovementSystem(SceneManager* sceneManager);
        ~ControllerMovementSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;
};

