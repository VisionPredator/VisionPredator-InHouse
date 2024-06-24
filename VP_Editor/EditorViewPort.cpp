#include "pch.h"
#include "EditorViewPort.h"
#include "Toolbar.h"
#include "HierarchySystem.h"
#include <InputManager.h>


EditorViewPort::EditorViewPort(SceneManager* sceneManager, EditorCamera* Camera) :m_SceneManager{ sceneManager }, m_Camera{ Camera }
{
}

void EditorViewPort::ImGuiRender()
{
	ImGuiWindowFlags wflags = 0;
	wflags |= ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("ViewPort" ,0, wflags);
	if (Toolbar::m_IsPlaying)
	{
		PlayingImGui();
	}
	else
	{
		EditingImGui();
	}
		ImGui::End();
}

void EditorViewPort::PlayingImGui()
{


}

void EditorViewPort::EditingImGui()
{  
	ImGuizmoRender();
}

void EditorViewPort::ImGuizmoRender()
{

	if (!m_SceneManager->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;

	TransformComponent* transformComp 
		= m_SceneManager->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);
	
	ImGuizmo::SetDrawlist();	// 먼지 모름 
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	VPMath::Matrix worldMatrix = transformComp->WorldTransform;

	VPMath::Matrix view = m_Camera->GetView();
	VPMath::Matrix proj = m_Camera->GetProj();

	float* snapValue = 0;
	if (InputManager::GetInstance().GetKey(KEY::LCTRL))
		snapValue = reinterpret_cast<float*>(m_CurrentModeSnap);

	ImGuizmo::Manipulate(
		&view.m[0][0],
		&proj.m[0][0],
		Operation,
		Mode,
		&worldMatrix.m[0][0], 0, snapValue
	);

	UsingImGuizmo();
}

void EditorViewPort::UsingImGuizmo()
{
	if (!ImGuizmo::IsUsing())
		return;

}
