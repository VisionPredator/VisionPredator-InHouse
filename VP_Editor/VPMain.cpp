// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "VP_Editor.h"
#include "VPMain.h"


#define MAX_LOADSTRING 100
//memory leak
#include <stdlib.h>
#include <crtdbg.h>
#include <DataRegister.h>

/// <summary>
/// 누수 메모!!!
/// 445까지는 못잡겠음..... 일단 new로 인한 누수가 아닌 stl 또는 static에 의한 누수같음...
/// <returns></returns>
int initProject()
{
	//_CrtSetBreakAlloc(441);
	return 0;  // It's good practice to have a return value
}

// Define the Initializer class and create a global instance
class Initializer {
public:
	Initializer() {
		initProject();
	}
};

Initializer global_initializer;

void ConfigureCRTDebug()
{
	// Enable debug heap allocations and memory leak checking
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(dbgFlags);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	// Optionally set a specific allocation number to break on
	 //_CrtSetBreakAlloc(208331);  // Uncomment and set the number if needed
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	ConfigureCRTDebug();
	VPRegister::Register_Metadata();
	VP_Editor* Process;
	Process = new VP_Editor(hInstance, "VisionPredator", 1920, 1080);
	Process->Loop();
	delete Process;
	EventManager::GetInstance().Release();

	entt::meta_reset();
	_CrtDumpMemoryLeaks();
}


