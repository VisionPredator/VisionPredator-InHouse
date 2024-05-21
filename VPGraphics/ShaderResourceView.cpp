#include "pch.h"
#include "ShaderResourceView.h"
#include "DDSTextureLoader.h"
#include <WICTextureLoader.h>
#include <filesystem>
#include <DirectXTex.h>
#include "Texture2D.h"

#include "RenderTargetView.h"

#include <algorithm>
#include <filesystem>
#include <wrl/client.h>

#include "Defines.h"

ShaderResourceView::ShaderResourceView(Device* device) : Resource(device), m_tex(nullptr), m_view(nullptr), m_samplerState(nullptr)
{


}

ShaderResourceView::ShaderResourceView(Device* device, std::wstring filename, D3D11_SAMPLER_DESC sampler) : Resource(device), m_tex(nullptr)
{

	std::filesystem::path _path(filename);
	std::wstring strExtension = _path.extension();
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	DirectX::TexMetadata metadata1;
	DirectX::ScratchImage scratchImage;

	HRESULT hr = S_OK;
	if (strExtension == L".dds")
	{
		(hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, &metadata1, scratchImage));
	}
	else if (strExtension == L".tga")
	{
		(hr = DirectX::LoadFromTGAFile(filename.c_str(), &metadata1, scratchImage));
	}
	else if (strExtension == L".hdr")
	{
		(hr = DirectX::LoadFromHDRFile(filename.c_str(), &metadata1, scratchImage));
	}
	else // 기타..
	{
		(hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
	}

	(hr = DirectX::CreateShaderResourceView(m_Device->Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, &m_view));


	m_Device->Get()->CreateSamplerState(&sampler, &m_samplerState);
}

ShaderResourceView::ShaderResourceView(Device* device, Texture2D* texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
{
	m_tex = texture->Get();

	m_Device->Get()->CreateShaderResourceView(m_tex, &desc, &m_view);
}

ShaderResourceView::ShaderResourceView(Device* device, RenderTargetView* rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
{
	ID3D11Texture2D* renderTargetTexture = nullptr;
	rtv->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(&renderTargetTexture));

	D3D11_TEXTURE2D_DESC textureDesc;
	renderTargetTexture->GetDesc(&textureDesc);
	desc.Format = textureDesc.Format;
	desc.Texture2D.MipLevels = textureDesc.MipLevels;

	m_Device->Get()->CreateShaderResourceView(renderTargetTexture, &desc, &m_view);
}

ShaderResourceView::ShaderResourceView(Device* device, RenderTargetView* rtv)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> rtvTexture;
	rtv->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(rtvTexture.GetAddressOf()));

	D3D11_TEXTURE2D_DESC textureDesc = {};
	rtvTexture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	HR_CHECK(device->Get()->CreateShaderResourceView(rtvTexture.Get(), &srvDesc, &m_view));
}


ShaderResourceView::~ShaderResourceView()
{
	//m_samplerState->Release();
	//m_view->Release();
}

//bool ShaderResourceView::Load()
//{
//	HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device->Get(), m_Device->Context(), L"../img/WoodCrate01.dds", nullptr, &m_view);
//
//	if (FAILED(hr))
//	{
//		MessageBox(0, L"Texture Load Failed", 0, 0);
//		return false;
//	}
//
//	D3D11_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	m_Device->Get()->CreateSamplerState(&samplerDesc, &m_samplerState);
//
//	// 텍스처와 샘플러를 셰이더에 바인딩
//	m_Device->Context()->PSSetShaderResources(0, 1, &m_view);
//	m_Device->Context()->PSSetSamplers(0, 1, &m_samplerState);
//	/*
//	*/
//
//	return true;
//}

//bool ShaderResourceView::Load(std::wstring filename)
//{
//	std::filesystem::path path(filename);
//	std::wstring strExtension = path.extension();
//	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);
//
//	DirectX::TexMetadata metadata1;
//	DirectX::ScratchImage scratchImage;
//
//	HRESULT hr = S_OK;
//	if (strExtension == L".dds")
//	{
//		(hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, &metadata1, scratchImage));
//	}
//	else if (strExtension == L".tga")
//	{
//		(hr = DirectX::LoadFromTGAFile(filename.c_str(), &metadata1, scratchImage));
//	}
//	else if (strExtension == L".hdr")
//	{
//		(hr = DirectX::LoadFromHDRFile(filename.c_str(), &metadata1, scratchImage));
//	}
//	else // 기타..
//	{
//		(hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
//	}
//
//	(hr = DirectX::CreateShaderResourceView(m_Device->Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, &m_view));
//
//	/*
//	*/
//	D3D11_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	m_Device->Get()->CreateSamplerState(&samplerDesc, &m_samplerState);
//
//	return true;
//}

ID3D11ShaderResourceView* ShaderResourceView::Get() const
{
	return m_view;
}

ID3D11ShaderResourceView** ShaderResourceView::GetAddress()
{
	return &m_view;
}

ID3D11SamplerState* ShaderResourceView::GetSampler() const
{
	return m_samplerState;
}

ID3D11SamplerState** ShaderResourceView::GetSamplerAddress()
{

	return &m_samplerState;
}

void ShaderResourceView::SetSampler(ID3D11SamplerState* sampler)
{
	m_samplerState = sampler;
}

void ShaderResourceView::Release()
{
	if (m_samplerState != nullptr)
	{
		m_samplerState->Release();
	}
	if (m_view != nullptr)
	{
		m_view->Release();
	}
}

void ShaderResourceView::Create()
{

}
