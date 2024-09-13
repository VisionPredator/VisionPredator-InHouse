#include "pch.h"
#include "LifeTimeSystem.h"

LifeTimeSystem::LifeTimeSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}

void LifeTimeSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(LifeTimeComponent,comp)
	{
		if (comp.LifeTime < 0)
			GetSceneManager()->DestroyEntity(comp.GetEntityID());
		comp.LifeTime -= deltaTime;
	}

}
