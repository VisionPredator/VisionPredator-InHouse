#pragma once
#include <System.h>
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable
{

public:
    PlayerSystem(SceneManager* sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;


    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;
};

