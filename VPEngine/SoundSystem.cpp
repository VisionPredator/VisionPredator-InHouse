#include "pch.h"
#include "SoundSystem.h"

SoundSystem::SoundSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void SoundSystem::SoundUpdate(float deltaTime)
{
	COMPLOOP(SoundComponent, soundcomp)
	{
		if (m_SoundEngine->ChannelMusicFinished(soundcomp.GetEntityID()))
		{
			GetSceneManager()->DestroyEntity(soundcomp.GetEntityID());
		}
		else
		{
			auto trans = soundcomp.GetComponent<TransformComponent>();
			m_SoundEngine->SetChannelPosition(soundcomp.GetEntityID(), trans->World_Location);
		}

	}
	m_SoundEngine->Update();

}

void SoundSystem::Update(float deltaTime)
{
}

void SoundSystem::Initialize()
{
	COMPLOOP(SoundComponent, soundcomp)
	{
		Start(soundcomp.GetEntityID());
	}

}

void SoundSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity->HasComponent<SoundComponent>())
		return;
	auto soundcomp = entity->GetComponent<SoundComponent>();

	m_SoundEngine->Play(soundcomp->GetEntityID(), soundcomp->SoundPath, soundcomp->Volume, soundcomp->Is2D, soundcomp->Loop, soundcomp->GetComponent<TransformComponent>()->World_Location);
}

void SoundSystem::Finish(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity->HasComponent<SoundComponent>())
		return;
	m_SoundEngine->CleanChannel(gameObjectId);
}

void SoundSystem::Finalize()
{
	COMPLOOP(SoundComponent, soundcomp)
	{
		Finish(soundcomp.GetEntityID());
	}
}
