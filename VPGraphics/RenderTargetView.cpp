#include "pch.h"
#include "Defines.h"
#include <wrl/client.h>
#include "RenderTargetView.h"
#include "Texture2D.h"


RenderTargetView::RenderTargetView(std::shared_ptr<Device> device) : Resource(device)
{
	ID3D11Texture2D* backBuffer = nullptr;

	HRESULT hr;
	hr = m_Device.lock()->SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"SwapChain GetBuffer Failed", 0, 0);
	}

	if (backBuffer != 0)
	{
		hr = m_Device.lock()->Get()->CreateRenderTargetView(backBuffer, 0, &m_RTV);
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateRenderTargetView Failed", 0, 0);
	}

	backBuffer->Release();
}

RenderTargetView::RenderTargetView(std::shared_ptr<Device>device, std::weak_ptr<Texture2D> backbuffer) : Resource(device), m_tex(backbuffer)
{
	HRESULT hr;

	hr = m_Device.lock()->Get()->CreateRenderTargetView(backbuffer.lock()->Get(), 0, &m_RTV);

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateRenderTargetView Failed", 0, 0);
	}
}

RenderTargetView::RenderTargetView(std::shared_ptr<Device> device, std::weak_ptr<Texture2D> backbuffer, D3D11_RENDER_TARGET_VIEW_DESC desc) : Resource(device)
{
	m_Device.lock()->Get()->CreateRenderTargetView(backbuffer.lock()->Get(), &desc, &m_RTV);
}

RenderTargetView::RenderTargetView(std::shared_ptr<Device> device, const RenderTargetViewType& type, const uint32_t& width, const uint32_t& height)
{
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
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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
	default:
		break;
	}
}

RenderTargetView::~RenderTargetView()
{

}

ID3D11RenderTargetView* RenderTargetView::Get() const
{
	return m_RTV;
}

ID3D11RenderTargetView** RenderTargetView::GetAddress()
{
	return &m_RTV;
}

void RenderTargetView::Release()
{
	m_RTV->Release();
}

std::weak_ptr<Texture2D> RenderTargetView::Texture()
{
	return m_tex;
}
