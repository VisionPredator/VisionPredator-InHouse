#include "pch.h"
#include "SoundSystem.h"

SoundSystem::SoundSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void SoundSystem::SoundUpdate(float deltaTime)
{
	m_SoundEngine->Update();
}

void SoundSystem::Update(float deltaTime)
{
}
