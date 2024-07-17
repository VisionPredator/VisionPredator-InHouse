#include "pch.h"
#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(SceneManager* sceneManager) : System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&AnimationSystem::OnAddedComponent));
	EventManager::GetInstance().Subscribe("OnReleasedComponent", CreateSubscriber(&AnimationSystem::OnReleasedComponent));
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

void AnimationSystem::Update(float deltaTime)
{
	for (AnimationComponent& aniComp : COMPITER(AnimationComponent))
	{
		aniComp.isPlay = true;
		aniComp.isChange = false;


		if (aniComp.preAnimation != aniComp.curAnimation
			|| aniComp.duration > m_Graphics->GetDuration(aniComp.curAnimation))
		{
			aniComp.preAnimation = aniComp.curAnimation;
			aniComp.preDuration = aniComp.duration;
			aniComp.duration = 0;
			aniComp.isChange = true;
		}
		
		if(!aniComp.curAnimation.empty())
		{
			aniComp.duration += deltaTime * aniComp.speed;
		}		
	}
}

void AnimationSystem::FixedUpdate(float deltaTime)
{

}

void AnimationSystem::RenderUpdate(float deltaTime)
{

}
