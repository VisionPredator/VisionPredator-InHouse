// DemoApp.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "pch.h"
#include "framework.h"
#include "VPTestProcess.h"
#include "VPMain.h"


#define MAX_LOADSTRING 100
//memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/// <summary>
/// ���� �޸�!!!
/// 215 216 217 218 357 �� �������..... �ϴ� new�� ���� ������ �ƴ� stl �Ǵ� static�� ���� ��������...
/// <returns></returns>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	VPTestProcess* Process;
	Process = new VPTestProcess(hInstance, "VisionPredator", 1920, 1080);
	Process->Loop();
	delete Process;

	entt::meta_reset();

	_CrtDumpMemoryLeaks();
}


