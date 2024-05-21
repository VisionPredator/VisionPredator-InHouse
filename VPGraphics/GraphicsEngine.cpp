#include "pch.h"	

#include "GraphicsEngine.h"

#include "SimpleMath.h"
#include <d3d11.h>

#include "Device.h"
#include "ResourceManager.h"
#include "ModelLoader.h"
#include "Animator.h"

#include "Vertex.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "Object.h"

#include "StaticData.h"

#include "RenderState.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Desc.h"

#include "Texture2D.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


GraphicsEngine::GraphicsEngine(HWND hWnd) : m_device(nullptr), m_VP(nullptr), m_ResourceManager(nullptr), m_Loader(nullptr), m_Animator(nullptr), m_hWnd(hWnd), m_wndSize()
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

	m_device = new Device(m_hWnd);
	if (m_device != nullptr)
	{
		m_device->Initialize();
		m_ResourceManager = new ResourceManager(m_device);
		m_ResourceManager->Initialize();

		//m_Loader = new ModelLoader(m_ResourceManager);
		//m_Loader->Initialize();

		m_Animator = new Animator();

		if (m_device->SwapChain())
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
			m_device->Context()->OMSetRenderTargets(1, m_RTVs[0]->GetAddress(), m_DSVs[0]->Get());
			m_device->Context()->RSSetViewports(1, m_VP);


			return true;
		}
	}


	return false;
}

void GraphicsEngine::Update(double dt, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	m_ResourceManager->Update(dt, view, proj);

	//여기서 각종 상수버퍼들 업데이트 필요함 그릴 오브젝트들 여기서 일괄 업데이트하자
	for (auto& ob : m_FowardRenderObjects)
	{
		ob.second->Update<ConstantBuffer<WorldTransformCB>>();
		//ob.second->Update<ConstantBuffer<LocalTransformCB>>();
	}


	//m_Animator->Update(dt, m_FowardRenderObjects);

}

void GraphicsEngine::Update(double dt)
{

}

bool GraphicsEngine::Finalize()
{
	//delete m_Loader;
	delete m_ResourceManager;

	delete m_VP;

	delete m_device;

	return true;
}

void GraphicsEngine::Render()
{

	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	m_device->BeginRender(m_RTVs[0]->Get(), m_DSVs[0]->Get(), Black);

	for (auto& ob : m_FowardRenderObjects)
	{
		m_device->ForwardRender(ob.second);
	}

	m_device->Context()->OMSetRenderTargets(1, m_RTVs[0]->GetAddress(), m_DSVs[0]->Get());
	m_device->Context()->RSSetViewports(1, m_VP);
	m_device->EndRender();
}

void GraphicsEngine::DeferredRender()
{
	m_device->BeginDeferredRender(m_RTVs, m_DSVs[0]->Get());
	m_device->Context()->RSSetViewports(1, m_VP);

	ID3D11RenderTargetView* MRT[5];
	MRT[0] = m_RTVs[1]->Get(); //albedo
	MRT[1] = m_RTVs[2]->Get(); //normal
	MRT[2] = m_RTVs[3]->Get(); //position
	MRT[3] = m_RTVs[4]->Get(); //depth
	MRT[4] = m_RTVs[5]->Get(); //tangent

	ShaderResourceView* SRVS[5];
	SRVS[0] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_1");
	SRVS[1] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_2");
	SRVS[2] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_3");
	SRVS[3] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_4");
	SRVS[4] = m_ResourceManager->Get<ShaderResourceView>(L"OffScreenSRV_5");

	//렌더타겟 바인딩
	m_device->Context()->OMSetRenderTargets(5, MRT, m_DSVs[0]->Get());

	//pass 1
	for (int i = 1; i < m_RTVs.size(); i++)
	{
		m_device->Context()->PSSetShader(m_ResourceManager->Create<PixelShader>(L"../x64/Debug/GeoMetryPS.cso", L"GeoMetry")->GetPS(), nullptr, 0);

		for (auto& ob : m_FowardRenderObjects)
		{
			m_device->DeferredRender(ob.second, m_RTVs, m_DSVs);
		}

		//m_device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
	}

	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	////ImGui::Text("test");
	//float aspectRatio = 16.0f / 9.0f;
	//float newWidth = 300 * aspectRatio;

	m_device->Context()->OMSetRenderTargets(1, m_RTVs[0]->GetAddress(), m_DSVs[0]->Get());

	//pass2
	//UINT size = static_cast<UINT>(sizeof(QuadVertex));
	//VertexBuffer* vb = m_ResourceManager->Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	//IndexBuffer* ib = m_ResourceManager->Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	//ConstantBuffer<WorldTransformCB>* cb = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"QuadTransform", BufferDESC::Constant::DefaultWorld);
	//VertexShader* vs = m_ResourceManager->Create<VertexShader>(L"../x64/Debug/QuadVS.cso", VERTEXFILTER::QUAD, L"Quad");
	//PixelShader* ps = m_ResourceManager->Create<PixelShader>(L"../x64/Debug/DeferredPS.cso", L"Deferred");
	//RenderState* rs = m_ResourceManager->Get<RenderState>(L"Solid");
	//
	//
	//m_device->Context()->IASetInputLayout(vs->InputLayout());
	//
	//m_device->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	//m_device->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);
	//
	//m_device->Context()->IASetPrimitiveTopology(Quad::PRIMITIVE_TOPOLOGY);
	//m_device->Context()->RSSetState(rs->Get());
	//
	//m_device->Context()->VSSetShader(vs->GetVS(), nullptr, 0);
	//
	//
	//m_device->Context()->VSSetConstantBuffers(1, 1, cb->GetAddress());
	//m_device->Context()->PSSetConstantBuffers(0, 1, cb->GetAddress());
	//
	//for (int i = 0; i < 5; i++)
	//{
	//	m_device->Context()->PSSetShaderResources(i, 1, SRVS[i]->GetAddress());
	//}
	//
	//m_device->Context()->PSSetSamplers(1, 1, SRVS[0]->GetSamplerAddress());
	//
	//m_device->Context()->PSSetShader(ps->GetPS(), nullptr, 0);
	//m_device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	//
	m_device->EndRender();
}

bool GraphicsEngine::LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx, std::wstring VSname, std::wstring PSname)
{

	//m_ob = new Object(m_device);
	Object* newObject = new Object();
	newObject->m_name = name;


	std::wstring VSPath = L"../x64/Debug/" + VSname + L"VS.cso";
	std::wstring PSPath = L"../x64/Debug/" + PSname + L"PS.cso";

	//계속 늘어나면 어쩔거임? 한정적이지 않을까?
	switch (mesh)
	{
		case MeshFilter::Axis:
		{
			/*ModelData* AxisModel = new ModelData();

			AxisModel->m_Meshes.push_back(new Mesh());

			AxisModel->m_Meshes[0]->m_primitive = Axis::PRIMITIVE_TOPOLOGY;

			UINT size = static_cast<UINT>(sizeof(BaseVertex));
			AxisModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(name + L"_VB", Axis::Vertex::Desc, Axis::Vertex::Data, size);
			AxisModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(name + L"_IB", Axis::Index::Desc, Axis::Index::Data, Axis::Index::count);

			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::BASE, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));

			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Wire"));
			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");
			newObject->Set<ModelData>(AxisModel);*/

		}
		break;
		case MeshFilter::Grid:
		{
			ModelData* GridModel = new ModelData();

			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Wire"));

			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::BASE, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));

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

			//GridModel->m_Meshes.push_back(new Mesh());
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
			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");

			GridModel->m_Meshes[0]->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			newObject->Set<ModelData>(GridModel);

		}
		break;
		case MeshFilter::Box:
		{
			ModelData* BoxModel = new ModelData();

			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Solid"));
			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::BASE, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));

			//BoxModel->m_Meshes.push_back(new Mesh());
			UINT size = static_cast<UINT>(sizeof(BaseVertex));

			BoxModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"Box_VB", Box::Vertex::Desc, Box::Vertex::Data, size);
			BoxModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"Box_IB", Box::Index::Desc, Box::Index::Data, Box::Index::count);
			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");

			BoxModel->m_Meshes[0]->m_primitive = Box::PRIMITIVE_TOPOLOGY;
			newObject->Set<ModelData>(BoxModel);


		}
		break;
		case MeshFilter::TextureBox:
		{
			ModelData* BoxModel = new ModelData();

			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Solid"));
			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::TEXTURE, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));

			//BoxModel->m_Meshes.push_back(new Mesh());
			UINT size = static_cast<UINT>(sizeof(TextureVertex));
			BoxModel->m_Meshes[0]->m_VB = m_ResourceManager->Create<VertexBuffer>(L"TextureBox_VB", TextureBox::Vertex::Desc, TextureBox::Vertex::Data, size);
			BoxModel->m_Meshes[0]->m_IB = m_ResourceManager->Create<IndexBuffer>(L"TextureBox_IB", TextureBox::Index::Desc, TextureBox::Index::Data, TextureBox::Index::count);


			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; //동적 리소스 만들거면 dynamic , upadate시 map,unmap 사용
			bufferDesc.ByteWidth = sizeof(WorldTransformCB); // 상수 버퍼의 크기는 상수 데이터의 크기와 같아야 합니다.
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;	//동적 리소스면 write
			bufferDesc.MiscFlags = 0;

			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");

			BoxModel->m_Materials.push_back(new Material(m_device));
			BoxModel->m_Materials[0]->m_DiffuseSRV = m_ResourceManager->Create<ShaderResourceView>(L"../img/WoodCrate01.dds", L"../img/WoodCrate01.dds", SamplerDESC::Linear);

			BoxModel->m_Meshes[0]->m_primitive = TextureBox::PRIMITIVE_TOPOLOGY;
			newObject->Set<ModelData>(BoxModel);

		}
		break;
		case MeshFilter::LoadModel:
		{
			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::TEXTURE, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));
			newObject->Set<ModelData>(m_ResourceManager->Get<ModelData>(name));
			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Solid"));
			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");
			newObject->Set<LocalTransformCB>(m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(name + L"Local", BufferDESC::Constant::DefaultWorld), name + L"Local");
		}
		break;

		case MeshFilter::Skinning:
		{

			newObject->Set<RenderState>(m_ResourceManager->Get<RenderState>(L"Solid"));
			newObject->Set<VertexShader>(m_ResourceManager->Create<VertexShader>(VSPath, VERTEXFILTER::SKINNING, VSname));
			newObject->Set<PixelShader>(m_ResourceManager->Create<PixelShader>(PSPath, PSname));

			newObject->Set<ModelData>(m_ResourceManager->Get<ModelData>(fbx));

			if (m_FowardRenderObjects.find(name) != m_FowardRenderObjects.end())
			{
				name += L"_copy";
			}

			newObject->Set<WorldTransformCB>(m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(name + L"Transform", BufferDESC::Constant::DefaultWorld), name + L"Transform");
			newObject->Set<LocalTransformCB>(m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(name + L"Local", BufferDESC::Constant::DefaultWorld), name + L"Local");
			newObject->Set<MatrixPallete>(m_ResourceManager->Create<ConstantBuffer<MatrixPallete>>(name + L"MatrixPallete", BufferDESC::Constant::DefaultMatrixPallete), name + L"MatrixPallete");

		}
		break;

		case MeshFilter::None:
			break;
		default:
			break;
	}

	m_FowardRenderObjects.insert({ name,newObject });

	return true;
}


void GraphicsEngine::UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct)
{
	if (m_FowardRenderObjects.find(name) != m_FowardRenderObjects.end())
	{
		XMStoreFloat4x4(&constantstruct.world, XMMatrixTranspose(XMLoadFloat4x4(&constantstruct.world)));
		m_FowardRenderObjects[name]->GetCB<WorldTransformCB>(cbname)->m_struct = constantstruct;
	}
}

void GraphicsEngine::UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)
{
	m_ResourceManager->Get<ConstantBuffer<DirectionLightCB>>(L"DirectionLight")->m_struct = constantstruct;
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

	

	m_device->OnResize();
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


	m_device->Context()->OMSetRenderTargets(1, m_RTVs[0]->GetAddress(), m_DSVs[0]->Get());
	m_device->Context()->RSSetViewports(1, m_VP);
}

void GraphicsEngine::DrawQuad(ShaderResourceView* srv)
{




	UINT size = static_cast<UINT>(sizeof(QuadVertex));

	VertexBuffer* vb = m_ResourceManager->Create<VertexBuffer>(L"Quard_VB", Quad::Vertex::Desc, Quad::Vertex::Data, size);
	IndexBuffer* ib = m_ResourceManager->Create<IndexBuffer>(L"Quard_IB", Quad::Index::Desc, Quad::Index::Data, Quad::Index::count);
	ConstantBuffer<WorldTransformCB>* cb = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"QuadTransform", BufferDESC::Constant::DefaultWorld);

	VertexShader* vs = m_ResourceManager->Create<VertexShader>(L"../x64/Debug/QuadVS.cso", VERTEXFILTER::QUAD, L"Quad");
	PixelShader* ps = m_ResourceManager->Create<PixelShader>(L"../x64/Debug/QuadPS.cso", L"Quad");
	RenderState* rs = m_ResourceManager->Get<RenderState>(L"Solid");



	m_device->Context()->IASetInputLayout(vs->InputLayout());

	m_device->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_device->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_device->Context()->IASetPrimitiveTopology(Quad::PRIMITIVE_TOPOLOGY);
	m_device->Context()->RSSetState(rs->Get());

	m_device->Context()->VSSetShader(vs->GetVS(), nullptr, 0);


	m_device->Context()->VSSetConstantBuffers(1, 1, cb->GetAddress());
	m_device->Context()->PSSetConstantBuffers(0, 1, cb->GetAddress());


	m_device->Context()->PSSetShaderResources(0, 1, srv->GetAddress());
	m_device->Context()->PSSetSamplers(0, 1, srv->GetSamplerAddress());


	m_device->Context()->PSSetShader(ps->GetPS(), nullptr, 0);
	m_device->Context()->DrawIndexed(Quad::Index::count, 0, 0);





}
