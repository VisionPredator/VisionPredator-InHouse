#include "pch.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "D3DCompiler.lib")

#include "Device.h"
#include "VertexShader.h"

#include <wrl/client.h>

#include "Defines.h"
#include "VertexData.h"

VertexShader::VertexShader(std::wstring filename) : Shader(filename), m_Kind_of_Vertex(VERTEXFILTER::END)
{

}

VertexShader::VertexShader(std::shared_ptr<Device> device, std::wstring filename /*= L"need name"*/) : Shader(device, filename), m_Kind_of_Vertex(VERTEXFILTER::END)
{
	m_filename = m_filename + L"VS.cso";

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

	//TODO:: ���⼭ desc�� �Ź� ������ ������ inputlayout�� �����ؾ��ҵ�
	//hr = m_device->Get()->CreateInputLayout(BaseInputDesc, BaseDescCount/*desc ���� ����*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
	hr = m_Device.lock()->Get()->CreateInputLayout(TextureVertexInputDesc, TextureDescCount/*desc ���� ����*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);

	if (FAILED(hr))
	{
		MessageBox(0, L"VS CreateInputLayout Fail", 0, 0);
	}
}


VertexShader::VertexShader(std::shared_ptr<Device>device, VERTEXFILTER kind_of_vertex, std::wstring filename /*= L"need name"*/) : Shader(device, filename), m_Kind_of_Vertex(kind_of_vertex)
{
	//m_filename = m_filename + L"VS.hlsl";
	std::wstring path = L"..\\..\\..\\VPGraphics\\";
	m_filename = path + m_filename + L"VS.hlsl";

	ID3DBlob* pErrorBlob = nullptr;
	ID3DBlob* VSBlob = nullptr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr;


	//TODO:: ���⼭ desc�� �Ź� ������ ������ inputlayout�� �����ؾ��ҵ�
	//hr = m_device->Get()->CreateInputLayout(BaseInputDesc, BaseDescCount/*desc ���� ����*/, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);

	switch (m_Kind_of_Vertex)
	{
		case VERTEXFILTER::STATIC:
		{
			hr = D3DCompileFromFile(m_filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",dwShaderFlags, 0, &VSBlob, &pErrorBlob);

			if (FAILED(hr))
			{
				MessageBox(0, L"VS Load Fail", 0, 0);
			}

			hr = m_Device.lock()->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &m_VS);

			if (FAILED(hr))
			{
				MessageBox(0, L"CreateVS Fail", 0, 0);
			}
			hr = m_Device.lock()->Get()->CreateInputLayout(TextureVertexInputDesc, TextureDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
		}
			break;

		case VERTEXFILTER::SKINNING:
		{
			D3D_SHADER_MACRO macro[] =
			{
				{"SKINNING",""}, // ��ũ�� �̸��� ���� ����
				{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
			};
			
			hr = D3DCompileFromFile(m_filename.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",dwShaderFlags, 0, &VSBlob, &pErrorBlob);
			hr = m_Device.lock()->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &m_VS);

			hr = m_Device.lock()->Get()->CreateInputLayout(SkinningVertexInputDesc, SkinningDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
		}
		break;

		case VERTEXFILTER::QUAD:
		{
			hr = D3DCompileFromFile(m_filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", dwShaderFlags, 0, &VSBlob, &pErrorBlob);
			hr = m_Device.lock()->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &m_VS);
			hr = m_Device.lock()->Get()->CreateInputLayout(QuadVertexInputDesc, QuadDescCount, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &m_inputLayout);
		}
		break;

		case VERTEXFILTER::END:
		{

		}

		break;

		default:
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"VS CreateInputLayout Fail", 0, 0);
	}
}

VertexShader::VertexShader(const std::shared_ptr<Device>& device, const std::wstring& filename, const int& a)
{
//	DWORD shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//	shaderFlag |= D3DCOMPILE_DEBUG;
//	shaderFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//
//	D3D_SHADER_MACRO* pDefines = nullptr;
//	const std::string& entryPoint = "main";
//	const std::string& shaderModel = "vs_5_0";
//
//	Microsoft::WRL::ComPtr<ID3DBlob> blob;
//	HR_CHECK(D3DCompileFromFile(filename.c_str(), pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), shaderModel.c_str(),
//		shaderFlag, 0, &blob, &blob));
//
//	HR_CHECK(device->Get()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_VS));
//
//
//	/// ���� ��ǲ ���̾ƿ� �����
//	ID3D11ShaderReflection* pReflector = nullptr;
//
//	// Create Reflector..
//	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
//
//	// Shader Refection
//	D3D11_SHADER_DESC shaderDesc;
//	pReflector->GetDesc(&shaderDesc);
//
//	/// Input Layout Reflection
//	// Shader Input Layout..
//	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
//	for (unsigned inputIndex = 0; inputIndex < shaderDesc.InputParameters; inputIndex++)
//	{
//		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
//		pReflector->GetInputParameterDesc(inputIndex, &paramDesc);
//
//		// Shader Input Data�� ������� ����..
//		D3D11_INPUT_ELEMENT_DESC elementDesc;
//		elementDesc.SemanticName = paramDesc.SemanticName;
//		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
//		elementDesc.InputSlot = 0;
//		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//		elementDesc.InstanceDataStepRate = 0;
//
//		// Shader Data ������� DXGI format ����..
//		if (paramDesc.Mask == 1)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 3)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 7)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 15)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//		}
//
//		// �� InputLayout ������ ����..
//		inputLayoutDesc.push_back(elementDesc);
//	}
//
//	// Shader InputLayout ����..
//	HR_CHECK(
//		device->Get()->CreateInputLayout(
//			&inputLayoutDesc[0], 
//			(UINT)inputLayoutDesc.size(),
//			blob->GetBufferPointer(), 
//			blob->GetBufferSize(),
//			&m_inputLayout
//		);
//	);
}

VertexShader::~VertexShader()
{

}



void VertexShader::Release()
{
	m_inputLayout->Release();
	m_VS->Release();
}