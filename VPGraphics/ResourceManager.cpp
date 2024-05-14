#include "pch.h"

#include "ResourceManager.h"
#include "ConstantBuffer.h"
#include "Object.h"
#include "RenderState.h"
#include "RenderTargetView.h"
#include "Texture2D.h"

//자주 쓰는 구조체 정리를 해보자
#include "StaticData.h"
#include "Desc.h"

ResourceManager::ResourceManager(Device * device) : m_device(device), m_Camera(nullptr), m_DirectionalLight(nullptr)
{
}

ResourceManager::~ResourceManager()
{

	for (int i = 0; i < static_cast<int>(ResourceType::End); i++)
	{
		for (auto& resource : m_ResourceArray[i])
		{
			resource.second->Release();
			delete resource.second;
		}

		m_ResourceArray[i].clear();
	}


}



void ResourceManager::Update(double dt, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	DirectX::XMFLOAT4X4 cb_worldviewproj;
	DirectX::XMFLOAT4X4 cb_view;
	DirectX::XMFLOAT4X4 cb_proj;
	DirectX::XMFLOAT4X4 cb_viewInverse;
	cb_worldviewproj = DirectX::SimpleMath::Matrix(view) * proj;

	//상수 버퍼는 계산 순서때문에 전치한다
	XMStoreFloat4x4(&cb_worldviewproj, XMMatrixTranspose(XMLoadFloat4x4(&cb_worldviewproj)));
	XMStoreFloat4x4(&cb_view, XMMatrixTranspose(XMLoadFloat4x4(&view)));
	XMStoreFloat4x4(&cb_proj, XMMatrixTranspose(XMLoadFloat4x4(&proj)));

	DirectX::XMMATRIX viewInverse = XMMatrixInverse(nullptr, (XMLoadFloat4x4(&view)));
	XMStoreFloat4x4(&cb_viewInverse, XMMatrixTranspose(viewInverse));

	m_Camera->m_struct.worldviewproj = cb_worldviewproj;
	m_Camera->m_struct.view = cb_view;
	m_Camera->m_struct.proj = cb_proj;
	m_Camera->m_struct.viewInverse = cb_viewInverse;
	m_Camera->Update();

	m_DirectionalLight->Update();
}


void ResourceManager::Initialize()
{
	//디퍼드 용 쉐이더 테스트
	Create<VertexShader>(L"../x64/Debug/DeferredVS.cso", VERTEXFILTER::SKINNING, L"Deferred");
	Create<PixelShader>(L"../x64/Debug/DeferredPS.cso", L"Deferred");
	Create<PixelShader>(L"../x64/Debug/DeferredPS2.cso", L"Deferred");

	//기본 RS
	Create<RenderState>(L"Solid", RenderStateDESC::Solid::Desc);
	Create<RenderState>(L"Wire", RenderStateDESC::Wire::Desc);


	D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
	texDesc.Width = m_device->GetWndSize().right - m_device->GetWndSize().left;
	texDesc.Height = m_device->GetWndSize().bottom - m_device->GetWndSize().top;

	//출력용 backbuffer
	Create<RenderTargetView>(L"RTV_1");

	///Deferred용
	//임시로 5개
	for (int i = 1; i <= 5; i++)
	{
		std::wstring index = std::to_wstring(i);
		std::wstring rtvindex = std::to_wstring(i + 1);

		Texture2D* offscreenTex = Create<Texture2D>(L"OffScreenTex_" + index, texDesc);
		Create<RenderTargetView>(L"RTV_" + rtvindex, offscreenTex);
	}

	//쿼드용
	UINT size = sizeof(QuadVertex);
	Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);

	//출력용
	D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
	dsd.Width = m_device->GetWndSize().right - m_device->GetWndSize().left;
	dsd.Height = m_device->GetWndSize().bottom - m_device->GetWndSize().top;
	Create<DepthStencilView>(L"DSV_1", dsd);

	Create<DepthStencilView>(L"DSV_2", dsd);

	//한번만 연결해주면 계속 쓸 것들
	Create<ConstantBuffer<WorldTransformCB>>(L"SunTransform", BufferDESC::Constant::DefaultWorld);

	m_DirectionalLight = Create<ConstantBuffer<DirectionLightCB>>(L"DirectionLight", BufferDESC::Constant::DefaultDirLight);
	m_device->Context()->PSSetConstantBuffers(2, 1, m_DirectionalLight->GetAddress());

	m_Camera = Create<ConstantBuffer<CameraCB>>(L"Camera", BufferDESC::Constant::DefaultCamera);

	m_device->Context()->VSSetConstantBuffers(0, 1, (m_Camera->GetAddress()));
	m_device->Context()->PSSetConstantBuffers(1, 1, (m_Camera->GetAddress()));
}


void ResourceManager::OnResize()
{
	auto& OffScreenMap = m_ResourceArray[static_cast<int>(Resource::GetResourceType<Texture2D>())];
	for (auto& tex : OffScreenMap)
	{
		tex.second->Release();
	}
	OffScreenMap.clear();

	auto& RTVmap = m_ResourceArray[static_cast<int>(Resource::GetResourceType<RenderTargetView>())];
	int numRTV = static_cast<int>(RTVmap.size());

	for (auto& rtv : RTVmap)
	{
		rtv.second->Release();
	}
	RTVmap.clear();

	D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
	texDesc.Width = m_device->GetWndSize().right - m_device->GetWndSize().left;
	texDesc.Height = m_device->GetWndSize().bottom - m_device->GetWndSize().top;

	//출력용 backbuffer
	Create<RenderTargetView>(L"RTV_1");

	//디퍼드용
	for (int i = 1; i < numRTV; i++)
	{
		std::wstring index = std::to_wstring(i);
		std::wstring rtvindex = std::to_wstring(i + 1);

		// 렌더 타겟 뷰의 설명을 설정합니다.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = texDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		Texture2D* offscreenTex = Create<Texture2D>(L"OffScreenTex_" + index, texDesc);
		RenderTargetView* newRTV = Create<RenderTargetView>(L"RTV_" + rtvindex, offscreenTex, renderTargetViewDesc);


		// 셰이더 리소스 뷰의 설명을 설정합니다.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = texDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//기존에 있으면 지우고
		Erase<ShaderResourceView>(L"OffScreenSRV_" + index);
		// 셰이더 리소스 뷰를 만듭니다.

		ID3D11SamplerState* samplerState;
		m_device->Get()->CreateSamplerState(&SamplerDESC::Linear, &samplerState);

		//Create<ShaderResourceView>(L"OffScreenSRV_" + index, offscreenTex, shaderResourceViewDesc)->SetSampler(samplerState);
		Create<ShaderResourceView>(L"OffScreenSRV_" + index, newRTV, shaderResourceViewDesc)->SetSampler(samplerState);
	}

	auto& DSVmap = m_ResourceArray[static_cast<int>(Resource::GetResourceType<DepthStencilView>())];
	int numDSV = static_cast<int>(DSVmap.size());

	for (auto& dsv : DSVmap)
	{
		dsv.second->Release();
	}
	DSVmap.clear();


	Create<DepthStencilView>(L"DSV_1", texDesc);

	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	desc.Format = texDesc.Format;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Flags = 0;

	for (int i = 1; i < numDSV; i++)
	{

		std::wstring index = std::to_wstring(i + 1);
		Texture2D* texture = Get<Texture2D>(L"OffScreenTex_" + index);
		//Create<DepthStencilView>(L"DSV_" + index,desc,texture);
	}
}
