#include "pch.h"
#include "Device.h"

#include "Device.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderState.h"

#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"

Device::Device(HWND hWnd) : ableMSAA(false), m_Device(nullptr), m_Context(nullptr), m_FeatureLevel(), MSAAQuality(), m_hWnd(hWnd), m_SwapChain(nullptr), m_wndSize()
{
}

Device::~Device()
{
	m_SwapChain->Release();
	m_Context->Release();
	m_Device->Release();

}

void Device::Initialize()
{
	HRESULT result;

#pragma region CreateDevice
	result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_Device,
		&m_FeatureLevel,
		&m_Context);

	if (FAILED(result))
	{
		MessageBox(0, L"CreateDevice Failed", 0, 0);
		return;
	}

	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"D3D_FEATURE_LEVEL_11_0 UnSupport", 0, 0);
		return;
	}

#pragma endregion CreateDevice

#pragma region CheckMSAA


	result = m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4
		, &MSAAQuality);

	if (MSAAQuality <= 0)
	{
		MessageBox(0, L"MSAAQuality < 0", 0, 0);
		return;
	}
	else
	{
		ableMSAA = true;
	}

#pragma endregion CheckMSAA

	GetClientRect(m_hWnd, &m_wndSize);
	CreateSwapChain();


}

void Device::OnResize()
{
	GetClientRect(m_hWnd, &m_wndSize);
	m_SwapChain->Release();

	float Width = static_cast<float>(m_wndSize.right - m_wndSize.left);
	float Height = static_cast<float>(m_wndSize.bottom - m_wndSize.top);
	//m_SwapChain->ResizeBuffers(1, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);


	CreateSwapChain();
}

ID3D11DeviceContext* Device::Context() const
{
	return m_Context;
}

ID3D11Device* Device::Get() const
{
	return m_Device;
}

IDXGISwapChain* Device::SwapChain() const
{
	return m_SwapChain;
}

RECT Device::GetWndSize() const
{
	return m_wndSize;
}

void Device::BeginRender(ID3D11RenderTargetView* RTV, ID3D11DepthStencilView* DSV, const float* color)
{
	m_Context->ClearRenderTargetView(RTV, color);
	m_Context->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Device::EndRender()
{
	m_SwapChain->Present(0, 0);
	m_Context->RSSetState(0);
}

void Device::BeginDeferredRender(std::vector<RenderTargetView*>& RTVs, ID3D11DepthStencilView* DSVs)
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	FLOAT Red[4] = { 1.f,0.f,0.f,1.f };
	FLOAT Green[4] = { 0.f,1.f,0.f,1.f };
	FLOAT Blue[4] = { 0.f,0.f,1.f,1.f };
	FLOAT Yellow[4] = { 1.f,1.f,0.f,1.f };
	FLOAT Purple[4] = { 1.f,0.f,1.f,1.f };

	ID3D11RenderTargetView* MRT0 = RTVs[0]->Get();
	ID3D11RenderTargetView* MRT1 = RTVs[1]->Get();
	ID3D11RenderTargetView* MRT2 = RTVs[2]->Get();

	m_Context->ClearRenderTargetView(RTVs[0]->Get(), Black); //출력할 렌더 타겟

	///SRV 뽑아낼 렌더타겟
	m_Context->ClearRenderTargetView(RTVs[1]->Get(), Red);
	m_Context->ClearRenderTargetView(RTVs[2]->Get(), Green);
	m_Context->ClearRenderTargetView(RTVs[3]->Get(), Blue);
	m_Context->ClearRenderTargetView(RTVs[4]->Get(), Yellow);
	m_Context->ClearRenderTargetView(RTVs[5]->Get(), Purple);

	m_Context->ClearDepthStencilView(DSVs, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void Device::DrawQuard(ShaderResourceView* srv)
{
	m_Context->PSSetShaderResources(0, 1, srv->GetAddress());
}

bool Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = m_wndSize.right - m_wndSize.left;
	sd.BufferDesc.Height = m_wndSize.bottom - m_wndSize.top;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/*
	//디퍼드땜에 안쓰는중
	if (ableMSAA)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = MSAAQuality - 1;
	}
	else
	*/
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//D12 부터는 지원안함 대신 DXGI_SWAP_EFFECT_FLIP_DISCARD 사용
	sd.Flags = 0;


	HRESULT hr;
	IDXGIDevice* dxgiDeivce = 0;
	hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDeivce);
	if (FAILED(hr))
	{
		MessageBox(0, L"Device QueryInterface Failed", 0, 0);
		return false;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDeivce->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"Device GetParent Failed", 0, 0);
		return false;
	}

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"Adapter GetParent Failed", 0, 0);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"Factory CreateSwapChain Failed", 0, 0);
		return false;
	}

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDeivce->Release();

	return true;
}
