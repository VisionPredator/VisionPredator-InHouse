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
    void LoadAllSound() override;
    void CreatFolder();
    void All2DSound();
    void All3DSound();
    void SetSystemVolume(int master, int bgm, int effect) override; 
    int GetMasterVolume() override;
    int GetBGMVolume() override;
    int GetEffectVolume() override;

    void LoadSound(const std::string& path, const std::string& key);
    void Load2DSound(const std::string& path, const std::string& key, SoundType type);
    void Load3DSound(const std::string& path, const std::string& key, SoundType type);
    bool Play(const uint32_t& id, const std::string& key, int volume, VPMath::Vector3 pose) override;
    bool Play(const uint32_t& id, const std::string& key, int basevolume , bool Is2D, bool IsLoop, VPMath::Vector3 pose) override;
    void Stop(const uint32_t& id, const std::string& soundKey) override;
    void Stop(const uint32_t& id) override;
    void SetListenerPosition(VPMath::Vector3 pos , VPMath::Vector3 Up, VPMath::Vector3 Forward) override;
    void SetChannelPosition(const uint32_t& id, VPMath::Vector3 pos ) override;
    bool ChannelMusicFinished(const uint32_t& id) override;
    void CleanAllChannel() override;
    void CleanChannel(const uint32_t& id) override;
private:
    std::string m_SoundPath;

    FMOD::System* m_System;
    std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;
    std::unordered_map<uint32_t, FMOD::Channel*> m_EntityChannels;

    float m_MasterVolume=100;
    float m_BgmVolume=100;
    float m_EffectVolume =100;
    float m_MasterPercent =1;
    float m_BgmPercent =1;
    float m_EffectPercent =1;

    FMOD_VECTOR m_ListenerPos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR m_ListenerVel = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR m_ListenerForward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR m_ListenerUp = { 0.0f, 1.0f, 0.0f };
	bool IsMusicLoaded = false;

	std::filesystem::path m_BGMPath{};
	std::filesystem::path m_2DEffectPath{};
	std::filesystem::path m_3DEffectPath{};


    // ISound을(를) 통해 상속됨
    float GetLength(const std::string& key) override;
    bool IsPlayingSound(uint32_t channelid, const std::string& soundKey = {}) override;
};

