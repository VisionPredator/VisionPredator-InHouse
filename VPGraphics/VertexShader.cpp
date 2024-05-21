#include "pch.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "D3DCompiler.lib")

#include "Device.h"
#include "VertexShader.h"
#include "VertexData.h"

VertexShader::VertexShader(std::wstring filename) : Shader(filename)
{

}

VertexShader::VertexShader(std::shared_ptr<Device> device, std::wstring filename /*= L"need name"*/) : Shader(device, filename)
{
	m_filename = L"../x64/Debug/" + m_filename + L"VS.cso";

	ID3DBlob* VSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(m_filename.c_str(), &VSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"VS Load Fail", 0, 0);
	}

	hr = m_Device.lock()->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &m_VS);

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateVS Fail", 0, 0);
	}

	//TODO:: 여기서 desc를 매번 셋팅해 각각의 inputlayout이 존재해야할듯
	//hr = m_device->Get()->CreateInputLayout(BaseInputDesc, BaseDescCount/*desc 원소 갯수*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
	hr = m_Device.lock()->Get()->CreateInputLayout(TextureVertexInputDesc, TextureDescCount/*desc 원소 갯수*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);

	if (FAILED(hr))
	{
		MessageBox(0, L"VS CreateInputLayout Fail", 0, 0);
	}
}


VertexShader::VertexShader(std::shared_ptr<Device>device, VERTEXFILTER kind_of_vertex, std::wstring filename /*= L"need name"*/) : Shader(device, filename), m_Kind_of_Vertex(kind_of_vertex)
{
	m_filename = L"../x64/Debug/" + m_filename + L"VS.cso";

	ID3DBlob* VSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(m_filename.c_str(), &VSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"VS Load Fail", 0, 0);
	}

	hr = m_Device.lock()->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &m_VS);

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateVS Fail", 0, 0);
	}

	//TODO:: 여기서 desc를 매번 셋팅해 각각의 inputlayout이 존재해야할듯
	//hr = m_device->Get()->CreateInputLayout(BaseInputDesc, BaseDescCount/*desc 원소 갯수*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);

	switch (m_Kind_of_Vertex)
	{
		case VERTEXFILTER::BASE:
			hr = m_Device.lock()->Get()->CreateInputLayout(BaseInputDesc, BaseDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
			break;
		case VERTEXFILTER::TEXTURE:
			hr = m_Device.lock()->Get()->CreateInputLayout(TextureVertexInputDesc, TextureDescCount/*desc 원소 갯수*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
			break;
		case VERTEXFILTER::SKINNING:
			hr = m_Device.lock()->Get()->CreateInputLayout(SkinningVertexInputDesc, SkinningDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
			break;
		case VERTEXFILTER::QUAD:
			//쉐이더에 들어가는거랑 다르면 안만듬
			hr = m_Device.lock()->Get()->CreateInputLayout(QuadVertexInputDesc, QuadDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
			break;

		case VERTEXFILTER::END:
			break;
		default:
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"VS CreateInputLayout Fail", 0, 0);
	}
}

VertexShader::~VertexShader()
{

}



void VertexShader::Release()
{
	m_inputLayout->Release();
	m_VS->Release();
}