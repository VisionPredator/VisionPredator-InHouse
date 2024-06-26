#include "pch.h"
#include "DebugDrawPass.h"
#include "Device.h"
#include "DebugDrawManager.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

void DebugDrawPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<DebugDrawManager>& debugDrawManager, const std::shared_ptr<ResourceManager>& resourceManager,
	const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj)
{
	m_Device = device;
	m_DebugDrawManager = debugDrawManager;
	m_ResourceManager = resourceManager;

	m_FullScreenRTV = m_ResourceManager->Get<RenderTargetView>(L"IMGUI").lock();
	//m_FullScreenRTV = m_ResourceManager->Get<RenderTargetView>(L"RTV_Main").lock();
	m_DepthStencilView = m_ResourceManager->Get<DepthStencilView>(L"DSV_Main").lock();

	m_View = view;
	m_Proj = proj;
}

void DebugDrawPass::Render()
{
	std::weak_ptr<ConstantBuffer<CameraData>> Camera = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	XMStoreFloat4x4(&m_View, XMMatrixTranspose(Camera.lock()->m_struct.view));
	XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(Camera.lock()->m_struct.proj));

	m_Device->Context()->OMSetRenderTargets(1, m_FullScreenRTV->GetAddress(), m_DepthStencilView->Get());
	//m_Device->Context()->OMSetRenderTargets(1, m_FullScreenRTV->GetAddress(), nullptr);

#pragma region TEST
	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere.Center = { 5, 5, 0 };
	sphereInfo.Sphere.Radius = 1;
	sphereInfo.Color = SimpleMath::Color{ 1, 0, 0, 1 };
	m_DebugDrawManager->AddTask(sphereInfo);

	debug::AABBInfo boxInfo;
	boxInfo.AABB.Center = { 0, 10, 0 };
	boxInfo.AABB.Extents = { 1.f, 1.f, 1.f };
	boxInfo.Color = SimpleMath::Color{ 0, 0, 1, 1 };
	m_DebugDrawManager->AddTask(boxInfo);

	debug::OBBInfo obbInfo;
	obbInfo.OBB.Center = { 0, 5, 0 };
	obbInfo.OBB.Extents = { 1, 1, 1 };
	obbInfo.yAxisAngle = 45.f;
	obbInfo.Color = SimpleMath::Color{ 0, 1, 0, 1 };
	m_DebugDrawManager->AddTask(obbInfo);

	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum.Origin = { 0, 5, 0 };
	frustumInfo.Frustum.Orientation = { 0, 0, 0, 1 };
	frustumInfo.Frustum.Near = 0.03f;
	frustumInfo.Frustum.Far = 10.f;
	frustumInfo.Frustum.LeftSlope = -0.736355f;
	frustumInfo.Frustum.RightSlope = 0.736355f;
	frustumInfo.Frustum.TopSlope = 0.4142f;
	frustumInfo.Frustum.BottomSlope = -0.4142;
	frustumInfo.Color = SimpleMath::Color{ 1, 1, 0, 1 };
	m_DebugDrawManager->AddTask(frustumInfo);

	debug::GridInfo gridInfo;
	gridInfo.Origin = SimpleMath::Vector3{ 0, -5, 0 };
	gridInfo.XAsix = SimpleMath::Vector3{ 1, 0, 0 };
	gridInfo.YAsix = SimpleMath::Vector3{ 0, 0, 1 };
	gridInfo.XDivs = 100;
	gridInfo.YDivs = 100;
	gridInfo.GridSize = 1000.f;
	gridInfo.Color = SimpleMath::Color{ 0.5, 0.5, 0.5, 1 };
	m_DebugDrawManager->AddTask(gridInfo);

	debug::RingInfo ringInfo;
	ringInfo.Origin = SimpleMath::Vector3{ 0.f, 7.5f, 0.f };
	ringInfo.MajorAxis = SimpleMath::Vector3{ 1, 0, 0 };
	ringInfo.MinorAxis = SimpleMath::Vector3{ 1, 1, 0 };
	ringInfo.Color = SimpleMath::Color{ 1, 0, 1, 1 };
	m_DebugDrawManager->AddTask(ringInfo);

	debug::RayInfo rayInfo;
	rayInfo.Origin = SimpleMath::Vector3{ 0, 5, 0 };
	rayInfo.Direction = SimpleMath::Vector3{ 0, 5, 50 };
	rayInfo.Normalize = false;
	rayInfo.Color = SimpleMath::Color{ 0.5, 0.5, 1.f, 1 };
	m_DebugDrawManager->AddTask(rayInfo);

	debug::QuadInfo quadInfo;
	quadInfo.PointA = SimpleMath::Vector2{ 0.f, 0.f };
	quadInfo.PointB = SimpleMath::Vector2{ 10.f, 0.f };
	quadInfo.PointC = SimpleMath::Vector2{ 10.f, 10.f };
	quadInfo.PointD = SimpleMath::Vector2{ 0.f, 10.f };
	quadInfo.Color = SimpleMath::Color{ 1, 0.5, 0.5f, 1 };
	m_DebugDrawManager->AddTask(quadInfo);

	debug::TriangleInfo triangleInfo;
	triangleInfo.PointA = SimpleMath::Vector2{ 2.f, 2.f };
	triangleInfo.PointB = SimpleMath::Vector2{ 12.f, 2.f };
	triangleInfo.PointC = SimpleMath::Vector2{ 7.f, 12.f };
	triangleInfo.Color = SimpleMath::Color{ 0, 1, 1,1 };
	m_DebugDrawManager->AddTask(triangleInfo);
#pragma endregion TEST

	m_DebugDrawManager->Execute(m_Device, m_View, m_Proj);
}
