#include "pch.h"
#include "VPEngine.h"
#include "Managers.h"
#include <fcntl.h>
#include <imgui.h>
#include <io.h>
#include "LogicSystems.h"
#include "DataRegister_Engine.h"
#include "../VPGraphics/GraphicsEngine.h"
#include "../PhysxEngine/PhysxEngine.h"
#include "../PhysxEngine/IPhysx.h"
#include "../SoundEngine/SoundEngine.h"
#include "../SoundEngine/ISound.h"
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

bool VPEngine::isResize = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VPEngine::VPEngine(HINSTANCE hInstance, std::string title, int width, int height) :m_DeltaTime(0.f)
{
	EngineRegister::Register_Metadata();

	auto wTitle = std::wstring(title.begin(), title.end());
	// 윈도우 클래스 구조체 정의
	m_hinstance = GetModuleHandle(NULL);
	WNDCLASS wndclass{};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hinstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = wTitle.c_str();
	RegisterClass(&wndclass);
	RECT rcClient = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		wTitle.c_str(),
		wTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);
	m_ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	InputManager::GetInstance().Initialize(m_hinstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);


	m_TimeManager = new TimeManager;
	m_PhysicEngine = new PhysxEngine;
	m_SceneManager = std::make_shared< SceneManager>();
	m_SystemManager = std::make_shared<SystemManager>();
	m_SoundEngine = std::make_shared<SoundEngine>();
	m_Graphics = new GraphicsEngine(m_hWnd, m_TimeManager);
	m_SoundEngine->Initialize();
	m_SoundEngine->LoadAllSound();
	m_Graphics->Initialize();
	m_SceneManager->Initialize();
	m_PhysicEngine->Initialize();
	m_SystemManager->Initialize(m_SceneManager, m_Graphics, m_PhysicEngine);
	/// 다 초기화 되고 윈도우 만들기
	this->Addsystem();
	EventManager::GetInstance().Subscribe("OnAddSystemLater", CreateSubscriber(&VPEngine::OnAddSystemLater));
	EventManager::GetInstance().ScheduleEvent("OnAddSystemLater");
	m_SoundEngine->Play(0, "MainTitle_BGM", 1, {});
}

VPEngine::~VPEngine()
{
	m_SceneManager = nullptr;
	m_SystemManager = nullptr;

	m_Graphics->Finalize();
	delete m_Graphics;
	delete m_PhysicEngine;
	delete m_TimeManager;
	InputManager::GetInstance().Release();
}
void VPEngine::Addsystem()
{
	m_SystemManager->AddSystem<SocketSystem>();
	m_SystemManager->AddSystem<ControllerMovementSystem>();
	m_SystemManager->AddSystem<PhysicSystem>();
	m_SystemManager->AddSystem<NavMeshSystem>();
	m_SystemManager->AddSystem<NavAgentSystem>();
	m_SystemManager->AddSystem<SceneSerializer>();
	m_SystemManager->AddSystem<LightSystem>();
	///그래픽스
	m_SystemManager->AddSystem<MeshSystem>();
	m_SystemManager->AddSystem<SkinnedMeshSystem>();
	m_SystemManager->AddSystem <GeometrySystem>();
	m_SystemManager->AddSystem <UISystem>();
	m_SystemManager->AddSystem<CameraSystem>();
	m_SystemManager->AddSystem<LifeTimeSystem>();
	m_SystemManager->AddSystem<SoundSystem>();


}
void VPEngine::OnAddSystemLater(std::any)
{
	m_SystemManager->AddSystem<AnimationSystem>();

	m_TransformSystem = m_SystemManager->AddSystem<TransformSystem>();
	m_SystemManager->AddSystem<RenderSystem>();

}
void VPEngine::Loop()
{
	MSG msg;

	// 기본 메시지 루프입니다:
	while (true)
	{

		if (VPEngine::isResize)
		{
			m_Graphics->OnResize(m_hWnd);
			EventManager::GetInstance().ImmediateEvent("OnResize", m_hWnd);
			VPEngine::isResize = false;
		}


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			Update();
			Render();
			/*
			//Fixed
			//Update
			//소켓 
			//static float tempTime = 0;
			//tempTime += m_DeltaTime;
			//while (tempTime > (1 / 60.f))
			//{
			//	//m_SystemManager->RenderUpdate(tempTime);
			//	m_SystemManager->BeginRenderUpdate(tempTime);
			//	m_Graphics->CulingUpdate(tempTime);
			//	m_Graphics->Update(tempTime);
			//	Render();
			//	EndRender();

			//	tempTime -= (1 / 60.f);
			//}

			//// 사용자가 ESC키를 눌렀는지 확인 후 종료 처리함
			//if (InputManager::GetInstance().IsEscapePressed())
			//{
			//	break;
			//}
			*/
		}
	}
}




void VPEngine::Update()
{
	m_TimeManager->Update();
	m_DeltaTime = m_TimeManager->GetDeltaTime();
	if (m_DeltaTime > 1)
		m_DeltaTime = 1 / 165.f;
	EventManager::GetInstance().Update(m_DeltaTime);
	InputManager::GetInstance().Update();

	m_SystemManager->PhysicUpdate(m_DeltaTime);
	m_SystemManager->FixedUpdate(m_DeltaTime);
	m_SystemManager->Update(m_DeltaTime);
	m_SystemManager->LateUpdate(m_DeltaTime);
	m_SystemManager->SoundUpdate(m_DeltaTime);

	if (m_TimeManager->GetPrevFPS() != m_TimeManager->GetFPS())
	{
		std::wstring newname = std::to_wstring(m_TimeManager->GetFPS());
		SetWindowTextW(m_hWnd, newname.c_str());
	}
}
void VPEngine::Render()
{
	RenderUpdate();
	BeginRender();
	EndRender();
}

void VPEngine::RenderUpdate()
{
	m_SystemManager->RenderSystemUpdate(m_DeltaTime);
}

void VPEngine::BeginRender()
{
	m_SystemManager->BeginRender();
}

bool VPEngine::ImguiBeginRender()
{
	return m_SystemManager->ImguiBeginRender();
}

void VPEngine::ImguiEndRender()
{
	m_SystemManager->ImguiEndRender();
}

void VPEngine::EndRender()
{
	m_SystemManager->EndRender();

}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return 0;
	switch (message)
	{
		case WM_SIZE:
		{
			int wmId = LOWORD(wParam);


			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{

				case SIZE_RESTORED:
				case SIZE_MAXIMIZED:
					VPEngine::isResize = true;
					break;

				case IDM_ABOUT:

					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}

		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
