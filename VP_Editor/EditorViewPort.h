#pragma once
#include "IImGui.h"
#include "EditorCamera.h"
class SceneManager;
using namespace VPMath;

class EditorViewPort:public IImGui
{
public:
	EditorViewPort(SceneManager* sceneManager, EditorCamera* m_Camera);
	// IImGui을(를) 통해 상속됨
	void ImGuiRender() override;
	void PlayingImGui();
	void EditingImGui();
	void ImGuizmoRender();
	void UsingImGuizmo();
private:

	Vector3 m_TranslationSnapValue = Vector3(1.0f);
	Vector3 m_RotationSnapValue = Vector3(15.0f);
	Vector3 m_ScaleSnapValue = Vector3(0.1f);
	Vector3* m_CurrentModeSnap = &m_TranslationSnapValue;

	SceneManager* m_SceneManager;
	EditorCamera* m_Camera;
	ImGuizmo::OPERATION Operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE Mode = ImGuizmo::MODE::LOCAL;
};

