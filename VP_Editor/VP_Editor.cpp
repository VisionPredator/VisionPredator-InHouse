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
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 20.0f);

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
	m_HierarchySystem = new HierarchySystem{m_SceneManager};
	m_ImGuis.push_back(new Toolbar);
	m_ImGuis.push_back(new FolderTool);
	m_ImGuis.push_back(new Hierarchy{m_SceneManager,m_HierarchySystem});
	m_ImGuis.push_back(new Inspector{ m_SceneManager,m_HierarchySystem });
	m_editorcamera = new EditorCamera;
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

	ImGui::DockSpaceOverViewport();

	// Rendering
	for (auto& ImGui : m_ImGuis)
	{
		ImGui->ImGuiRender();
	}

}

void VP_Editor::OnPlayButton(std::any)
{
	m_IsEditorMode = false;
}

void VP_Editor::OnStopButton(std::any)
{
	m_IsEditorMode = true;

}

