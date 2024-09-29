#pragma once
#include "System.h"


class SoundSystem :
    public System,public ISoundable, public IUpdatable,public IStartable
{
public:
    SoundSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SoundSystem() = default;

    // ISoundable을(를) 통해 상속됨
    void SoundUpdate(float deltaTime) override;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
};

