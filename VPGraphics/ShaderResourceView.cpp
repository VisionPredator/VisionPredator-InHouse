#include "pch.h"
#include "ShaderResourceView.h"
#include "DDSTextureLoader.h"
#include <WICTextureLoader.h>
#include <filesystem>
#include <DirectXTex.h>
#include "Texture2D.h"

#include "RenderTargetView.h"

#include <wrl/client.h>
#include "Defines.h"


ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device) : Resource(device), m_tex(nullptr), m_view(nullptr)
{


}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device>device, std::wstring filename) : Resource(device), m_tex(nullptr)
{
#ifdef _DEBUG
	std::wstring filePath = L"..\\..\\..\\Resource\\Texture\\" + filename;
#else
	const std::wstring filePath = L"..\\Data\\Texture\\" + filename;
#endif


	std::filesystem::path _path(filename);
	std::wstring strExtension = _path.extension();
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	DirectX::TexMetadata metadata1;
	DirectX::ScratchImage scratchImage;

	HRESULT hr = S_OK;
	if (strExtension == L".dds")
	{
		//HR_CHECK(DirectX::CreateDDSTextureFromFile(m_Device.lock()->Get(),
		//	m_Device.lock()->Context(),
		//	filePath.c_str(),
		//	nullptr, &m_view));

		(hr = DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata1, scratchImage));
	}
	else if (strExtension == L".tga")
	{
		(hr = DirectX::LoadFromTGAFile(filePath.c_str(), &metadata1, scratchImage));
	}
	else if (strExtension == L".hdr")
	{
		(hr = DirectX::LoadFromHDRFile(filePath.c_str(), &metadata1, scratchImage));
	}
	else // ±âÅ¸..
	{
		(hr = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
	}

	(hr = DirectX::CreateShaderResourceView(m_Device.lock()->Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, &m_view));


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

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, RenderTargetView* rtv) : Resource(device)
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

	HR_CHECK(m_Device.lock()->Get()->CreateShaderResourceView(rtvTexture.Get(), &srvDesc, &m_view));
}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, const std::shared_ptr<RenderTargetView>& rtv) : Resource(device)
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

	HR_CHECK(m_Device.lock()->Get()->CreateShaderResourceView(rtvTexture.Get(), &srvDesc, &m_view));
}

ID3D11ShaderResourceView* ShaderResourceView::Get() const
{
	return m_view;
}

ID3D11ShaderResourceView** ShaderResourceView::GetAddress()
{
	return &m_view;
}

void ShaderResourceView::Release()
{
	if (m_view != nullptr)
	{
		m_view->Release();
		m_view = nullptr;
	}
}
