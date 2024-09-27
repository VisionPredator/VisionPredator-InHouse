#include "pch.h"
#include "GunSystem.h"
#include "VisPredComponents.h"

GunSystem::GunSystem(std::shared_ptr<SceneManager> scenemanager) :System{ scenemanager }
{
}

void GunSystem::Update(float deltaTime)
{
	COMPLOOP(GunComponent, guncomp)
	{
		if (guncomp.CurrentBullet==0)
			guncomp.IsEmpty = true;
	}
}
