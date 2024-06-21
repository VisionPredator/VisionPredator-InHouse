#include "pch.h"
#include "DebugDrawPass.h"
#include "Device.h"
#include "DebugDrawManager.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

void DebugDrawPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<DebugDrawManager>& debugDrawManager, const std::shared_ptr<ResourceManager>& resourceManager,
	const std::shared_ptr<Camera>& camera)
{
	m_Device = device;
	m_DebugDrawManager = debugDrawManager;
	m_ResourceManager = resourceManager;
	m_Camera = camera;

	m_FullScreenRTV = m_ResourceManager->Get<RenderTargetView>(L"RTV_Main").lock();
	m_DepthStencilView = m_ResourceManager->Get<DepthStencilView>(L"DSV_Main").lock();
}

void DebugDrawPass::Render()
{
	m_Device->Context()->OMSetRenderTargets(1, m_FullScreenRTV->GetAddress(), m_DepthStencilView->Get());

#pragma region TEST
	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere.Center = { 0, 0, 0 };
	sphereInfo.Sphere.Radius = 5;
	sphereInfo.Color = SimpleMath::Color{ 1, 1, 1, 1 };
	m_DebugDrawManager->AddTask(sphereInfo);

	sphereInfo.Sphere.Center = { 5, 5, 5 };
	sphereInfo.Sphere.Radius = 2;
	sphereInfo.Color = SimpleMath::Color{ 1, 0, 0, 1 };
	m_DebugDrawManager->AddTask(sphereInfo);

	sphereInfo.Sphere.Center = { -5, -5, -5 };
	sphereInfo.Sphere.Radius = 3;
	sphereInfo.Color = SimpleMath::Color{ 0, 1, 0, 1 };
	m_DebugDrawManager->AddTask(sphereInfo);

	sphereInfo.Sphere.Center = { 3, 0, 1 };
	sphereInfo.Sphere.Radius = 4;
	sphereInfo.Color = SimpleMath::Color{ 0, 0, 1, 1 };
	m_DebugDrawManager->AddTask(sphereInfo);
#pragma endregion TEST

	m_DebugDrawManager->Execute(m_Device, m_Camera);
}
