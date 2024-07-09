// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "VPTestProcess.h"
#include "VPMain.h"


#define MAX_LOADSTRING 100
//memory leak
#include <stdlib.h>
#include <crtdbg.h>



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetBreakAlloc(201105);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	VPTestProcess* Process;
	Process = new VPTestProcess(hInstance, "Vision Predator", 1920, 1080);
	Process->Loop();
	delete Process;

	entt::meta_reset();
	_CrtDumpMemoryLeaks();
	
}


