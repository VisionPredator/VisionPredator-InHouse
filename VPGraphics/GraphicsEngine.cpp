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
	m_VP = new D3D11_VIEWPORT();
	m_VP->TopLeftX = static_cast<float>(m_wndSize.left);
	m_VP->TopLeftY = static_cast<float>(m_wndSize.top);
	m_VP->Width = static_cast<float>(m_wndSize.right - m_wndSize.left);
	m_VP->Height = static_cast<float>(m_wndSize.bottom - m_wndSize.top);
	m_VP->MinDepth = 0.0f;
	m_VP->MaxDepth = 1.0f;

	m_Camera = new Camera();
	m_Camera->Initialize(m_VP->Width / m_VP->Height);

	m_Device = std::make_shared<Device>(m_hWnd);
	if (m_Device != nullptr)
	{
		m_Device->Initialize();
		m_ResourceManager = std::make_shared<ResourceManager>(m_Device);
		m_ResourceManager->Initialize();

		m_Loader = std::make_shared <ModelLoader>(m_ResourceManager,m_Device);
		m_Loader->Initialize();

		m_Animator = std::make_shared <Animator>();

		if (m_Device->SwapChain())
		{
			//output
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_1"));

			//deferred
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_2"));
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_3"));
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_4"));
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_5"));
			m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_6"));

			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_1"));
			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_2"));
			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_3"));
			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_4"));
			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_5"));
			m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_6"));

			//출력병합기
			m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
			m_Device->Context()->RSSetViewports(1, m_VP);


			//test
			m_BasePass = new ForwardPass(m_Device, m_ResourceManager, m_VP);
			m_TexturePass = new TexturePass(m_Device, m_ResourceManager, m_VP);
			m_SkinningPass = new SkinnigPass(m_Device, m_ResourceManager, m_VP);

			LoadResource(MeshFilter::Axis, L"Axis");
			LoadResource(MeshFilter::Grid, L"Grid");
			LoadResource(MeshFilter::TextureBox, L"TextureBox");
			LoadResource(MeshFilter::Skinning, L"test",L"Flair");



			return true;
		}
	}


	return false;
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


	std::weak_ptr<ConstantBuffer<CameraCB>> test = m_ResourceManager->Get<ConstantBuffer<CameraCB>>(L"Camera");
	test.lock()->m_struct.view = cb_view;
	test.lock()->m_struct.viewInverse = cb_viewInverse;
	test.lock()->m_struct.worldviewproj = cb_worldviewproj;
	test.lock()->Update();


	m_Animator->Update(dt,m_RenderList);


	//비트 연산으로 해보자

	for (auto& model : m_RenderList)
	{
		PassState temp = model.second.first;

		temp &= PassState::Base;
		if (temp == PassState::Base)
		{
			m_BasePass->AddModelData(model.second.second);

		}

		temp = model.second.first;
		temp &= PassState::Texture;
		if (temp == PassState::Texture)
		{
			m_TexturePass->AddModelData(model.second.second);

		}

		temp = model.second.first;
		temp &= PassState::Skinning;
		if (temp == PassState::Skinning)
		{
			m_SkinningPass->AddModelData(model.second.second);
		}

	}







}

bool GraphicsEngine::Finalize()
{
	//delete m_Loader;
	//m_ResourceManager;

	delete m_Camera;
	delete m_VP;

	m_Device.reset();
	m_ResourceManager.reset();
	m_Loader.reset();
	m_Animator.reset();

	//test
	delete m_TexturePass;
	delete m_SkinningPass;
	delete m_BasePass;

	return true;
}

void GraphicsEngine::Render()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	m_Device->BeginRender(m_RTVs[0].lock()->Get(), m_DSVs[0].lock()->Get(), Black);

	m_BasePass->StaticRender();
	m_TexturePass->StaticRender();
	m_SkinningPass->Render();

	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_VP);
	m_Device->EndRender();
}

void GraphicsEngine::DeferredRender()
{
	//m_Device->BeginDeferredRender(m_RTVs, m_DSVs[0]->Get());
	//m_Device->Context()->RSSetViewports(1, m_VP);

	//ID3D11RenderTargetView* MRT[5];
	//MRT[0] = m_RTVs[1]->Get(); //albedo
	//MRT[1] = m_RTVs[2]->Get(); //normal
	//MRT[2] = m_RTVs[3]->Get(); //position
	//MRT[3] = m_RTVs[4]->Get(); //depth
	//MRT[4] = m_RTVs[5]->Get(); //tangent

	//std::weak_ptr<ShaderResourceView> SRVS[5];
	//SRVS[0] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_1");
	//SRVS[1] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_2");
	//SRVS[2] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_3");
	//SRVS[3] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_4");
	//SRVS[4] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_5");

	////렌더타겟 바인딩
	//m_Device->Context()->OMSetRenderTargets(5, MRT, m_DSVs[0]->Get());
	//{
	//	m_Device->Context()->PSSetShader(m_ResourceManager->Create<PixelShader>(L"../x64/Debug/GeoMetryPS.cso", L"GeoMetry").lock()->GetPS(), nullptr, 0);

	//	/*for (auto& ob : m_FowardRenderObjects)
	//	{
	//		m_Device->DeferredRender(ob.second, m_RTVs, m_DSVs);
	//	}*/

	//	//m_device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
	//}


	////ImGui::Text("test");
	//float aspectRatio = 16.0f / 9.0f;
	//float newWidth = 300 * aspectRatio;

	//m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0]->GetAddress(), m_DSVs[0]->Get());

	////pass2
	////UINT size = static_cast<UINT>(sizeof(QuadVertex));
	////VertexBuffer* vb = m_ResourceManager->Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	////IndexBuffer* ib = m_ResourceManager->Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	////ConstantBuffer<WorldTransformCB>* cb = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"QuadTransform", BufferDESC::Constant::DefaultWorld);
	////VertexShader* vs = m_ResourceManager->Create<VertexShader>(L"../x64/Debug/QuadVS.cso", VERTEXFILTER::QUAD, L"Quad");
	////PixelShader* ps = m_ResourceManager->Create<PixelShader>(L"../x64/Debug/DeferredPS.cso", L"Deferred");
	////RenderState* rs = m_ResourceManager->Get<RenderState>(L"Solid");
	////
	////
	////m_device->Context()->IASetInputLayout(vs->InputLayout());
	////
	////m_device->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	////m_device->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);
	////
	////m_device->Context()->IASetPrimitiveTopology(Quad::PRIMITIVE_TOPOLOGY);
	////m_device->Context()->RSSetState(rs->Get());
	////
	////m_device->Context()->VSSetShader(vs->GetVS(), nullptr, 0);
	////
	////
	////m_device->Context()->VSSetConstantBuffers(1, 1, cb->GetAddress());
	////m_device->Context()->PSSetConstantBuffers(0, 1, cb->GetAddress());
	////
	////for (int i = 0; i < 5; i++)
	////{
	////	m_device->Context()->PSSetShaderResources(i, 1, SRVS[i]->GetAddress());
	////}
	////
	////m_device->Context()->PSSetSamplers(1, 1, SRVS[0]->GetSamplerAddress());
	////
	////m_device->Context()->PSSetShader(ps->GetPS(), nullptr, 0);
	////m_device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	////
	//m_Device->EndRender();
}

bool GraphicsEngine::LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx)
{

	//std::wstring VSPath = L"../x64/Debug/" + VSname + L"VS.cso";
	//std::wstring PSPath = L"../x64/Debug/" + PSname + L"PS.cso";

	//계속 늘어나면 어쩔거임? 한정적이지 않을까?
	//생각보다 한정적이다

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

			AxisModel->m_pass = PassState::Base;

			m_ResourceManager->Add<ModelData>(L"Axis", AxisModel);
			m_RenderList.insert(std::pair<std::wstring,std::pair<PassState, std::shared_ptr<ModelData>>>(L"Axis", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Base, AxisModel)));
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

			GridModel->m_pass = PassState::Base;

			m_ResourceManager->Add<ModelData>(L"Grid", GridModel);
			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"Grid", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Base, GridModel)));
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

			BoxModel->m_pass = PassState::Base;

			BoxModel->m_Meshes[0]->m_primitive = Box::PRIMITIVE_TOPOLOGY;
			m_ResourceManager->Add<ModelData>(L"Box", BoxModel);

			m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"Box", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Base, BoxModel)));
		}
		break;
		case MeshFilter::TextureBox:
			{
				std::shared_ptr<ModelData> BoxModel = std::make_shared<ModelData>();

				BoxModel->RS = (m_ResourceManager->Get<RenderState>(L"Solid"));

				BoxModel->m_Meshes.push_back(std::make_shared<StaticMesh>());

				UINT size = static_cast<UINT>(sizeof(TextureVertex));
				BoxModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"TextureBox_VB", TextureBox::Vertex::Desc, TextureBox::Vertex::Data, size);
				BoxModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"TextureBox_IB", TextureBox::Index::Desc, TextureBox::Index::Data, TextureBox::Index::count);


				D3D11_BUFFER_DESC bufferDesc{};
				bufferDesc.Usage = D3D11_USAGE_DEFAULT; //동적 리소스 만들거면 dynamic , upadate시 map,unmap 사용
				bufferDesc.ByteWidth = sizeof(WorldTransformCB); // 상수 버퍼의 크기는 상수 데이터의 크기와 같아야 합니다.
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bufferDesc.CPUAccessFlags = 0;	//동적 리소스면 write
				bufferDesc.MiscFlags = 0;

				BoxModel->world = DirectX::SimpleMath::Matrix::Identity;
				BoxModel->local = DirectX::SimpleMath::Matrix::Identity;

				BoxModel->m_Materials.push_back(std::make_shared<Material>(m_Device));
				BoxModel->m_Materials[0]->m_DiffuseSRV = m_ResourceManager->Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"../Resource/Texture/base.png", SamplerDESC::Linear);
				BoxModel->m_Materials[0]->m_NormalSRV= m_ResourceManager->Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"../Resource/Texture/base.png", SamplerDESC::Linear);

				BoxModel->m_Meshes[0]->m_primitive = TextureBox::PRIMITIVE_TOPOLOGY;

				BoxModel->m_pass = PassState::Texture;

				m_ResourceManager->Add<ModelData>(L"TextureBox", BoxModel);
				m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"TextureBox", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Texture, BoxModel)));

			}
			break;
		case MeshFilter::LoadModel:
			/*{
				newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::TEXTURE, VSname));
				newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));
				newObject->Set<ModelData>(m_ResourceManager->Get<ModelData>(name));
				newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Solid"));
				newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");
				newObject->Set<LocalTransformCB>(m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(name + L"Local", BufferDESC::Constant::DefaultWorld), name + L"Local");
			}*/
			break;

		case MeshFilter::Skinning:
			{
				std::wstring fbxpath = L"../Resource/FBX/" + fbx + L".fbx";
				std::shared_ptr<ModelData> newModel = std::make_shared<ModelData>(m_ResourceManager->Get<ModelData>(fbxpath).lock());
				newModel->m_name = name;

				newModel->RS = (m_ResourceManager->Get<RenderState>(L"Solid"));
				newModel->m_pass = PassState::Skinning;

				newModel->world = DirectX::SimpleMath::Matrix::Identity;
				newModel->world._11 *= 0.05f;
				newModel->world._22 *= 0.05f;
				newModel->world._33 *= 0.05f;

				newModel->local = DirectX::SimpleMath::Matrix::Identity;

				m_RenderList.insert(std::pair<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>(L"test", std::pair<PassState, std::shared_ptr<ModelData>>(PassState::Skinning, newModel)));
				m_ResourceManager->Create<ConstantBuffer<MatrixPallete>>(name + L"MatrixPallete", BufferDESC::Constant::DefaultMatrixPallete);
			}
			break;

		case MeshFilter::None:
			break;
		default:
			break;
	}

	return true;
}


void GraphicsEngine::UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct)
{
	/*if (m_FowardRenderObjects.find(name) != m_FowardRenderObjects.end())
	{
		XMStoreFloat4x4(&constantstruct.world, XMMatrixTranspose(XMLoadFloat4x4(&constantstruct.world)));
		m_FowardRenderObjects[name]->GetCB<WorldTransformCB>(cbname)->m_struct = constantstruct;
	}*/
}

void GraphicsEngine::UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)
{
	m_ResourceManager->Get<ConstantBuffer<DirectionLightCB>>(L"DirectionLight").lock()->m_struct = constantstruct;
}

void GraphicsEngine::OnResize()
{
	delete m_VP;

	GetClientRect(m_hWnd, &m_wndSize);
	m_VP = new D3D11_VIEWPORT();
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

	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_1"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_2"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_3"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_4"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_5"));
	m_RTVs.push_back(m_ResourceManager->Get<RenderTargetView>(L"RTV_6"));

	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_1"));
	/*m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_2"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_3"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_4"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_5"));
	m_DSVs.push_back(m_ResourceManager->Get<DepthStencilView>(L"DSV_6"));*/


	m_Device->Context()->OMSetRenderTargets(1, m_RTVs[0].lock()->GetAddress(), m_DSVs[0].lock()->Get());
	m_Device->Context()->RSSetViewports(1, m_VP);
}

void GraphicsEngine::DrawQuad(ShaderResourceView* srv)
{




	UINT size = static_cast<UINT>(sizeof(QuadVertex));

	std::weak_ptr<VertexBuffer> vb = m_ResourceManager->Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	std::weak_ptr<IndexBuffer> ib = m_ResourceManager->Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	std::weak_ptr<ConstantBuffer<WorldTransformCB>> cb = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"QuadTransform", BufferDESC::Constant::DefaultWorld);

	std::weak_ptr<VertexShader> vs = m_ResourceManager->Create<VertexShader>(L"../x64/Debug/QuadVS.cso", VERTEXFILTER::QUAD, L"Quad");
	std::weak_ptr<PixelShader> ps = m_ResourceManager->Create<PixelShader>(L"../x64/Debug/QuadPS.cso", L"Quad");
	std::weak_ptr<RenderState> rs = m_ResourceManager->Get<RenderState>(L"Solid");



	m_Device->Context()->IASetInputLayout(vs.lock()->InputLayout());

	m_Device->Context()->IASetVertexBuffers(0, 1, vb.lock()->GetAddress(), vb.lock()->Size(), vb.lock()->Offset());
	m_Device->Context()->IASetIndexBuffer(ib.lock()->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device->Context()->IASetPrimitiveTopology(Quad::PRIMITIVE_TOPOLOGY);
	m_Device->Context()->RSSetState(rs.lock()->Get());

	m_Device->Context()->VSSetShader(vs.lock()->GetVS(), nullptr, 0);


	m_Device->Context()->VSSetConstantBuffers(1, 1, cb.lock()->GetAddress());
	m_Device->Context()->PSSetConstantBuffers(0, 1, cb.lock()->GetAddress());


	m_Device->Context()->PSSetShaderResources(0, 1, srv->GetAddress());
	m_Device->Context()->PSSetSamplers(0, 1, srv->GetSamplerAddress());


	m_Device->Context()->PSSetShader(ps.lock()->GetPS(), nullptr, 0);
	m_Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);





}
