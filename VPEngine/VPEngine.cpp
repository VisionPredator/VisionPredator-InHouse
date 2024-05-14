#include "pch.h"
#include "VPEngine.h"
#include "TimeManager.h"
#include "SystemManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "TransformSystem.h"
#include <fcntl.h>
#include <io.h>
#include "SceneSerializer.h"
#include "CameraSystem.h"
#include "MeshRenderSystem.h"



#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif
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

		m_TimeManager = new TimeManager;
		m_EntityManager = new EntityManager;
		m_SystemManager = new SystemManager;
		m_EntityManager->Initalize();
		m_SystemManager->Initialize(m_EntityManager);

		InputManager::GetInstance().Initialize();
		/// 다 초기화 되고 윈도우 만들기
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
		m_SystemManager->AddSystem<TransformSystem>();
		m_SystemManager->AddSystem<SceneSerializer>();
		m_SystemManager->AddSystem<CameraSystem>();
		m_SystemManager->AddSystem<MeshRenderSystem>();
	}

	VPEngine::~VPEngine()
	{
		delete m_TimeManager;
		delete m_EntityManager;
		delete m_SystemManager;
		InputManager::GetInstance().Release();
		EventManager::GetInstance().Release();
	}

	void VPEngine::Loop()
	{
		MSG msg;

		// 기본 메시지 루프입니다:
		while (true)
		{

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			else
			{
				Update();
				//Render();

			}
		}



	}


	void VPEngine::Update()
	{
		EventManager::GetInstance().Update(m_DeltaTime);
		InputManager::GetInstance().Update();
		m_TimeManager->Update();
		m_DeltaTime = m_TimeManager->GetDeltaTime();
		//if (m_DeltaTime > 1)
		//	m_DeltaTime = 1/165;
		m_SystemManager->Update(m_DeltaTime);
		m_SystemManager->FixedUpdate(m_DeltaTime);

		std::wstring newname = std::to_wstring(m_TimeManager->GetFPS());
		SetWindowTextW(m_hWnd, newname.c_str());
	}

	void VPEngine::Render()
	{

	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
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

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
