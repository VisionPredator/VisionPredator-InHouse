#include "pch.h"
#include "VPProcess.h"
#include "SceneManager.h"
#include "Managers.h"
#include "TestCameraSystem.h"
#include "PlayerSystem.h"
VPProcess::VPProcess(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
	Initialize();
}

void VPProcess::Initialize()
{
	AddSystems();
}


void VPProcess::AddSystems()
{
	m_SystemManager->AddSystem<PlayerSystem>();
	m_SystemManager->AddSystem<TestCameraSystem>();
}

VPProcess::~VPProcess()
{
}

void VPProcess::Update()
{
	VPEngine::Update();


}

void VPProcess::Render()
{
	VPEngine::Render();

}

