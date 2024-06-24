#include "pch.h"
#include "EditorViewPort.h"
#include "Toolbar.h"
#include "HierarchySystem.h"
#include <InputManager.h>
#include "..\VPGraphics\GraphicsEngine.h"


#include "..\include\imgui_internal.h"

EditorViewPort::EditorViewPort(SceneManager* sceneManager, EditorCamera* Camera, Graphics::Interface* Graphics) :m_SceneManager{ sceneManager }, m_Camera{ Camera }, m_Graphics{ Graphics }
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
	
	auto current = ImGui::GetCurrentContext();
	auto pos = ImVec2{ current->CurrentWindow->Pos.x, current->CurrentWindow->Pos.y + 25.f };
	auto size = ImVec2{ current->CurrentWindow->Size.x, current->CurrentWindow->Size.y - 25.f };
	auto idealSize = ImVec2(size.x, size.y);
	auto maxpos = ImVec2(pos.x + idealSize.x, pos.y + idealSize.y);
	auto borderSize = current->CurrentWindow->WindowBorderSize;


	ImGui::GetWindowDrawList()->AddImage(
		(void*)m_Graphics->GetSRV(L"IMGUI"),
		ImVec2(pos.x, pos.y),
		ImVec2(maxpos.x, maxpos.y));
	ImGuizmoRender();

}

void EditorViewPort::ImGuizmoRender()
{


	if (!m_SceneManager->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;

	TransformComponent* transformComp 
		= m_SceneManager->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);
	
	ImGuizmo::SetDrawlist();
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
