#include "pch.h"
#include "ObjectMaskPass.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"
#include "DepthStencilState.h"
#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#include "Slot.h"

void ObjectMaskPass::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	const uint32_t width = m_Device.lock()->GetWndWidth();
	const uint32_t height = m_Device.lock()->GetWndHeight();

	m_ObjectMaskRTV = m_ResourceManager.lock()->Create<RenderTargetView>(L"ObjectMaskRTV", RenderTargetViewType::ObjectMask, width, height).lock();
	m_RimLightMaskRTV = m_ResourceManager.lock()->Create<RenderTargetView>(L"RimLightMaskRTV", RenderTargetViewType::OffScreen, width, height).lock();
	m_DefaultDSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main").lock();
	m_DSS = m_ResourceManager.lock()->Get<DepthStencilState>(L"NoDepthWrites").lock();

	D3D_SHADER_MACRO macro[] =
	{
		{"SKINNING",""}, // 매크로 이름과 값을 설정
		{nullptr, nullptr}    // 배열의 끝을 나타내기 위해 nullptr로 끝낸다.
	};
	m_ObjectMaskSkeletalMeshVS = m_ResourceManager.lock()->Create<VertexShader>(L"ObjectMaskSkeletalMeshVS", L"ObjectMaskVS", "main", macro).lock();
	m_ObjectMaskStaticMeshVS = m_ResourceManager.lock()->Create<VertexShader>(L"ObjectMaskStaticMeshVS", L"ObjectMaskVS", "main").lock();
	m_ObjectMaskPS = m_ResourceManager.lock()->Create<PixelShader>(L"ObjectMask", L"ObjectMask").lock();

	m_MaskColorCB = std::make_shared<ConstantBuffer<MaskColorCB>>(device, ConstantBufferType::Default);
}

void ObjectMaskPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();

	const VPMath::Color clearColor = { 0.f, 0.f, 0.f, 1.f };
	Device->Context()->ClearRenderTargetView(m_ObjectMaskRTV->Get(), clearColor);
	Device->Context()->ClearRenderTargetView(m_RimLightMaskRTV->Get(), clearColor);
	Device->Context()->ClearDepthStencilView(m_DefaultDSV->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	std::vector<ID3D11RenderTargetView*> RTVs;
	int GBufferSize = 2;//최대 8개 밖에 안됨
	RTVs.reserve(GBufferSize);
	RTVs.push_back(m_ObjectMaskRTV->Get());
	RTVs.push_back(m_RimLightMaskRTV->Get());
	Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DefaultDSV->Get());

	Device->Context()->OMSetDepthStencilState(m_DSS->GetState().Get(), 0);

	Device->Context()->RSSetState(nullptr);

	// CB Update
	{
		std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
		std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();
		std::shared_ptr<ConstantBuffer<MatrixPallete>>SkeletalCB = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
		std::shared_ptr<ConstantBuffer<MaterialData>> MaterialCB = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
		std::shared_ptr<ConstantBuffer<LightArray>> light = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());

		/*Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());*/
		Device->Context()->PSSetConstantBuffers(0, 1, m_MaskColorCB->GetAddress());


	}
	
	Device->Context()->PSSetShader(m_ObjectMaskPS->GetShader(), nullptr, 0);

	for (const auto& curData : m_RenderList)
	{
		if (curData->isOverDraw)
		{
			continue;
		}
		
		// 마스킹 색상을 가지고 있는 오브젝트만 마스킹한다.
		if (curData->MaskingColor.A() == 0.f)
			continue;

		if (curData->MaskingColor.R() == 0.f && 
			curData->MaskingColor.G() == 0.f &&
			curData->MaskingColor.B() == 0.f)
			continue;

		MaskColorCB maskColor;
		maskColor.Color = curData->MaskingColor;
		m_MaskColorCB->Update(maskColor);

		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();

		if (curModel != nullptr)
		{
			for (const auto& mesh : curModel->m_Meshes)
			{
				Device->BindMeshBuffer(mesh);

				// Static Mesh Data Update & Bind
				if (!mesh->IsSkinned())
				{
					Device->BindVS(m_ObjectMaskStaticMeshVS);
					Device->Context()->IASetInputLayout(m_ObjectMaskStaticMeshVS->InputLayout());

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.local, XMMatrixTranspose(curData->world));
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					XMStoreFloat4x4(&renew.localInverse, (curData->world.Invert()));//전치 해주지말자 회전의 역행렬은 전치행렬임
					XMStoreFloat4x4(&renew.worldInverse, (curData->world.Invert()));//전치 해주지말자 회전의 역행렬은 전치행렬임
					position->Update(renew);	// == Bind
				}
				else
				{
					Device->BindVS(m_ObjectMaskSkeletalMeshVS);
					Device->Context()->IASetInputLayout(m_ObjectMaskSkeletalMeshVS->InputLayout());

					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));

					if (curMesh->m_node.lock() != nullptr)
					{
						renew.local = curMesh->m_node.lock()->m_World;
					}
					else
					{
						renew.local = VPMath::Matrix::Identity;
					}

					XMStoreFloat4x4(&renew.localInverse, (renew.local.Invert()));
					XMStoreFloat4x4(&renew.worldInverse, (renew.world.Invert()));

					position->Update(renew);
					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete;
					if (!curData->FBX.empty() && curData->isPlay)
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

				if (!curModel->m_Materials.empty())
				{
					std::shared_ptr<ConstantBuffer<MaterialData>> curMaterialData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
					std::shared_ptr<Material> curMaterial = curModel->m_Materials[mesh->m_material];

					if (curMaterial != nullptr)
					{
						MaterialData data = curMaterial->m_Data;
						curMaterialData->Update(data);

						Device->Context()->PSSetShaderResources(0, 1, curMaterial->m_NormalSRV.lock()->GetAddress());
						std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

						Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());

						Device->BindMaterialSRV(curMaterial);
					}
				}

				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}
		}
	}

	Device->UnBindSRV();
	Device->Context()->OMSetDepthStencilState(nullptr, 0);
}

void ObjectMaskPass::OnResize()
{
	m_ObjectMaskRTV->OnResize();

	m_RimLightMaskRTV->OnResize();
	m_ResourceManager.lock()->Erase<ShaderResourceView>(L"RimLightMaskSRV");
	m_RimLightSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(L"RimLightMaskSRV", m_RimLightMaskRTV).lock();

	m_DefaultDSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main").lock();
}
