#include "pch.h"
#include "SoundEngine.h"
#include <ranges>

void SoundEngine::Initialize()
{
	// ���� ���� ��� ����
	m_SoundPath = std::filesystem::current_path().parent_path().string();
	m_SoundPath += "/Data/Sound/";

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
}
void SoundEngine::Update()
{
}


void SoundEngine::LoadAllSound()
{
	if (!IsMusicLoaded)
	{
		All2DSound();
		All3DSound();
		IsMusicLoaded = true;
	}
}

void SoundEngine::All3DSound()
{
	// EFFECT 3DŽ��
	for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ "../Data/Sound/Effect_3D/" })) {
		if (entry.is_regular_file()) {
			auto extension = entry.path().extension();
			if (extension == ".wav" || extension == ".mp3") {
				auto fileName = entry.path().string();
				auto fileKey = entry.path().filename().replace_extension().string();
				Load3DSound(fileName, fileKey, SoundType::EFFECT);
			}
		}
	}
}

void SoundEngine::All2DSound()
{
	// BGM Ž��
	for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ "../Data/Sound/BGM/" }))
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

	// EFFECT 2D Ž��
	for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{ "../Data/Sound/Effect_2D/" }))
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


void SoundEngine::Load2DSound(const std::string& path, const std::string& key, SoundType type)
{
	if (m_SoundMap.find(key) != m_SoundMap.end())
		return;
	FMOD::Sound* sound = nullptr;

	switch (type)
	{
	case SoundType::BGM:
		m_System->createStream(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);
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

	std::string fullPath = m_SoundPath + path;

	/// TODO : �̰͵� ��� �ٲ㼭 ���ü� �ִµ� ����ϱ�
	switch (type)
	{
	case SoundType::BGM:
		m_System->createStream(fullPath.c_str(), FMOD_LOOP_NORMAL | FMOD_3D, nullptr, &sound);
		break;
	case SoundType::EFFECT:
		m_System->createSound(fullPath.c_str(), FMOD_DEFAULT | FMOD_3D, nullptr, &sound);
		break;
	}

	m_SoundMap.insert(std::make_pair(key, sound));
}

void SoundEngine::Play(const uint32_t& id, const std::string& key, float volume)
{
	// ���� �ʿ��� ���带 ã���ϴ�.
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end())
		return;
	FMOD::Sound* sound = soundIter->second;
}

void SoundEngine::Stop(const uint32_t& id, const std::string& soundKey)
{

}

void SoundEngine::SetListenerPosition(float* pos, float* vel)
{
}

void SoundEngine::CleanChannel()
{
}
