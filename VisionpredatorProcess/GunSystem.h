#pragma once
#include "System.h"
class GunSystem :
    public System, public IUpdatable
{
public:
    GunSystem(std::shared_ptr<SceneManager> scenemanager);
    ~GunSystem() = default;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
};

