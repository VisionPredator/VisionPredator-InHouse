#include "pch.h"
#include "QuestSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"

QuestSystem::QuestSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&QuestSystem::OnInterected));
}

void QuestSystem::FixedUpdate(float deltaTime)
{

	COMPLOOP(QuestComponent, quest)
	{
		quest.GetComponent<TextComponent>()->Color = { .4f,.4f,.4f };

		if (!quest.IsStarted || quest.IsCleared)
			continue;
		switch (quest.QuestType)
		{
		case VisPred::Game::QuestType::VPMOVE:			QuestCheck_VPMOVE(quest); break;
		case VisPred::Game::QuestType::VPJUMP:			QuestCheck_VPJUMP(quest); break;
		case VisPred::Game::QuestType::VPDASH:			QuestCheck_VPDASH(quest); break;
		case VisPred::Game::QuestType::VPCHANGE:		QuestCheck_VPCHANGE(quest); break;
		case VisPred::Game::QuestType::PLAYERSHOOT:		QuestCheck_PLAYERSHOOT(quest); break;
		case VisPred::Game::QuestType::PLAYERRUN:		QuestCheck_PLAYERRUN(quest); break;
		case VisPred::Game::QuestType::PLAYERJUMP:		QuestCheck_PLAYERJUMP(quest); break;
		case VisPred::Game::QuestType::PLAYERCROUCH:	QuestCheck_PLAYERCROUCH(quest); break;
		case VisPred::Game::QuestType::PLAYERSLIDE:		QuestCheck_PLAYERSLIDE(quest); break;
		case VisPred::Game::QuestType::PLAYERATTACK:	QuestCheck_PLAYERATTACK(quest); break;
		case VisPred::Game::QuestType::PLAYERPICKUP:	QuestCheck_PLAYERPICKUP(quest); break;
		case VisPred::Game::QuestType::PLAYERTHROW:		QuestCheck_PLAYERTHROW(quest); break;
		case VisPred::Game::QuestType::PLAYERINTERECT:	QuestCheck_PLAYERINTERECT(quest); break;
		default:
			break;
		}

	}
	if (!m_MainQuest)
		return;
	// Questptrs가 비어있는지 확인하여 예외 처리
	if (!m_MainQuest->Questptrs.empty())
	{
		// 첫 번째 퀘스트 시작 설정

		m_MainQuest->Questptrs.front()->IsStarted = true;

		// 첫 번째 퀘스트가 클리어된 경우, 리스트에서 제거
		if (m_MainQuest->Questptrs.front()->IsCleared)
		{
			m_MainQuest->Questptrs.pop_front();
		}
	}
	// Questptrs가 비어 있는지 확인 후 추가 작업
	if (m_MainQuest->Questptrs.empty())
	{
		// 모든 퀘스트가 완료된 경우에 대한 처리
		// 예: 로그 출력, 다음 단계 진행, UI 업데이트 등
		// 예: 로그 출력
		// 추가로 필요에 따라 다른 로직을 추가
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
		if (!m_MainQuest)
		{
			m_MainQuest = entity->GetComponent<MainQuestComponent>();
			for (auto& identity : m_MainQuest->QuestSequence)
			{
				auto quest = GetSceneManager()->GetEntityByIdentityName(identity);
				if (quest && quest->HasComponent<QuestComponent>())
				{
					m_MainQuest->Questptrs.push_back(quest->GetComponent<QuestComponent>());
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
	if (questcomp.QuestPlayer->IsVPMode&& questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::RUN)
	{
		questcomp.IsCleared=true;

	}
	
}

void QuestSystem::QuestCheck_VPJUMP(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		questcomp.IsCleared = true;


	}
}

void QuestSystem::QuestCheck_VPDASH(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Dash_Slide)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_VPCHANGE(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Transformation)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERSHOOT(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->HasGun && questcomp.QuestPlayer->IsAttacking)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERRUN(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::RUN)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERJUMP(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERCROUCH(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::CROUCH)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERSLIDE(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->CurrentFSM == VisPred::Game::PlayerFSM::Dash_Slide)
	{
		questcomp.IsCleared = true;

	}
}

void QuestSystem::QuestCheck_PLAYERATTACK(QuestComponent& questcomp)
{
	questcomp.GetComponent<TextComponent>()->Color = { 1.f,1.f,1.f };
	if (!questcomp.QuestPlayer->IsVPMode && questcomp.QuestPlayer->IsAttacking&& !questcomp.QuestPlayer->HasGun)
	{
		questcomp.IsCleared = true;

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
	if (!m_MainQuest)
		return;
	if (m_MainQuest->Questptrs.empty())
		return;
	if (m_MainQuest->Questptrs.front()->QuestType == VisPred::Game::QuestType::PLAYERINTERECT)
			m_MainQuest->Questptrs.front()->IsCleared = true;
}

void QuestSystem::Finalize()
{
	m_MainQuest = nullptr;	
}

void QuestSystem::Update(float deltaTime)
{
}
