#include "pch.h"
#include "EnemySystem.h"
#include "VisPredComponents.h"
EnemySystem::EnemySystem(std::shared_ptr<SceneManager> SceneMagener):System(SceneMagener)
{
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, comp)
	{
		if (comp.HP < 0)
			comp.GetEntity()->DestorySelf();
	}

}
