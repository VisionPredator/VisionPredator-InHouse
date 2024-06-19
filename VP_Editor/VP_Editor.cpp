#include "pch.h"
#include "VP_Editor.h"
#include "SystemManager.h"
#include "Toolbar.h"
#include <TransformSystem.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "EditorCamera.h"
#include "FolderTool.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "EventManager.h"
#include "HierarchySystem.h"
VP_Editor::VP_Editor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.5f);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    SetUnityDarkThemeColors();
	///Imgui Setting
	/*
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hInstance);
	ImGui_ImplDX11_Init(mpDevice.Get(), mpContext.Get());

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	*/
	m_editorcamera = new EditorCamera;
	m_HierarchySystem = new HierarchySystem{m_SceneManager};
	m_ImGuis.push_back(new Toolbar{ m_SceneManager });
	m_ImGuis.push_back(new FolderTool{ m_SceneManager });
	m_ImGuis.push_back(new Hierarchy{m_SceneManager,m_HierarchySystem});
	m_ImGuis.push_back(new Inspector{ m_SceneManager,m_HierarchySystem });
	m_ImGuis.push_back(m_editorcamera);
	EventManager::GetInstance().Subscribe("OnPlayButton", CreateSubscriber(&VP_Editor::OnPlayButton));
	EventManager::GetInstance().Subscribe("OnStopButton", CreateSubscriber(&VP_Editor::OnStopButton));
}

VP_Editor::~VP_Editor()
{
	for (auto& ImGui : m_ImGuis)
	{
		delete ImGui;
	}
	m_ImGuis.clear();
}

void VP_Editor::Update()
{
	if (m_IsEditorMode)
	{
		m_editorcamera->Update(m_DeltaTime);
		VPEngine::Update();

		m_Graphics->SetCamera(m_editorcamera->GetView(), m_editorcamera->GetProj());
	}
	else
	{
		VPEngine::Update();
		m_Graphics->SetCamera(m_editorcamera->GetView(), m_editorcamera->GetProj());

	}

}

void VP_Editor::Render()
{
	VPEngine::Render();

	// Create a window called "Hello, world!" and append into it.

	//ImGui::DockSpaceOverViewport();

	// Rendering
	for (auto& ImGui : m_ImGuis)
	{
		ImGui->ImGuiRender();
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
}

void VP_Editor::OnStopButton(std::any)
{
	m_IsEditorMode = true;

}

