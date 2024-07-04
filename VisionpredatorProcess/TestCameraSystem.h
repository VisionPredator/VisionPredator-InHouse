#pragma once
#include <System.h>
class TestCameraSystem :
    public System ,public IUpdatable,public IFixedUpdatable
{
public:
    TestCameraSystem(SceneManager* sceneManager);
    ~TestCameraSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    
    float moveSpeed = 10;
};

