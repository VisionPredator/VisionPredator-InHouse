#include "pch.h"
#include "SoundEngine.h"
#include <ranges>
#include <iostream>

void SoundEngine::Initialize()
{
	// 사운드 파일 경로 설정
	//m_SoundPath = std::filesystem::current_path().parent_path().string();
	//m_SoundPath += "/Data/Sound/";

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
	///폴더가 없다면 만들기

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
		// BGM 탐색
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

	// EFFECT 2D 탐색

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
	// EFFECT 3D탐색
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
	/// TODO : 이것도 모드 바꿔서 들고올수 있는데 고민하기
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
	// 사운드 맵에서 사운드를 찾습니다.
	auto soundIter = m_SoundMap.find(key);
	if (soundIter == m_SoundMap.end())
		return;

	FMOD::Sound* sound = soundIter->second;

	// 해당 엔티티의 채널이 이미 존재하는지 확인합니다.
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end())
	{
		FMOD::Channel* existingChannel = channelIter->second;
		existingChannel->stop();  // 기존 채널을 정지합니다.
	}

	// 새로운 채널을 생성하고 사운드를 재생합니다.
	FMOD::Channel* channel = nullptr;
	m_System->playSound(sound, nullptr, true, &channel);

	// 사운드의 반복 여부를 설정합니다.
	FMOD_MODE mode;
	if (sound->getMode(&mode) == FMOD_OK)
	{
		if ((mode & FMOD_LOOP_NORMAL) != 0) {
			channel->setMode(FMOD_LOOP_NORMAL); // 반복 모드 설정
		}
		else {
			channel->setMode(FMOD_LOOP_OFF); // 반복 없음
		}
	}

	// 볼륨 설정
	channel->setVolume(volume);

	// 3D 사운드일 경우 위치를 설정합니다.
	if (mode & FMOD_3D)
	{
		FMOD_VECTOR fmodPos = { pose.x, pose.y, pose.z };
		FMOD_VECTOR fmodVel = { 0.0f, 0.0f, 0.0f }; // 속도 벡터를 0으로 설정 (정지 상태 가정)

		channel->set3DAttributes(&fmodPos, &fmodVel);
	}

	// 새로운 채널을 엔티티에 할당합니다.
	m_EntityChannels[id] = channel;
	channel->setPaused(false); // 재생을 시작합니다.

}
void SoundEngine::Stop(const uint32_t& id, const std::string& soundKey)
{
	auto channelIter = m_EntityChannels.find(id);
	if (channelIter != m_EntityChannels.end()) {
		FMOD::Channel* channel = channelIter->second;

		if (channel != nullptr)
		{
			// 채널의 사운드가 해당 키와 일치하는지 확인
			FMOD::Sound* currentSound = nullptr;
			channel->getCurrentSound(&currentSound);

			if (currentSound != nullptr)
			{
				char currentKey[256];
				currentSound->getName(currentKey, sizeof(currentKey));

				// 사운드 키가 일치할 경우 채널 정지
				if (soundKey == std::string(currentKey))
				{
					channel->stop();
					m_EntityChannels.erase(channelIter); // 맵에서 채널 제거
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
