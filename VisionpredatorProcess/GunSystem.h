#pragma once
#include "System.h"
class GunSystem :
    public System, public IUpdatable,public IContactable
{
public:
    GunSystem(std::shared_ptr<SceneManager> scenemanager);
    ~GunSystem() = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IContactable을(를) 통해 상속됨
    void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;
    void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override;
};

