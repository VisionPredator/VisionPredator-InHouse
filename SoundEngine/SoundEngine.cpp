#include "pch.h"
#include "SoundEngine.h"
#include <ranges>

void SoundEngine::Initialize()
{
	// 사운드 파일 경로 설정
	m_SoundPath = std::filesystem::current_path().parent_path().string();
	m_SoundPath += "/Data/Sound/";

	// FMOD 시스템 생성
	FMOD::System_Create(&m_System);

	// FMOD 시스템 초기화
	// 512 채널을 사용할 수 있으며, 기본 초기화 옵션(FMOD_INIT_NORMAL)을 사용
	// 초기화 플래그나 드라이버 데이터를 위해 null을 사용
	m_System->init(512, FMOD_INIT_NORMAL, nullptr);

	// 3D 사운드 설정
	// Doppler 스케일: 1.0 (일반적인 도플러 효과)
	// 거리 팩터: 1.0 (단위로 미터를 사용)
	// 롤오프 스케일: 1.0 (기본 롤오프)
	float dopplerScale = 1.0f;          // Doppler effect scale
	float distanceFactor = 1.0f;        // Distance factor (1.0 = meters)
	float rolloffScale = 1.0f;          // Rolloff scale (1.0 = default rolloff)

	m_System->set3DSettings(dopplerScale, distanceFactor, rolloffScale);

	// 다른 기본 설정들 (옵션)
	// 예를 들어 사운드 로드나 메모리 관리를 위한 설정 추가 가능
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
	// EFFECT 3D탐색
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
	// BGM 탐색
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

	// EFFECT 2D 탐색
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

	/// TODO : 이것도 모드 바꿔서 들고올수 있는데 고민하기
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
	// 사운드 맵에서 사운드를 찾습니다.
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
