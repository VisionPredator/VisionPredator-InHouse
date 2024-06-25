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

//#include "DeferredShadingPipeline.h"
#include "ForwardPipeline.h"
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
#pragma endregion Manager

#pragma region IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#pragma endregion IMGUI

GraphicsEngine::GraphicsEngine(HWND hWnd) : m_Device(nullptr), m_CurViewPort(nullptr), m_ResourceManager(nullptr), m_Loader(nullptr), m_Animator(nullptr), m_hWnd(hWnd), m_wndSize()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
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
	m_Animator = std::make_shared <Animator>();
	m_DebugDrawManager = std::make_shared<DebugDrawManager>();
	m_DebugDrawManager->Initialize(m_Device);
	OnResize();

	// Pipeline
	//m_DeferredShadingPipeline = std::make_shared<DeferredShadingPipeline>();
	//m_DeferredShadingPipeline->Initialize(m_Device, m_ResourceManager, m_DebugDrawManager, m_View, m_Proj);

	m_ForwardPipeline = std::make_shared <ForwardPipeline>(m_Device, m_ResourceManager);
	m_ForwardPipeline->Initialize();

	//output
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));

	AddRenderModel(MeshFilter::Axis, 0, L"Axis", L"Axis");
	AddRenderModel(MeshFilter::Grid, 1, L"Grid", L"Grid");
	//AddRenderModel(MeshFilter::Static, 3,L"engine_sizedown_1", L"engine_sizedown_1");

	Dir.direction = DirectX::XMFLOAT3(0.f, -1.f, 1.f);
	Dir.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir.intensity = 1;

	LightData Dir2;
	Dir2.direction = DirectX::XMFLOAT3(0.f, 0.f, -1.f);
	Dir2.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir2.intensity = 10;

	LightData Dir3;
	Dir2.direction = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
	Dir2.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir2.intensity = 10;


	Point.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Point.color = DirectX::XMFLOAT3(1, 0, 0);
	Point.intensity = 1;
	Point.pos = DirectX::XMFLOAT3(0, 3, -2);
	Point.range = 10.f;

	LightData Point2;
	Point2.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Point2.color = DirectX::XMFLOAT3(1, 1, 1);
	Point2.intensity = 1;
	Point2.pos = DirectX::XMFLOAT3(25, 3, -2);
	Point2.range = 100;

	Spot.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Spot.color = DirectX::XMFLOAT3(1, 1, 1);
	Spot.intensity = 100;
	Spot.pos = DirectX::XMFLOAT3(0, 3, -2);
	Spot.range = 100;
	Spot.spot = 8;

	AddLight(999,L"Sun", Kind_of_Light::Direction, Dir);
	//EraseLight(999, L"Sun", Kind_of_Light::Direction);
	//AddLight(L"Sun2", Kind_of_Light::Direction, Dir2);
	//AddLight(L"Sun3", Kind_of_Light::Direction, Dir3);
	//AddLight(L"point", Kind_of_Light::Point, Point);
	//AddLight(L"point2", Kind_of_Light::Point, Point2);
	//AddLight(L"point", Kind_of_Light::Spot, Spot);


	InitializeImGui();
	return true;

}

void GraphicsEngine::Update(double dt)
{
	m_Animator->Update(dt, m_AnimationModel);

	//m_DeferredShadingPipeline->Update(m_RenderList);
	//m_ForwardPipeline->Update(m_RenderList);
	m_ForwardPipeline->Update(m_RenderList);

	m_LightManager->Update(m_LightList);
}

bool GraphicsEngine::Finalize()
{
	m_AnimationModel.clear();
	m_CurViewPort.reset();

	m_Device.reset();
	m_ResourceManager.reset();
	m_Loader.reset();
	m_Animator.reset();

	return true;
}

void GraphicsEngine::BeginRender()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	const DirectX::SimpleMath::Color white = { 1.f, 1.f, 1.f, 1.f };
	const DirectX::SimpleMath::Color red = { 1.f, 0.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color green = { 0.f, 1.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color blue = { 0.f, 0.f, 1.f, 1.f };

	m_Device->BeginRender(m_RTVs[0].lock()->Get(), m_DSVs[0].lock()->Get(), Black);
	m_Device->BeginRender(m_RTVs[1].lock()->Get(), m_DSVs[1].lock()->Get(), white);
	m_Device->BeginRender(m_RTVs[2].lock()->Get(), m_DSVs[1].lock()->Get(), red);
	m_Device->BeginRender(m_RTVs[3].lock()->Get(), m_DSVs[1].lock()->Get(), green);
	m_Device->BeginRender(m_RTVs[4].lock()->Get(), m_DSVs[1].lock()->Get(), blue);
	m_Device->BeginRender(m_RTVs[5].lock()->Get(), m_DSVs[1].lock()->Get(), blue + red);
	m_Device->BeginRender(m_RTVs[6].lock()->Get(), m_DSVs[1].lock()->Get(), green + red);
	m_Device->BeginRender(m_RTVs[7].lock()->Get(), m_DSVs[1].lock()->Get(), Black);
	m_Device->BeginRender(m_RTVs[8].lock()->Get(), m_DSVs[1].lock()->Get(), Black);
	m_Device->BeginRender(m_RTVs[9].lock()->Get(), m_DSVs[1].lock()->Get(), Black);
}

void GraphicsEngine::Render()
{
	// 디퍼드 렌더링 기법을 사용한 파이프라인.
	// 디퍼드 패스 + 포워드 패스.

	//m_DeferredShadingPipeline->Render();

	// 디퍼드 렌더링 기법을 사용하지 않은 파이프라인
	// 오로지 포워드 패스만 존재.
	m_ForwardPipeline->Render();

	BeginImGui();

}

void GraphicsEngine::EndRender()
{
	EndImGui();

	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
	m_Device->EndRender();
}

bool GraphicsEngine::AddRenderModel(MeshFilter mesh, uint32_t EntityID, std::wstring name, std::wstring fbx)
{

	std::shared_ptr<RenderData> newData = std::make_shared<RenderData>();
	newData->Name = name;
	newData->EntityID = EntityID;
	newData->FBX = fbx;
	newData->Filter = mesh;

	switch (mesh)
	{
		case MeshFilter::Axis:
		{
			newData->Pass = PassState::Debug;
			newData->local = DirectX::SimpleMath::Matrix::Identity;
			newData->world = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList[EntityID] = newData;
		}
			break;

		case MeshFilter::Grid:
		{
			newData->Pass = PassState::Debug;
			newData->local = DirectX::SimpleMath::Matrix::Identity;
			newData->world = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList[EntityID] = newData;
		}
			break;

		case MeshFilter::Box:
		{
			newData->Pass = PassState::Debug;
			newData->local = DirectX::SimpleMath::Matrix::Identity;
			newData->world = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList[EntityID] = newData;
		}
			break;

		case MeshFilter::Static:
		{
			newData->Pass = PassState::Deferred;

			newData->FBX = fbx + L".fbx";
			newData->local._11 *= 0.03f;
			newData->local._22 *= 0.03f;
			newData->local._33 *= 0.03f;

			m_RenderList[EntityID] = newData;
		}
			break;

		case MeshFilter::Skinning:
		{
			newData->Pass = PassState::Deferred;

			newData->FBX = fbx + L".fbx";

			newData->local._11 *= 0.05f;
			newData->local._22 *= 0.05f;
			newData->local._33 *= 0.05f;

			newData->world *= newData->local;
			m_RenderList[EntityID] = newData;

			m_AnimationModel.push_back(m_ResourceManager->Get<ModelData>(newData->FBX).lock());

		}
			break;

		case MeshFilter::Circle:
		{	
			newData->Pass = PassState::Debug;

			m_RenderList[EntityID] = newData;
		}
			break;

		case MeshFilter::None:
			break;

		default:
			break;
	}

	return true;
}

void GraphicsEngine::EraseObject(uint32_t EntityID)
{
	if (m_RenderList.find(EntityID) != m_RenderList.end())
	{

		//애니메이션 빼줘야할거같은데?

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
	cb_worldviewproj = m_ViewProj;

	//상수 버퍼는 계산 순서때문에 전치한다
	XMStoreFloat4x4(&cb_worldviewproj, XMMatrixTranspose(m_ViewProj));
	XMStoreFloat4x4(&cb_view, XMMatrixTranspose(m_View));
	XMStoreFloat4x4(&cb_proj, XMMatrixTranspose(m_Proj));

	DirectX::XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
	XMStoreFloat4x4(&cb_viewInverse, XMMatrixTranspose(viewInverse));

	std::weak_ptr<ConstantBuffer<CameraData>> Camera = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	Camera.lock()->m_struct.view = cb_view;
	Camera.lock()->m_struct.viewInverse = cb_viewInverse;
	Camera.lock()->m_struct.worldviewproj = cb_worldviewproj;
	Camera.lock()->Update();
}

void GraphicsEngine::UpdateModel(uint32_t EntityID, std::shared_ptr<RenderData> data)
{
	m_RenderList[EntityID] = data;
}

void GraphicsEngine::AddLight(uint32_t EntityID, std::wstring name, Kind_of_Light kind, LightData data)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData>& curMap = m_LightList[index];

	if (curMap.find(name) == curMap.end())
	{
		curMap.insert(std::pair<std::wstring, LightData>(name, data));
		AddRenderModel(MeshFilter::Circle, EntityID, name,L"Circle");

		curMap[name] = data;

		std::shared_ptr<RenderData> curData = m_RenderList[EntityID];

		//scale
		if (data.range > 0)
		{
			curData->local._11 = data.range;
			curData->local._22 = data.range;
			curData->local._33 = data.range;
		}

		//상수버퍼로 넘겨야하니까 전치해서 보내버리자
		//translate
		curData->local._14 = data.pos.x;
		curData->local._24 = data.pos.y;
		curData->local._34 = data.pos.z;

	}
	else
	{
		MessageBox(0, L"This Light already Exist", 0, 0);
		return;
	}

}


void GraphicsEngine::EraseLight(uint32_t EntityID, std::wstring name, Kind_of_Light kind)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData>& curMap = m_LightList[index];

	if (curMap.find(name) != curMap.end())
	{
		curMap.erase(name);
		EraseObject(EntityID);
	}
	else
	{
		MessageBox(0, L"This Light Is Not Exist", 0, 0);
		return;
	}
}

void GraphicsEngine::UpdateLightData(uint32_t EntityID, std::wstring name, Kind_of_Light kind, LightData data)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData> curMap = m_LightList[index];

	if (curMap.find(name) != curMap.end())
	{
		std::shared_ptr<RenderData> curData = m_RenderList[EntityID];

		curMap[name] = data;
		curData->local._31 = data.pos.x;
		curData->local._32 = data.pos.y;
		curData->local._33 = data.pos.z;

		if (data.range > 0)
		{
			curData->local* data.range;
		}
	}
	else
	{
		MessageBox(0, L"This Light Is Not Exist", 0, 0);
		return;
	}
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

void GraphicsEngine::OnResize()
{
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

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Deferred"));



	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
}



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