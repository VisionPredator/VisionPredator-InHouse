#include "pch.h"
#include "QuestSystem.h"
#include "QuestComponent.h"
#include "MainQuestComponent.h"
#include "PlayerComponent.h"
#include "EventManager.h"

QuestSystem::QuestSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&QuestSystem::OnInterected));
	EventManager::GetInstance().Subscribe("OnTutorialClear", CreateSubscriber(&QuestSystem::OnTutorialClear));
}

void QuestSystem::FixedUpdate(float deltaTime)
{

	COMPLOOP(QuestComponent, quest)
	{
		quest.GetComponent<TextComponent>()->Color = { .4f,.4f,.4f };
		
		if (!quest.QuestPlayer ||!quest.IsStarted || quest.IsCleared)
			continue;
		switch (quest.QuestType)
		{
		case VisPred::Game::QuestType::VPMOVE:			QuestCheck_VPMOVE(quest); break;
		case VisPred::Game::QuestType::VPJUMP:			QuestCheck_VPJUMP(quest); break;
		case VisPred::Game::QuestType::VPDASH:			QuestCheck_VPDASH(quest); break;
		case VisPred::Game::QuestType::VPCHANGE:		QuestCheck_VPCHANGE(quest); break;
		case VisPred::Game::QuestType::PLAYERRUN:		QuestCheck_PLAYERRUN(quest); break;
		case VisPred::Game::QuestType::PLAYERJUMP:		QuestCheck_PLAYERJUMP(quest); break;
		case VisPred::Game::QuestType::PLAYERCROUCH:	QuestCheck_PLAYERCROUCH(quest); break;
		case VisPred::Game::QuestType::PLAYERSLIDE:		QuestCheck_PLAYERSLIDE(quest); break;
		case VisPred::Game::QuestType::PLAYERATTACK:	QuestCheck_PLAYERATTACK(quest); break;
		case VisPred::Game::QuestType::PLAYERINTERECT:	QuestCheck_PLAYERINTERECT(quest); break;
		case VisPred::Game::QuestType::PLAYERPICKUP:	QuestCheck_PLAYERPICKUP(quest); break;
		case VisPred::Game::QuestType::PLAYERSHOOT:		QuestCheck_PLAYERSHOOT(quest); break;
		case VisPred::Game::QuestType::PLAYERTHROW:		QuestCheck_PLAYERTHROW(quest); break;
		default:
			break;
		}

	}
	if (!m_MainQuestEntity.lock())
		return;
	auto mainquestcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
	// Questptrs가 비어있는지 확인하여 예외 처리
	if (!mainquestcomp->Questptrs.empty())
	{
		// 첫 번째 퀘스트 시작 설정

		mainquestcomp->Questptrs.front()->IsStarted = true;

		// 첫 번째 퀘스트가 클리어된 경우, 리스트에서 제거
		if (mainquestcomp->Questptrs.front()->IsCleared)
		{
			mainquestcomp->Questptrs.pop_front();
		}
	}
	MainquestSetting();

	// Questptrs가 비어 있는지 확인 후 추가 작업
	if (mainquestcomp->Questptrs.empty())
	{
		EventManager::GetInstance().ImmediateEvent("OnTutorialClear");
	}


}

void QuestSystem::Initialize()
{
	COMPLOOP(MainQuestComponent, queset)
	{
		Start(queset.GetEntityID());

	}
	COMPLOOP(QuestComponent, queset)
	{

		Start(queset.GetEntityID());
	}
}

void QuestSystem::Start(uint32_t gameObjectId)
{

	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (entity->HasComponent<MainQuestComponent>())
	{
		if (!m_MainQuestEntity.lock())
		{
			m_MainQuestEntity = entity;
			auto questcomp = entity->GetComponent<MainQuestComponent>();
			for (auto& identity : questcomp->QuestSequence)
			{
				auto quest = GetSceneManager()->GetEntityByIdentityName(identity);
				if (quest && quest->HasComponent<QuestComponent>())
				{
					questcomp->Questptrs.push_back(quest->GetComponent<QuestComponent>());
				}
			}
		}


	}
	if (entity->HasComponent<QuestComponent>())
	{

		auto questcomp = entity->GetComponent<QuestComponent>();
		auto playerentity = GetSceneManager()->GetEntityByIdentityName(questcomp->PlayerIdentity);
		if (playerentity && playerentity->HasComponent<PlayerComponent>())
			questcomp->QuestPlayer = playerentity->GetComponent<PlayerComponent>();
	}
}

void QuestSystem::Finish(uint32_t gameObjectId)
{
}

void QuestSystem::QuestCheck_VPMOVE(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	questcomp.QuestPlayer->IsSearchable = false;

	if (questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::RUN)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});

	}

}

void QuestSystem::QuestCheck_VPJUMP(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});

	}
}

void QuestSystem::QuestCheck_VPDASH(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Dash_Slide)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_VPCHANGE(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Transformation)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERSHOOT(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->HasGun && questcomp.QuestPlayer->IsAttacking)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERRUN(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::RUN)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERJUMP(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERCROUCH(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::CROUCH)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERSLIDE(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Dash_Slide)
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERATTACK(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->IsAttacking && !questcomp.QuestPlayer->HasGun)
	{
		questcomp.IsCleared = true;
		questcomp.QuestPlayer->IsSearchable = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERPICKUP(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	auto curani = questcomp.QuestPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni;
	if (curani == static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_Pistol)
		|| curani == static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_Rifle) || curani == static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_ShotGun))
	{
		questcomp.IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::QuestCheck_PLAYERTHROW(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	auto curani = questcomp.QuestPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni;
	if (curani == static_cast<int>(VisPred::Game::PlayerAni::ToThrow_Rifle)
		|| curani == static_cast<int>(VisPred::Game::PlayerAni::ToThrow_Pistol) || curani == static_cast<int>(VisPred::Game::PlayerAni::ToThrow_ShotGun))
	{
		questcomp.IsCleared = true;
	}
}

void QuestSystem::QuestCheck_PLAYERINTERECT(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
}

void QuestSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);
	if (!m_MainQuestEntity.lock())
		return;
	auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();

	if (questcomp->Questptrs.empty())
		return;
	if (questcomp->Questptrs.front()->QuestType == VisPred::Game::QuestType::PLAYERINTERECT)
	{
		questcomp->Questptrs.front()->IsCleared = true;
		auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
		GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Subquest, questcomp->Volume_Subquest, true, false, {});
	}
}

void QuestSystem::OnTutorialClear(std::any none)
{
	EventManager::GetInstance().ImmediateEvent("OnInterected",std::make_pair(m_MainQuestEntity.lock(), m_MainQuestEntity.lock()));
	auto questcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();
	GetSceneManager()->SpawnSoundEntity(questcomp->SounKey_Mainquest, questcomp->Volume_Mainquest, true, false, {});
	EventManager::GetInstance().ImmediateEvent("OnChangeTopic",VisPred::Game::TopicType::FINDBELL);
	m_MainQuestEntity.lock()->DestorySelf();
}

void QuestSystem::MainquestSetting()
{
	if (INPUTKEY(KEYBOARDKEY::O))
	{
		if (!m_MainQuestEntity.lock())
			return;
		auto mainquestcomp = m_MainQuestEntity.lock()->GetComponent<MainQuestComponent>();

		mainquestcomp->IsSkipMode = true;

		// Queue-like behavior: 첫 번째 요소 활성화 후 pop
		while (!mainquestcomp->Questptrs.empty())
		{
			auto& frontQuest = mainquestcomp->Questptrs.front();
			frontQuest->IsStarted = true;
			frontQuest->IsCleared = true;
			if (frontQuest->QuestType == VisPred::Game::QuestType::PLAYERINTERECT)
				frontQuest->QuestPlayer->IsSearchable = true;
			// 첫 번째 요소 제거
			mainquestcomp->Questptrs.pop_front();
		}
	}
}

void QuestSystem::Finalize()
{
	//m_MainQuest = nullptr;
}

void QuestSystem::Update(float deltaTime)
{
}
