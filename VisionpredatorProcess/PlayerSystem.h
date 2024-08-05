#pragma once
#include <System.h>
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable
{

public:
    PlayerSystem(std::shared_ptr<SceneManager> sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;


    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;
};

