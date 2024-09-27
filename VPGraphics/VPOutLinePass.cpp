#include "pch.h"
#include "VPOutLinePass.h"
#include "Sampler.h"
#include "ResourceManager.h"
#include "StaticData.h"


VPOutLinePass::VPOutLinePass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass()
{
	m_ResourceManager = manager;
	m_Device = device;

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_VPOutLine = manager->Get<PixelShader>(L"Quad");

	m_Normal = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_Depth = manager->Get<ShaderResourceView>(L"Depth").lock();
}

VPOutLinePass::~VPOutLinePass()
{

}

void VPOutLinePass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<VertexBuffer> vb = m_QuadVB.lock();
	std::shared_ptr<IndexBuffer> ib = m_QuadIB.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	Device->UnBindSRV();

	Device->BindVS(m_QuadVS.lock());
	Device->Context()->PSSetShader(m_VPOutLine.lock()->GetPS(), nullptr, 0);

	Device->Context()->PSSetConstantBuffers(0, 1, m_Screen->GetAddress());

	Device->Context()->PSSetShaderResources(0, 1, m_Normal.lock()->GetAddress());
	Device->Context()->PSSetShaderResources(1, 1, m_Depth.lock()->GetAddress());
	
	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();

	Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
	Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
	Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
}

void VPOutLinePass::OnResize()
{
	std::shared_ptr<ResourceManager> manager = m_ResourceManager.lock();

	const uint32_t width = m_Device.lock()->GetWndWidth();
	const uint32_t height = m_Device.lock()->GetWndHeight();

	m_Screen = manager->Get<ConstantBuffer<VPMath::XMFLOAT4>>(L"TexelSize").lock();
	
	m_Screen->m_struct.x = static_cast<float>(width);
	m_Screen->m_struct.y = static_cast<float>(height);
	m_Screen->Update();

	m_Normal = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_Depth = manager->Get<ShaderResourceView>(L"Depth").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_VPOutLine = manager->Get<PixelShader>(L"VPOutLine");
}
