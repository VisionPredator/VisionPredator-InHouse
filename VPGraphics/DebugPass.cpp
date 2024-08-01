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

DebugPass::DebugPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, std::shared_ptr<DebugDrawManager> debug) : RenderPass(device, manager), m_DebugDrawManager(debug)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Emissive");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_state = PassState::Debug;

	m_View = DirectX::SimpleMath::Matrix::Identity;
	m_Proj = DirectX::SimpleMath::Matrix::Identity;
}

DebugPass::~DebugPass()
{

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
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());

	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());

	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());

	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());

	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());

	XMStoreFloat4x4(&m_View, XMMatrixTranspose(CameraCB->m_struct.view));
	XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(CameraCB->m_struct.proj));
	Device->Context()->OMSetRenderTargets(1, m_RTV.lock()->GetAddress(), DSV->Get());

	while (!m_RenderDataQueue.empty())
	{

		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();

		switch (curData->Filter)
		{
			case MeshFilter::Grid:
			{
				debug::GridInfo gridInfo;
				gridInfo.Origin = SimpleMath::Vector3{ 0, 0, 0 };
				gridInfo.XAsix = SimpleMath::Vector3{ 1, 0, 0 };
				gridInfo.YAsix = SimpleMath::Vector3{ 0, 0, 1 };
				gridInfo.XDivs = 200;
				gridInfo.YDivs = 200;
				gridInfo.GridSize = 200.f;
				gridInfo.Color = SimpleMath::Color{ 1,1,1, 1 };
				debugManager->AddTask(gridInfo);
			}
				break;

			case MeshFilter::Axis:
			{
				float distance = 10;

				//x
				debug::RayInfo x;
				x.Origin = SimpleMath::Vector3{ 0, 0, 0 };
				x.Direction = SimpleMath::Vector3{ distance, 0, 0 };
				x.Normalize = false;
				x.Color = SimpleMath::Color{ 1, 0, 0, 1 };
				debugManager->AddTask(x);

				//y
				debug::RayInfo y;
				y.Origin = SimpleMath::Vector3{ 0, 0, 0 };
				y.Direction = SimpleMath::Vector3{ 0, distance, 0 };
				y.Normalize = false;
				y.Color = SimpleMath::Color{ 0, 1, 0, 1 };
				debugManager->AddTask(y);

				//z
				debug::RayInfo z;
				z.Origin = SimpleMath::Vector3{ 0, 0, 0 };
				z.Direction = SimpleMath::Vector3{ 0, 0, distance };
				z.Normalize = false;
				z.Color = SimpleMath::Color{ 0, 0, 1, 1 };
				debugManager->AddTask(z);

			}
				break;

			default:
				break;
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
