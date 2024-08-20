#include "pch.h"

#include "ResourceManager.h"

//자주 쓰는 구조체 정리를 해보자
#include "BlendState.h"
#include "DepthStencilState.h"
#include "StaticData.h"
#include "Desc.h"
#include "Slot.h"

#include "ModelData.h"
#include "Mesh.h"

ResourceManager::ResourceManager()
{
	m_OffScreenName[0] = L"Albedo";
	m_OffScreenName[1] = L"Normal";
	m_OffScreenName[2] = L"Position";
	m_OffScreenName[3] = L"Depth";
	m_OffScreenName[4] = L"Metalic_Roughness";
	m_OffScreenName[5] = L"LightMap";
	m_OffScreenName[6] = L"AO";
	m_OffScreenName[7] = L"Emissive";
	m_OffScreenName[8] = L"GBuffer";
	m_OffScreenName[9] = L"IMGUI";
	//m_OffScreenName[9] = L"RTV_Main";
}


ResourceManager::~ResourceManager()
{

	for (int i = 0; i < static_cast<int>(ResourceType::End); i++)
	{
		for (auto& resource : m_ResourceArray[i])
		{
			resource.second->Release();
		}

		m_ResourceArray[i].clear();
	}


}


void ResourceManager::Initialize(std::weak_ptr<Device> device)
{
	m_Device = device;
	UINT width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	UINT height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

	///Built in

	// Depth Stencil State
	{
		Create<DepthStencilState>(L"DefaultDSS", DepthStencilStateType::Default);
		Create<DepthStencilState>(L"DisableDepth", DepthStencilStateType::DisableDepth);
		Create<DepthStencilState>(L"NoDepthWrites", DepthStencilStateType::NoDepthWrites);
	}

	// Blend State
	{
		Create<BlendState>(L"AlphaBlending", BlendStateType::AlphaBlending);
		Create<BlendState>(L"AdditiveBlending", BlendStateType::AdditiveBlending);
		Create<BlendState>(L"DefaultBlending", BlendStateType::Default);
	}

	//Sampler
	{
		Create<Sampler>(L"Linear", SamplerDESC::Linear);
		Create<Sampler>(L"Point", SamplerDESC::Point);
	}

	//RS
	{
		Create<RenderState>(L"Solid", RenderStateDESC::Solid::Desc);
		Create<RenderState>(L"Wire", RenderStateDESC::Wire::Desc);
	}

	//VS
	{
		Create<VertexShader>(L"Base", VERTEXFILTER::STATIC, L"Mesh");
		Create<VertexShader>(L"Skinning", VERTEXFILTER::SKINNING, L"Mesh");
		Create<VertexShader>(L"Quad", VERTEXFILTER::QUAD, L"Quad");
	}

	//PS
	{
		Create<PixelShader>(L"Base", L"Base");
		Create<PixelShader>(L"Mesh", L"Mesh");
		Create<PixelShader>(L"MeshDeferredLight", L"MeshDeferredLight");
		Create<PixelShader>(L"MeshDeferredGeometry", L"MeshDeferredGeometry");
		Create<PixelShader>(L"Quad", L"Quad");
	}

	//VB
	{
		//쿼드용
		UINT size = sizeof(QuadVertex);
		Create<VertexBuffer>(L"Quad_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);

		size = sizeof(BaseVertex);
		Create<VertexBuffer>(L"TextureBox_VB", TextureBox::Vertex::Desc, TextureBox::Vertex::Data, size);

	}

	//IB
	{
		Create<IndexBuffer>(L"Quad_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
		Create<IndexBuffer>(L"TextureBox_IB", TextureBox::Index::Desc, TextureBox::Index::Data, TextureBox::Index::count);
	}

	//SRV
	{

#ifdef _DEBUG
		std::wstring filePath = L"..\\..\\..\\Resource\\Texture\\base.png";
#else
		const std::wstring filePath = L"..\\Data\\Texture\\base.png";
#endif
		//텍스처 이미지가 없으면 임시로 쓸 기본 base.png
		Create<ShaderResourceView>(filePath, L"base.png");

		// Default UI Image
		Create<ShaderResourceView>(L"DefaultUI", L"DefaultUI.png");
	}

	//RTV
	{
		//출력용 backbuffer
		Create<RenderTargetView>(L"RTV_Main", RenderTargetViewType::Default, width, height);

		//Deferred용
		for (int i = 0; i < m_OffScreenName.size(); i++)
		{
			Create<RenderTargetView>(m_OffScreenName[i], RenderTargetViewType::OffScreen, width, height);
		}

	}

	// DSV
	{
		//출력용
		D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
		dsd.Width = width;
		dsd.Height = height;
		Create<DepthStencilView>(L"DSV_Main", dsd);

		//포스트프로세싱용으로 텍스처를 저장하려고 쓸거
		Create<DepthStencilView>(L"DSV_Deferred", dsd);
	}

	// CB
	{
		m_Camera = Create<ConstantBuffer<CameraData>>(L"Camera", BufferDESC::Constant::DefaultCamera);
		m_Transform = Create<ConstantBuffer<TransformData>>(L"Transform", BufferDESC::Constant::DefaultTransform);
		m_UsingLights = Create<ConstantBuffer<LightArray>>(L"LightArray", BufferDESC::Constant::DefaultLightArray);
		m_UsingMaterial = Create<ConstantBuffer<MaterialData>>(L"MaterialData", BufferDESC::Constant::DefaultMaterial);
		m_Pallete = Create<ConstantBuffer<MatrixPallete>>(L"MatrixPallete", BufferDESC::Constant::DefaultMatrixPallete);

		//Bind CB only Once
		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));
		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());
		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, m_Pallete.lock()->GetAddress());

		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));
		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());
		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, m_UsingMaterial.lock()->GetAddress());
		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, m_UsingLights.lock()->GetAddress());

		Create<ConstantBuffer<DirectX::XMFLOAT4>>(L"Color", BufferDESC::Constant::DefaultFloat4);
	}

	//LightMap
	{
		Create<ShaderResourceView>(L"Lightmap-0_comp_light.png", L"Lightmap-0_comp_light.png");
		Create<ShaderResourceView>(L"indirect_Lightmap-0_comp_light.png", L"indirect_Lightmap-0_comp_light.png");
	}
}


void ResourceManager::OnResize(RECT& wndsize)
{
	UINT width = wndsize.right - wndsize.left;
	UINT height = wndsize.bottom - wndsize.top;

	Erase<ViewPort>(L"Main");
	Create<ViewPort>(L"Main", wndsize);

	/*for (auto tex : m_OffScreenName)
	{
		Erase<Texture2D>(tex);
	}*/

	Erase<RenderTargetView>(L"RTV_Main");

	for (auto tex : m_OffScreenName)
	{
		Erase<RenderTargetView>(tex);
	}

	//	/*
	//	auto& RTVmap = m_ResourceArray[static_cast<int>(Resource::GetResourceType<RenderTargetView>())];
	//	int numRTV = static_cast<int>(RTVmap.size());

	//	for (auto& rtv : RTVmap)
	//	{
	//		rtv.second->Release();
	//	}
	//	RTVmap.clear();
	//	*/

	//	//출력용 backbuffer
	Create<RenderTargetView>(L"RTV_Main", RenderTargetViewType::Default, width, height);

	//디퍼드용
	//이름 바꿔야할듯
	//배열에 담아놓고 쓰던가
	for (int i = 0; i < m_OffScreenName.size(); i++)
	{
		//기존에 있으면 지우고
		Erase<ShaderResourceView>(m_OffScreenName[i]);
		Erase<Texture2D>(m_OffScreenName[i]);

		std::weak_ptr <RenderTargetView> newRTV = Create<RenderTargetView>(m_OffScreenName[i], RenderTargetViewType::OffScreen, width, height);
		Create<ShaderResourceView>(m_OffScreenName[i], newRTV.lock());
	}

	{
		D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
		texDesc.Width = width;
		texDesc.Height = height;

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

}
