#pragma once
#include "System.h"
class GunSystem :
    public System, public IUpdatable
{
public:
    GunSystem(std::shared_ptr<SceneManager> scenemanager);
    ~GunSystem() = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
};

