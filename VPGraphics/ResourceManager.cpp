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

ResourceManager::ResourceManager(std::weak_ptr<Device> device) : m_Device(device), m_Camera(), m_DirectionalLight()
{
	m_OffScreenName[0] = L"Albedo";
	m_OffScreenName[1] = L"Normal";
	m_OffScreenName[2] = L"Position";
	m_OffScreenName[3] = L"Depth";
	m_OffScreenName[4] = L"Metalic";
	m_OffScreenName[5] = L"Roughness";
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
			//delete resource.second;
		}

		m_ResourceArray[i].clear();
	}


}

void ResourceManager::Initialize()
{
	///Built in

	// Depth Stencil State
	{
		Create<DepthStencilState>(L"DefaultDSS", DepthStencilStateType::Default);
		Create<DepthStencilState>(L"DisableDepth", DepthStencilStateType::DisableDepth);
		Create<DepthStencilState>(L"NoDepthWrites", DepthStencilStateType::NoDepthWrites);
	}

	// Blend State
	{
		Create<BlendState>(L"AdditiveBlending", BlendStateType::AdditiveBlending);
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

	}

	//IB
	{
		Create<IndexBuffer>(L"Quad_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	}

	//SRV
	{
		//텍스처 이미지가 없으면 임시로 쓸 기본 base.png
		Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");
	}

	//RTV
	{
		//출력용 backbuffer
		Create<RenderTargetView>(L"RTV_Main");

		D3D11_TEXTURE2D_DESC texDesc = TextureDESC::OffScreen;
		texDesc.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
		texDesc.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

		//Deferred용
		for (int i = 0; i < m_OffScreenName.size(); i++)
		{
			std::weak_ptr<Texture2D> offscreenTex = Create<Texture2D>(m_OffScreenName[i], texDesc);
			Create<RenderTargetView>(m_OffScreenName[i], offscreenTex);
		}

	}

	//DSV
	{
		//출력용
		D3D11_TEXTURE2D_DESC dsd = TextureDESC::DSVDesc;
		dsd.Width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
		dsd.Height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;
		Create<DepthStencilView>(L"DSV_Main", dsd);

		//포스트프로세싱용으로 텍스처를 저장하려고 쓸거
		Create<DepthStencilView>(L"DSV_Deferred", dsd);
	}

	//CB
	{
		m_Camera = Create<ConstantBuffer<CameraData>>(L"Camera", BufferDESC::Constant::DefaultCamera);
		m_Transform = Create<ConstantBuffer<TransformData>>(L"Transform", BufferDESC::Constant::DefaultTransform);
		m_UsingLights = Create<ConstantBuffer<LightArray>>(L"LightArray", BufferDESC::Constant::DefaultLightArray);
		m_UsingMaterial = Create<ConstantBuffer<MaterialData>>(L"MaterialData", BufferDESC::Constant::DefaultMaterial);
		m_Pallete = Create<ConstantBuffer<MatrixPallete>>(L"MatrixPallete", BufferDESC::Constant::DefaultMatrixPallete);

		//Bind CB only Once
		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));
		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, (m_Camera.lock()->GetAddress()));

		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());
		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, m_Transform.lock()->GetAddress());

		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, m_UsingMaterial.lock()->GetAddress());

		m_Device.lock()->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, m_UsingLights.lock()->GetAddress());

		m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, m_Pallete.lock()->GetAddress());
	}

	//Axis Grid Box
	CreateAxisGridBox();

}


void ResourceManager::OnResize(RECT& wndsize)
{

	Erase<ViewPort>(L"Main");
	Create<ViewPort>(L"Main", wndsize);

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

		/*
#pragma region SUMIN
		if (m_OffScreenName[i] == L"IMGUI")
		{
			Create<ShaderResourceView>(m_OffScreenName[i], Get<RenderTargetView>(L"RTV_Main").lock());
			continue;
		}
#pragma endregion SUMIN
		 */

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

void ResourceManager::CreateAxisGridBox()
{
	//Axis
	{
		std::shared_ptr<ModelData> AxisModel = std::make_shared<ModelData>();
		AxisModel->m_name = L"Axis";
		AxisModel->RS = Get<RenderState>(L"Wire");

		AxisModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

		AxisModel->m_Meshes[0]->m_primitive = Axis::PRIMITIVE_TOPOLOGY;

		UINT size = static_cast<UINT>(sizeof(BaseVertex));
		AxisModel->m_Meshes[0]->m_VB = Create<VertexBuffer>(AxisModel->m_name + L"_VB", Axis::Vertex::Desc, Axis::Vertex::Data, size);
		AxisModel->m_Meshes[0]->m_IB = Create<IndexBuffer>(AxisModel->m_name + L"_IB", Axis::Index::Desc, Axis::Index::Data, Axis::Index::count);

		Add<ModelData>(L"Axis", AxisModel);
	}

	//Grid
	{
		std::shared_ptr<ModelData> GridModel = std::make_shared<ModelData>();
		GridModel->m_name = L"Grid";
		GridModel->RS = Get<RenderState>(L"Wire");

		//범위 -100~100
		int row = 200;
		int col = 200;

		std::vector<BaseVertex> vertexBuffer;

		//울타리 말뚝 오류 == +1
		for (int i = 0; i < row + 1; i++)
		{
			BaseVertex gridstart{};
			gridstart.pos = DirectX::XMFLOAT4(-100.f, 0, -100.f + i, 1.0f);
			gridstart.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexBuffer.push_back(gridstart);

			BaseVertex gridend{};
			gridend.pos = DirectX::XMFLOAT4(100.f, 0, -100.f + i, 1.0f);
			gridend.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexBuffer.push_back(gridend);
		}

		for (int j = 0; j < col + 1; j++)
		{

			BaseVertex gridstart{};
			gridstart.pos = DirectX::XMFLOAT4(-100.f + j, 0, -100, 1.0f);
			gridstart.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexBuffer.push_back(gridstart);

			BaseVertex gridend{};
			gridend.pos = DirectX::XMFLOAT4(-100.f + j, 0, 100, 1.0f);
			gridend.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexBuffer.push_back(gridend);
		}

		std::vector<UINT> indexList;

		int GridSize = (col + 1 + row + 1) * 2;

		for (int index = 0; index < GridSize; ++index)
		{
			indexList.push_back(index);
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(BaseVertex) * static_cast<int>(vertexBuffer.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &(vertexBuffer[0]);

		GridModel->m_Meshes.push_back(std::make_shared<StaticMesh>());
		UINT size = static_cast<UINT>(sizeof(BaseVertex));
		GridModel->m_Meshes[0]->m_VB = Create<VertexBuffer>(GridModel->m_name + L"_VB", vbd, data, size);


		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * static_cast<int>(indexList.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(indexList[0]);

		GridModel->m_Meshes[0]->m_IB = Create<IndexBuffer>(GridModel->m_name + L"_IB", ibd, iinitData, static_cast<int>(indexList.size()));
		GridModel->m_Meshes[0]->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		Add<ModelData>(L"Grid", GridModel);
	}

	//Box
	{
		std::shared_ptr<ModelData> BoxModel = std::make_shared<ModelData>();

		BoxModel->m_name = L"Box";
		BoxModel->RS = Get<RenderState>(L"Solid");

		BoxModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

		UINT size = static_cast<UINT>(sizeof(BaseVertex));

		BoxModel->m_Meshes[0]->m_VB = Create<VertexBuffer>(L"Box_VB", Box::Vertex::Desc, Box::Vertex::Data, size);
		BoxModel->m_Meshes[0]->m_IB = Create<IndexBuffer>(L"Box_IB", Box::Index::Desc, Box::Index::Data, Box::Index::count);

		BoxModel->m_Meshes[0]->m_primitive = Box::PRIMITIVE_TOPOLOGY;

		Add<ModelData>(L"AABB", BoxModel);
		Add<ModelData>(L"Box", BoxModel);
	}

	//Circle
	{
		std::shared_ptr<ModelData> CircleModel = std::make_shared<ModelData>();

		CircleModel->m_name = L"Circle";
		CircleModel->RS = Get<RenderState>(L"Solid");

		CircleModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

		UINT size = static_cast<UINT>(sizeof(BaseVertex));

		int segmentCount = 60;
		int radius = 1;
		const float angleStep = DirectX::XM_2PI / segmentCount; //2pi = 360

		std::vector<BaseVertex> vertices;

		//xy
		for (int i = 0; i <= segmentCount; ++i)
		{
			float angle = i * angleStep;
			float x = radius * cosf(angle);
			float y = radius * sinf(angle);

			BaseVertex temp1;
			temp1.pos = DirectX::XMFLOAT4(x, y, 0, 1.f);	//점은 w가 1이다 이거 잘 안지키면 안나옴
			temp1.color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			vertices.push_back(temp1);

		}

		//xz
		for (int i = 0; i <= segmentCount; ++i)
		{
			float angle = i * angleStep;
			float x = radius * cosf(angle);
			float y = radius * sinf(angle);

			BaseVertex temp1;
			temp1.pos = DirectX::XMFLOAT4(x, 0, y, 1.f);	//점은 w가 1이다 이거 잘 안지키면 안나옴 0은 벡터
			temp1.color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			vertices.push_back(temp1);

		}


		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(BaseVertex) * static_cast<int>(vertices.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &(vertices[0]);

		size = static_cast<UINT>(sizeof(BaseVertex));
		CircleModel->m_Meshes[0]->m_VB = Create<VertexBuffer>(L"Circle_VB", vbd, data, size);

		std::vector<UINT> indexList;

		for (int i = 0; i < vertices.size() / 2; i++)
		{
			indexList.push_back(i);
		}

		indexList.back() = 0;

		int verticesize = static_cast<int>(vertices.size());

		for (int i = verticesize / 2; i < verticesize; i++)
		{
			indexList.push_back(i);
		}
		indexList.back() = verticesize / 2;

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * static_cast<int>(indexList.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(indexList[0]);

		CircleModel->m_Meshes[0]->m_IB = Create<IndexBuffer>(L"Circle_IB", ibd, iinitData, static_cast<int>(indexList.size()));
		CircleModel->m_Meshes[0]->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

		Add<ModelData>(CircleModel->m_name, CircleModel);
	}
}
