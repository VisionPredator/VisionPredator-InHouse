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
	if (Toolbar::m_IsPlaying && !Toolbar::m_IsPaused)
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
	ImGuizmoSettingrender();
	// RenderMode 버튼 설정
	ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_None | ImGuiWindowFlags_NoTitleBar;
	ImGuiChildFlags child_flags2 = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	ImVec2 childWindowPos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 125, ImGui::GetWindowPos().y + 20);
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

	if (ImGui::RadioButton("Translate", m_ImGuizmoMode == ImGuizmo::TRANSLATE && !m_IsSocket) ||
		(INPUTKEY(KEYBOARDKEY::Q) && !INPUTKEY(MOUSEKEY::RBUTTON) && true))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		m_CurrentModeSnap = &m_TranslationSnapValue;
		m_IsSocket = false;  // Socket 모드 해제
	}

	if (ImGui::RadioButton("Rotate", m_ImGuizmoMode == ImGuizmo::ROTATE && !m_IsSocket) ||
		(INPUTKEY(KEYBOARDKEY::W) && !INPUTKEY(MOUSEKEY::RBUTTON) && true))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::ROTATE;
		m_CurrentModeSnap = &m_RotationSnapValue;
		m_IsSocket = false;  // 일반 회전 모드
	}

	if (ImGui::RadioButton("Scale", m_ImGuizmoMode == ImGuizmo::SCALE) ||
		(INPUTKEY(KEYBOARDKEY::E) && !INPUTKEY(MOUSEKEY::RBUTTON) && true))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::SCALE;
		m_CurrentModeSnap = &m_ScaleSnapValue;
		m_IsSocket = false;  // Socket 모드 해제
	}
	if (ImGui::RadioButton("Socket_Translate", m_ImGuizmoMode == ImGuizmo::TRANSLATE && m_IsSocket) ||
		(INPUTKEY(KEYBOARDKEY::R) && !INPUTKEY(MOUSEKEY::RBUTTON) && true))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		m_CurrentModeSnap = &m_RotationSnapValue;
		m_IsSocket = true;  // Socket 모드 활성화
	}

	if (ImGui::RadioButton("Socket_Rotate", m_ImGuizmoMode == ImGuizmo::ROTATE && m_IsSocket) ||
		(INPUTKEY(KEYBOARDKEY::T) && !INPUTKEY(MOUSEKEY::RBUTTON) && true))
	{
		m_ImGuizmoMode = ImGuizmo::OPERATION::ROTATE;
		m_CurrentModeSnap = &m_RotationSnapValue;
		m_IsSocket = true;  // Socket 모드 활성화
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	if (!m_SceneManager.lock()->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;

	TransformComponent* transformComp = m_SceneManager.lock()->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);
	float* snapValue = nullptr;

	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::LCONTROL))
		snapValue = reinterpret_cast<float*>(m_CurrentModeSnap);


	if (m_IsSocket && m_ImGuizmoMode == ImGuizmo::ROTATE)
	{
		if (!transformComp->HasComponent<SocketComponent>())
			return;

		auto& socketcomp = *transformComp->GetComponent<SocketComponent>();

		// 회전 매트릭스와 오프셋(위치)을 함께 곱한 매트릭스 생성
		VPMath::Matrix socketMatrix = VPMath::Matrix::CreateFromQuaternion(socketcomp.OffsetQuaternion) * VPMath::Matrix::CreateTranslation(socketcomp.Offset);
		VPMath::Matrix finalMatrix = socketMatrix * socketcomp.AttachmentMatrix;

		// ImGuizmo에 회전 매트릭스만 전달
		ImGuizmo::Manipulate(
			&view.m[0][0],
			&proj.m[0][0],
			m_ImGuizmoMode,
			m_GuizmoMode,
			&finalMatrix.m[0][0],  // 회전 매트릭스만 전달
			nullptr,
			snapValue
		);

		if (ImGuizmo::IsUsing())
		{
			// ImGuizmo가 조작 중일 때만 회전과 위치를 업데이트
			VPMath::Vector3 scale, translation;
			VPMath::Quaternion rotation;

			// 현재 finalMatrix에서 회전 및 위치 추출
			socketMatrix = finalMatrix * socketcomp.AttachmentMatrix.Invert();
			socketMatrix.NewDecompose(scale, rotation, translation);

			// 회전만 업데이트
			socketcomp.UseQuaternion = true;
			socketcomp.OffsetQuaternion = rotation;

			// 업데이트된 소켓 정보 이벤트 전송
			EventManager::GetInstance().ImmediateEvent("OnSocketUpdate", socketcomp.GetEntityID());
		}

		return;
	}
	else if (m_IsSocket && m_ImGuizmoMode == ImGuizmo::TRANSLATE)
	{
		if (!transformComp->HasComponent<SocketComponent>())
			return;

		auto& socketcomp = *transformComp->GetComponent<SocketComponent>();

		// 회전 매트릭스와 오프셋(위치)을 함께 곱한 매트릭스 생성
		VPMath::Matrix socketMatrix = VPMath::Matrix::CreateFromQuaternion(socketcomp.OffsetQuaternion) * VPMath::Matrix::CreateTranslation(socketcomp.Offset);
		VPMath::Matrix finalMatrix = socketMatrix * socketcomp.AttachmentMatrix;

		// ImGuizmo에 회전 매트릭스만 전달
		ImGuizmo::Manipulate(
			&view.m[0][0],
			&proj.m[0][0],
			m_ImGuizmoMode,
			m_GuizmoMode,
			&finalMatrix.m[0][0],  // 회전 매트릭스만 전달
			nullptr,
			snapValue
		);

		if (ImGuizmo::IsUsing())
		{
			// ImGuizmo가 조작 중일 때만 회전과 위치를 업데이트
			VPMath::Vector3 scale, translation;
			VPMath::Quaternion rotation;

			// 현재 finalMatrix에서 회전 및 위치 추출
			socketMatrix = finalMatrix * socketcomp.AttachmentMatrix.Invert();
			socketMatrix.NewDecompose(scale, rotation, translation);

			// 회전만 업데이트
			//socketcomp.UseQuaternion = true;
			//socketcomp.OffsetQuaternion = rotation;
			socketcomp.Offset = translation;

			// 업데이트된 소켓 정보 이벤트 전송
			EventManager::GetInstance().ImmediateEvent("OnSocketUpdate", socketcomp.GetEntityID());
		}

		return;
	}






	VPMath::Matrix ImGuizmoMatrix = transformComp->WorldTransform;

	ImGuizmo::Manipulate(
		&view.m[0][0],
		&proj.m[0][0],
		m_ImGuizmoMode,
		m_GuizmoMode,
		&ImGuizmoMatrix.m[0][0], nullptr, snapValue
	);
	if (ImGuizmo::IsUsing())
	{
		TransformComponent* selectedTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);

		if (m_SceneManager.lock()->HasComponent<Parent>(HierarchySystem::m_SelectedEntityID))
		{
			Parent* parent = m_SceneManager.lock()->GetComponent<Parent>(HierarchySystem::m_SelectedEntityID);
			TransformComponent* parentTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(parent->ParentID);
			// 부모 변환과 ImGuizmoMatrix 결합
			ImGuizmoMatrix = ImGuizmoMatrix * parentTransform->WorldTransform.Invert();
		}
		// 매트릭스를 로컬 변환 요소로 분해
		VPMath::Vector3 scale, translation;
		VPMath::Quaternion rotation;
		ImGuizmoMatrix.NewDecompose(scale, rotation, translation);

		switch (m_ImGuizmoMode)
		{
		case ImGuizmo::OPERATION::TRANSLATE:
			selectedTransform->SetLocalLocation(translation);
			break;

		case ImGuizmo::OPERATION::ROTATE:
			selectedTransform->SetLocalQuaternion(rotation);
			break;

		case ImGuizmo::OPERATION::SCALE:
			selectedTransform->SetLocalScale(scale);
			break;

		default:
			break;
		}
	}
}

void EditorViewPort::ImGuizmoSettingrender()
{
	ImGuizmo::SetDrawlist();

	// 저장된 크기와 위치를 기반으로 Rect 설정
	ImGuizmo::SetRect(m_DrawPos.x + 50, m_DrawPos.y, m_DrawSize.x + 50, m_DrawSize.y);

	VPMath::Matrix view = m_Camera.lock()->GetView();
	VPMath::Matrix proj = m_Camera.lock()->GetProj();

	// ImGuizmo 모드 선택
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;
	ImGuiChildFlags child_flags = ImGuiWindowFlags_None | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::BeginChild("imguizosetting", ImVec2{ 120, 0 }, child_flags, window_flags);

	if (ImGui::RadioButton("Local", m_GuizmoMode == ImGuizmo::MODE::LOCAL && m_IsLocalMode))
	{
		m_GuizmoMode = ImGuizmo::MODE::LOCAL;
		m_IsLocalMode = true;
	}

	if (ImGui::RadioButton("World", m_GuizmoMode == ImGuizmo::MODE::WORLD && !m_IsLocalMode))
	{
		m_GuizmoMode = ImGuizmo::MODE::WORLD;
		m_IsLocalMode = false;
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();


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
	case RENDERMODE::LightMap:
		return L"LightMap";
	default:
		return L"Unknown";
	}
}