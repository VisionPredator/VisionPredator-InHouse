  #include "pch.h"

#include "ResourceManager.h"

//���� ���� ����ü ������ �غ���
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


std::weak_ptr<ModelData> ResourceManager::Get(const int modelID)
{
	if (modelID > 0)
	{
		std::unordered_map<std::wstring, std::shared_ptr<Resource>>& curMap = m_ResourceArray[static_cast<int>(ResourceType::ModelData)];

		for (auto& model : curMap)
		{
			auto curmodel = dynamic_pointer_cast<ModelData>(model.second);

			if (curmodel)
			{
				if (curmodel->UID == modelID)
				{
					return curmodel;
				}
			}
		}
	}


	return {};
}

void ResourceManager::Initialize(std::weak_ptr<Device> device)
{
	m_Device = device;
	UINT width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	UINT height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

	///*************************************************************************************
	/// Create Built in Resources                                                            *
	///*************************************************************************************


	// ----------------------------------------------------------------------------------------
	// Depth Stencil State
	// ----------------------------------------------------------------------------------------
	Create<DepthStencilState>(L"DefaultDSS", DepthStencilStateType::Default);
	Create<DepthStencilState>(L"DisableDepth", DepthStencilStateType::DisableDepth);
	Create<DepthStencilState>(L"NoDepthWrites", DepthStencilStateType::NoDepthWrites);


	// ----------------------------------------------------------------------------------------
	// Blend State
	// ----------------------------------------------------------------------------------------
	Create<BlendState>(L"AlphaBlend", BlendStateType::AlphaBlend);
	Create<BlendState>(L"AdditiveBlending", BlendStateType::AdditiveBlending);
	Create<BlendState>(L"Opaque", BlendStateType::Opaque);


	// ----------------------------------------------------------------------------------------
	// Sampler State
	// ----------------------------------------------------------------------------------------
	Create<Sampler>(L"LinearWrap", SamplerStateType::LinearWrap);
	Create<Sampler>(L"PointClamp", SamplerStateType::PointClamp);
	Create<Sampler>(L"LinearClamp", SamplerStateType::LinearClamp);
	Create<Sampler>(L"Default", SamplerStateType::Default);


	// ----------------------------------------------------------------------------------------
	// Rasterizer State
	// ----------------------------------------------------------------------------------------
	Create<RenderState>(L"Solid", RenderStateDESC::Solid::Desc);
	Create<RenderState>(L"Wire", RenderStateDESC::Wire::Desc);


	// ----------------------------------------------------------------------------------------
	// Vertex Shader
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO macro[] =
	{
		{"SKINNING",""}, // ��ũ�� �̸��� ���� ����
		{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
	};
	Create<VertexShader>(L"Base", L"MeshVS", "main");
	Create<VertexShader>(L"Skinning", L"MeshVS", "main", macro);
	Create<VertexShader>(L"Quad", L"QuadVS", "main");	
	Create<VertexShader>(L"InstancingVS", L"InstancingVS");	//Instancing VS
	Create<VertexShader>(L"DecalVS", L"DecalVS",Instancing::DecalDesc,Instancing::DecalCount);	//Decal VS

	// ----------------------------------------------------------------------------------------
	// Pixel Shader
	// ----------------------------------------------------------------------------------------
	Create<PixelShader>(L"BasePS", L"BasePS", "main");
	Create<PixelShader>(L"Mesh", L"MeshPS", "main");
	Create<PixelShader>(L"MeshDeferredLight", L"MeshDeferredLightPS", "main");
	Create<PixelShader>(L"MeshDeferredGeometry", L"MeshDeferredGeometryPS", "main");
	Create<PixelShader>(L"Quad", L"QuadPS", "main");
	Create<PixelShader>(L"VPOutLine", L"VPOutLine", "main");
	Create<PixelShader>(L"RimLight", L"RimLight", "main");
	Create<PixelShader>(L"InstancingPS", L"InstancingPS", "main");
	Create<PixelShader>(L"DecalPS", L"DecalPS", "main");

	// ----------------------------------------------------------------------------------------
	// Vertex Buffer
	// ----------------------------------------------------------------------------------------
	UINT size = sizeof(QuadVertex);
	Create<VertexBuffer>(L"Quad_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);

	size = sizeof(BaseVertex);
	Create<VertexBuffer>(L"TextureBox_VB", TextureBox::Vertex::Desc, TextureBox::Vertex::Data, size);
	
	size = sizeof(BaseVertex);
	Create<VertexBuffer>(L"Decal_VB", DecalVolume::Vertex::Desc, DecalVolume::Vertex::Data, size);


	// ----------------------------------------------------------------------------------------
	// Index Buffer
	// ----------------------------------------------------------------------------------------
	Create<IndexBuffer>(L"Quad_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	Create<IndexBuffer>(L"TextureBox_IB", TextureBox::Index::Desc, TextureBox::Index::Data, TextureBox::Index::count);
	Create<IndexBuffer>(L"Decal_IB", DecalVolume::Index::Desc, DecalVolume::Index::Data, DecalVolume::Index::count);


	// ----------------------------------------------------------------------------------------
	// Shader Resource View
	// ----------------------------------------------------------------------------------------
#ifdef _DEBUG
	std::wstring filePath = L"..\\..\\..\\Resource\\Texture\\base.png";
#else
	const std::wstring filePath = L"..\\Data\\Texture\\base.png";
#endif
	//�ؽ�ó �̹����� ������ �ӽ÷� �� �⺻ base.png
	Create<ShaderResourceView>(filePath, L"base.png");

	// BackBuffer UI Image
	Create<ShaderResourceView>(L"DefaultUI", L"DefaultUI.png");
	

	// ----------------------------------------------------------------------------------------
	// Render Target View
	// ----------------------------------------------------------------------------------------
	Create<RenderTargetView>(L"RTV_Main", RenderTargetViewType::BackBuffer, width, height);
	for (int i = 0; i < m_OffScreenName.size(); i++)
	{
		Create<RenderTargetView>(m_OffScreenName[i], RenderTargetViewType::OffScreen, width, height);
	}


	// ----------------------------------------------------------------------------------------
	// Depth Stencil View
	// ----------------------------------------------------------------------------------------
	//��¿�
	D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
	dsd.Width = width;
	dsd.Height = height;
	Create<DepthStencilView>(L"DSV_Main", DepthStencilViewType::Default);
	//����Ʈ���μ��̿����� �ؽ�ó�� �����Ϸ��� ����
	Create<DepthStencilView>(L"DSV_Deferred", dsd);


	// ----------------------------------------------------------------------------------------
	// Constant Buffer
	// ----------------------------------------------------------------------------------------
	m_Camera = Create<ConstantBuffer<CameraData>>(L"Camera", ConstantBufferType::Default);
	m_Transform = Create<ConstantBuffer<TransformData>>(L"Transform", ConstantBufferType::Default);
	m_UsingLights = Create<ConstantBuffer<LightArray>>(L"LightArray", ConstantBufferType::Default);
	m_UsingMaterial = Create<ConstantBuffer<MaterialData>>(L"MaterialData", ConstantBufferType::Default);
 	m_Pallete = Create<ConstantBuffer<MatrixPallete>>(L"MatrixPallete", ConstantBufferType::Default);
	Create<ConstantBuffer<VPMath::XMFLOAT4>>(L"TexelSize", ConstantBufferType::Default);

	m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));
	m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());
	m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, m_Pallete.lock()->GetAddress());

	m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));
	m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());
	m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, m_UsingMaterial.lock()->GetAddress());
	m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, m_UsingLights.lock()->GetAddress());
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
	//	//��¿� backbuffer
	Create<RenderTargetView>(L"RTV_Main", RenderTargetViewType::BackBuffer, width, height);

	//���۵��
	//�̸� �ٲ���ҵ�
	//�迭�� ��Ƴ��� ������
	for (int i = 0; i < m_OffScreenName.size(); i++)
	{
		//������ ������ �����
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

		//Create<DepthStencilView>(L"DSV_Main", texDesc);
		Create<DepthStencilView>(L"DSV_Main", DepthStencilViewType::Default);
		Create<DepthStencilView>(L"DSV_Deferred", texDesc);
	}
}
