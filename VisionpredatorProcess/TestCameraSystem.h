#pragma once
#include <System.h>
class TestCameraSystem :
    public System ,public IUpdatable,public IFixedUpdatable
{
public:
    TestCameraSystem(SceneManager* sceneManager);
    ~TestCameraSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
    
    float moveSpeed = 10;
};

