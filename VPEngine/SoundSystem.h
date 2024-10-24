#pragma once
#include "System.h"


class SoundSystem :
    public System,public ISoundable, public IUpdatable,public IStartable
{
public:
    SoundSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SoundSystem() = default;

    // ISoundable��(��) ���� ��ӵ�
    void SoundUpdate(float deltaTime) override;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
};

