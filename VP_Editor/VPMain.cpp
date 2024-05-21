// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "VP_Editor.h"
#include "VPMain.h"


#define MAX_LOADSTRING 100
//memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/// <summary>
/// 누수 메모!!!
/// 215 216 217 218 357 은 못잡겠음..... 일단 new로 인한 누수가 아닌 stl 또는 static에 의한 누수같음...
/// <returns></returns>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetBreakAlloc(475);
	VP_Editor* Process;
	Process = new VP_Editor(hInstance, "VisionPredator", 1920, 1080);
	Process->Loop();
	delete Process;

	entt::meta_reset();
	_CrtDumpMemoryLeaks();
}


