#include "pch.h"
#include "UnorderedAccessView.h"
#include "Defines.h"
#include "RenderTargetView.h"
#include "Texture2D.h"

UnorderedAccessView::UnorderedAccessView(const std::shared_ptr<Device>& device) : Resource(device)
{
	uint32_t width = m_Device.lock()->GetWndWidth();
	uint32_t height = m_Device.lock()->GetWndHeight();

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	HR_CHECK(m_Device.lock()->Get()->CreateTexture2D(&textureDesc, nullptr, &pTexture));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = textureDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	HR_CHECK(m_Device.lock()->Get()->CreateUnorderedAccessView(pTexture.Get(), &uavDesc, m_View.GetAddressOf()));
}

UnorderedAccessView::UnorderedAccessView(const std::shared_ptr<Device>& device, const std::shared_ptr<RenderTargetView>& renderTargetView)
	: Resource(device)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> rtvTexture;
	renderTargetView->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(rtvTexture.GetAddressOf()));

	D3D11_TEXTURE2D_DESC textureDesc = {};
	rtvTexture->GetDesc(&textureDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = textureDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	HR_CHECK(m_Device.lock()->Get()->CreateUnorderedAccessView(rtvTexture.Get(), &uavDesc, m_View.GetAddressOf()));
}

UnorderedAccessView::UnorderedAccessView(const std::shared_ptr<Device>& device, const std::shared_ptr<Texture2D>& texture2D)
	: Resource(device)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture2D->Get()->GetDesc(&textureDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = textureDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	HR_CHECK(m_Device.lock()->Get()->CreateUnorderedAccessView(texture2D->Get(), &uavDesc, m_View.GetAddressOf()));
}
