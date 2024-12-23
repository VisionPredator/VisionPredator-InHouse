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
	auto aniblenddata = std::any_cast<VisPred::Engine::AniBlendData>(aniBlendData);

	if (GetSceneManager()->HasComponent<AnimationComponent>(aniblenddata.EntityID))
	{
		auto aniComp = GetSceneManager()->GetComponent<AnimationComponent>(aniblenddata.EntityID);
		aniComp->isLoop = aniblenddata.Loop;

		if (aniComp->curAni != aniblenddata.Index || aniblenddata.IsAgain)	//애니메이션이 다를때만 변경 같으면 그대로
		{
			aniComp->preAni = aniComp->curAni;
			aniComp->curAni = aniblenddata.Index;
			aniComp->AniDuration = m_Graphics->GetDuration(aniComp->FBX, aniComp->curAni);
			aniComp->IsBlending = true;
			aniComp->IsFinished = false;

			aniComp->preDuration = aniComp->duration;
			if (aniblenddata.Speed > 0.001f)
			{
				// AnimationSpeed 벡터에서 해당 인덱스의 speed를 가져오기
				if (aniComp->curAni >= 0 && aniComp->curAni < aniComp->AnimationSpeed_Transition.size())
				{
					std::get<1>(aniComp->AnimationSpeed_Transition[aniComp->curAni])= aniblenddata.Speed;
					aniComp->speed = std::get<1>(aniComp->AnimationSpeed_Transition[aniComp->curAni]);
				}
				else
				{
					aniComp->speed = aniblenddata.Speed;
				}
			}
			if (aniblenddata.TransitionTime > 0.0001f)
			{
				// AnimationSpeed 벡터에서 해당 인덱스의 speed를 가져오기
				if (aniComp->curAni >= 0 && aniComp->curAni < aniComp->AnimationSpeed_Transition.size())
				{
					std::get<2>(aniComp->AnimationSpeed_Transition[aniComp->curAni]) = aniblenddata.TransitionTime;
					aniComp->transitionDuration = std::get<2>(aniComp->AnimationSpeed_Transition[aniComp->curAni]);
				}
				else
				{
					aniComp->transitionDuration = aniblenddata.TransitionTime;
				}

			}
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
		// AnimationSpeed 벡터에서 해당 인덱스의 speed를 가져오기
		if (aniComp.curAni >= 0 && aniComp.curAni < aniComp.AnimationSpeed_Transition.size()) 
		{
			// curAni에 해당하는 speed 값 가져오기 (std::tuple에서 두 번째 요소를 가져옴)
			float speed = std::get<1>(aniComp.AnimationSpeed_Transition[aniComp.curAni]);
			float transition = std::get<2>(aniComp.AnimationSpeed_Transition[aniComp.curAni]);

			if (speed >0.001f) 
			{
				aniComp.speed = speed;
			}

			if (transition >0.001f)
			{
				aniComp.transitionDuration = transition;
			}
		}

		if (aniComp.FBX.empty())
			return;
		aniComp.IsFinished = false;
		aniComp.PlayerCurAni = static_cast<VisPred::Game::PlayerAni>(aniComp.curAni);
		aniComp.AniDuration = m_Graphics->GetDuration(aniComp.FBX, aniComp.curAni);

		//애니메이션 계속 재생
		if (aniComp.preAni == aniComp.curAni)
		{
			aniComp.IsBlending = false;

			// 애니메이션이 끝났는지 확인
			if (aniComp.duration >= aniComp.AniDuration)
			{
				if (aniComp.isLoop)
				{
					aniComp.duration -= aniComp.AniDuration;
					aniComp.IsFinished = false;
				}
				else
				{
					aniComp.IsFinished = true;
					aniComp.duration = aniComp.AniDuration;
				}
			}
			else
			{
				aniComp.IsFinished = false;
			}

			aniComp.duration += deltaTime* aniComp.speed;
		}
		else
		{
			aniComp.IsBlending = true;
			if (aniComp.duration > aniComp.transitionDuration)
			{
				aniComp.preAni = aniComp.curAni;
				aniComp.duration = 0;
				aniComp.IsFinished = false;
			}
			aniComp.duration += deltaTime;
		}
	}
}

