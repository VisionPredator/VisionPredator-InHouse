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


ShaderResourceView::ShaderResourceView(const std::shared_ptr<Device>& device) : Resource(device)
{
}

ShaderResourceView::ShaderResourceView(const std::shared_ptr<Device>& device, const std::wstring& filename) : Resource(device)
{
#ifdef _DEBUG
	std::wstring filePath = L"..\\..\\..\\Resource\\Texture\\" + filename;
#else
	const std::wstring filePath = L"..\\Data\\Texture\\" + filename;
#endif

	std::filesystem::path _path(filename);
	std::wstring strExtension = _path.extension();
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;

	HRESULT hr = S_OK;
	if (strExtension == L".dds")
	{
		hr = DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
	}
	else if (strExtension == L".tga")
	{
		hr = DirectX::LoadFromTGAFile(filePath.c_str(), &metadata, scratchImage);
	}
	else if (strExtension == L".hdr")
	{
		hr = DirectX::LoadFromHDRFile(filePath.c_str(), &metadata, scratchImage);
	}
	else // 기타..
	{
		hr = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	}

	m_Width = metadata.width;
	m_Height = metadata.height;

	(hr = DirectX::CreateShaderResourceView(m_Device.lock()->Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, &m_SRV));

#ifdef _DEBUG 
	if (FAILED(hr))
	{
		std::wstring text = L"Create Failed this File : ";
		text = text + filePath;
		MessageBox(0, text.c_str(), 0, 0);
	}
#endif

	// TODO: 텍스처를 찾지 못했을 때의 대처가 필요.
}


ShaderResourceView::ShaderResourceView(const std::shared_ptr<Device>& device, const std::shared_ptr<RenderTargetView>& rtv) : Resource(device)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> rtvTexture;
	rtv->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(rtvTexture.GetAddressOf()));

	D3D11_TEXTURE2D_DESC textureDesc = {};
	rtvTexture->GetDesc(&textureDesc);

	m_Width = textureDesc.Width;
	m_Height = textureDesc.Height;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	HR_CHECK(m_Device.lock()->Get()->CreateShaderResourceView(rtvTexture.Get(), &srvDesc, m_SRV.GetAddressOf()));
}

ShaderResourceView::ShaderResourceView(const std::shared_ptr<Device>& device, const std::shared_ptr<Texture2D>& texture2D)
	: Resource(device)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture2D->Get()->GetDesc(&textureDesc);

	m_Width = textureDesc.Width;
	m_Height = textureDesc.Height;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	HR_CHECK(m_Device.lock()->Get()->CreateShaderResourceView(texture2D->Get(), &srvDesc, m_SRV.GetAddressOf()));
}


//
//ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, std::weak_ptr<Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
//{
//	m_tex = texture;
//
//	m_Device.lock()->Get()->CreateShaderResourceView(m_tex.lock()->Get(), &desc, &m_SRV);
//}

//ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, std::weak_ptr<RenderTargetView> rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc) : Resource(device)
//{
//	std::shared_ptr<RenderTargetView> _rtv = rtv.lock();
//	std::shared_ptr<Texture2D> tex = rtv.lock()->Texture().lock();
//	m_tex = tex;
//	//ID3D11Texture2D* renderTargetTexture = nullptr;
//	//_rtv->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(tex->GetAddress())); //이게 누수였네
//	//이 함수는 리소스의 참조 수를 1씩 증가하므로 애플리케이션이 완료될 때 반환된 포인터에서 Release 를 호출해야 합니다.
//	//  Release가 호출되기 전에 반환된 포인터를 삭제(또는 손실)하면 메모리 누수가 발생합니다.
//
//	D3D11_TEXTURE2D_DESC textureDesc;
//	tex->Get()->GetDesc(&textureDesc);
//	desc.Format = textureDesc.Format;
//	desc.Texture2D.MipLevels = textureDesc.MipLevels;
//
//	m_Device.lock()->Get()->CreateShaderResourceView(tex->Get(), &desc, &m_SRV);
//}
//
//ShaderResourceView::ShaderResourceView(std::shared_ptr<Device> device, RenderTargetView* rtv) : Resource(device)
//{
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> rtvTexture;
//	rtv->Get()->GetResource(reinterpret_cast<ID3D11Resource**>(rtvTexture.GetAddressOf()));
//
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	rtvTexture->GetDesc(&textureDesc);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = textureDesc.Format;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
//
//	HR_CHECK(m_Device.lock()->Get()->CreateShaderResourceView(rtvTexture.Get(), &srvDesc, &m_SRV));
//}


ID3D11ShaderResourceView* ShaderResourceView::Get() const
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** ShaderResourceView::GetAddress()
{
	return m_SRV.GetAddressOf();
}

void ShaderResourceView::Release()
{
	ULONG refCount = m_SRV.Reset();
	assert(refCount == 0);
}
