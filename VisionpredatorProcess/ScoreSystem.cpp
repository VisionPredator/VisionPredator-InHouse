#include "pch.h"
#include "ScoreSystem.h"
#include "EventManager.h"
#include "TextBounceComponent.h"
#include "ScoreComponent.h"
#include "BestScoreComponent.h"
ScoreSystem::ScoreSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnEnemyKilled", CreateSubscriber(&ScoreSystem::OnEnemyKilled));
	EventManager::GetInstance().Subscribe("OnHideScore", CreateSubscriber(&ScoreSystem::OnHideScore));
	EventManager::GetInstance().Subscribe("OnScoreToMiddle", CreateSubscriber(&ScoreSystem::OnScoreToMiddle));
	EventManager::GetInstance().Subscribe("OnEndingBestScore", CreateSubscriber(&ScoreSystem::OnEndingBestScore));
}

void ScoreSystem::OnEnemyKilled(std::any)
{
	if (!m_Score.lock())
		return; 

	auto scorecomp = m_Score.lock()->GetComponent<ScoreComponent>();
	scorecomp->PlayerPoint++;

	if (scorecomp->HasComponent<TextComponent>())
		scorecomp->GetComponent<TextComponent>()->Text = scorecomp->Ment + std::to_wstring(scorecomp->PlayerPoint);
	if (scorecomp->HasComponent<TextBounceComponent>())
		scorecomp->GetComponent<TextBounceComponent>()->AddedBounce = true;

}

void ScoreSystem::OnHideScore(std::any)
{
	if (!m_Score.lock())
		return;

	auto scorecomp = m_Score.lock()->GetComponent<ScoreComponent>();
	if (scorecomp->HasComponent<TextComponent>())
		scorecomp->GetComponent<TextComponent>()->Color.w=0 ;

}

void ScoreSystem::OnScoreToMiddle(std::any)
{
	if (!m_Score.lock())
		return;
	auto scorecomp = m_Score.lock()->GetComponent<ScoreComponent>();
	if (!scorecomp->HasComponent<TextComponent>())
		return;
		scorecomp->GetComponent<TextComponent>()->Text = scorecomp->EndMent + std::to_wstring(scorecomp->PlayerPoint);

		auto text = scorecomp->GetComponent<TextComponent>();
		text->PosXPercent = scorecomp->MiddlePose.x;
		text->PosYPercent = scorecomp->MiddlePose.y;
		text->Scale = scorecomp->MiddleScale;
		text->Color = scorecomp->EndFontColor;
}

void ScoreSystem::OnEndingBestScore(std::any)
{

	if (!m_BestScore.lock())
		return;
	if (!m_Score.lock())
		return;

	auto score = m_Score.lock()->GetComponent<ScoreComponent>();
	auto best = m_BestScore.lock()->GetComponent<BestScoreComponent>();
	if (best->Score < score->PlayerPoint)
	{
		best->Score = score->PlayerPoint;
		GetSceneManager()->SerializePrefab(best->GetEntityID(),true);

		auto text = best->GetComponent<TextComponent>();
		text->Text = best->NewBestScoreMent + std::to_wstring(best->Score);
		text->Color = best->NewColor;
	}
	else
	{
		auto text = best->GetComponent<TextComponent>();
		text->Text = best->BestScoreMent + std::to_wstring(best->Score);
		text->Color = best->BasicColor;
	}

}

void ScoreSystem::Initialize()
{
	COMPLOOP(ScoreComponent, comp)
	{
		if (m_Score.lock())
			break;
		Start(comp.GetEntityID());

	}	
	COMPLOOP(BestScoreComponent, comp)
	{
		if (m_BestScore.lock())
			break;
		Start(comp.GetEntityID());

	}
}

void ScoreSystem::Start(uint32_t gameObjectId)
{
	if (GetSceneManager()->HasComponent<ScoreComponent>(gameObjectId))
	{
		if (!m_Score.lock())
		{
			auto entity = GetSceneManager()->GetEntity(gameObjectId);
			if (entity->HasComponent<TextComponent>())
			{
				auto scorecomp = entity->GetComponent<ScoreComponent>();
				entity->GetComponent<TextComponent>()->Color = scorecomp->FontColor;
				scorecomp->GetComponent<TextComponent>()->Text = scorecomp->Ment + std::to_wstring(scorecomp->PlayerPoint);
				m_Score = entity;
			}
		}
	}

	if (GetSceneManager()->HasComponent<BestScoreComponent>(gameObjectId))
	{
		if (!m_BestScore.lock())
		{
			auto entity = GetSceneManager()->GetEntity(gameObjectId);
			if (entity->HasComponent<TextComponent>())
			{
				auto scorecomp = entity->GetComponent<BestScoreComponent>();
				entity->GetComponent<TextComponent>()->Color = {0,0,0,0};
				m_BestScore = entity;
			}	
		}
	}
}

void ScoreSystem::Finish(uint32_t gameObjectId)
{
}

void ScoreSystem::Finalize()
{
}

