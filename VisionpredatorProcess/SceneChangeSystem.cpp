#include "pch.h"
#include "SceneChangeSystem.h"
#include "InputManager.h"
#include "VisPredComponents.h"
SceneChangeSystem::SceneChangeSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
}

void SceneChangeSystem::Update(float deltaTime)
{
	COMPLOOP(SceneChangeComponent, resetcomp)
	{
		if (!resetcomp.SceneChangeable)
			continue;

		if (INPUTKEYDOWN(resetcomp.ResetButton))
		{
			GetSceneManager()->ChangeScene(resetcomp.ScenePath,false);
		}
	
	}

}
