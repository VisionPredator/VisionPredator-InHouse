#include "../VPEngine/framework.h"
#include "GameProcessor.h"


#define MAX_LOADSTRING 100
//memory leak
#include <stdlib.h>
#include <crtdbg.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetBreakAlloc(475);
	GameProcessor* Process;
	Process = new GameProcessor(hInstance, "Vision Predator", 1920, 1080);
	Process->Loop();
	delete Process;

	entt::meta_reset();
	_CrtDumpMemoryLeaks();
}
