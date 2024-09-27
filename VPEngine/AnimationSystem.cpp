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
		if (!aniComp.FBX.empty())
		{

			//애니메이션 계속 재생
			if (aniComp.preAni == aniComp.curAni)
			{
				double curDuration = m_Graphics->GetDuration(aniComp.FBX, aniComp.curAni);

				if (aniComp.duration > curDuration)
				{
					if (aniComp.isLoop)
					{
						aniComp.duration -= curDuration;
					}
					else
					{
						aniComp.duration = curDuration;
					}
				}
				else
				{
					int a = 0;
				}
			}
			else
			{
				//각각 애니메이션 사이 보간
				if (aniComp.duration > aniComp.transitionDuration)
				{
					aniComp.preAni = aniComp.curAni;
					aniComp.duration = 0;
				}
			}
			aniComp.duration += deltaTime * aniComp.speed;
		}

		auto ent = aniComp.GetEntity();
		if (ent->HasComponent<SkinningMeshComponent>())
		{
			auto skinned = ent->GetComponent<SkinningMeshComponent>();
			skinned->Renderdata->curAni = aniComp.curAni;
			skinned->Renderdata->preAni = aniComp.preAni;
			skinned->Renderdata->isPlay = aniComp.isPlay;
			skinned->Renderdata->preDuration = aniComp.preDuration;
			skinned->Renderdata->duration = aniComp.duration;
			skinned->Renderdata->transitionDuration = aniComp.transitionDuration;
		}
	}

}



void AnimationSystem::OnChangeAnimation(std::any pairdata_entityid_AniIndex)
{
	auto [entityid, index] = std::any_cast<std::pair<uint32_t, int>>(pairdata_entityid_AniIndex);

	if (GetSceneManager()->HasComponent<AnimationComponent>(entityid))
	{
		auto aniComp = GetSceneManager()->GetComponent<AnimationComponent>(entityid);

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

