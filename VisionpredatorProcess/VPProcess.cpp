#include "pch.h"
#include "VPProcess.h"
#include "SceneManager.h"
#include "Managers.h"
#include "GameLogicSystem.h"
#include "DataRegister_Vispred.h"

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
	m_SystemManager->AddSystem<TrunSystem>();
	m_SystemManager->AddSystem<PlayerUISystem>();
	m_SystemManager->AddSystem<PlayerFSMSystem>();
	m_SystemManager->AddSystem<PlayerSystem>();
	m_SystemManager->AddSystem<TestCameraSystem>();
	m_SystemManager->AddSystem<BulletSystem>();
	m_SystemManager->AddSystem<EnemySystem>();
	m_SystemManager->AddSystem<GunSystem>();
	m_SystemManager->AddSystem<AreaAttackSystem>();
	m_SystemManager->AddSystem<InterectiveSystem>();
	m_SystemManager->AddSystem<DoorSystem>();
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
	VPEngine::RenderUpdate();
	VPEngine::BeginRender();
	VPEngine::EndRender();
}		

  