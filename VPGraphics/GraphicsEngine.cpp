#include "pch.h"	
#include "GraphicsEngine.h"

#pragma region DX
#include <d3d11.h>
#include "Device.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderState.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Texture2D.h"

#include "RenderPass.h"

#include "DeferredShadingPipeline.h"
#include "ForwardPipeline.h"
#pragma endregion DX

#pragma region Util
#include "Camera.h"
#include "Desc.h"
#include "SimpleMath.h"
#include "VertexData.h"
#include "StaticData.h"
#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#pragma endregion Util

#pragma region Manager
#include "ResourceManager.h"
#include "ModelLoader.h"
#include "Animator.h"
#include "LightManager.h"
#include "DebugDrawManager.h"
#pragma endregion Manager




GraphicsEngine::GraphicsEngine(HWND hWnd) : m_Device(nullptr), m_VP(nullptr), m_ResourceManager(nullptr), m_Loader(nullptr), m_Animator(nullptr), m_hWnd(hWnd), m_wndSize()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	GetClientRect(m_hWnd, &m_wndSize);
	m_VP = std::make_shared<D3D11_VIEWPORT>();
	m_VP->TopLeftX = static_cast<float>(m_wndSize.left);
	m_VP->TopLeftY = static_cast<float>(m_wndSize.top);
	m_VP->Width = static_cast<float>(m_wndSize.right - m_wndSize.left);
	m_VP->Height = static_cast<float>(m_wndSize.bottom - m_wndSize.top);
	m_VP->MinDepth = 0.0f;
	m_VP->MaxDepth = 1.0f;

	//m_Camera = new Camera();
	m_Camera = std::make_shared<Camera>();
	m_Camera->Initialize(m_VP->Width / m_VP->Height);

	m_Device = std::make_shared<Device>(m_hWnd);

	if (m_Device == nullptr)
		return false;

	m_Device->Initialize();

	if (m_Device->SwapChain() == nullptr)
		return false;

	m_ResourceManager = std::make_shared<ResourceManager>(m_Device);
	m_ResourceManager->Initialize();

	
	m_Loader = std::make_shared <ModelLoader>(m_ResourceManager, m_Device);
	m_Loader->Initialize();
	m_LightManager = std::make_shared<LightManager>(m_ResourceManager);
	m_Animator = std::make_shared <Animator>();
	m_DebugDrawManager = std::make_shared<DebugDrawManager>();
	m_DebugDrawManager->Initialize(m_Device);
	OnResize();

	// Pipeline
	m_DeferredShadingPipeline = std::make_shared<DeferredShadingPipeline>();
	m_DeferredShadingPipeline->Initialize(m_Device, m_ResourceManager, m_DebugDrawManager, m_Camera);

	m_ForwardPipeline = std::make_shared <ForwardPipeline>(m_Device, m_ResourceManager);
	m_ForwardPipeline->Initialize();

	//output
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));

	//출력병합기
	// TODO: RTV와 뷰포트 바인딩은 이제 여기서 안하고 패스마다 필요시에 바인딩할 것임. 즉. 여기 있는거 삭제해야함.
	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_VP.get());

	AddRenderModel(MeshFilter::Axis, L"Axis");
	AddRenderModel(MeshFilter::Grid, L"Grid");
	AddRenderModel(MeshFilter::Static, L"cerberus", L"cerberus");
	//AddRenderModel(MeshFilter::Skinning, L"test", L"Flair");


	Dir.direction = DirectX::XMFLOAT3(0.f, -1.f, -1.f);
	Dir.color = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

	AddLight(L"Sun", Kind_of_Light::Direction, Dir);

	return true;

}

void GraphicsEngine::Update(double dt)
{
	m_Camera->Update(dt);

	DirectX::XMFLOAT4X4 view = m_Camera->View();
	DirectX::XMFLOAT4X4 proj = m_Camera->Proj();

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


	std::weak_ptr<ConstantBuffer<CameraData>> test = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");

	test.lock()->m_struct.view = cb_view;
	test.lock()->m_struct.viewInverse = cb_viewInverse;
	test.lock()->m_struct.worldviewproj = cb_worldviewproj;
	test.lock()->Update();


	m_Animator->Update(dt, m_RenderList);

	m_DeferredShadingPipeline->Update(m_RenderList);
	m_ForwardPipeline->Update(m_RenderList);

	m_LightManager->Update(m_LightList);
}

bool GraphicsEngine::Finalize()
{
	//delete m_Camera;

	m_RenderList.clear();
	m_VP.reset();

	m_Device.reset();
	m_ResourceManager.reset();
	m_Loader.reset();
	m_Animator.reset();

	return true;
}

void GraphicsEngine::BeginRender()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	const DirectX::SimpleMath::Color white = { 1.f, 1.f, 1.f, 1.f };
	const DirectX::SimpleMath::Color red = { 1.f, 0.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color green = { 0.f, 1.f, 0.f, 1.f };
	const DirectX::SimpleMath::Color blue = { 0.f, 0.f, 1.f, 1.f };

	m_Device->BeginRender(m_RTVs[0].lock()->Get(), m_DSVs[0].lock()->Get(), Black);
	m_Device->BeginRender(m_RTVs[1].lock()->Get(), m_DSVs[0].lock()->Get(), white);
	m_Device->BeginRender(m_RTVs[2].lock()->Get(), m_DSVs[0].lock()->Get(), red);
	m_Device->BeginRender(m_RTVs[3].lock()->Get(), m_DSVs[0].lock()->Get(), green);
	m_Device->BeginRender(m_RTVs[4].lock()->Get(), m_DSVs[0].lock()->Get(), blue);
	m_Device->BeginRender(m_RTVs[5].lock()->Get(), m_DSVs[0].lock()->Get(), Black);


}

void GraphicsEngine::Render()
{
	// 디퍼드 렌더링 기법을 사용한 파이프라인.
	// 디퍼드 패스 + 포워드 패스.
	m_DeferredShadingPipeline->Render();

	// 디퍼드 렌더링 기법을 사용하지 않은 파이프라인
	// 오로지 포워드 패스만 존재.
	m_ForwardPipeline->Render();
}

void GraphicsEngine::EndRender()
{
	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_VP.get());
	m_Device->EndRender();
}

bool GraphicsEngine::AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx)
{
	switch (mesh)
	{
		case MeshFilter::Axis:
		{
			std::shared_ptr<ModelData> AxisModel = std::make_shared<ModelData>();
			AxisModel->m_name = name;
			AxisModel->RS = m_ResourceManager->Get<RenderState>(L"Wire");

			AxisModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

			AxisModel->m_Meshes[0]->m_primitive = Axis::PRIMITIVE_TOPOLOGY;

			UINT size = static_cast<UINT>(sizeof(BaseVertex));
			AxisModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(name + L"_VB", Axis::Vertex::Desc, Axis::Vertex::Data, size);
			AxisModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(name + L"_IB", Axis::Index::Desc, Axis::Index::Data, Axis::Index::count);

			AxisModel->world = DirectX::SimpleMath::Matrix::Identity;
			AxisModel->local = DirectX::SimpleMath::Matrix::Identity;

			AxisModel->m_pass = PassState::Debug;

			m_ResourceManager->Add<ModelData>(L"Axis", AxisModel);
			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"Axis", std::pair<PassState, std::shared_ptr<ModelData>>(AxisModel->m_pass, AxisModel)));
		}
		break;
		case MeshFilter::Grid:
		{
			std::shared_ptr<ModelData> GridModel = std::make_shared<ModelData>();
			GridModel->m_name = name;
			GridModel->RS = m_ResourceManager->Get<RenderState>(L"Wire");

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
			GridModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(name + L"_VB", vbd, data, size);


			D3D11_BUFFER_DESC ibd;
			ibd.Usage = D3D11_USAGE_IMMUTABLE;
			ibd.ByteWidth = sizeof(UINT) * static_cast<int>(indexList.size());
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;
			ibd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA iinitData;
			iinitData.pSysMem = &(indexList[0]);

			GridModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(name + L"_IB", ibd, iinitData, static_cast<int>(indexList.size()));
			GridModel->m_Meshes[0]->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

			GridModel->world = DirectX::SimpleMath::Matrix::Identity;
			GridModel->local = DirectX::SimpleMath::Matrix::Identity;

			GridModel->m_pass = PassState::Debug;

			m_ResourceManager->Add<ModelData>(L"Grid", GridModel);
			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"Grid", std::pair<PassState, std::shared_ptr<ModelData>>(GridModel->m_pass, GridModel)));
		}
		break;
		case MeshFilter::Box:
		{
			std::shared_ptr<ModelData> BoxModel = std::make_shared<ModelData>();

			BoxModel->m_name = name;
			BoxModel->RS = m_ResourceManager->Get<RenderState>(L"Solid");

			BoxModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

			UINT size = static_cast<UINT>(sizeof(BaseVertex));

			BoxModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"Box_VB", Box::Vertex::Desc, Box::Vertex::Data, size);
			BoxModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"Box_IB", Box::Index::Desc, Box::Index::Data, Box::Index::count);

			BoxModel->world = DirectX::SimpleMath::Matrix::Identity;
			BoxModel->local = DirectX::SimpleMath::Matrix::Identity;

			BoxModel->m_pass = PassState::Static;

			BoxModel->m_Meshes[0]->m_primitive = Box::PRIMITIVE_TOPOLOGY;
			m_ResourceManager->Add<ModelData>(L"AABB", BoxModel);

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"AABB", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Static, BoxModel)));
		}
		break;
		case MeshFilter::TextureBox:
		{
			std::shared_ptr<ModelData> BoxModel = std::make_shared<ModelData>();

			BoxModel->RS = (m_ResourceManager->Get<RenderState>(L"Solid"));

			BoxModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

			UINT size = static_cast<UINT>(sizeof(BaseVertex));
			BoxModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"TextureBox_VB", TextureBox::Vertex::Desc, TextureBox::Vertex::Data, size);
			BoxModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"TextureBox_IB", TextureBox::Index::Desc, TextureBox::Index::Data, TextureBox::Index::count);


			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; //동적 리소스 만들거면 dynamic , upadate시 map,unmap 사용
			bufferDesc.ByteWidth = sizeof(TransformData); // 상수 버퍼의 크기는 상수 데이터의 크기와 같아야 합니다.
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;	//동적 리소스면 write
			bufferDesc.MiscFlags = 0;

			BoxModel->world = DirectX::SimpleMath::Matrix::Identity;
			BoxModel->local = DirectX::SimpleMath::Matrix::Identity;

			BoxModel->m_Materials.push_back(std::make_shared<Material>(m_Device));
			BoxModel->m_Materials[0]->m_AlbedoSRV = m_ResourceManager->Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");
			BoxModel->m_Materials[0]->m_NormalSRV = m_ResourceManager->Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");

			BoxModel->m_Meshes[0]->m_primitive = TextureBox::PRIMITIVE_TOPOLOGY;

			BoxModel->m_pass = PassState::Static;

			m_ResourceManager->Add<ModelData>(L"TextureBox", BoxModel);
			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"TextureBox", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Static, BoxModel)));

		}
		break;
		case MeshFilter::Static:
		{
			std::wstring fbxpath = fbx + L".fbx";
			std::shared_ptr<ModelData> newModel = std::make_shared<ModelData>(m_ResourceManager->Get<ModelData>(fbxpath).lock());
			newModel->m_name = name;

			newModel->RS = m_ResourceManager->Get<RenderState>(L"Solid");
			//newModel->m_pass = PassState::Static;
			newModel->m_pass = PassState::Deferred;

			newModel->local = DirectX::SimpleMath::Matrix::Identity;
			newModel->local._11 *= 0.05f;
			newModel->local._22 *= 0.05f;
			newModel->local._33 *= 0.05f;

			newModel->world = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"static", std::pair<PassState, std::shared_ptr<ModelData>>(newModel->m_pass, newModel)));
		}
		break;

		case MeshFilter::Skinning:
		{
			std::wstring fbxpath = fbx + L".fbx";
			std::shared_ptr<ModelData> newModel = std::make_shared<ModelData>(m_ResourceManager->Get<ModelData>(fbxpath).lock());
			newModel->m_name = name;

			newModel->RS = (m_ResourceManager->Get<RenderState>(L"Solid"));
			newModel->m_pass = PassState::Skinning;

			newModel->world = DirectX::SimpleMath::Matrix::Identity;
			newModel->world._11 *= 0.05f;
			newModel->world._22 *= 0.05f;
			newModel->world._33 *= 0.05f;

			newModel->local = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(name, std::pair<PassState, std::shared_ptr<ModelData>>(newModel->m_pass, newModel)));
			m_ResourceManager->Create<ConstantBuffer<MatrixPallete>>(name + L"MatrixPallete", BufferDESC::Constant::DefaultMatrixPallete);
		}
		break;

		case MeshFilter::Circle:
		{
			std::shared_ptr<ModelData> CircleModel = std::make_shared<ModelData>();

			CircleModel->m_name = name;
			CircleModel->RS = m_ResourceManager->Get<RenderState>(L"Solid");

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
			CircleModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"Circle_VB", vbd, data, size);

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

			CircleModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"Circle_IB", ibd, iinitData, static_cast<int>(indexList.size()));
			CircleModel->m_Meshes[0]->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

			CircleModel->world = DirectX::SimpleMath::Matrix::Identity;
			CircleModel->local = DirectX::SimpleMath::Matrix::Identity;

			CircleModel->m_pass = PassState::Debug;

			m_ResourceManager->Add<ModelData>(name, CircleModel);
			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(name, std::pair<PassState, std::shared_ptr<ModelData>>(CircleModel->m_pass, CircleModel)));
		}

		break;
		case MeshFilter::None:
			break;
		default:
			break;
	}

	return true;
}

void GraphicsEngine::EraseObject(std::wstring name)
{
	m_RenderList.erase(name);
}

void GraphicsEngine::SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_View = view;
	m_Proj = proj;
	m_ViewProj = view * proj;

	DirectX::XMFLOAT4X4 cb_worldviewproj;
	DirectX::XMFLOAT4X4 cb_view;
	DirectX::XMFLOAT4X4 cb_proj;
	DirectX::XMFLOAT4X4 cb_viewInverse;
	cb_worldviewproj = m_ViewProj;

	//상수 버퍼는 계산 순서때문에 전치한다
	XMStoreFloat4x4(&cb_worldviewproj, XMMatrixTranspose(m_ViewProj));
	XMStoreFloat4x4(&cb_view, XMMatrixTranspose(m_View));
	XMStoreFloat4x4(&cb_proj, XMMatrixTranspose(m_Proj));

	DirectX::XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
	XMStoreFloat4x4(&cb_viewInverse, XMMatrixTranspose(viewInverse));

	std::weak_ptr<ConstantBuffer<CameraData>> Camera = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera");
	Camera.lock()->m_struct.view = cb_view;
	Camera.lock()->m_struct.viewInverse = cb_viewInverse;
	Camera.lock()->m_struct.worldviewproj = cb_worldviewproj;
	Camera.lock()->Update();
}

void GraphicsEngine::UpdateModelTransform(std::wstring name, DirectX::SimpleMath::Matrix world)
{
	m_RenderList[name].second->world = world;
}

void GraphicsEngine::AddLight(std::wstring name, Kind_of_Light kind, LightData data)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData>& curMap = m_LightList[index];

	if (curMap.find(name) == curMap.end())
	{
		curMap.insert(std::pair<std::wstring, LightData>(name, data));
		AddRenderModel(MeshFilter::Circle, name);

		curMap[name] = data;
		std::shared_ptr<ModelData> debugmodel = m_ResourceManager->Get<ModelData>(name).lock();

		//scale
		if (data.range > 0)
		{
			debugmodel->local._11 = data.range;
			debugmodel->local._22 = data.range;
			debugmodel->local._33 = data.range;
		}

		//상수버퍼로 넘겨야하니까 전치해서 보내버리자
		//translate
		debugmodel->local._14 = data.pos.x;
		debugmodel->local._24 = data.pos.y;
		debugmodel->local._34 = data.pos.z;

	}
	else
	{
		MessageBox(0, L"This Light already Exist", 0, 0);
		return;
	}

}


void GraphicsEngine::EraseLight(std::wstring name, Kind_of_Light kind)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData>& curMap = m_LightList[index];

	if (curMap.find(name) != curMap.end())
	{
		curMap.erase(name);
		EraseObject(name);
	}
	else
	{
		MessageBox(0, L"This Light Is Not Exist", 0, 0);
		return;
	}
}

void GraphicsEngine::UpdateLightData(std::wstring name, Kind_of_Light kind, LightData data)
{
	int index = static_cast<int>(kind);
	std::unordered_map <std::wstring, LightData> curMap = m_LightList[index];

	if (curMap.find(name) != curMap.end())
	{
		curMap[name] = data;
		m_ResourceManager->Get<ModelData>(name).lock()->local._31 = data.pos.x;
		m_ResourceManager->Get<ModelData>(name).lock()->local._32 = data.pos.y;
		m_ResourceManager->Get<ModelData>(name).lock()->local._33 = data.pos.z;

		if (data.range > 0)
		{
			m_ResourceManager->Get<ModelData>(name).lock()->local * data.range;
		}
	}
	else
	{
		MessageBox(0, L"This Light Is Not Exist", 0, 0);
		return;
	}
}

void GraphicsEngine::DrawSphere(const debug::SphereInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawBox(const debug::AABBInfo& info)
{

}

void GraphicsEngine::DrawOBB(const debug::OBBInfo& info)
{

}

void GraphicsEngine::DrawFrustum(const debug::FrustumInfo& info)
{

}

void GraphicsEngine::DrawGrid(const debug::GridInfo& info)
{

}

void GraphicsEngine::DrawRing(const debug::RingInfo& info)
{

}

void GraphicsEngine::DrawTriangle(const debug::TriangleInfo& info)
{

}

void GraphicsEngine::DrawQuad(const debug::QuadInfo& info)
{

}

void GraphicsEngine::DrawRay(const debug::RayInfo& info)
{

}

void GraphicsEngine::OnResize()
{
	m_VP.reset();

	GetClientRect(m_hWnd, &m_wndSize);
	m_VP = std::make_shared<D3D11_VIEWPORT>();
	m_VP->TopLeftX = static_cast<float>(m_wndSize.left);
	m_VP->TopLeftY = static_cast<float>(m_wndSize.top);
	m_VP->Width = static_cast<float>(m_wndSize.right - m_wndSize.left);
	m_VP->Height = static_cast<float>(m_wndSize.bottom - m_wndSize.top);
	m_VP->MinDepth = 0.0f;
	m_VP->MaxDepth = 1.0f;

	m_RTVs.clear();
	m_DSVs.clear();



	m_Device->OnResize();
	m_ResourceManager->OnResize();

	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Albedo"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Normal"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Position"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Depth"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Tangent"));

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));



	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_VP.get());
}

