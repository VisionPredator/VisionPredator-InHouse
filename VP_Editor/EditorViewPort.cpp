#include "pch.h"
#include "EditorViewPort.h"
#include "EventManager.h"
#include "Toolbar.h"
#include "HierarchySystem.h"
#include <InputManager.h>

EditorViewPort::EditorViewPort(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<EditorCamera> Camera, Graphics::Interface* Graphics)
	: m_SceneManager{ sceneManager }, m_Camera{ Camera }, m_Graphics{ Graphics }
{
	// ImGuizmo 스타일 설정
	auto& style = ImGuizmo::GetStyle();
	style.ScaleLineThickness = 3.f;
	style.TranslationLineThickness = 3.f;
	style.RotationLineThickness = 5.f;

	// 화면 크기 변경 이벤트 구독
	EventManager::GetInstance().Subscribe("OnResize", CreateSubscriber(&EditorViewPort::OnResize));
}

void EditorViewPort::ImGuiRender()
{
	// ImGui 창 설정
	ImGuiWindowFlags wflags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImGui::Begin("ViewPort", nullptr, wflags);
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
	RenderImGuiViewport();
}

void EditorViewPort::EditingImGui()
{
	RenderImGuiViewport();
	ImGuizmoRender();

	// RenderMode 버튼 설정
	ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_None | ImGuiWindowFlags_NoTitleBar;
	ImGuiChildFlags child_flags2 = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	ImVec2 childWindowPos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 125, ImGui::GetWindowPos().y+20);
	ImGui::SetNextWindowPos(childWindowPos);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::BeginChild("Tools", ImVec2{ 125, 0 }, child_flags2, window_flags2);
	if (ImGui::RadioButton("Albedo", m_CurrentRenderMode == RENDERMODE::Albedo))
		m_CurrentRenderMode = RENDERMODE::Albedo;
	if (ImGui::RadioButton("Normal", m_CurrentRenderMode == RENDERMODE::Normal))
		m_CurrentRenderMode = RENDERMODE::Normal;
	if (ImGui::RadioButton("Position", m_CurrentRenderMode == RENDERMODE::Position))
		m_CurrentRenderMode = RENDERMODE::Position;
	if (ImGui::RadioButton("Depth", m_CurrentRenderMode == RENDERMODE::Depth))
		m_CurrentRenderMode = RENDERMODE::Depth;
	if (ImGui::RadioButton("Metalic_Roughness", m_CurrentRenderMode == RENDERMODE::Metalic_Roughness))
		m_CurrentRenderMode = RENDERMODE::Metalic_Roughness;
	if (ImGui::RadioButton("Emissive", m_CurrentRenderMode == RENDERMODE::Emissive))
		m_CurrentRenderMode = RENDERMODE::Emissive;
	if (ImGui::RadioButton("GBuffer", m_CurrentRenderMode == RENDERMODE::GBuffer))
		m_CurrentRenderMode = RENDERMODE::GBuffer;
	if (ImGui::RadioButton("IMGUI", m_CurrentRenderMode == RENDERMODE::IMGUI))
		m_CurrentRenderMode = RENDERMODE::IMGUI;
	if (ImGui::RadioButton("LightMap", m_CurrentRenderMode == RENDERMODE::LightMap))
		m_CurrentRenderMode = RENDERMODE::LightMap;
	ImGui::EndChild();
	ImGui::PopStyleVar();
}

void EditorViewPort::RenderImGuiViewport()
{
	float FrameHeight = ImGui::GetFrameHeight();
	auto current = ImGui::GetCurrentContext();
	auto pos = ImVec2{ current->CurrentWindow->Pos.x, current->CurrentWindow->Pos.y + FrameHeight };
	auto size = ImVec2{ current->CurrentWindow->Size.x, current->CurrentWindow->Size.y - FrameHeight };

	// 이미지의 종횡비 계산
	float imageAspectRatio = (float)m_Rectsize.x / (float)m_Rectsize.y;
	// 사용 가능한 영역의 종횡비 계산
	float areaAspectRatio = size.x / size.y;

	// 이미지 크기 조정
	ImVec2 drawSize;
	if (imageAspectRatio > areaAspectRatio)
	{
		// 이미지가 가로로 더 넓은 경우
		drawSize.x = size.x;
		drawSize.y = size.x / imageAspectRatio;
	}
	else
	{
		// 이미지가 세로로 더 긴 경우
		drawSize.y = size.y;
		drawSize.x = size.y * imageAspectRatio;
	}

	// 이미지를 중앙에 배치
	ImVec2 imagePos = ImVec2(pos.x + (size.x - drawSize.x) / 2, pos.y + (size.y - drawSize.y) / 2);
	ImVec2 imageMaxPos = ImVec2(imagePos.x + drawSize.x, imagePos.y + drawSize.y);

	// 배경 그리기 (검정색)
	ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(10, 10, 10, 255));

	// 이미지 그리기
	ImGui::GetWindowDrawList()->AddImage((void*)m_Graphics->GetSRV(GetRenderModeString(m_CurrentRenderMode)), imagePos, imageMaxPos);

	// ImGuizmo를 위한 크기 및 위치 저장
	m_DrawPos = imagePos;
	m_DrawSize = drawSize;
}

void EditorViewPort::ImGuizmoRender()
{
	ImGuizmo::SetDrawlist();

	// 저장된 크기와 위치를 기반으로 Rect 설정
	ImGuizmo::SetRect(m_DrawPos.x, m_DrawPos.y, m_DrawSize.x, m_DrawSize.y);

	VPMath::Matrix view = m_Camera.lock()->GetView();
	VPMath::Matrix proj = m_Camera.lock()->GetProj();

	// ImGuizmo 모드 선택
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;
	ImGuiChildFlags child_flags = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::BeginChild("Setting", ImVec2{ 120, 0 }, child_flags, window_flags);
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

	if (!m_SceneManager.lock()->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;

	TransformComponent* transformComp = m_SceneManager.lock()->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);
	float* snapValue = nullptr;

	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::LCONTROL))
		snapValue = reinterpret_cast<float*>(m_CurrentModeSnap);

	VPMath::Matrix ImGuizmoMatrix = transformComp->WorldTransform;

	ImGuizmo::Manipulate(
		&view.m[0][0],
		&proj.m[0][0],
		m_ImGuizmoMode,
		ImGuizmo::WORLD,
		&ImGuizmoMatrix.m[0][0], nullptr, snapValue
	);

	if (ImGuizmo::IsUsing())
	{
		TransformComponent* selectedTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);

		if (Parent* parent = m_SceneManager.lock()->GetComponent<Parent>(HierarchySystem::m_SelectedEntityID); parent)
		{
			TransformComponent* parentTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(parent->ParentID);
			// 부모 변환과 ImGuizmoMatrix 결합
			ImGuizmoMatrix = ImGuizmoMatrix * parentTransform->WorldTransform.Invert();
		}
		// 매트릭스를 로컬 변환 요소로 분해
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
	}
}

void EditorViewPort::OnResize(std::any hwnd)
{
	auto tempHwnd = std::any_cast<HWND>(hwnd);
	RECT tempsize{};
	GetClientRect(tempHwnd, &tempsize);

	// 뷰포트 크기 업데이트
	m_Rectsize.x = tempsize.right - tempsize.left;
	m_Rectsize.y = tempsize.bottom - tempsize.top;
}

std::wstring EditorViewPort::GetRenderModeString(RENDERMODE renderMode)
{
	switch (renderMode)
	{
	case RENDERMODE::Albedo:
		return L"Albedo";
	case RENDERMODE::Normal:
		return L"Normal";
	case RENDERMODE::Position:
		return L"Position";
	case RENDERMODE::Depth:
		return L"Depth";
	case RENDERMODE::Metalic_Roughness:
		return L"Metalic_Roughness";
	case RENDERMODE::Emissive:
		return L"Emissive";
	case RENDERMODE::GBuffer:
		return L"GBuffer";
	case RENDERMODE::IMGUI:
		return L"IMGUI";
	case RENDERMODE::LightMap:
		return L"LightMap";
	default:
		return L"Unknown";
	}
}