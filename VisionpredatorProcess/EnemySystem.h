#pragma once
#include "../VPEngine/System.h"
class EnemySystem :
    public System,public IFixedUpdatable
{
public:
    EnemySystem(std::shared_ptr<SceneManager> SceneMagener);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
};

