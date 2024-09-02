#include "pch.h"
#include "Defines.h"
#include <wrl/client.h>
#include "RenderTargetView.h"
#include "Texture2D.h"


RenderTargetView::RenderTargetView(const std::shared_ptr<Device>& device, const RenderTargetViewType& type, const uint32_t& width, const uint32_t& height)
{
	OnResize(device, type, width, height);
}

ID3D11RenderTargetView* RenderTargetView::Get() const
{
	return m_RTV.Get();
}

ID3D11RenderTargetView** RenderTargetView::GetAddress()
{
	return m_RTV.GetAddressOf();
}

void RenderTargetView::OnResize(const std::shared_ptr<Device>& device, const RenderTargetViewType& type,
	const uint32_t& width, const uint32_t& height)
{
	Release();

	switch (type)
	{
		case RenderTargetViewType::Default:
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
			const Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = device->SwapChain();

			HR_CHECK(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
			HR_CHECK(device->Get()->CreateRenderTargetView(backBuffer.Get(), 0, &m_RTV));
		}
		break;
		case RenderTargetViewType::OffScreen:
		{
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
			HR_CHECK(device->Get()->CreateTexture2D(&textureDesc, nullptr, &texture));

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = textureDesc.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
			HR_CHECK(device->Get()->CreateRenderTargetView(texture.Get(), &rtvDesc, &m_RTV));

			break;
		}
		case RenderTargetViewType::ObjectMask:
		{
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
			HR_CHECK(device->Get()->CreateTexture2D(&textureDesc, nullptr, &texture));

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = textureDesc.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
			HR_CHECK(device->Get()->CreateRenderTargetView(texture.Get(), &rtvDesc, m_RTV.GetAddressOf()));

			break;
		}
		default:
			break;
	}
}

void RenderTargetView::Release()
{
	const ULONG refCount = m_RTV.Reset();
	assert(refCount == 0);
}
