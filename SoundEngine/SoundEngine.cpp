#include "pch.h"
#include "SoundEngine.h"
#include <ranges>
#include <iostream>

void SoundEngine::Initialize()
{
	// ���� ���� ��� ����
	//m_SoundPath = std::filesystem::current_path().parent_path().string();
	//m_SoundPath += "/Data/Sound/";

	// FMOD �ý��� ����
	FMOD_RESULT result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		std::cerr << "FMOD::System_Create failed: " << result << std::endl;
		return; // If the system fails to create, return immediately
	}
	else
	{
		std::cerr << "FMOD::System_Create Succed: " << result << std::endl;
	}

	// FMOD �ý��� �ʱ�ȭ
	// 512 ä���� ����� �� ������, �⺻ �ʱ�ȭ �ɼ�(FMOD_INIT_NORMAL)�� ���
	// �ʱ�ȭ �÷��׳� ����̹� �����͸� ���� null�� ���
	result = m_System->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		std::cerr << "FMOD system init failed: " << result << std::endl;
		return;
	}
	else
	{
		std::cerr << "FMOD system init Succed: " << result << std::endl;
	}

	// 3D ���� ����
	// Doppler ������: 1.0 (�Ϲ����� ���÷� ȿ��)
	// �Ÿ� ����: 1.0 (������ ���͸� ���)
	// �ѿ��� ������: 1.0 (�⺻ �ѿ���)
	float dopplerScale = 1.0f;          // Doppler effect scale
	float distanceFactor = 1.0f;        // Distance factor (1.0 = meters)
	float rolloffScale = 1.0f;          // Rolloff scale (1.0 = default rolloff)

	m_System->set3DSettings(dopplerScale, distanceFactor, rolloffScale);

	// �ٸ� �⺻ ������ (�ɼ�)
	// ���� ��� ���� �ε峪 �޸� ������ ���� ���� �߰� ����
	m_BGMPath =  "../Data/Sound/BGM/" ;
	m_2DEffectPath= "../Data/Sound/Effect_2D/";
	m_3DEffectPath = "../Data/Sound/Effect_3D/";

}
void SoundEngine::Update()
{
	m_System->update();

	for (auto it = m_EntityChannels.begin(); it != m_EntityChannels.end(); )
	{
		bool isPlaying = false;
		// Check if the channel is still playing
		if (it->second->isPlaying(&isPlaying) == FMOD_OK && !isPlaying)
			// If the channel has stopped, erase it from the map
			it = m_EntityChannels.erase(it);
		else
			// Move to the next channel if it is still playing
			++it;
	}

}


void SoundEngine::LoadAllSound()
{
	if (!IsMusicLoaded)
	{
		CreatFolder();
		All2DSound();
		All3DSound();
		IsMusicLoaded = true;
	}
}

void SoundEngine::CreatFolder()
{
	///������ ���ٸ� �����

	if (!std::filesystem::exists(m_2DEffectPath))
		std::filesystem::create_directories(m_2DEffectPath);

	if (!std::filesystem::exists(m_3DEffectPath)) 
		std::filesystem::create_directories(m_3DEffectPath);

	if (!std::filesystem::exists(m_BGMPath))
		std::filesystem::create_directories(m_BGMPath);
}
void SoundEngine::All2DSound()
{

	if (std::filesystem::is_empty(m_BGMPath))
		std::cout << "The BGM sound directory is empty: " << m_BGMPath << std::endl;
	else
	{
		// BGM Ž��
		for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ m_BGMPath }))
		{
			if (entry.is_regular_file())
			{
				auto extension = entry.path().extension();
				if (extension == ".wav" || extension == ".mp3")
				{
					auto fileName = entry.path().string();
					auto fileKey = entry.path().filename().replace_extension().string();
					Load2DSound(fileName, fileKey, SoundType::BGM);
				}
			}
		}
	}

	// EFFECT 2D Ž��

	if (std::filesystem::is_empty(m_2DEffectPath))
		std::cout << "The 2D Effect sound directory is empty: " << m_2DEffectPath << std::endl;
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ m_2DEffectPath }))
		{
			if (entry.is_regular_file())
			{
				auto extension = entry.path().extension();
				if (extension == ".wav" || extension == ".mp3")
				{
					auto fileName = entry.path().string();
					auto fileKey = entry.path().filename().replace_extension().string();
					LoadSound(fileName, fileKey);
				}
			}
		}
	}

}

void SoundEngine::All3DSound()
{

	if (std::filesystem::is_empty(m_3DEffectPath))
	{
		std::cout << "The 3D Effect sound directory is empty: " << m_3DEffectPath << std::endl;
		return;
	}
	// EFFECT 3DŽ��
	for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ m_3DEffectPath }))
	{
		if (entry.is_regular_file()) 
		{
			auto extension = entry.path().extension();
			if (extension == ".wav" || extension == ".mp3") 
			{
				auto fileName = entry.path().string();
				auto fileKey = entry.path().filename().replace_extension().string();
				LoadSound(fileName, fileKey);
			}
		}
	}
}

void SoundEngine::SetSystemVolume(int master, int bgm, int effect) 
{
	float masterpercent = static_cast<float> (master / 100.f);
	float bgmrpercent = static_cast<float> (bgm / 100.f);
	float effectpercent = static_cast<float> (effect / 100.f);


	for (auto& [id, channel] : m_EntityChannels) 
	{
		float currentVolume = 0.0f;
		channel->getVolume(&currentVolume);

		FMOD_MODE mode;
		channel->getMode(&mode);

		// Determine which percent to use based on loop mode
		float previousVolumePercent = (mode & FMOD_LOOP_NORMAL) ? m_BgmPercent : m_EffectPercent;

		// Reverse-calculate original volume
		float originalVolume = currentVolume / (m_MasterPercent * previousVolumePercent);

		// Calculate new volume using updated master, bgm, and effect percentages
		float newVolumePercent = (mode & FMOD_LOOP_NORMAL) ? bgmrpercent : effectpercent;
		float newVolume = originalVolume * masterpercent * newVolumePercent;

		// Apply new volume to the channel
		channel->setVolume(newVolume);
	}

	// Update stored volume percentages
	m_MasterPercent = masterpercent;
	m_BgmPercent = bgmrpercent;
	m_EffectPercent = effectpercent;
}

int SoundEngine::GetMasterVolume()
{
	return static_cast<int> (m_MasterPercent * 100);
}

int SoundEngine::GetBGMVolume()
{
	return static_cast<int> (m_BgmPercent * 100);
}

int SoundEngine::GetEffectVolume()
{
	return static_cast<int> (m_EffectPercent * 100);
}


void SoundEngine::LoadSound(const std::string& path, const std::string& key)
{
	if (m_SoundMap.find(key) != m_SoundMap.end())
		return;

	FMOD::Sound* sound = nullptr;
	m_System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound);
	m_SoundMap.insert({ key, sound });
}

void SoundEngine::Load2DSound(const std::string& path, const std::string& key, SoundType type)
{
	if (m_SoundMap.find(key) != m_SoundMap.end())
		return;
	FMOD::Sound* sound = nullptr;

	switch (type)
	{
	case SoundType::BGM:
		m_System->createStream(path.c_str(), FMOD_LOOP_NORMAL | FMOD_3D, nullptr, &sound);
		break;
	case SoundType::EFFECT:
		m_System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound);
		break;
	}
	m_SoundMap.insert(std::make_pair(key, sound));

	return;
}

void SoundEngine::Load3DSound(const std::string& path, const std::string& key, SoundType type)
{
	if (m_SoundMap.find(key) != m_SoundMap.end())
		return;
	FMOD::Sound* sound = nullptr;
	FMOD_MODE mode = FMOD_DEFAULT;

	switch (type) {
	case SoundType::BGM:
		mode = FMOD_LOOP_NORMAL | FMOD_3D;
		m_System->createStream(path.c_str(), mode, nullptr, &sound);
		break;
	case SoundType::EFFECT:
		mode = FMOD_DEFAULT | FMOD_3D;
		m_System->createSound(path.c_str(), mode, nullptr, &sound);
		break;
	}
	m_SoundMap.insert(std::make_pair(key, sound));
}

bool SoundEngine::Play(const uint32_t& id, const std::string& key, int volume, VPMath::Vector3 pose)
{
	// ���� �ʿ��� ���带 ã���ϴ�.
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end())
		return false;

	FMOD::Sound* sound = soundIter->second;
	// �ش� ��ƼƼ�� ä���� �̹� �����ϴ��� Ȯ���մϴ�.
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end())
	{
		FMOD::Channel* existingChannel = channelIter->second;
		existingChannel->stop();  // ���� ä���� �����մϴ�.
	}
	// ���ο� ä���� �����ϰ� ���带 ����մϴ�.
	FMOD::Channel* channel = nullptr;

	m_System->playSound(sound, nullptr, true, &channel);

	// ������ �ݺ� ���θ� �����մϴ�.
	FMOD_MODE mode;
	if (sound->getMode(&mode) == FMOD_OK)
	{
		if ((mode & FMOD_LOOP_NORMAL) != 0) {
			channel->setMode(FMOD_LOOP_NORMAL); // �ݺ� ��� ����
		}
		else {
			channel->setMode(FMOD_LOOP_OFF); // �ݺ� ����
		}
	}

	// ���� ����
	channel->setVolume(volume);

	// 3D ������ ��� ��ġ�� �����մϴ�.
	if (mode & FMOD_3D)
	{
		FMOD_VECTOR fmodPos = { pose.x, pose.y, pose.z };
		FMOD_VECTOR fmodVel = { 0.0f, 0.0f, 0.0f }; // �ӵ� ���͸� 0���� ���� (���� ���� ����)

		channel->set3DAttributes(&fmodPos, &fmodVel);
	}

	// ���ο� ä���� ��ƼƼ�� �Ҵ��մϴ�.
	m_EntityChannels[id] = channel;
	channel->setPaused(false); // ����� �����մϴ�.
	return true;
}

bool SoundEngine::Play(const uint32_t& id, const std::string& key, int baseVolume, bool Is2D, bool IsLoop, VPMath::Vector3 pose)
{

	float basevolumPercent = baseVolume / 100.f;
	// Find the sound in the sound map
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end())
		return false;

	FMOD::Sound* sound = soundIter->second;

	// Adjust mode based on Is2D and IsLoop parameters
	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= (Is2D ? FMOD_2D : FMOD_3D);
	mode |= (IsLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	sound->setMode(mode);

	// Stop existing channel for this entity if it exists
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) {
		FMOD::Channel* existingChannel = channelIter->second;
		existingChannel->stop();
	}

	// Play sound on a new channel
	FMOD::Channel* channel = nullptr;
	m_System->playSound(sound, nullptr, true, &channel);

	// Calculate volume based on master, bgm, and effect percentages
	float volumeMultiplier = m_MasterPercent * (IsLoop ? m_BgmPercent : m_EffectPercent);
	float finalVolume = basevolumPercent * volumeMultiplier;
	channel->setVolume(finalVolume);

	// Set 3D position if in 3D mode
	if (!Is2D) {
		FMOD_VECTOR fmodPos = { pose.x, pose.y, pose.z };
		FMOD_VECTOR fmodVel = { 0.0f, 0.0f, 0.0f }; // Assuming stationary object
		channel->set3DAttributes(&fmodPos, &fmodVel);
	}

	// Assign new channel to entity and start playback
	m_EntityChannels[id] = channel;
	channel->setPaused(false); // Start playback

	return true;
}

void SoundEngine::Stop(const uint32_t& id, const std::string& soundKey) 
{
	// Check if the sound exists in the map
	auto soundIter = m_SoundMap.find(soundKey);
	if (soundIter == m_SoundMap.end())
		return;  // Sound not found

	// Check if there's a channel associated with the given id
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) {
		FMOD::Channel* channel = channelIter->second;

		// Stop the channel
		channel->stop();

		// Remove the channel from the entity channels map
		m_EntityChannels.erase(channelIter);
	}
}


void SoundEngine::Stop(const uint32_t& id) 
{
	// Find the channel for the given ID
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) {
		FMOD::Channel* channel = channelIter->second;

		if (channel != nullptr) {
			// Stop the channel
			channel->stop();
		}

		// Remove the channel from the map
		m_EntityChannels.erase(channelIter);
	}
}


void SoundEngine::SetListenerPosition(VPMath::Vector3 pos, VPMath::Vector3 Up, VPMath::Vector3 Forward)
{
	m_ListenerPos = { pos.x,pos.y,pos.z };
	m_ListenerForward = { Forward.x,Forward.y,Forward.z };
	m_ListenerUp = { Up.x,Up.y,Up.z };
	m_System->set3DListenerAttributes(0, &m_ListenerPos, &m_ListenerVel, &m_ListenerForward, &m_ListenerUp);

}

void SoundEngine::SetChannelPosition(const uint32_t& id, VPMath::Vector3 pos)
{
	// Check if the channel exists in m_EntityChannels
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end())
	{
		FMOD::Channel* channel = channelIter->second;
		if (channel != nullptr)
		{
			// Convert VPMath::Vector3 to FMOD_VECTOR
			FMOD_VECTOR fmodPos = { pos.x, pos.y, pos.z };
			FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f }; // Assume zero velocity for now

			// Set the 3D position of the channel
			channel->set3DAttributes(&fmodPos, &velocity);
		}
	}
}


bool SoundEngine::ChannelMusicFinished(const uint32_t& id)
{
	// Check if m_EntityChannels is empty
	if (m_EntityChannels.empty())
	{
		return true; // If no channels exist, assume no music is playing 
	}

	// Find the channel for the given ID
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end())
	{
		FMOD::Channel* channel = channelIter->second;
		if (channel != nullptr)
		{
			bool isPlaying = false;
			channel->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				// The channel is no longer playing, meaning the music has finished
				m_EntityChannels.erase(channelIter); // Remove the channel from the map
				return true;
			}
		}
	}
	else
	{
		// Return true if the channel ID is not found
		return true;
	}

	// If the channel was found and is still playing
	return false;
}


void SoundEngine::CleanAllChannel()
{
	// Check if m_EntityChannels is empty
	if (m_EntityChannels.empty()) 
		return; // Nothing to clean if the map is empty

	// Iterate over all channels in the map
	for (auto& pair : m_EntityChannels) 
	{
		FMOD::Channel* channel = pair.second;

		if (channel != nullptr)
			// Stop the channel
			channel->stop();
	}

	// Clear the entire map to remove all entries
	m_EntityChannels.clear();
}

void SoundEngine::CleanChannel(const uint32_t& id) 
{
	// Check if m_EntityChannels is empty
	if (m_EntityChannels.empty()) 
		return; // Nothing to clean if the map is empty

	// Find the channel for the given ID
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) 
	{
		FMOD::Channel* channel = channelIter->second;

		if (channel != nullptr)
			// Stop the channel
			channel->stop();

		// Remove the channel from the map
		m_EntityChannels.erase(channelIter);
	}
}


float SoundEngine::GetLength(const std::string& key)
{
	// ���� �ʿ��� ���带 ã���ϴ�.
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end()) {
		return 0.0f;
	}

	FMOD::Sound* sound = soundIter->second;

	// ������ ���̸� �����ɴϴ� (�и��� ����)
	unsigned int length = 0;
	FMOD_RESULT result = sound->getLength(&length, FMOD_TIMEUNIT_MS);

	if (result != FMOD_OK) 
	{
		return 0.0f; // ���� �� 0.0�� ��ȯ
	}

	// �� ������ ��ȯ�Ͽ� ��ȯ
	return static_cast<float>(length) / 1000.0f;
}

bool SoundEngine::IsPlayingSound(uint32_t channelid, const std::string& soundKey)
{
	// �ش� ID�� ä���� ã���ϴ�.
	auto channelIter = m_EntityChannels.find(channelid);
	if (channelIter == m_EntityChannels.end())
	{
		return false; // ä���� ã�� �� ������ false ��ȯ
	}

	FMOD::Channel* channel = channelIter->second;
	if (channel != nullptr)
	{	
		// ä���� ��ȿ���� Ȯ��
		bool isPlaying = false;
		channel->isPlaying(&isPlaying);

		if (!isPlaying)
		{
			return false; // ä���� ��� ������ ������ false ��ȯ
		}

		// soundKey�� ��������� �ܼ��� ��� �������� ��ȯ
		if (soundKey.empty())
		{
			return true; // ä���� ��� ���̸� true ��ȯ
		}

		// soundKey�� �ִ� ��� ���� ��� ���� ������ Ű�� ��
		FMOD::Sound* currentSound = nullptr;
		if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound != nullptr)
		{
			char currentKey[256];
			currentSound->getName(currentKey, sizeof(currentKey));

			// ���� �̸����� Ȯ���ڸ� ����
			std::string currentKeyStr = std::filesystem::path(currentKey).stem().string();

			// ���� Ű�� ��ġ�ϴ��� Ȯ��
			return soundKey == currentKeyStr;
		}
	}

	return false; // ������ �������� ������ false ��ȯ
}
