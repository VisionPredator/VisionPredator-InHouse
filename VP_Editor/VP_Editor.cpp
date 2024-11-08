#include "pch.h"
#include "Managers.h"
#include <TransformSystem.h>
#include "VP_Editor.h"
#include "Toolbar.h"
#include "EditorCamera.h"
#include "FolderTool.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "HierarchySystem.h"
#include "EditorViewPort.h"
#include "LightSystem.h"
#include "ImGuiFileDialog.h"
#include "EditorSystem.h"
VP_Editor::VP_Editor(HINSTANCE hInstance, std::string title, int width, int height) :VPProcess(hInstance, title, width, height)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgunbd.ttf",20.f, NULL, io.Fonts->GetGlyphRangesKorean());

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    SetUnityDarkThemeColors();
	///Imgui Setting
	m_EditorSystem = m_SystemManager->AddSystem<EditorSystem>();
    m_editorcamera = std::make_shared<EditorCamera>(m_SceneManager);
	m_HierarchySystem = std::make_shared<HierarchySystem>(m_SceneManager);
	m_ImGuis.push_back(std::make_shared<Toolbar>(m_SceneManager, m_PhysicEngine));
	m_ImGuis.push_back(std::make_shared<FolderTool>(m_SceneManager));
	m_ImGuis.push_back(std::make_shared<Hierarchy>(m_SceneManager, m_HierarchySystem));
	m_ImGuis.push_back(std::make_shared<Inspector>(m_SceneManager, m_HierarchySystem));
	m_ImGuis.push_back(std::make_shared<EditorViewPort>(m_SceneManager, m_editorcamera, m_Graphics));
	m_ImGuis.push_back(m_editorcamera);
	EventManager::GetInstance().Subscribe("OnPlayButton", CreateSubscriber(&VP_Editor::OnPlayButton));
	EventManager::GetInstance().Subscribe("OnPauseButton", CreateSubscriber(&VP_Editor::OnPauseButton));
	EventManager::GetInstance().Subscribe("OnResumeButton", CreateSubscriber(&VP_Editor::OnResumeButton));
	EventManager::GetInstance().Subscribe("OnStopButton", CreateSubscriber(&VP_Editor::OnStopButton));
}

VP_Editor::~VP_Editor()
{
	for (auto& ImGui : m_ImGuis)
	{
        ImGui.reset();
	}
	m_ImGuis.clear();
    ImGuiFileDialog::Instance()->ClearFilesStyle();

    m_HierarchySystem.reset();
}

void VP_Editor::Update()
{

	if (m_IsEditorMode)
	{
        m_TimeManager->Update();
        m_DeltaTime = m_TimeManager->GetDeltaTime();
        EventManager::GetInstance().Update(m_DeltaTime);
        m_TransformSystem->Update(m_DeltaTime);
        m_EditorSystem->Update(m_DeltaTime);
        InputManager::GetInstance().Update();
		m_editorcamera->Update(m_DeltaTime);
        if (m_TimeManager->GetPrevFPS() != m_TimeManager->GetFPS())
        {
            std::wstring newname = std::to_wstring(m_TimeManager->GetFPS());
            SetWindowTextW(m_hWnd, newname.c_str());
        }
        m_SoundEngine->SetListenerPosition(m_editorcamera->GetPose(), m_editorcamera->GetUp(), m_editorcamera->GetForward());
        m_SoundEngine->Update();
	}
    else if (m_IsPauseMode)
    {

        m_TimeManager->Update();
        m_DeltaTime = m_TimeManager->GetDeltaTime();
        EventManager::GetInstance().Update(m_DeltaTime);
        m_TransformSystem->Update(m_DeltaTime);
        InputManager::GetInstance().Update();
        m_editorcamera->Update(m_DeltaTime);
        m_SoundEngine->SetListenerPosition(m_editorcamera->GetPose(), m_editorcamera->GetUp(), m_editorcamera->GetForward());
        m_SoundEngine->Update();

    }
	else
	{

		VPEngine::Update();
	}

}

void VP_Editor::Render()
{
	if (m_IsEditorMode||m_IsPauseMode)
	{
		m_Graphics->SetCamera(m_editorcamera->GetView(), m_editorcamera->GetProj(), m_editorcamera->GetOrthoProj());
		EditorRenderUpdate();
		VPEngine::BeginRender();
		ImguiRender();
		VPEngine::EndRender();
	}
	else
	{
		VPEngine::RenderUpdate();
		VPEngine::BeginRender();
		ImguiRender();
		VPEngine::EndRender();
	}
}
void VP_Editor::EditorRenderUpdate()
{
    m_SystemManager->EditorRenderSystemUpdate(m_DeltaTime);
}

void VP_Editor::ImguiRender()
{
   bool isUpdate = VPEngine::ImguiBeginRender();
   if (isUpdate)
   {
       ImGui::DockSpaceOverViewport();

       // Render
       for (auto& ImGui : m_ImGuis)
       {
           //if (dynamic_cast<Hierarchy*>(ImGui)|| dynamic_cast<EditorViewPort*>(ImGui))
           //    continue;
           ImGui->ImGuiRender();
       }
       VPEngine::ImguiEndRender();
   }

}


void VP_Editor::SetUnityDarkThemeColors()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(0.89f, 0.89f, 0.89f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.37f, 0.50f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.90f, 0.29f, 0.48f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.89f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.89f, 0.89f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.90f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.60f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.60f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(5, 2);
    style.CellPadding = ImVec2(6, 6);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(6, 6);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.GrabMinSize = 10.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    style.WindowRounding = 3.0f;
    style.ChildRounding = 3.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
}


void VP_Editor::OnPlayButton(std::any)
{
	m_IsEditorMode = false;
    ShowCursor(FALSE);
    EventManager::GetInstance().ImmediateEvent("OnStartScene");
}

void VP_Editor::OnStopButton(std::any)
{
	m_IsEditorMode = true;
    ShowCursor(TRUE);

    EventManager::GetInstance().ImmediateEvent("OnEndScene");

}

void VP_Editor::OnPauseButton(std::any)
{
    ShowCursor(TRUE);

    m_IsPauseMode = true;
}

void VP_Editor::OnResumeButton(std::any)
{
    ShowCursor(FALSE);

    m_IsPauseMode = false;
}

