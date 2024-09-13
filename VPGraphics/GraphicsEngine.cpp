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
#include "ParticleManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#pragma endregion Manager

#pragma region IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#pragma endregion IMGUI

#include <memory>

#include "Animation.h"
GraphicsEngine::GraphicsEngine(HWND hWnd, TimeManager* timeManager)
	: m_TimeManager(timeManager)
	, m_hWnd(hWnd)
	, m_wndSize()
	, m_Device(std::make_shared<Device>())
	, m_ResourceManager(std::make_shared<ResourceManager>())
	, m_Loader(std::make_shared <ModelLoader>())
	, m_LightManager(std::make_shared<LightManager>())
	, m_Animator(std::make_shared <Animator>())
	, m_DebugDrawManager(std::make_shared<DebugDrawManager>())
	, m_ParticleManager(std::make_shared<ParticleManager>())
	, m_UIManager(std::make_shared<UIManager>())
	, m_PassManager(std::make_shared <PassManager>())
{
}

bool GraphicsEngine::Initialize()
{
	//srv 생성을 위한 com 초기화
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	GetClientRect(m_hWnd, &m_wndSize);

	m_Device->Initialize(m_hWnd);
	m_ResourceManager->Initialize(m_Device);
	m_Loader->Initialize(m_ResourceManager, m_Device);
	m_LightManager->Initialize(m_ResourceManager);
	m_Animator->Initialize(m_ResourceManager);
	m_DebugDrawManager->Initialize(m_Device, m_ResourceManager);
	m_ParticleManager->Initialize(m_Device, m_ResourceManager, m_TimeManager);
	m_UIManager->Initialize(m_Device, m_ResourceManager);
	m_PassManager->Initialize(m_Device, m_ResourceManager, m_DebugDrawManager, m_ParticleManager, m_UIManager, m_LightManager);

	m_CurViewPort = m_ResourceManager->Create<ViewPort>(L"Main", m_wndSize).lock();

	OnResize(m_hWnd);

	InitializeImGui();

	return true;
}

void GraphicsEngine::Update(double dt)
{
	/*
	m_Animator->Update(dt, m_RenderList);
	m_PassManager->Update(m_RenderList);
	m_LightManager->Update(m_Lights);
	*/
	Culling();
	m_Animator->Update(dt, m_AfterCulling);
	m_PassManager->Update(m_AfterCulling, m_RenderVector);
	m_LightManager->Update(m_Lights);

	m_RenderVector.clear();
	m_AfterCulling.clear();
}

bool GraphicsEngine::Finalize()
{
	DestroyImGui();
	m_CurViewPort.reset();
	m_Device.reset();
	m_ResourceManager.reset();
	m_UIManager.reset();
	m_Loader.reset();
	m_Animator.reset();
	m_PassManager.reset();
	m_ParticleManager.reset();
	m_DebugDrawManager.reset();


	return true;
}

void GraphicsEngine::BeginRender()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	const VPMath::Color white = { 1.f, 1.f, 1.f, 1.f };
	const VPMath::Color red = { 1.f, 0.f, 0.f, 1.f };
	const VPMath::Color green = { 0.f, 1.f, 0.f, 1.f };
	const VPMath::Color blue = { 0.f, 0.f, 1.f, 1.f };
	const VPMath::Color gray = { 0.5f, 0.5f, 0.5f, 1.f };

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

bool GraphicsEngine::AddRenderModel(std::shared_ptr<RenderData> data)
{
	if (m_RenderList.find(data->EntityID) != m_RenderList.end())
	{
		m_RenderList[data->EntityID] = data;
	}
	else
	{
		if (data->isSkinned)
		{
			std::wstring id = std::to_wstring(data->EntityID);

			if (m_ResourceManager->Get<ConstantBuffer<MatrixPallete>>(id).lock() == nullptr)
			{
				m_ResourceManager->Create<ConstantBuffer<MatrixPallete>>(id, ConstantBufferType::Default);
			}
		}

		m_RenderList[data->EntityID] = data;
	}

	return true;
}

void GraphicsEngine::EraseObject(uint32_t EntityID)
{
	if (m_RenderList.find(EntityID) != m_RenderList.end())
	{
		m_RenderList.erase(EntityID);
	}
}

void GraphicsEngine::SetCamera(VPMath::Matrix view, VPMath::Matrix proj, const VPMath::Matrix& orthoProj)
{
	m_View = view;
	m_Proj = proj;
	m_ViewProj = view * proj;

	VPMath::Matrix cb_worldviewproj;
	VPMath::Matrix cb_view;
	VPMath::Matrix cb_proj;
	VPMath::Matrix cb_viewInverse;
	VPMath::Matrix cb_projInverse;
	cb_worldviewproj = m_ViewProj;

	VPMath::Matrix viewInverse = view.Invert();
	//상수 버퍼는 계산 순서때문에 전치한다
	cb_worldviewproj = m_ViewProj.Transpose();
	cb_view = m_View.Transpose();
	cb_proj = m_Proj.Transpose();

	cb_viewInverse = viewInverse.Transpose();
	VPMath::Matrix projInverse = proj.Invert();
	cb_projInverse = projInverse.Transpose();

	//절두체
	DirectX::BoundingFrustum::CreateFromMatrix(m_Frustum, m_Proj);



	//회전이 왜 반대로 먹음..? -> view 자체가 카메라의 기준의 세상을 표현한 행렬
	//우리가 frustum을 구성하려면 카메라 자체의 위치와 회전 값이 필요함
	//view == camera invert , 우린 camera 자체가 필요함 즉 view invert를 써야함


	m_Frustum.Orientation = VPMath::Quaternion::CreateFromRotationMatrix(viewInverse);

	//카메라위치
	m_Frustum.Origin = { viewInverse._41,viewInverse._42,viewInverse._43 };

	std::weak_ptr<ConstantBuffer<CameraData>> Camera = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	Camera.lock()->m_struct.view = cb_view;
	Camera.lock()->m_struct.proj = cb_proj;
	Camera.lock()->m_struct.viewInverse = cb_viewInverse;
	Camera.lock()->m_struct.projInverse = cb_projInverse;
	Camera.lock()->m_struct.worldviewproj = cb_worldviewproj;
	Camera.lock()->m_struct.orthoProj = orthoProj;
	Camera.lock()->Update();
}

void GraphicsEngine::testCulling(VPMath::Matrix view, VPMath::Matrix proj)
{
	VPMath::Matrix viewInverse = view.Invert();

	//절두체
	DirectX::BoundingFrustum::CreateFromMatrix(m_Frustum, m_Proj);

	m_Frustum.Orientation = VPMath::Quaternion::CreateFromRotationMatrix(viewInverse);

	//카메라위치
	m_Frustum.Origin = { viewInverse._41,viewInverse._42,viewInverse._43 };

}

void GraphicsEngine::UpdateModel(uint32_t EntityID)
{

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

const double GraphicsEngine::GetDuration(std::wstring name, int index)
{

	std::shared_ptr<ModelData> curAni = m_ResourceManager->Get<ModelData>(name).lock();
	if (curAni != nullptr)
	{
		if (!curAni->m_Animations.empty())
		{
			//전체 tick / 초당 틱 == 애니메이션 재생시간
			return curAni->m_Animations[index]->m_Duration / curAni->m_Animations[index]->m_TickFrame;
		}
	}


	return 0;
}

void GraphicsEngine::CreateParticleObject(uint32_t entityID, const effect::ParticleInfo& info)
{
	m_ParticleManager->CreateParticleObject(entityID, info);
}

void GraphicsEngine::UpdateParticleObject(uint32_t entityID, const effect::ParticleInfo& info)
{
	m_ParticleManager->UpdateParticleInfoByID(entityID, info);
}

void GraphicsEngine::DeleteParticleObjectByID(uint32_t id)
{
	m_ParticleManager->DeleteParticleObjectByID(id);
}

void GraphicsEngine::CreateImageObject(uint32_t id, const ui::ImageInfo& info)
{
	m_UIManager->CreateImageObject(id, info);
}

void GraphicsEngine::UpdateImageObject(uint32_t id, const ui::ImageInfo& info)
{
	m_UIManager->UpdateImageObject(id, info);
}

void GraphicsEngine::DeleteImageObject(uint32_t id)
{
	m_UIManager->DeleteImageObject(id);
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
	std::shared_ptr<ShaderResourceView> srv = m_ResourceManager->Get<ShaderResourceView>(name).lock();
	if (srv != nullptr)
	{
		return srv->Get();
	}

	return nullptr;
}

std::vector<VPMath::Vector3> GraphicsEngine::GetVertices(std::string fbx)
{
	std::wstring tempfbx{};
	tempfbx.assign(fbx.begin(), fbx.end());
	std::weak_ptr<ModelData> curFBX = m_ResourceManager->Get<ModelData>(tempfbx);

	if (curFBX.lock() != nullptr)
	{
		return curFBX.lock()->vertices;
	}

	return {};
}

void GraphicsEngine::OnResize(HWND hwnd)
{
	/// TODO: 매니저들의 OnResize 함수 불러오기. 실제로 Resize 되도록 고치기.

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
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Metalic_Roughness"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"LightMap"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"AO"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Emissive"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"GBuffer"));

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Deferred"));
	/*
	*/


	m_PassManager->OnResize();


	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
}

void GraphicsEngine::Culling()
{
	for (auto& object : m_RenderList)
	{
		std::wstring fbx = object.second->FBX;
		std::shared_ptr<ModelData> curFBX = m_ResourceManager->Get<ModelData>(fbx).lock();

		if (curFBX != nullptr)
		{
			{
				VPMath::Vector3 s;
				VPMath::Quaternion r;
				VPMath::Vector3 t;
				object.second->world.Decompose(s, r, t);

				VPMath::Matrix rot = VPMath::Matrix::CreateFromQuaternion(r);
				VPMath::Matrix scale = VPMath::Matrix::CreateScale(s);

				VPMath::Vector3 Max;

				VPMath::Vector3 Min;

				for (auto& mesh : curFBX->m_Meshes)
				{
					//S
					VPMath::Vector3 afterMax = mesh->MaxBounding * s;
					VPMath::Vector3 afterMin = mesh->MinBounding * s;

					VPMath::Vector3 distance = afterMax - afterMin;
					VPMath::Vector3 half = distance / 2;

					DirectX::BoundingOrientedBox obbInfo;

					obbInfo.Center = t + afterMin + half;
					obbInfo.Extents = half;
					obbInfo.Orientation = r;

					DirectX::ContainmentType a = m_Frustum.Contains(obbInfo);
					object.second->isVisible = m_Frustum.Contains(obbInfo);
				}
			}
		}

		if (object.second->isVisible
			|| object.second->Pass == (object.second->Pass & PassState::Debug_Geometry))
		{
			m_AfterCulling.insert(object);

			//m_RenderQueue.push(object.second);

			m_RenderVector.push_back(object.second);
		}
	}
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