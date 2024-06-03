#include "pch.h"
#include "VP_Editor.h"
#include "SystemManager.h"
#include <TransformSystem.h>

VP_Editor::VP_Editor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
}

VP_Editor::~VP_Editor()
{
}

void VP_Editor::Update()
{
	VPEngine::Update();
	static float test = 0;

	test += m_DeltaTime;
	if (test>5&& test < 9)
	{
		m_SystemManager->ReleaseSystem<TransformSystem>();
	}
	if (test > 8)
	{
		m_SystemManager->AddSystem<TransformSystem>();
	}
}

void VP_Editor::Render()
{
}

