#pragma once
#include "VPEngine.h"
#include "IImGui.h"
class EditorCamera;
class VP_Editor :public VPEngine
{
public:
	VP_Editor(HINSTANCE hInstance, std::string title, int width, int height);
	~VP_Editor();
	virtual void Update();
	virtual void Render();
	std::vector<IImGui*> m_ImGuis;
	EditorCamera* test;
};

