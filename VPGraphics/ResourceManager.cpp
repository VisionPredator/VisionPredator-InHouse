#include "pch.h"

#include "ResourceManager.h"

//자주 쓰는 구조체 정리를 해보자
#include "StaticData.h"
#include "Desc.h"

ResourceManager::ResourceManager(std::weak_ptr<Device> device) : m_Device(device), m_Camera(), m_DirectionalLight()
{
}

ResourceManager::~ResourceManager()
{

	for (int i = 0; i < static_cast<int>(ResourceType::End); i++)
	{
		for (auto& resource : m_ResourceArray[i])
		{
			resource.second->Release();
			//delete resource.second;
		}

		m_ResourceArray[i].clear();
	}


}

void ResourceManager::Initialize()
{
	///--------확정-----
	//기본 RS
	Create<RenderState>(L"Solid", RenderStateDESC::Solid::Desc);
	Create<RenderState>(L"Wire", RenderStateDESC::Wire::Desc);

	Create<VertexShader>(L"Base", VERTEXFILTER::STATIC,L"Mesh");
	Create<VertexShader>(L"Skinning", VERTEXFILTER::SKINNING, L"Mesh");

	m_Camera = Create<ConstantBuffer<CameraData>>(L"Camera", BufferDESC::Constant::DefaultCamera);
	m_Camera.lock()->Update();

	m_Device.lock()->Context()->VSSetConstantBuffers(0, 1, (m_Camera.lock()->GetAddress()));
	m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, (m_Camera.lock()->GetAddress()));

	Create<ConstantBuffer<TransformData>>(L"Transform", BufferDESC::Constant::DefaultTransform);

	Create<Sampler>(L"Linear", SamplerDESC::Linear);

	m_UsingLights = Create<ConstantBuffer<LightArray>>(L"LightArray", BufferDESC::Constant::DefaultLightArray);

	///------미정------
	//디퍼드 용 쉐이더 테스트
	//Create<VertexShader>(L"../x64/Debug/DeferredVS.cso", VERTEXFILTER::SKINNING, L"Deferred");
	//Create<PixelShader>(L"../x64/Debug/DeferredPS.cso", L"Deferred");
	//Create<PixelShader>(L"../x64/Debug/DeferredPS2.cso", L"Deferred");

	Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");


	Create<PixelShader>(L"../x64/Debug/BasePS.cso", L"Base");
	Create<PixelShader>(L"../x64/Debug/SkinningPS.cso",  L"Skinning");
	Create<PixelShader>(L"../x64/Debug/TexturePS.cso", L"Texture");

	


	D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
	texDesc.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	texDesc.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

	//출력용 backbuffer
	Create<RenderTargetView>(L"RTV_1");

	///Deferred용
	//임시로 5개
	for (int i = 1; i <= 5; i++)
	{
		std::wstring index = std::to_wstring(i);
		std::wstring rtvindex = std::to_wstring(i + 1);

		std::weak_ptr<Texture2D> offscreenTex = Create<Texture2D>(L"OffScreenTex_" + index, texDesc);
		Create<RenderTargetView>(L"RTV_" + rtvindex, offscreenTex);
	}

	//쿼드용
	UINT size = sizeof(QuadVertex);
	Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);

	//출력용
	D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
	dsd.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	dsd.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;
	Create<DepthStencilView>(L"DSV_1", dsd);
	Create<DepthStencilView>(L"DSV_2", dsd);

	//한번만 연결해주면 계속 쓸 것들
	Create<ConstantBuffer<TransformData>>(L"SunTransform", BufferDESC::Constant::DefaultTransform);


	m_DirectionalLight = Create<ConstantBuffer<LightData>>(L"DirectionLight", BufferDESC::Constant::DefaultLight);
	m_Device.lock()->Context()->PSSetConstantBuffers(2, 1, m_UsingLights.lock()->GetAddress());

	
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
	texDesc.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	texDesc.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

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

		std::weak_ptr <Texture2D> offscreenTex = Create<Texture2D>(L"OffScreenTex_" + index, texDesc);
		std::weak_ptr <RenderTargetView> newRTV = Create<RenderTargetView>(L"RTV_" + rtvindex, offscreenTex, renderTargetViewDesc);


		// 셰이더 리소스 뷰의 설명을 설정합니다.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = texDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//기존에 있으면 지우고
		Erase<ShaderResourceView>(L"OffScreenSRV_" + index);
		// 셰이더 리소스 뷰를 만듭니다.

		Create<ShaderResourceView>(L"OffScreenSRV_" + index, newRTV, shaderResourceViewDesc);
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
		std::weak_ptr<Texture2D> texture = Get<Texture2D>(L"OffScreenTex_" + index);
		//Create<DepthStencilView>(L"DSV_" + index,desc,texture);
	}
}
