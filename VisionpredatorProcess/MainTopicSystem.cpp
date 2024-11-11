#include "pch.h"
#include "MainTopicSystem.h"
#include "EventManager.h"
#include "VisPredComponents.h"
MainTopicSystem::MainTopicSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnChangeTopic", CreateSubscriber(&MainTopicSystem::OnChangeTopic));
}

#include <array>
#include <string>

enum class TopicType
{
    FINDBELL,
    KILLALL,
    NONE,
    // 다른 TopicType 추가
    END // END는 마지막으로 사용하여 배열의 크기를 결정합니다.
};


void MainTopicSystem::OnChangeTopic(std::any mode)
{
	VisPred::Game::TopicType Mode = std::any_cast<VisPred::Game::TopicType>(mode);
	if (!m_MainTopicEntity.lock())
		return;
	auto topic = m_MainTopicEntity.lock()->GetComponent<MainTopicComponent>();
	auto image = m_MainTopicEntity.lock()->GetComponent<ImageComponent>();
	auto text = m_MainTopicEntity.lock()->GetComponent<TextComponent>();
	if (topic->Mode == Mode)
		return;

	if (topic->HasComponent<TextBounceComponent>())
	{
		topic->GetComponent<TextBounceComponent>()->AddedBounce = true;
	}
	if (topic->HasComponent<ImageBounceComponent>())
	{
		topic->GetComponent<ImageBounceComponent>()->AddedBounce = true;
	}

	topic->Mode = Mode;
	switch (Mode)
	{
	case VisPred::Game::TopicType::FINDBELL:
	{
		text->Text = topic->Ment[(int)VisPred::Game::TopicType::FINDBELL];
		text->Color = topic->MentColor;
		image->Color = topic->BackGroundColor;
		topic->IsReset = true;
	}
	break;

	case VisPred::Game::TopicType::KILLALL:
	{
		text->Text = topic->Ment[(int)VisPred::Game::TopicType::KILLALL];
		text->Color = topic->MentColor;
		image->Color = topic->BackGroundColor;
		topic->IsReset = true;

	}
	break;

	case VisPred::Game::TopicType::NONE:
	{
		text->Text = topic->Ment[(int)VisPred::Game::TopicType::NONE];
		text->Color.w = 0;
		image->Color.w = 0;
	
	}
	break;

	case VisPred::Game::TopicType::END:
		break;

	default:
		break;
    }
}


void MainTopicSystem::Initialize()
{
	COMPLOOP(MainTopicComponent, comp)
	{

		Start(comp.GetEntityID());
		if (m_MainTopicEntity.lock())
		{
			break;
		}
	}
}

void MainTopicSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity || !entity->HasComponent<MainTopicComponent>() || !entity->HasComponent<ImageComponent>()|| !entity->HasComponent<TextComponent>())
		return;												   
	if (m_MainTopicEntity.lock())
		return;

	m_MainTopicEntity = entity;

	auto topiccomp = m_MainTopicEntity.lock()->GetComponent<MainTopicComponent>();
	OnChangeTopic(VisPred::Game::TopicType::NONE);
}
void MainTopicSystem::Finish(uint32_t gameObjectId)
{
}

void MainTopicSystem::Finalize()
{
}

void MainTopicSystem::FixedUpdate(float deltaTime)
{
	if (!m_MainTopicEntity.lock())
		return;
	auto comp = m_MainTopicEntity.lock()->GetComponent<MainTopicComponent>();
	if (comp->IsReset)
	{
		comp->IsReset = false;
		comp->IsShowing = true;
	}
	if (comp->IsShowing)
	{
		comp->Progress += deltaTime;

		if (comp->Progress > comp->Duration)
		{
			comp->Progress = 0;
			comp->IsShowing = false;
			OnChangeTopic(VisPred::Game::TopicType::NONE);
		}
	}
}
