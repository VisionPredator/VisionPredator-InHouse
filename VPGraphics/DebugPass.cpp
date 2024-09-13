#include "pch.h"
#include "DebugPass.h"
#include "ResourceManager.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"

#include "Slot.h"
#include "StaticData.h"
#include "DebugDrawManager.h"

DebugPass::DebugPass(const std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager> manager, std::shared_ptr<DebugDrawManager> debug)
{
	m_Device = device;
	m_ResourceManager = manager;
	m_DebugDrawManager = debug;

	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Emissive");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_state = PassState::Debug;

	m_View = VPMath::Matrix::Identity;
	m_Proj = VPMath::Matrix::Identity;
}

DebugPass::~DebugPass()
{

}

void DebugPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DebugDrawManager>& debugDrawManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_DebugDrawManager = debugDrawManager;

	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Emissive");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_state = PassState::Debug;

	m_View = VPMath::Matrix::Identity;
	m_Proj = VPMath::Matrix::Identity;
}

void DebugPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<ResourceManager> resourceManager = m_ResourceManager.lock();
	std::shared_ptr<DebugDrawManager> debugManager = m_DebugDrawManager.lock();

	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
	std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();
	std::shared_ptr<ConstantBuffer<MatrixPallete>>SkeletalCB = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
	std::shared_ptr<ConstantBuffer<MaterialData>> MaterialCB = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
	std::shared_ptr<ConstantBuffer<LightArray>> light = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();

	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());

	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());

	XMStoreFloat4x4(&m_View, XMMatrixTranspose(CameraCB->m_struct.view));
	XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(CameraCB->m_struct.proj));
	Device->Context()->OMSetRenderTargets(1, m_RTV.lock()->GetAddress(), DSV->Get());

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();

		switch (curData->Filter)
		{
			case GeoMetryFilter::Grid:
			{
				debug::GridInfo gridInfo;
				gridInfo.Origin = VPMath::Vector3{ 0, 0, 0 };
				gridInfo.XAsix = VPMath::Vector3{ 1, 0, 0 };
				gridInfo.YAsix = VPMath::Vector3{ 0, 0, 1 };
				gridInfo.XDivs = 200;
				gridInfo.YDivs = 200;
				gridInfo.GridSize = 200.f;
				gridInfo.Color = VPMath::Color{ 1,1,1, 1 };
				debugManager->AddTask(gridInfo);
			}
			break;

			case GeoMetryFilter::Axis:
			{
				float distance = 10;

				//x
				debug::RayInfo x;
				x.Origin = VPMath::Vector3{ 0, 0, 0 };
				x.Direction = VPMath::Vector3{ distance, 0, 0 };
				x.Normalize = false;
				x.Color = VPMath::Color{ 1, 0, 0, 1 };
				debugManager->AddTask(x);

				//y
				debug::RayInfo y;
				y.Origin = VPMath::Vector3{ 0, 0, 0 };
				y.Direction = VPMath::Vector3{ 0, distance, 0 };
				y.Normalize = false;
				y.Color = VPMath::Color{ 0, 1, 0, 1 };
				debugManager->AddTask(y);

				//z
				debug::RayInfo z;
				z.Origin = VPMath::Vector3{ 0, 0, 0 };
				z.Direction = VPMath::Vector3{ 0, 0, distance };
				z.Normalize = false;
				z.Color = VPMath::Color{ 0, 0, 1, 1 };
				debugManager->AddTask(z);

			}
			break;

			case GeoMetryFilter::Frustum:
			{

				debug::FrustumInfo frustumInfo;

				frustumInfo.Frustum.Origin = { curData->world._41,curData->world._42,curData->world._43 };
				frustumInfo.Frustum.Orientation = { 0, 0, 0, 1 };
				frustumInfo.Frustum.Near = 1.f;
				frustumInfo.Frustum.Far = 1000.f;
				frustumInfo.Frustum.LeftSlope = -0.736355f;
				frustumInfo.Frustum.RightSlope = 0.736355f;
				frustumInfo.Frustum.TopSlope = 0.4142f;
				frustumInfo.Frustum.BottomSlope = -0.4142;
				frustumInfo.Color = VPMath::Color{ 1, 1, 0, 1 };

				debugManager->AddTask(frustumInfo);

			}
			break;

			default:
				break;
		}

		//bounding box
		{
			std::shared_ptr<ModelData> curFBX = resourceManager->Get<ModelData>(curData->FBX).lock();
			if (curFBX != nullptr)
			{

				VPMath::Vector3 s;
				VPMath::Quaternion r;
				VPMath::Vector3 t;
				curData->world.Decompose(s, r, t);

				VPMath::Matrix rot = VPMath::Matrix::CreateFromQuaternion(r);
				VPMath::Matrix scale = VPMath::Matrix::CreateScale(s);

				for (auto& mesh : curFBX->m_Meshes)
				{
					//S
					VPMath::Vector3 afterMax = mesh->MaxBounding * s;
					VPMath::Vector3 afterMin = mesh->MinBounding * s;

					VPMath::Vector3 distance = afterMax - afterMin;
					VPMath::Vector3 half = distance / 2;



					debug::OBBInfo obbInfo;
					obbInfo.OBB.Center = t + afterMin + half;
					obbInfo.OBB.Extents = half;
					obbInfo.xAxisAngle = curData->rotation.x;
					obbInfo.yAxisAngle = curData->rotation.y;
					obbInfo.zAxisAngle = curData->rotation.z;

					obbInfo.Color = { 1,0,0,1 };
					debugManager->AddTask(obbInfo);

				}

			}

		}


		m_RenderDataQueue.pop();
	}

	debugManager->Execute(Device, m_View, m_Proj);

}

void DebugPass::OnResize()
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Emissive");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
}
