#include "pch.h"
#include "EditorSystem.h"

EditorSystem::EditorSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{

}

void EditorSystem::Update(float deltaTime)
{
	if (INPUTKEYDOWN(KEYBOARDKEY::F1))
	{
		isDebugRender = !isDebugRender;
		m_Graphics->DebugRenderONOFF(isDebugRender);
	}
}

void EditorSystem::BeginRenderUpdate(float deltaTime)
{
	

}

void EditorSystem::RenderUpdate(float deltaTime)
{
}

void EditorSystem::LateRenderUpdate(float deltaTime)
{
}

void EditorSystem::EditorRenderUpdate(float deltaTime)
{

}
