#pragma once
#include "../VPEngine/VPEngine.h"
#include "../VisionpredatorProcess/VPProcess.h"
#include "EventSubscriber.h"
class EditorCamera;
class IImGui;
class HierarchySystem;
class EditorSystem;
class VP_Editor :public VPProcess
{
public:
	VP_Editor(HINSTANCE hInstance, std::string title, int width, int height);
	~VP_Editor();
	virtual void Update();
	virtual void Render();
	std::shared_ptr<HierarchySystem> m_HierarchySystem;
	EditorSystem* m_EditorSystem{};
	std::vector<std::shared_ptr<IImGui>> m_ImGuis;
	std::shared_ptr<EditorCamera> m_editorcamera;
	void SetUnityDarkThemeColors();
	void EditorRenderUpdate();
	void ImguiRender();
	bool m_IsEditorMode = true;
	bool m_IsPauseMode = false;
	void OnPlayButton(std::any);
	void OnStopButton(std::any);
	void OnPauseButton(std::any);
	void OnResumeButton(std::any);
	
};

