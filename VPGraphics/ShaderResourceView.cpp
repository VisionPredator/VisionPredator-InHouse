#include "pch.h"
#include "ShaderResourceView.h"
#include "DDSTextureLoader.h"
#include <WICTextureLoader.h>
#include <filesystem>
#include <DirectXTex.h>
#include "Texture2D.h"

#include "RenderTargetView.h"



ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device) : Resource(device), m_tex(nullptr), m_view(nullptr), m_samplerState(nullptr)
{


}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device>device, std::wstring filename, D3D11_SAMPLER_DESC sampler) : Resource(device), m_tex(nullptr)
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
	else // ±âÅ¸..
	{
		(hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
	}

	(hr = DirectX::CreateShaderResourceView(m_Device.lock()->Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, &m_view));


	m_Device.lock()->Get()->CreateSamplerState(&sampler, &m_samplerState);
}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, std::weak_ptr<Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
{
	m_tex = texture.lock()->Get();

	m_Device.lock()->Get()->CreateShaderResourceView(m_tex, &desc, &m_view);
}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, std::weak_ptr<RenderTargetView> rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
{
	ID3D11Texture2D* renderTargetTexture = nullptr;
	rtv.lock()->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(&renderTargetTexture));

	D3D11_TEXTURE2D_DESC textureDesc;
	renderTargetTexture->GetDesc(&textureDesc);
	desc.Format = textureDesc.Format;
	desc.Texture2D.MipLevels = textureDesc.MipLevels;

	m_Device.lock()->Get()->CreateShaderResourceView(renderTargetTexture, &desc, &m_view);
}


ShaderResourceView::~ShaderResourceView()
{
	//m_samplerState->Release();
	//m_view->Release();
}

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
