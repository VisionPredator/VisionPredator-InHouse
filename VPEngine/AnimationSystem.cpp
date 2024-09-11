#include "pch.h"
#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&AnimationSystem::OnAddedComponent));
	EventManager::GetInstance().Subscribe("OnReleasedComponent", CreateSubscriber(&AnimationSystem::OnReleasedComponent));
	EventManager::GetInstance().Subscribe("OnChangeAnimation", CreateSubscriber(&AnimationSystem::OnChangeAnimation));
}

void AnimationSystem::OnAddedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<AnimationComponent>())
	{
		AnimationComponent* aniComp = static_cast<AnimationComponent*>(comp);
		return;
	}
}

void AnimationSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<AnimationComponent>())
	{
		AnimationComponent* aniComp = static_cast<AnimationComponent*>(comp);

		return;
	}
}

void AnimationSystem::RenderUpdate(float deltaTime)
{

}

void AnimationSystem::Update(float deltaTime)
{

	for (AnimationComponent& aniComp : COMPITER(AnimationComponent))
	{
		aniComp.isPlay = true;

		if (!aniComp.FBX.empty())
		{
			aniComp.duration += deltaTime * aniComp.speed;

			//애니메이션 계속 재생
			if (aniComp.preAni == aniComp.curAni)
			{
				double curDuration = m_Graphics->GetDuration(aniComp.FBX, aniComp.curAni);
				if (aniComp.duration > curDuration)
				{
					aniComp.duration -= curDuration;
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
		aniComp->preAni = aniComp->curAni;
		aniComp->preDuration = aniComp->duration;

		aniComp->curAni = index;
		aniComp->duration = 0.0f;
	}
}
