#include "pch.h"
#include "AnimationSystem.h"
#include "EngineStructs.h"
AnimationSystem::AnimationSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnChangeAnimation", CreateSubscriber(&AnimationSystem::OnChangeAnimation));
}


void AnimationSystem::BeginRenderUpdate(float deltaTime)
{
	for (AnimationComponent& aniComp : COMPITER(AnimationComponent))
	{
		if (aniComp.HasComponent<SkinningMeshComponent>())
		{
			auto skinned = aniComp.GetComponent<SkinningMeshComponent>();
			skinned->Renderdata->curAni = aniComp.curAni;
			skinned->Renderdata->preAni = aniComp.preAni;
			skinned->Renderdata->isPlay = aniComp.isPlay;
			skinned->Renderdata->preDuration = aniComp.preDuration;
			skinned->Renderdata->duration = aniComp.duration;
			skinned->Renderdata->transitionDuration = aniComp.transitionDuration;
		}
	}

}


void AnimationSystem::OnChangeAnimation(std::any aniBlendData)
{
	auto aniblenddata= std::any_cast<VisPred::Engine::AniBlendData>(aniBlendData);

	if (GetSceneManager()->HasComponent<AnimationComponent>(aniblenddata.EntityID))
	{
		auto aniComp = GetSceneManager()->GetComponent<AnimationComponent>(aniblenddata.EntityID);
		aniComp->isLoop = aniblenddata.Loop;
		if (aniComp->curAni != aniblenddata.Index)	//애니메이션이 다를때만 변경 같으면 그대로
		{
			aniComp->preAni = aniComp->curAni;
			aniComp->preDuration = aniComp->duration;
			aniComp->speed = aniblenddata.Speed;
			aniComp->curAni = aniblenddata.Index;
			aniComp->duration = 0.0f;
		}
	}
}

void AnimationSystem::EditorRenderUpdate(float deltaTime)
{

}

void AnimationSystem::RenderUpdate(float deltaTime)
{
	
}

void AnimationSystem::LateRenderUpdate(float deltaTime)
{
}

void AnimationSystem::FixedUpdate(float deltaTime)
{

}

void AnimationSystem::Update(float deltaTime)
{
	COMPLOOP(AnimationComponent, aniComp)
	{
		if (aniComp.FBX.empty())
			return;
		aniComp.IsFinished = false;
		aniComp.PlayerCurAni = static_cast<VisPred::Game::PlayerAni>(aniComp.curAni);
		//애니메이션 계속 재생
		if (aniComp.preAni == aniComp.curAni)
		{
			aniComp.IsBlending = false;
			double curDuration = m_Graphics->GetDuration(aniComp.FBX, aniComp.curAni);
			if (aniComp.duration >= curDuration)
			{
				if (aniComp.isLoop)
				{
					aniComp.duration -= curDuration;
					aniComp.IsFinished = false;
				}
				else
				{
					aniComp.IsFinished = true;
					aniComp.duration = curDuration;
				}
			}

		}
		else
		{
			aniComp.IsBlending = true;
			//각각 애니메이션 사이 보간
			if (aniComp.duration > aniComp.transitionDuration)
			{
				aniComp.preAni = aniComp.curAni;
				aniComp.duration = 0;
			}
		}
		aniComp.duration += deltaTime * aniComp.speed;
	}
}

