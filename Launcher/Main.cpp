// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "VP_Launcher.h"
#include "Main.h"


#define MAX_LOADSTRING 100
//memory leak
#include <stdlib.h>
#include <crtdbg.h>
#include "../VPEngine/DataRegister.h"

/// <summary>
/// 누수 메모!!!
/// 445까지는 못잡겠음..... 일단 new로 인한 누수가 아닌 stl 또는 static에 의한 누수같음...
/// <returns></returns>


void ConfigureCRTDebug()
{

}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	VPRegister::Register_Metadata();
	VP_Launcher* Process;
	Process = new VP_Launcher(hInstance, "VisionPredator", 1920, 1080);
	Process->SetStartScene("../Data/Scene/FireTest.scene");
	Process->Loop();
	delete Process;
	EventManager::GetInstance().Release();

	entt::meta_reset();

}


