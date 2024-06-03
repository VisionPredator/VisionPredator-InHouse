#include "pch.h"
#include "VP_Editor.h"
#include "SystemManager.h"
#include "Toolbar.h"
#include <TransformSystem.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "EditorCamera.h"
VP_Editor::VP_Editor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
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
	//m_ImGuis.push_back(new Toolbar);
	test = new EditorCamera;


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
	test->Update(m_DeltaTime);
	VPEngine::Update();
	static float test = 0;

	test += m_DeltaTime;
	if (test>5&& test < 9)
	{
		m_SystemManager->ReleaseSystem<TransformSystem>();
	}
	if (test > 8)
	{
		m_SystemManager->AddSystem<TransformSystem>();
	}
}

void VP_Editor::Render()
{
	//for (auto& ImGui : m_ImGuis)
	//{
	//	ImGui->ImGuiRender();
	//}
}

