#include "pch.h"
#include "RenderPass.h"
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

RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger) : m_Device(device), m_ResourceManager(manger), m_SkeletalMeshVS(), m_DebugPS()
{

}

RenderPass::~RenderPass()
{
	m_SkeletalMeshVS.reset();
	m_DebugPS.reset();

	m_RTV.reset();
	m_DSV.reset();

	m_Device.reset();
	m_ResourceManager.reset();
}

void RenderPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

	Device->UnBindSRV();
	Device->Context()->OMSetRenderTargets(1, RTV->GetAddress(), DSV->Get());

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();
		if (curModel != nullptr)
		{
			Device->Context()->RSSetState(curModel->RS.lock()->Get());

			int materialindex = 0; //mesh의 수와 같음

			for (auto& mesh : curModel->m_Meshes)
			{
				Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
				Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
				Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);
				Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

				if (mesh->IsSkinned())
				{
					Device->BindVS(m_SkeletalMeshVS.lock());

					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					renew.local = curMesh->m_node.lock()->m_World;

					position->Update(renew);
					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete;
					if (!curData->curAnimation.empty() && curData->isPlay)
					{
						std::wstring id = std::to_wstring(curData->EntityID);
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(id).lock();
					}
					else
					{
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
					}
					pallete->Update();
					Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());
				}
				else
				{
					BindStatic(curData);
				}

				if (!curModel->m_Materials.empty())
				{
					std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];
					std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();

					MaterialData curMaterialData = curMaterial->m_Data;
					curData->Update(curMaterialData);

					Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
					{
						Device->BindMaterialSRV(curMaterial);
					}
				}

				materialindex++;

				///Draw
				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}

		}
		m_RenderDataQueue.pop();
	}
}

void RenderPass::AddModelData(std::shared_ptr<RenderData> model)
{

	m_RenderDataQueue.push(model);
}

void RenderPass::BindStatic(std::shared_ptr<RenderData> curModel)
{
	std::shared_ptr<Device> Device = m_Device.lock();

	Device->Context()->IASetInputLayout(m_StaticMeshVS.lock()->InputLayout());
	Device->Context()->VSSetShader(m_StaticMeshVS.lock()->GetVS(), nullptr, 0);
	Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

	std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();


	TransformData renew;
	XMStoreFloat4x4(&renew.local, XMMatrixTranspose(curModel->world));
	XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curModel->world));
	position->Update(renew);
}

void RenderPass::BindSkeletal(std::shared_ptr<RenderData> curModel, std::shared_ptr<Mesh> mesh)
{
	std::shared_ptr<Device> Device = m_Device.lock();

	Device->Context()->IASetInputLayout(m_SkeletalMeshVS.lock()->InputLayout());
	Device->Context()->VSSetShader(m_SkeletalMeshVS.lock()->GetVS(), nullptr, 0);
	Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

	std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);
	std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

	TransformData renew;
	XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curModel->world));
	renew.local = curMesh->m_node.lock()->m_World;

	position->Update(renew);

	if (!curMesh->m_BoneData.empty())
	{
		MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);

		std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
		pallete->Update(matrixPallete);
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());
	}
}

FowardPass::FowardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"IMGUI");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main");

	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");

	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"Mesh");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");

	m_state = PassState::Foward;
}

FowardPass::~FowardPass()
{
	m_RTV.reset();
	m_DSV.reset();
}


DebugPass::DebugPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, std::shared_ptr<DebugDrawManager> debug) : RenderPass(device, manager), m_DebugDrawManager(debug)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"GBuffer");
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
	/*
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();


	Device->BindVS(m_StaticMeshVS.lock());
	Device->Context()->PSSetShader(m_DebugPS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderDataQueue.empty())
	{

		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();
		if (curModel != nullptr)
		{


			Device->Context()->RSSetState(curModel->RS.lock()->Get());

			int materialindex = 0; //mesh의 숫자와 동일

			for (auto& mesh : curModel->m_Meshes)
			{
				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
				position->m_struct.world = curData->world;
				position->m_struct.local = curData->local;
				position->Update();

				Device->BindMeshBuffer(mesh);

				m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
				m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());

				if (!curModel->m_Materials.empty())
				{
					static int materialindex = 0;

					std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();

					std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];

					MaterialData curMaterialData = curMaterial->m_Data;
					curData->Update(curMaterialData);

					Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
					Device->BindMaterialSRV(curMaterial);

					if (curModel->m_Materials.size() < materialindex)
					{
						materialindex++;
					}
				}

				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}
		}
		m_RenderDataQueue.pop();
	}
	*/


	/*
	*/
	std::weak_ptr<ConstantBuffer<CameraData>> Camera = resourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	XMStoreFloat4x4(&m_View, XMMatrixTranspose(Camera.lock()->m_struct.view));
	XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(Camera.lock()->m_struct.proj));

	Device->Context()->OMSetRenderTargets(1, RTV->GetAddress(), DSV->Get());
	//m_Device->Context()->OMSetRenderTargets(1, m_FullScreenRTV->GetAddress(), nullptr);


	/*while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();

		switch (curData->Filter)
		{
			default:
				break;
		}
	}*/

#pragma region TEST
	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere.Center = { 5, 5, 0 };
	sphereInfo.Sphere.Radius = 1;
	sphereInfo.Color = SimpleMath::Color{ 1, 0, 0, 1 };
	debugManager->AddTask(sphereInfo);

	debug::AABBInfo boxInfo;
	boxInfo.AABB.Center = { 0, 10, 0 };
	boxInfo.AABB.Extents = { 1.f, 1.f, 1.f };
	boxInfo.Color = SimpleMath::Color{ 0, 0, 1, 1 };
	debugManager->AddTask(boxInfo);

	debug::OBBInfo obbInfo;
	obbInfo.OBB.Center = { 0, 5, 0 };
	obbInfo.OBB.Extents = { 1, 1, 1 };
	obbInfo.yAxisAngle = 45.f;
	obbInfo.Color = SimpleMath::Color{ 0, 1, 0, 1 };
	debugManager->AddTask(obbInfo);

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
	debugManager->AddTask(frustumInfo);

	/*debug::GridInfo gridInfo;
	gridInfo.Origin = SimpleMath::Vector3{ 0, -5, 0 };
	gridInfo.XAsix = SimpleMath::Vector3{ 1, 0, 0 };
	gridInfo.YAsix = SimpleMath::Vector3{ 0, 0, 1 };
	gridInfo.XDivs = 100;
	gridInfo.YDivs = 100;
	gridInfo.GridSize = 1000.f;
	gridInfo.Color = SimpleMath::Color{ 0.5, 0.5, 0.5, 1 };
	debugManager->AddTask(gridInfo);*/

	debug::RingInfo ringInfo;
	ringInfo.Origin = SimpleMath::Vector3{ 0.f, 7.5f, 0.f };
	ringInfo.MajorAxis = SimpleMath::Vector3{ 1, 0, 0 };
	ringInfo.MinorAxis = SimpleMath::Vector3{ 1, 1, 0 };
	ringInfo.Color = SimpleMath::Color{ 1, 0, 1, 1 };
	debugManager->AddTask(ringInfo);

	debug::RayInfo rayInfo;
	rayInfo.Origin = SimpleMath::Vector3{ 0, 5, 0 };
	rayInfo.Direction = SimpleMath::Vector3{ 0, 5, 50 };
	rayInfo.Normalize = false;
	rayInfo.Color = SimpleMath::Color{ 0.5, 0.5, 1.f, 1 };
	debugManager->AddTask(rayInfo);

	debug::QuadInfo quadInfo;
	quadInfo.PointA = SimpleMath::Vector2{ 0.f, 0.f };
	quadInfo.PointB = SimpleMath::Vector2{ 10.f, 0.f };
	quadInfo.PointC = SimpleMath::Vector2{ 10.f, 10.f };
	quadInfo.PointD = SimpleMath::Vector2{ 0.f, 10.f };
	quadInfo.Color = SimpleMath::Color{ 1, 0.5, 0.5f, 1 };
	debugManager->AddTask(quadInfo);

	debug::TriangleInfo triangleInfo;
	triangleInfo.PointA = SimpleMath::Vector2{ 2.f, 2.f };
	triangleInfo.PointB = SimpleMath::Vector2{ 12.f, 2.f };
	triangleInfo.PointC = SimpleMath::Vector2{ 7.f, 12.f };
	triangleInfo.Color = SimpleMath::Color{ 0, 1, 1,1 };
	debugManager->AddTask(triangleInfo);
#pragma endregion TEST

	debugManager->Execute(Device, m_View, m_Proj);

}

DeferredPass::DeferredPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	m_DepthStencilView = manager->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = manager->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = manager->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = manager->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = manager->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRTV = manager->Get<RenderTargetView>(L"Metalic").lock();
	m_RoughnessRTV = manager->Get<RenderTargetView>(L"Roughness").lock();
	m_AORTV = manager->Get<RenderTargetView>(L"AO").lock();
	m_EmissiveRTV = manager->Get<RenderTargetView>(L"Emissive").lock();

	m_StaticMeshVS = manager->Get<VertexShader>(L"Base").lock();
	m_StaticMeshVS = manager->Get<VertexShader>(L"Skinning").lock();
	m_GeometryPS = manager->Get<PixelShader>(L"MeshDeferredGeometry").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_QuadPS = manager->Get<PixelShader>(L"Quad");

	m_Deferred = manager->Get<PixelShader>(L"MeshDeferredLight");

	m_Albedo = manager->Get<ShaderResourceView>(L"Albedo").lock();
	m_Normal = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_Position = manager->Get<ShaderResourceView>(L"Position").lock();
	m_Depth = manager->Get<ShaderResourceView>(L"Depth").lock();
	m_Metalic = manager->Get<ShaderResourceView>(L"Metalic").lock();
	m_Roughness = manager->Get<ShaderResourceView>(L"Roughness").lock();
	m_AO = manager->Get<ShaderResourceView>(L"AO").lock();
	m_Emissive = manager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBuffer = manager->Get<ShaderResourceView>(L"GBuffer").lock();


	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"MeshDeferredGeometry");
}

DeferredPass::~DeferredPass()
{

}

void DeferredPass::Render()
{
	Geometry();
	Light();
}

void DeferredPass::Geometry()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

	// Bind Common Resources
	{
		Device->UnBindSRV();
		std::vector<ID3D11RenderTargetView*> RTVs;
		int GBufferSize = static_cast<int>(Slot_T::End) - 1;
		RTVs.reserve(GBufferSize);

		RTVs.push_back(m_AlbedoRTV.lock()->Get());
		RTVs.push_back(m_NormalRTV.lock()->Get());
		RTVs.push_back(m_PositionRTV.lock()->Get());
		RTVs.push_back(m_DepthRTV.lock()->Get());
		RTVs.push_back(m_MetalicRTV.lock()->Get());
		RTVs.push_back(m_RoughnessRTV.lock()->Get());
		RTVs.push_back(m_AORTV.lock()->Get());
		RTVs.push_back(m_EmissiveRTV.lock()->Get());
		Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView->Get());

		Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);
		Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());


		std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
		std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
		std::shared_ptr<ConstantBuffer<MatrixPallete>>SkeletalCB = m_ResourceManager.lock()->Create<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());

	}

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();

		if (curModel != nullptr)
		{
			int materialindex = 0;

			for (const auto& mesh : curModel->m_Meshes)
			{
				Device->BindMeshBuffer(mesh);

				// Static Mesh Data Update & Bind
				//if (!mesh->IsSkinned())
				if (curData->Filter == MeshFilter::Static)
				{

					Device->BindVS(m_StaticMeshVS.lock());

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.local, XMMatrixTranspose(curData->world));
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					position->Update(renew);	// == Bind
				}

				// Skeletal Mesh Update & Bind
				if (curData->Filter == MeshFilter::Skinning)
				{
					Device->BindVS(m_SkeletalMeshVS.lock());

					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					renew.local = curMesh->m_node.lock()->m_World;

					position->Update(renew);
					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete;
					if (!curData->curAnimation.empty() && curData->isPlay)
					{
						std::wstring id = std::to_wstring(curData->EntityID);
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(id).lock();
					}
					else
					{
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
					}
					pallete->Update();
					Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());
				}

				// 텍스처와 샘플러를 셰이더에 바인딩
				if (!curModel->m_Materials.empty())
				{
					std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
					std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];

					if (curMaterial != nullptr)
					{
						MaterialData curMaterialData = curMaterial->m_Data;
						curData->Update(curMaterialData);

						Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());

						Device->BindMaterialSRV(curMaterial);
					}
				}

				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

				materialindex++;

			}
		}
		m_RenderDataQueue.pop();
	}
	//렌더타겟 해제해줘야지 srv도 해제
	Device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
}

void DeferredPass::Light()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();
	std::shared_ptr<VertexBuffer> vb = m_QuadVB.lock();
	std::shared_ptr<IndexBuffer> ib = m_QuadIB.lock();

	//Save GBuffer texture
	{
		std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();
		std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Deferred").lock();

		Device->BindVS(m_QuadVS.lock());
		Device->Context()->PSSetShader(m_Deferred.lock()->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, m_Albedo.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, m_Normal.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Position), 1, m_Position.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Depth), 1, m_Depth.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, m_Metalic.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Roughness), 1, m_Roughness.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, m_AO.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Emissive), 1, m_Emissive.lock()->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), dsv->Get());
		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}


	//draw quad
	{
		std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"IMGUI").lock();
		std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Main").lock();

		Device->UnBindSRV();
		Device->BindVS(m_QuadVS.lock());
		Device->Context()->PSSetShader(m_QuadPS.lock()->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());

		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::GBuffer), 1, m_GBuffer.lock()->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}
}
