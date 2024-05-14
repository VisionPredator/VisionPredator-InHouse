#include "pch.h"
#include "RenderTargetView.h"
#include "Texture2D.h"


RenderTargetView::RenderTargetView(Device* device) : Resource(device)
{
	ID3D11Texture2D* backBuffer = nullptr;

	HRESULT hr;
	hr = m_Device->SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"SwapChain GetBuffer Failed", 0, 0);
	}

	if (backBuffer != 0)
	{
		hr = m_Device->Get()->CreateRenderTargetView(backBuffer, 0, &m_RTV);
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateRenderTargetView Failed", 0, 0);
	}

	backBuffer->Release();
}

RenderTargetView::RenderTargetView(Device* device, Texture2D* backbuffer) : Resource(device), m_tex(backbuffer)
{
	HRESULT hr;

	hr = m_Device->Get()->CreateRenderTargetView(backbuffer->Get(), 0, &m_RTV);

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateRenderTargetView Failed", 0, 0);
	}
}

RenderTargetView::RenderTargetView(Device* device, Texture2D* backbuffer, D3D11_RENDER_TARGET_VIEW_DESC desc) : Resource(device)
{
	device->Get()->CreateRenderTargetView(backbuffer->Get(), &desc, &m_RTV);
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

Texture2D* RenderTargetView::Texture()
{
	return m_tex;
}
