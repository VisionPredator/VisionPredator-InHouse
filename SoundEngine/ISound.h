#pragma once
namespace Sound
{ 
class ISound
{
public:
	enum class SoundType
	{
		BGM, // 이건 루프
		EFFECT, // 이건 루프 아님
	};
	virtual ~ISound() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void LoadAllSound() = 0;

	//virtual void Load2DSound(const std::string& path, const std::string& key, SoundType type) = 0;

	//virtual void Load3DSound(const std::string& path, const std::string& key, SoundType type) = 0;

	virtual bool Play(const uint32_t& id, const std::string& key, float volume, VPMath::Vector3 pose = {}) = 0;
	virtual bool Play(const uint32_t& id, const std::string& key, float volume, bool Is2D, bool IsLoop, VPMath::Vector3 pose = {}) = 0;

	virtual float GetLength( const std::string& key) = 0;

	virtual void Stop(const uint32_t& id, const std::string& soundKey) = 0;
	virtual void Stop(const uint32_t& id) = 0;
	virtual bool ChannelMusicFinished(const uint32_t& entityID) = 0;
	virtual void SetListenerPosition(VPMath::Vector3 pos, VPMath::Vector3 Up, VPMath::Vector3 Forward) = 0;
	virtual void CleanChannel(const uint32_t& id) = 0;
	virtual void CleanAllChannel() = 0;
	virtual bool IsPlayingSound(uint32_t channelid, const std::string& soundKey = {}) = 0;


};
}

