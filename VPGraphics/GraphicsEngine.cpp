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

#pragma region IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#pragma endregion IMGUI

GraphicsEngine::GraphicsEngine(HWND hWnd) : m_Device(nullptr), m_CurViewPort(nullptr), m_ResourceManager(nullptr), m_Loader(nullptr), m_Animator(nullptr), m_hWnd(hWnd), m_wndSize()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	GetClientRect(m_hWnd, &m_wndSize);

	m_Device = std::make_shared<Device>(m_hWnd);

	if (m_Device == nullptr)
		return false;

	m_Device->Initialize();

	if (m_Device->SwapChain() == nullptr)
		return false;

	m_ResourceManager = std::make_shared<ResourceManager>(m_Device);
	m_ResourceManager->Initialize();
	m_CurViewPort = m_ResourceManager->Create<ViewPort>(L"Main", m_wndSize).lock();

	m_Loader = std::make_shared <ModelLoader>(m_ResourceManager, m_Device);
	m_Loader->Initialize();
	m_LightManager = std::make_shared<LightManager>(m_ResourceManager);
	m_Animator = std::make_shared <Animator>();
	m_DebugDrawManager = std::make_shared<DebugDrawManager>();
	m_DebugDrawManager->Initialize(m_Device);
	OnResize();

	// Pipeline
	m_DeferredShadingPipeline = std::make_shared<DeferredShadingPipeline>();
	m_DeferredShadingPipeline->Initialize(m_Device, m_ResourceManager, m_DebugDrawManager, m_View, m_Proj);

	m_ForwardPipeline = std::make_shared <ForwardPipeline>(m_Device, m_ResourceManager);
	m_ForwardPipeline->Initialize();

	//output
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));

	AddRenderModel(MeshFilter::Axis, L"Axis");
	AddRenderModel(MeshFilter::Grid, L"Grid");
	//AddRenderModel(MeshFilter::Skinning, L"test", L"Flair");
	AddRenderModel(MeshFilter::Static, L"cerberus", L"cerberus");
	AddRenderModel(MeshFilter::Static, L"engine_sizedown_1", L"engine_sizedown_1");

	Dir.direction = DirectX::XMFLOAT3(0.f, -1.f, 1.f);
	Dir.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir.intensity = 1;

	LightData Dir2;
	Dir2.direction = DirectX::XMFLOAT3(0.f, 0.f, -1.f);
	Dir2.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir2.intensity = 10;

	LightData Dir3;
	Dir2.direction = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
	Dir2.color = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Dir2.intensity = 10;


	Point.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Point.color = DirectX::XMFLOAT3(1, 0, 0);
	Point.intensity = 1;
	Point.pos = DirectX::XMFLOAT3(0, 3, -2);
	Point.range = 10.f;

	LightData Point2;
	Point2.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Point2.color = DirectX::XMFLOAT3(1, 1, 1);
	Point2.intensity = 1;
	Point2.pos = DirectX::XMFLOAT3(25, 3, -2);
	Point2.range = 100;

	Spot.attenuation = DirectX::XMFLOAT3(0, 0, 1);
	Spot.color = DirectX::XMFLOAT3(1, 1, 1);
	Spot.intensity = 100;
	Spot.pos = DirectX::XMFLOAT3(0, 3, -2);
	Spot.range = 100;
	Spot.spot = 8;

	AddLight(L"Sun", Kind_of_Light::Direction, Dir);
	//AddLight(L"Sun2", Kind_of_Light::Direction, Dir2);
	//AddLight(L"Sun3", Kind_of_Light::Direction, Dir3);
	//AddLight(L"point", Kind_of_Light::Point, Point);
	//AddLight(L"point2", Kind_of_Light::Point, Point2);
	//AddLight(L"point", Kind_of_Light::Spot, Spot);


	InitializeImGui();
	return true;

}

void GraphicsEngine::Update(double dt)
{
	m_Animator->Update(dt, m_RenderList);

	m_DeferredShadingPipeline->Update(m_RenderList);
	m_ForwardPipeline->Update(m_RenderList);

	m_LightManager->Update(m_LightList);
}

bool GraphicsEngine::Finalize()
{
	m_RenderList.clear();
	m_CurViewPort.reset();

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
	m_Device->BeginRender(m_RTVs[1].lock()->Get(), m_DSVs[1].lock()->Get(), white);
	m_Device->BeginRender(m_RTVs[2].lock()->Get(), m_DSVs[1].lock()->Get(), red);
	m_Device->BeginRender(m_RTVs[3].lock()->Get(), m_DSVs[1].lock()->Get(), green);
	m_Device->BeginRender(m_RTVs[4].lock()->Get(), m_DSVs[1].lock()->Get(), blue);
	m_Device->BeginRender(m_RTVs[5].lock()->Get(), m_DSVs[1].lock()->Get(), blue + red);
	m_Device->BeginRender(m_RTVs[6].lock()->Get(), m_DSVs[1].lock()->Get(), green + red);
	m_Device->BeginRender(m_RTVs[7].lock()->Get(), m_DSVs[1].lock()->Get(), Black);
	m_Device->BeginRender(m_RTVs[8].lock()->Get(), m_DSVs[1].lock()->Get(), blue + green);
	m_Device->BeginRender(m_RTVs[9].lock()->Get(), m_DSVs[1].lock()->Get(), blue + green);
}

void GraphicsEngine::Render()
{
	// 디퍼드 렌더링 기법을 사용한 파이프라인.
	// 디퍼드 패스 + 포워드 패스.

	//m_DeferredShadingPipeline->Render();

	// 디퍼드 렌더링 기법을 사용하지 않은 파이프라인
	// 오로지 포워드 패스만 존재.
	m_ForwardPipeline->Render();

	BeginImGui();

}

void GraphicsEngine::EndRender()
{
	EndImGui();

	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
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

			BoxModel->m_pass = PassState::Debug;

			BoxModel->m_Meshes[0]->m_primitive = Box::PRIMITIVE_TOPOLOGY;
			m_ResourceManager->Add<ModelData>(L"AABB", BoxModel);

			BoxModel->m_Materials.push_back(std::make_shared<Material>(m_Device));
			BoxModel->m_Materials.back()->m_Data.useAMRO = DirectX::XMFLOAT4(0, 0, 0, 0);
			BoxModel->m_Materials.back()->m_Data.useNE.x = 0;

			m_ResourceManager->Add<ModelData>(L"Box", BoxModel);

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(name, std::pair<PassState, std::shared_ptr<ModelData>>(BoxModel->m_pass, BoxModel)));
		}
			break;

		case MeshFilter::Static:
		{
			std::wstring fbxpath = fbx + L".fbx";
			std::shared_ptr<ModelData> newModel = std::make_shared<ModelData>(m_ResourceManager->Get<ModelData>(fbxpath).lock());
			newModel->m_name = name;

			newModel->RS = m_ResourceManager->Get<RenderState>(L"Solid");
			newModel->m_pass = PassState::Deferred;

			newModel->local = DirectX::SimpleMath::Matrix::Identity;
			newModel->local._11 *= 0.03f;
			newModel->local._22 *= 0.03f;
			newModel->local._33 *= 0.03f;

			newModel->world = DirectX::SimpleMath::Matrix::Identity;

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(name, std::pair<PassState, std::shared_ptr<ModelData>>(newModel->m_pass, newModel)));
		}
			break;

		case MeshFilter::Skinning:
		{
			std::wstring fbxpath = fbx + L".fbx";
			std::shared_ptr<ModelData> newModel = std::make_shared<ModelData>(m_ResourceManager->Get<ModelData>(fbxpath).lock());
			newModel->m_name = name;

			newModel->RS = (m_ResourceManager->Get<RenderState>(L"Solid"));
			newModel->m_pass = PassState::Deferred;

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
			m_ResourceManager->Get<ModelData>(name).lock()->local* data.range;
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
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawOBB(const debug::OBBInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawFrustum(const debug::FrustumInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawGrid(const debug::GridInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawRing(const debug::RingInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawTriangle(const debug::TriangleInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawQuad(const debug::QuadInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

void GraphicsEngine::DrawRay(const debug::RayInfo& info)
{
	m_DebugDrawManager->AddTask(info);
}

ID3D11ShaderResourceView* GraphicsEngine::GetSRV(std::wstring name)
{
	return m_ResourceManager->Get<ShaderResourceView>(name).lock()->Get();
}

void GraphicsEngine::OnResize()
{
	GetClientRect(m_hWnd, &m_wndSize);

	m_RTVs.clear();
	m_DSVs.clear();

	m_Device->OnResize();
	m_ResourceManager->OnResize(m_wndSize);
	m_CurViewPort = m_ResourceManager->Create<ViewPort>(L"Main", m_wndSize).lock();


	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_Main"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Albedo"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Normal"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Position"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Depth"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Metalic"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Roughness"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"AO"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"Emissive"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"GBuffer"));

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Main"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_Deferred"));



	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_CurViewPort->Get());
}



void GraphicsEngine::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_Device->Get(), m_Device->Context());


}

void GraphicsEngine::BeginImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void GraphicsEngine::EndImGui()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
void GraphicsEngine::DestroyImGui()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}