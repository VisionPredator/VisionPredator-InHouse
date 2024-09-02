#pragma once
#include "ISound.h"
#include "../include/fmod/fmod.hpp"
class SoundEngine :
    public Sound::ISound
{
public: 
    // ISound을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void All3DSound();
    void All2DSound();
    void Load2DSound(const std::string& path, const std::string& key, SoundType type) override;
    void Load3DSound(const std::string& path, const std::string& key, SoundType type) override;
    void Play(const uint32_t& id, const std::string& key, float volume) override;
    void Stop(const uint32_t& id, const std::string& soundKey) override;
    void SetListenerPosition(float* pos, float* vel) override;
    void CleanChannel() override;
private:
    std::string m_SoundPath;

    FMOD::System* m_System;
    std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;
    std::unordered_map<uint32_t, FMOD::Channel*> m_ChannelMap;

    float m_MasterVolume;

    FMOD_VECTOR m_ListenerPos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR m_ListenerVel = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR m_ListenerForward = { 0.0f, 0.0f, 1.0f };
    FMOD_VECTOR m_ListenerUp = { 0.0f, 1.0f, 0.0f };
    bool IsMusicLoaded = false;

    // ISound을(를) 통해 상속됨
    void LoadAllSound() override;
};

