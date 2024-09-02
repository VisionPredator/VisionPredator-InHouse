#pragma once
#include "System.h"
class SoundSystem :
    public System
{
public:
    SoundSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SoundSystem() = default;
};

