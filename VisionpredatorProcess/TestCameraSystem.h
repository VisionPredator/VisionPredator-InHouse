#pragma once
#include "../VPEngine/System.h"
class TestCameraSystem :
    public System ,public IUpdatable
{
public:
    TestCameraSystem(SceneManager* sceneManager);
    ~TestCameraSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;


    
    float moveSpeed = 100;
};

