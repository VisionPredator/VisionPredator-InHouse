#include "pch.h"
#include "VPTestProcess.h"
#include "VPEngine.h"
#include "SystemManager.h"
#include "TestSystem.h"

VPTestProcess::VPTestProcess(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
	m_SystemManager->AddSystem<TestSystem>();
}

VPTestProcess::~VPTestProcess()
{

}
