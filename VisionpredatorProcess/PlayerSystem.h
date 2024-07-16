#pragma once
#include <System.h>
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,  public IFixedUpdatable
{

public:
    PlayerSystem(SceneManager* sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
};

