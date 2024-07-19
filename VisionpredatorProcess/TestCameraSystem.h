#pragma once
#include "../VPEngine/System.h"
class TestCameraSystem :
    public System ,public IUpdatable
{
public:
    TestCameraSystem(SceneManager* sceneManager);
    ~TestCameraSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;


    
    float moveSpeed = 100;
};

