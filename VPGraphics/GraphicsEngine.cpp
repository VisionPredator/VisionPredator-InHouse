#include "pch.h"	
#include "GraphicsEngine.h"

#pragma region DX
#include <d3d11.h>
#include "Device.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderState.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Texture2D.h"

#include "RenderPass.h"
#include "PassManager.h"
#pragma endregion DX

#pragma region Util
#include "Camera.h"
#include "Desc.h"
#include "SimpleMath.h"
#include "VertexData.h"
#include "StaticData.h"
#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#pragma endregion Util

#pragma region Manager
#include "ResourceManager.h"
#include "ModelLoader.h"
#include "Animator.h"
#include "LightManager.h"
#include "DebugDrawManager.h"
//#include "ParticleManager.h"
#include "TimeManager.h"
#pragma endregion Manager

#pragma region IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#pragma endregion IMGUI

#include "Animation.h"
GraphicsEngine::GraphicsEngine(HWND hWnd, TimeManager* timeManager)
	: m_TimeManager(timeManager), m_hWnd(hWnd), m_wndSize(){
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	//srv 생성을 위한 com 초기화
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	GetClientRect(m_hWnd, &m_wndSize);

	m_Device = std::make_shared<Device>(m_hWnd);

	if (m_Device == nullptr)
		return false;

	m_Device->Initialize();

	if (m_Device->SwapChain() == nullptr)
		return false;

	m_ResourceManager = std::make_shared<ResourceManager>(m_Device);
	m_ResourceManager->Initialize();
	m_CurViewPort = m_ResourceManager->Create<ViewPort>(L"Main", m_wndSize).lock();

	m_Loader = std::make_shared <ModelLoader>(m_ResourceManager, m_Device);
	m_Loader->Initialize();
	m_LightManager = std::make_shared<LightManager>(m_ResourceManager);
	m_Animator = std::make_shared <Animator>(m_ResourceManager);
	m_DebugDrawManager = std::make_shared<DebugDrawManager>();
	m_DebugDrawManager->Initialize(m_Device);
	
	m_PassManager = std::make_shared <PassManager>(m_Device, m_ResourceManager,m_DebugDrawManager);
	m_PassManager->Initialize();
	//m_ParticleManager = std::make_shared<ParticleManager>();
	//m_ParticleManager->Initialize(m_Device, m_ResourceManager, m_TimeManager);
	OnResize(m_hWnd);

	InitializeImGui();

	return true;
}

void GraphicsEngine::Update(double dt)
{
	m_Animator->Update(dt, m_RenderList);

	m_PassManager->Update(m_RenderList);

	m_LightManager->Update(m_Lights);
}

bool GraphicsEngine::Finalize()
{
	m_CurViewPort.reset();

	m_Device.reset();
	m_ResourceManager.reset();
	m_Loader.reset();
	m_Animator.reset();
	DestroyImGui();


	return true;
}

void GraphicsEngine::BeginRender()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	const DirectX::SimpleMath::Color white = { 1.f, 1.f, 1.f, 1.f };
	const DirectX::SimpleMath::Color red = { 1.f, 0.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color green = { 0.f, 1.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color blue = { 0.f, 0.f, 1.f, 1.f };
	const DirectX::SimpleMath::Color gray = { 0.5f, 0.5f, 0.5f, 1.f };

	for (int i = 0; i < m_RTVs.size(); i++)
	{
		if (i == 0)
		{
			m_Device->BeginRender(m_RTVs[i].lock()->Get(), m_DSVs[0].lock()->Get(), Black);
		}
		else
		{
			m_Device->BeginRender(m_RTVs[i].lock()->Get(), m_DSVs[1].lock()->Get(), Black);
		}
	}
}

void GraphicsEngine::Render()
{
	m_PassManager->Render();
	BeginImGui();
}

void GraphicsEngine::EndRender()
{
	EndImGui();

	m_Device->EndRender();
}

bool GraphicsEngine::AddRenderModel(MeshFilter mesh, uint32_t EntityID, std::wstring fbx)
{

	std::shared_ptr<RenderData> newData = std::make_shared<RenderData>();
	newData->EntityID = EntityID;
	newData->FBX = fbx;
	newData->Filter = mesh;

	std::wstring id = std::to_wstring(EntityID);
	if (newData->Filter == MeshFilter::Skinning && m_ResourceManager->Get<ConstantBuffer<MatrixPallete>>(id).lock() == nullptr)
	{
		m_ResourceManager->Create<ConstantBuffer<MatrixPallete>>(id, BufferDESC::Constant::DefaultMatrixPallete);
	}

	m_RenderList[EntityID] = newData;

	return true;
}

void GraphicsEngine::EraseObject(uint32_t EntityID)
{
	if (m_RenderList.find(EntityID) != m_RenderList.end())
	{
		m_RenderList.erase(EntityID);
	}
}

void GraphicsEngine::SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_View = view;
	m_Proj = proj;
	m_ViewProj = view * proj;

	DirectX::XMFLOAT4X4 cb_worldviewproj;
	DirectX::XMFLOAT4X4 cb_view;
	DirectX::XMFLOAT4X4 cb_proj;
	DirectX::XMFLOAT4X4 cb_viewInverse;
	DirectX::XMFLOAT4X4 cb_projInverse;
	cb_worldviewproj = m_ViewProj;

	//상수 버퍼는 계산 순서때문에 전치한다
	XMStoreFloat4x4(&cb_worldviewproj, XMMatrixTranspose(m_ViewProj));
	XMStoreFloat4x4(&cb_view, XMMatrixTranspose(m_View));
	XMStoreFloat4x4(&cb_proj, XMMatrixTranspose(m_Proj));

	DirectX::XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
	XMStoreFloat4x4(&cb_viewInverse, XMMatrixTranspose(viewInverse));

	DirectX::XMMATRIX projInverse = XMMatrixInverse(nullptr, proj);
	XMStoreFloat4x4(&cb_projInverse, XMMatrixTranspose(projInverse));

	std::weak_ptr<ConstantBuffer<CameraData>> Camera = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	Camera.lock()->m_struct.view = cb_view;
	Camera.lock()->m_struct.proj = cb_proj;
	Camera.lock()->m_struct.viewInverse = cb_viewInverse;
	Camera.lock()->m_struct.projInverse = cb_projInverse;
	Camera.lock()->m_struct.worldviewproj = cb_worldviewproj;
	Camera.lock()->Update();
}

void GraphicsEngine::UpdateModel(uint32_t EntityID, RenderData& data)
{
	m_RenderList[EntityID]->EntityID = data.EntityID;
	m_RenderList[EntityID]->Filter = data.Filter;
	m_RenderList[EntityID]->Name = std::move(data.Name);
	m_RenderList[EntityID]->FBX = std::move(data.FBX);
	m_RenderList[EntityID]->world = data.world;
	m_RenderList[EntityID]->local = data.local;
	m_RenderList[EntityID]->duration = data.duration;
	m_RenderList[EntityID]->preDuration = data.preDuration;
	m_RenderList[EntityID]->isChange = data.isChange;
	m_RenderList[EntityID]->isPlay = data.isPlay;
	m_RenderList[EntityID]->curAnimation = std::move(data.curAnimation);
	m_RenderList[EntityID]->preAnimation = std::move(data.preAnimation);


	switch (data.Filter)
	{
		case MeshFilter::Axis:
		case MeshFilter::Grid:
		{
			m_RenderList[EntityID]->Pass = PassState::Debug;

		}
		break;

		case MeshFilter::Box:
			m_RenderList[EntityID]->Pass = PassState::GeoMetry;
			break;
		case MeshFilter::Static:
		case MeshFilter::Skinning:
		{
		}
		break;

		case MeshFilter::None:
			break;

		default:
			break;
	}
}

void GraphicsEngine::AddLight(uint32_t EntityID, LightType kind, LightData data)
{
	if (m_Lights.find(EntityID) == m_Lights.end())
	{
		m_Lights.insert(std::pair<uint32_t, LightData>(EntityID, data));
	}
}


void GraphicsEngine::EraseLight(uint32_t EntityID, LightType kind)
{
	if (m_Lights.find(EntityID) != m_Lights.end())
	{
		m_Lights.erase(EntityID);
		EraseObject(EntityID);
		m_LightManager->EraseData(EntityID, kind);
	}
}

void GraphicsEngine::UpdateLightData(uint32_t EntityID, LightType kind, LightData data)
{
	if (m_Lights.find(EntityID) != m_Lights.end())
	{
		m_Lights[EntityID] = data;
	}
}

const double GraphicsEngine::GetDuration(std::wstring name)
{

	std::weak_ptr<ModelData> curAni = m_ResourceManager->Get<ModelData>(name);
	if (curAni.lock() != nullptr)
	{
		return curAni.lock()->m_Animations[0]->m_Duration / curAni.lock()->m_Animations[0]->m_TickFrame;
	}


	return 0;
}

void GraphicsEngine::DrawSphere(const debug::SphereInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawBox(const debug::AABBInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawOBB(const debug::OBBInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawFrustum(const debug::FrustumInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawGrid(const debug::GridInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawRing(const debug::RingInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawTriangle(const debug::TriangleInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawQuad(const debug::QuadInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawRay(const debug::RayInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

ID3D11ShaderResourceView* GraphicsEngine::GetSRV(std::wstring name)
{
	return m_ResourceManager->Get<ShaderResourceView>(name).lock()->Get();
}

void GraphicsEngine::OnResize(HWND hwnd)
{
	m_hWnd = hwnd;
	GetClientRect(m_hWnd, &m_wndSize);

	m_RTVs.clear();
	m_DSVs.clear();

	m_Device->OnResize();
	m_ResourceManager->OnResize(m_wndSize);
	m_CurViewPort = m_ResourceManager->Create<ViewPort>(L"Main", m_wndSize).lock();


	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Albedo"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Normal"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Position"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Depth"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Metalic"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Roughness"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"AO"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Emissive"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"GBuffer"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"IMGUI"));

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Deferred"));


	m_PassManager->OnResize();


	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
}

/// Editor
void GraphicsEngine::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_Device->Get(), m_Device->Context());


}

void GraphicsEngine::BeginImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GraphicsEngine::EndImGui()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GraphicsEngine::DestroyImGui()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}