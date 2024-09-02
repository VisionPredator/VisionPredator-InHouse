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

	virtual void Load2DSound(const std::string& path, const std::string& key, SoundType type) = 0;

	virtual void Load3DSound(const std::string& path, const std::string& key, SoundType type) = 0;

	virtual void Play(const uint32_t& id, const std::string& key, float volume = 1.0f) = 0;

	virtual void Stop(const uint32_t& id, const std::string& soundKey) = 0;
	virtual void SetListenerPosition(float* pos, float* vel) = 0;
	virtual void CleanChannel() = 0;


};
}

