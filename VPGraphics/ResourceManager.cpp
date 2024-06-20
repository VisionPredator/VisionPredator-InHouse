#include "pch.h"

#include "ResourceManager.h"

//자주 쓰는 구조체 정리를 해보자
#include "StaticData.h"
#include "Desc.h"

ResourceManager::ResourceManager(std::weak_ptr<Device> device) : m_Device(device), m_Camera(), m_DirectionalLight()
{
	m_OffScreenName[0] = L"Albedo";
	m_OffScreenName[1] = L"Normal";
	m_OffScreenName[2] = L"Position";
	m_OffScreenName[3] = L"Depth";
	m_OffScreenName[4] = L"Metalic";
	m_OffScreenName[5] = L"Roughness";
	m_OffScreenName[6] = L"AO";
	m_OffScreenName[7] = L"GBuffer";
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
	Create<Sampler>(L"Linear", SamplerDESC::Linear);
	Create<Sampler>(L"Point", SamplerDESC::Point);

	//기본 RS
	Create<RenderState>(L"Solid", RenderStateDESC::Solid::Desc);
	Create<RenderState>(L"Wire", RenderStateDESC::Wire::Desc);

	Create<VertexShader>(L"Base", VERTEXFILTER::STATIC,L"Mesh");
	Create<VertexShader>(L"Skinning", VERTEXFILTER::SKINNING, L"Mesh");

	m_Camera = Create<ConstantBuffer<CameraData>>(L"Camera", BufferDESC::Constant::DefaultCamera);
	m_Camera.lock()->Update();

	m_Device.lock()->Context()->VSSetConstantBuffers(0, 1, (m_Camera.lock()->GetAddress()));
	m_Device.lock()->Context()->PSSetConstantBuffers(0, 1, (m_Camera.lock()->GetAddress()));

	std::shared_ptr<ConstantBuffer<TransformData>> transform =  Create<ConstantBuffer<TransformData>>(L"Transform", BufferDESC::Constant::DefaultTransform).lock();
	m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, transform->GetAddress());
	m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, transform->GetAddress());

	m_UsingLights = Create<ConstantBuffer<LightArray>>(L"LightArray", BufferDESC::Constant::DefaultLightArray);
	m_Device.lock()->Context()->PSSetConstantBuffers(3, 1, m_UsingLights.lock()->GetAddress());

	//쿼드용
	UINT size = sizeof(QuadVertex);
	Create<VertexBuffer>(L"Quad_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	Create<IndexBuffer>(L"Quad_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	Create<VertexShader>(L"Quad", VERTEXFILTER::QUAD, L"Quad");
	Create<PixelShader>(L"Quad",L"Quad");

	Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");

	Create<PixelShader>(L"MeshDeferredLight",L"MeshDeferredLight");
	Create<PixelShader>(L"MeshDeferredGeometry",L"MeshDeferredGeometry");

	m_UsingMaterial = Create<ConstantBuffer<MaterialData>>(L"MaterialData", BufferDESC::Constant::DefaultMaterial);
	m_Device.lock()->Context()->PSSetConstantBuffers(2, 1, m_UsingMaterial.lock()->GetAddress());

	//출력용 backbuffer
	Create<RenderTargetView>(L"RTV_Main");

	//출력용
	D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
	dsd.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	dsd.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;
	Create<DepthStencilView>(L"DSV_Main", dsd);

	//포스트프로세싱용으로 텍스처를 저장하려고 쓸거
	Create<DepthStencilView>(L"DSV_Deferred", dsd);

	Create<PixelShader>(L"Base", L"Base");
	Create<PixelShader>(L"Mesh", L"Mesh");


	///------미정------
	//디퍼드 용 쉐이더 테스트
	//Create<VertexShader>(L"../x64/Debug/DeferredVS.cso", VERTEXFILTER::SKINNING, L"Deferred");
	//Create<PixelShader>(L"../x64/Debug/DeferredPS.cso", L"Deferred");
	//Create<PixelShader>(L"../x64/Debug/DeferredPS2.cso", L"Deferred");



	


	//Create<PixelShader>(L"../x64/Debug/SkinningPS.cso", L"Skinning");
	Create<PixelShader>(L"../x64/Debug/TexturePS.cso", L"Texture");

	


	D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
	texDesc.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	texDesc.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;


	///Deferred용
	//임시로 5개
	for (int i = 0; i < m_OffScreenName.size(); i++)
	{
		std::weak_ptr<Texture2D> offscreenTex = Create<Texture2D>(m_OffScreenName[i], texDesc);
		Create<RenderTargetView>(m_OffScreenName[i], offscreenTex);
	}
	
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
	Create<RenderTargetView>(L"RTV_Main");

	//디퍼드용
	//이름 바꿔야할듯
	//배열에 담아놓고 쓰던가

	for (int i = 0; i < m_OffScreenName.size(); i++)
	{
		// 렌더 타겟 뷰의 설명을 설정합니다.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = texDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		std::weak_ptr <Texture2D> offscreenTex = Create<Texture2D>(m_OffScreenName[i], texDesc);
		std::weak_ptr <RenderTargetView> newRTV = Create<RenderTargetView>(m_OffScreenName[i], offscreenTex, renderTargetViewDesc);


		// 셰이더 리소스 뷰의 설명을 설정합니다.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = texDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//기존에 있으면 지우고
		Erase<ShaderResourceView>(m_OffScreenName[i]);
		// 셰이더 리소스 뷰를 만듭니다.
		Create<ShaderResourceView>(m_OffScreenName[i], newRTV, shaderResourceViewDesc);
	}

	auto& DSVmap = m_ResourceArray[static_cast<int>(Resource::GetResourceType<DepthStencilView>())];
	int numDSV = static_cast<int>(DSVmap.size());

	for (auto& dsv : DSVmap)
	{
		dsv.second->Release();
	}
	DSVmap.clear();

	Create<DepthStencilView>(L"DSV_Main", texDesc);
	Create<DepthStencilView>(L"DSV_Deferred", texDesc);
}
