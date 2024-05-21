#include "pch.h"
#include "VP_Editor.h"

VP_Editor::VP_Editor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
}

VP_Editor::~VP_Editor()
{
}

void VP_Editor::Update()
{
	VPEngine::Update();

}

void VP_Editor::Render()
{
}

