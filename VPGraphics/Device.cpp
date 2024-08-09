#include "pch.h"
#include "Device.h"

#include "Device.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderState.h"

#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "Material.h"
#include "Mesh.h"

#include "Slot.h"

Device::Device(HWND hWnd) : ableMSAA(false), m_Device(nullptr), m_Context(nullptr), m_FeatureLevel(), MSAAQuality(), m_hWnd(hWnd), m_SwapChain(nullptr), m_wndSize()
{
}

Device::~Device()
{
	m_SwapChain->Release();
	m_Context->Release();
	m_Device->Release();

	m_Device = nullptr;
	m_Context = nullptr;
	m_SwapChain = nullptr;

}

void Device::Initialize()
{
	HRESULT result;
	uint32_t createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

#pragma region CreateDevice
	result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
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
	//m_Context->RSSetState(0);
}

void Device::UnBindSRV()
{
	//기존 바인된 rtv srv 해제
	const int num = static_cast<int>(Slot_T::End);
	ID3D11ShaderResourceView* pSRV[num] = {};

	for (int i = 0; i < num; i++)
	{
		pSRV[i] = nullptr;
	}


	m_Context->PSSetShaderResources(0, num, pSRV);
	m_Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Device::BindMaterialSRV(std::shared_ptr<Material> curMaterial)
{
	MaterialData curMaterialData = curMaterial->m_Data;

	if (curMaterialData.useAMRO.x > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, (curMaterial->m_AlbedoSRV.lock()->GetAddress()));
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, (curMaterial->m_AlbedoSRV.lock()->GetAddress()));
	}

	if (curMaterialData.useAMRO.y > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, curMaterial->m_MetalicSRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, curMaterial->m_MetalicSRV.lock()->GetAddress());
	}

	if (curMaterialData.useAMRO.z > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Roughness), 1, curMaterial->m_RoughnessSRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Roughness), 1, curMaterial->m_RoughnessSRV.lock()->GetAddress());
	}

	if (curMaterialData.useAMRO.w > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, curMaterial->m_AOSRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, curMaterial->m_AOSRV.lock()->GetAddress());
	}

	if (curMaterialData.useNEO.x > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, curMaterial->m_NormalSRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, curMaterial->m_NormalSRV.lock()->GetAddress());
	}

	if (curMaterialData.useNEO.y > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Emissive), 1, curMaterial->m_EmissiveSRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Emissive), 1, curMaterial->m_EmissiveSRV.lock()->GetAddress());
	}

	if (curMaterialData.useNEO.z > 0)
	{
		m_Context->PSSetShaderResources(static_cast<UINT>(Slot_T::Opacity), 1, curMaterial->m_OpacitySRV.lock()->GetAddress());
		m_Context->VSSetShaderResources(static_cast<UINT>(Slot_T::Opacity), 1, curMaterial->m_OpacitySRV.lock()->GetAddress());
	}
}

void Device::BindMeshBuffer(std::shared_ptr<Mesh> mesh)
{
	// VB & IB Binding
	m_Context->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
	m_Context->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
	m_Context->IASetPrimitiveTopology(mesh->m_primitive);
}

void Device::BindVS(std::shared_ptr<VertexShader> vs)
{
	// Shader Binding
	m_Context->IASetInputLayout(vs->InputLayout());
	m_Context->VSSetShader(vs->GetVS(), nullptr, 0);
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
