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
	wflags |= ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoResize;
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
	float FrameHeight = ImGui::GetFrameHeight();
	auto current = ImGui::GetCurrentContext();
	auto pos = ImVec2{ current->CurrentWindow->Pos.x, current->CurrentWindow->Pos.y + FrameHeight };
	auto size = ImVec2{ current->CurrentWindow->Size.x, current->CurrentWindow->Size.y - FrameHeight };
	auto idealSize = ImVec2(size.x, size.y);
	auto maxpos = ImVec2(pos.x + idealSize.x, pos.y + idealSize.y);
	auto borderSize = current->CurrentWindow->WindowBorderSize;

	//뷰포트 설정된 것 출력하기
	switch (m_CurrentRenderMode)
	{
	case EditorViewPort::RENDERMODE::Albedo:
		EditViewPortImGui(L"Albedo", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Normal:
		EditViewPortImGui(L"Normal", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Position:
		EditViewPortImGui(L"Position", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Depth:
		EditViewPortImGui(L"Depth", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Metalic:
		EditViewPortImGui(L"Metalic", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Roughness:
		EditViewPortImGui(L"Roughness", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::Emissive:
		EditViewPortImGui(L"Emissive", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::GBuffer:
		EditViewPortImGui(L"GBuffer", pos, maxpos);
		break;
	case EditorViewPort::RENDERMODE::IMGUI:
		EditViewPortImGui(L"IMGUI", pos, maxpos);
		break;
	default:
		break;
	}





	ImGuizmoRender();
	ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_None | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoTitleBar;
	ImGuiChildFlags child_flags2 = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	ImVec2 childWindowPos = ImVec2(maxpos.x - 125, pos.y);
	ImGui::SetNextWindowPos(childWindowPos);
	/// RenderMode 버튼 설정.
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
		ImGui::BeginChild("Tools", ImVec2{ 125,0 }, child_flags2, window_flags2);
	if (ImGui::RadioButton("Albedo", m_CurrentRenderMode == RENDERMODE::Albedo))
		m_CurrentRenderMode = RENDERMODE::Albedo;
	if (ImGui::RadioButton("Normal", m_CurrentRenderMode == RENDERMODE::Normal))
		m_CurrentRenderMode = RENDERMODE::Normal;
	if (ImGui::RadioButton("Position", m_CurrentRenderMode == RENDERMODE::Position))
		m_CurrentRenderMode = RENDERMODE::Position;
	if (ImGui::RadioButton("Depth", m_CurrentRenderMode == RENDERMODE::Depth))
		m_CurrentRenderMode = RENDERMODE::Depth;
	if (ImGui::RadioButton("Metalic", m_CurrentRenderMode == RENDERMODE::Metalic))
		m_CurrentRenderMode = RENDERMODE::Metalic;
	if (ImGui::RadioButton("Roughness", m_CurrentRenderMode == RENDERMODE::Roughness))
		m_CurrentRenderMode = RENDERMODE::Roughness;
	if (ImGui::RadioButton("Emissive", m_CurrentRenderMode == RENDERMODE::Emissive))
		m_CurrentRenderMode = RENDERMODE::Emissive;
	if (ImGui::RadioButton("GBuffer", m_CurrentRenderMode == RENDERMODE::GBuffer))
		m_CurrentRenderMode = RENDERMODE::GBuffer;
	if (ImGui::RadioButton("IMGUI", m_CurrentRenderMode == RENDERMODE::IMGUI))
		m_CurrentRenderMode = RENDERMODE::IMGUI;
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

}

void EditorViewPort::EditViewPortImGui(std::wstring mode, ImVec2 pos, ImVec2 maxPos)
{
	ImGui::GetWindowDrawList()->AddImage((void*)m_Graphics->GetSRV(mode), ImVec2(pos.x, pos.y), ImVec2(maxPos.x, maxPos.y));
}



void EditorViewPort::ImGuizmoRender()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	VPMath::Matrix view = m_Camera->GetView();
	VPMath::Matrix proj = m_Camera->GetProj();


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;
	ImGuiChildFlags child_flags = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	/// ImGuizmo 모드 선택
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::BeginChild("Setting", ImVec2{ 120,0 }, child_flags, window_flags);
	if (ImGui::RadioButton("Translate", m_ImGuizmoMode == ImGuizmo::TRANSLATE))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		m_CurrentModeSnap = &m_TranslationSnapValue;
	}
	if (ImGui::RadioButton("Rotate", m_ImGuizmoMode == ImGuizmo::ROTATE))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::ROTATE;
		m_CurrentModeSnap = &m_RotationSnapValue;
	}
	if (ImGui::RadioButton("Scale", m_ImGuizmoMode == ImGuizmo::SCALE))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::SCALE;
		m_CurrentModeSnap = &m_ScaleSnapValue;
	}
	ImGui::EndChild();
	ImGui::PopStyleVar();


	if (!m_SceneManager->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;
	TransformComponent* transformComp
		= m_SceneManager->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);
	float* snapValue =nullptr;

	if (InputManager::GetInstance().GetKey(KEY::LCTRL))
		snapValue = reinterpret_cast<float*>(m_CurrentModeSnap);

	VPMath::Matrix ImGuizmoMatrix = transformComp->WorldTransform;


	ImGuizmo::Manipulate(
		&view.m[0][0],
		&proj.m[0][0],
		m_ImGuizmoMode,
		Mode,
		&ImGuizmoMatrix.m[0][0], nullptr, snapValue
	);

	if (ImGuizmo::IsUsing())
	{
		TransformComponent* selectedTransform = m_SceneManager->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);

		if (Parent* parent = m_SceneManager->GetComponent<Parent>(HierarchySystem::m_SelectedEntityID); parent)
		{
			TransformComponent* parentTransform = m_SceneManager->GetComponent<TransformComponent>(parent->ParentID);
			// Combine the parent transform with the ImGuizmoMatrix
			ImGuizmoMatrix = ImGuizmoMatrix * parentTransform->WorldTransform.Invert();
		}
		// Decompose the matrix back to the local transform components
		VPMath::Vector3 scale, translation;
		VPMath::Quaternion rotation;
		ImGuizmoMatrix.Decompose(scale, rotation, translation);


		switch (m_ImGuizmoMode)
		{
		case ImGuizmo::OPERATION::TRANSLATE:
			selectedTransform->Local_Location = translation;
			break;

		case ImGuizmo::OPERATION::ROTATE:
			selectedTransform->Local_Quaternion = rotation;

			break;
		case ImGuizmo::OPERATION::SCALE:
			selectedTransform->Local_Scale = scale;
			break;

		default:
			break;
		}

		///여기서 바뀐 Quaternion 을 활용하여, Local_Rotation을 다시 넣고싶어





	}




}

