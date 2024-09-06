#pragma once
#include "../VPEngine/System.h"
class EnemySystem :
    public System,public IFixedUpdatable
{
public:
    EnemySystem(std::shared_ptr<SceneManager> SceneMagener);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
};

