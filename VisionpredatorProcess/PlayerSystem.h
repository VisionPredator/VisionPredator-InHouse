#pragma once
#include <System.h>
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,  public IFixedUpdatable
{

public:
    PlayerSystem(SceneManager* sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
};

