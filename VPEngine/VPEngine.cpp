#include "pch.h"
#include "VPEngine.h"
#include "Managers.h"
#include "TransformSystem.h"
#include <fcntl.h>
#include <io.h>
#include "SceneSerializer.h"
#include "RenderSystem.h"
#include "LightSystem.h"
#include "DataRegister.h"
#include "CameraSystem.h"
#include "AnimationSystem.h"
#include "../VPGraphics/GraphicsEngine.h"
#include <imgui.h>
#include "../PhysxEngine/PhysxEngine.h"
#include "../PhysxEngine/IPhysx.h"
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif
#include "PhysicSystem.h"

bool VPEngine::isResize = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VPEngine::VPEngine(HINSTANCE hInstance, std::string title, int width, int height) :m_DeltaTime(0.f)
{
	auto wTitle = std::wstring(title.begin(), title.end());
	// 윈도우 클래스 구조체 정의
	WNDCLASS wndclass{};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = wTitle.c_str();
	RegisterClass(&wndclass);
	RECT rcClient = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowW(
		wTitle.c_str(),
		wTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	VPRegister::Register_Metadata();

	m_TimeManager = new TimeManager;
	m_PhysicEngine = new PhysxEngine;
	m_SceneManager = new SceneManager;
	m_SystemManager = new SystemManager;
	m_Graphics = new GraphicsEngine(m_hWnd, m_TimeManager);
	m_Graphics->Initialize();
	m_SceneManager->Initialize();
	m_PhysicEngine->Initialize();
	m_SystemManager->Initialize(m_SceneManager,m_Graphics,m_PhysicEngine);
	InputManager::GetInstance().Initialize();
	/// 다 초기화 되고 윈도우 만들기
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
	this->Addsystem();
	EventManager::GetInstance().Subscribe("OnAddSystemLater",CreateSubscriber(&VPEngine::OnAddSystemLater));
	EventManager::GetInstance().ScheduleEvent("OnAddSystemLater");
}

VPEngine::~VPEngine()
{
	delete m_TimeManager;
	delete m_SceneManager;
	delete m_SystemManager;

	m_Graphics->Finalize();
	delete m_Graphics;
	delete m_PhysicEngine;
	InputManager::GetInstance().Release();
}
void VPEngine::Addsystem()
{
	m_SystemManager->AddSystem<SceneSerializer>();
	m_SystemManager->AddSystem<RenderSystem>();
	m_SystemManager->AddSystem<LightSystem>();
	m_SystemManager->AddSystem<CameraSystem>();
	m_SystemManager->AddSystem<PhysicSystem>();
	m_SystemManager->AddSystem<AnimationSystem>();

}
void VPEngine::OnAddSystemLater(std::any)
{
	m_SystemManager->AddSystem<TransformSystem>();
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
			static float tempTime = 0;
			tempTime += m_DeltaTime;
			while (tempTime > (1/90.f))
			{
				m_SystemManager->RenderUpdate(m_DeltaTime);
				Render();
				EndRender();
				tempTime -= (1/90.f);
			}


		}
	}



}




void VPEngine::Update()
{
	m_TimeManager->Update();
	m_DeltaTime = m_TimeManager->GetDeltaTime();
	if (m_DeltaTime > 1)
		m_DeltaTime = 1 / 165;
	EventManager::GetInstance().Update(m_DeltaTime);
	InputManager::GetInstance().Update();

	m_SystemManager->PhysicUpdatable(m_DeltaTime);
	m_SystemManager->FixedUpdate(m_DeltaTime);
	m_SystemManager->Update(m_DeltaTime);
	m_SystemManager->LateUpdate(m_DeltaTime);

	std::wstring newname = std::to_wstring(m_TimeManager->GetFPS());
	SetWindowTextW(m_hWnd, newname.c_str());
	m_Graphics->Update(m_DeltaTime);
}


void VPEngine::Render()
{
	m_Graphics->BeginRender();
	m_Graphics->Render();

}

void VPEngine::EndRender()
{
	m_Graphics->EndRender();
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
	case WM_KEYDOWN:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(wParam), KEY_STATE::DOWN };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}
	case WM_KEYUP:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(wParam), KEY_STATE::UP };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(VK_LBUTTON), KEY_STATE::DOWN };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}
	case WM_LBUTTONUP:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(VK_LBUTTON), KEY_STATE::UP };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(VK_RBUTTON), KEY_STATE::DOWN };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}
	case WM_RBUTTONUP:
	{
		std::pair<int, KEY_STATE> data = { static_cast<int>(VK_RBUTTON), KEY_STATE::UP };
		EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::any(data));
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
