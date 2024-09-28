#include "pch.h"
#include "AnimationSystem.h"

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


void AnimationSystem::OnChangeAnimation(std::any tupleData_ID_Index_Loop)
{
	auto [entityid, index,loop] = std::any_cast<std::tuple<uint32_t, int, bool >>(tupleData_ID_Index_Loop);

	if (GetSceneManager()->HasComponent<AnimationComponent>(entityid))
	{
		auto aniComp = GetSceneManager()->GetComponent<AnimationComponent>(entityid);
		aniComp->isLoop = loop;
		if (aniComp->curAni != index)	//애니메이션이 다를때만 변경 같으면 그대로
		{
			aniComp->preAni = aniComp->curAni;
			aniComp->preDuration = aniComp->duration;

			aniComp->curAni = index;
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
	COMPLOOP(AnimationComponent, aniComp)
	{
		if (aniComp.FBX.empty())
			return;
		aniComp.IsFinished = false;

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

