#pragma once
#include "VPEngine.h"
#include "IImGui.h"
#include "EventSubscriber.h"
class EditorCamera;
class HierarchySystem;
class VP_Editor :public VPEngine,public EventSubscriber
{
public:
	VP_Editor(HINSTANCE hInstance, std::string title, int width, int height);
	~VP_Editor();
	virtual void Update();
	virtual void Render();
	HierarchySystem* m_HierarchySystem;
	std::vector<IImGui*> m_ImGuis;
	EditorCamera* m_editorcamera;

	bool m_IsEditorMode = true;
	void OnPlayButton(std::any);
	void OnStopButton(std::any);
};

