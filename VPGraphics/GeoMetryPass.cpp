#include "pch.h"
#include "GeoMetryPass.h"
#include "ResourceManager.h"
#include "StaticData.h"
#include "Slot.h"

GeoMetryPass::GeoMetryPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger)
{
	m_Device = device;
	m_ResourceManager = manger;
}

GeoMetryPass::~GeoMetryPass()
{

}

void GeoMetryPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	Device->UnBindSRV();
	Device->Context()->OMSetRenderTargets(1, RTV->GetAddress(), DSV->Get());

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curOb = m_RenderDataQueue.front().lock();

		if (curOb->Filter == GeoMetryFilter::Box)
		{

			std::shared_ptr<VertexBuffer> vb = m_ResourceManager.lock()->Get<VertexBuffer>(L"TextureBox_VB").lock();
			std::shared_ptr<IndexBuffer> ib = m_ResourceManager.lock()->Get<IndexBuffer>(L"TextureBox_IB").lock();

			if (curOb->useTexture)
			{

				std::shared_ptr<ShaderResourceView> srv = m_ResourceManager.lock()->Get<ShaderResourceView>(curOb->textureName).lock();
				if (srv == nullptr)
				{
					srv = m_ResourceManager.lock()->Get<ShaderResourceView>(L"base.png").lock();
				}

				Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, srv->GetAddress());
			}


			std::shared_ptr<ConstantBuffer<CameraData>> camera = m_ResourceManager.lock()->Create<ConstantBuffer<CameraData>>(L"Camera", ConstantBufferType::Default).lock();

			Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, camera->GetAddress());

			std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform", ConstantBufferType::Default).lock();
			TransformData renew;
			XMStoreFloat4x4(&renew.local, XMMatrixTranspose(curOb->world));
			XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curOb->world));
			XMStoreFloat4x4(&renew.localInverse, (curOb->world.Invert().Transpose()));
			XMStoreFloat4x4(&renew.worldInverse, (curOb->world.Invert().Transpose()));
			position->Update(renew);

			Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
			Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());

			std::shared_ptr<RenderState> rs = m_ResourceManager.lock()->Get<RenderState>(L"Solid").lock();

			Device->Context()->RSSetState(rs->Get());
			Device->Context()->IASetInputLayout(m_StaticMeshVS.lock()->InputLayout());
			Device->Context()->VSSetShader(m_StaticMeshVS.lock()->GetVS(), nullptr, 0);

			Device->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
			Device->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);
			Device->Context()->IASetPrimitiveTopology(TextureBox::PRIMITIVE_TOPOLOGY);
			Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);


			std::shared_ptr<ConstantBuffer<DirectX::XMFLOAT4>> color = m_ResourceManager.lock()->Create<ConstantBuffer<DirectX::XMFLOAT4>>(L"Color", ConstantBufferType::Default).lock();
			Device->Context()->PSSetConstantBuffers(2, 1, color->GetAddress());
			color->Update(curOb->color);

			Device->Context()->DrawIndexed(TextureBox::Index::count, 0, 0);

		}

		m_RenderDataQueue.pop();
	}
}

void GeoMetryPass::OnResize()
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Emissive");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred");

	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"BasePS");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");

}
