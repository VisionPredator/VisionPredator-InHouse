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
	FMOD::System_Create(&m_System);

	// FMOD �ý��� �ʱ�ȭ
	// 512 ä���� ����� �� ������, �⺻ �ʱ�ȭ �ɼ�(FMOD_INIT_NORMAL)�� ���
	// �ʱ�ȭ �÷��׳� ����̹� �����͸� ���� null�� ���
	m_System->init(512, FMOD_INIT_NORMAL, nullptr);

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
					Load2DSound(fileName, fileKey, SoundType::EFFECT);
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
				Load3DSound(fileName, fileKey, SoundType::EFFECT);
			}
		}
	}
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
	/// TODO : �̰͵� ��� �ٲ㼭 ���ü� �ִµ� ����ϱ�
	switch (type)
	{
	case SoundType::BGM:
		m_System->createStream(path.c_str(), FMOD_LOOP_NORMAL | FMOD_3D, nullptr, &sound);
		break;
	case SoundType::EFFECT:
		m_System->createSound(path.c_str(), FMOD_DEFAULT | FMOD_3D, nullptr, &sound);
		break;
	}

	m_SoundMap.insert(std::make_pair(key, sound));
}

void SoundEngine::Play(const uint32_t& id, const std::string& key, float volume, VPMath::Vector3 pose)
{
	// ���� �ʿ��� ���带 ã���ϴ�.
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end())
		return;

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

}
void SoundEngine::Stop(const uint32_t& id, const std::string& soundKey)
{
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) {
		FMOD::Channel* channel = channelIter->second;

		if (channel != nullptr)
		{
			// ä���� ���尡 �ش� Ű�� ��ġ�ϴ��� Ȯ��
			FMOD::Sound* currentSound = nullptr;
			channel->getCurrentSound(&currentSound);

			if (currentSound != nullptr)
			{
				char currentKey[256];
				currentSound->getName(currentKey, sizeof(currentKey));

				// ���� Ű�� ��ġ�� ��� ä�� ����
				if (soundKey == std::string(currentKey))
				{
					channel->stop();
					m_EntityChannels.erase(channelIter); // �ʿ��� ä�� ����
				}
			}
		}
	}
}

void SoundEngine::SetListenerPosition(VPMath::Vector3 pos, VPMath::Vector3 Up, VPMath::Vector3 Forward)
{
	m_ListenerPos = { pos.x,pos.y,pos.z };
	m_ListenerForward = { Forward.x,Forward.y,Forward.z };
	m_ListenerUp = { Up.x,Up.y,Up.z };
	m_System->set3DListenerAttributes(0, &m_ListenerPos, &m_ListenerVel, &m_ListenerForward, &m_ListenerUp);

}

void SoundEngine::CleanChannel()
{
}
