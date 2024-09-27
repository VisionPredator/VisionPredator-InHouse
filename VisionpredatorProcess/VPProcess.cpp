#include "pch.h"
#include "VPProcess.h"
#include "SceneManager.h"
#include "Managers.h"
#include "GameLogicSystem.h"
#include "DataRegister_Vispred.h"
#include "PlayerUISystem.h"

VPProcess::VPProcess(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
	VispredRegister::Register_Metadata();
	Initialize();
}

void VPProcess::Initialize()
{
	AddSystems();
}


void VPProcess::AddSystems()
{
	m_SystemManager->AddSystem<PlayerUISystem>();
	m_SystemManager->AddSystem<PlayerSystem>();
	m_SystemManager->AddSystem<TestCameraSystem>();
	m_SystemManager->AddSystem<BulletSystem>();
	m_SystemManager->AddSystem<EnemySystem>();
	m_SystemManager->AddSystem<GunSystem>();
}

VPProcess::~VPProcess()
{
}

void VPProcess::Update()
{
	VPEngine::Update();
}

void VPProcess::RenderUpdate()
{
	VPEngine::RenderUpdate();
	VPEngine::BeginRender();
	VPEngine::EndRender();
}		

  