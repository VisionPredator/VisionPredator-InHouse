#include "pch.h"	

#include "DepthStencilView.h"
#include "Texture2D.h"
#include "Desc.h"

DepthStencilView::DepthStencilView(Device* device, D3D11_TEXTURE2D_DESC desc) : Resource(device)
{
	D3D11_TEXTURE2D_DESC dsd{};
	dsd.Width = m_Device->GetWndSize().right - m_Device->GetWndSize().left;
	dsd.Height = m_Device->GetWndSize().bottom - m_Device->GetWndSize().top;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	/*
	if (m_Device->ableMSAA)
	{
		dsd.SampleDesc.Count = 4;
		dsd.SampleDesc.Quality = m_Device->MSAAQuality - 1;
	}
	else
	*/
	{
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
	}

	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	ID3D11Texture2D* DSBuffer;

	HRESULT hr;
	hr = m_Device->Get()->CreateTexture2D(&dsd, 0, &DSBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateTexture Failed", 0, 0);
	}
	else
	{
		hr = m_Device->Get()->CreateDepthStencilView(DSBuffer, 0, &m_DSV);
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateDepthStencilView Failed", 0, 0);
	}

	DSBuffer->Release();
}

DepthStencilView::DepthStencilView(Device* device, D3D11_DEPTH_STENCIL_VIEW_DESC desc, Texture2D* texture) : Resource(device)
{

	HRESULT hr;
	hr = m_Device->Get()->CreateDepthStencilView(texture->Get(), &desc, &m_DSV);

	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateDepthStencilView Failed", 0, 0);
	}
}

DepthStencilView::DepthStencilView(Device* device, D3D11_DEPTH_STENCIL_VIEW_DESC desc) : Resource(device)
{
	/*HRESULT hr;
	hr = m_Device->Get()->CreateDepthStencilView(DSBuffer, 0, &m_DSV);


	if (FAILED(hr))
	{
		MessageBox(0, L"Device CreateDepthStencilView Failed", 0, 0);
	}

	DSBuffer->Release();*/
}

DepthStencilView::~DepthStencilView()
{

}

ID3D11DepthStencilView* DepthStencilView::Get() const
{
	return m_DSV;
}

ID3D11DepthStencilView** DepthStencilView::GetAddress()
{
	return &m_DSV;
}

void DepthStencilView::Release()
{
	m_DSV->Release();
}