#include "pch.h"
#include "GeometryShader.h"

#include <d3dcompiler.h>

#include "Defines.h"

GeometryShader::GeometryShader(const std::shared_ptr<Device>& device, const std::wstring& filename,
                               const std::string& entryPoint, const std::string& shaderModel, const D3D_SHADER_MACRO* macro)
{
	DWORD shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	shaderFlag |= D3DCOMPILE_DEBUG;
	shaderFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HR_CHECK(D3DCompileFromFile(filename.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), shaderModel.c_str(),
		shaderFlag, 0, &blob, &errorBlob));

	if (errorBlob)
	{
		OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
	}

	ID3D11ShaderReflection* pReflector = nullptr;

	// Create Reflector..
	D3DReflect(blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&pReflector);

	// Shader Reflection
	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	std::vector<D3D11_SO_DECLARATION_ENTRY> streamOutput;
	streamOutput.resize(shaderDesc.OutputParameters);

	// 스트림 출력 선언 생성
	for (unsigned InputIndex = 0; InputIndex < shaderDesc.OutputParameters; InputIndex++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflector->GetOutputParameterDesc(InputIndex, &paramDesc);

		D3D11_SO_DECLARATION_ENTRY& elementDesc = streamOutput[InputIndex];

		elementDesc.Stream = 0;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;

		elementDesc.StartComponent = 0;

		if (paramDesc.Mask & (1 << 0))
		{
			elementDesc.ComponentCount++;
		}
		if (paramDesc.Mask & (1 << 1))
		{
			elementDesc.ComponentCount++;
		}
		if (paramDesc.Mask & (1 << 2))
		{
			elementDesc.ComponentCount++;
		}
		if (paramDesc.Mask & (1 << 3))
		{
			elementDesc.ComponentCount++;
		}

		elementDesc.OutputSlot = 0;
	}

	UINT rasterizerStream = 0;
	
	HR_CHECK(device->Get()->CreateGeometryShaderWithStreamOutput(
		blob->GetBufferPointer()
		, blob->GetBufferSize()
		, streamOutput.data()
		, static_cast<UINT>(streamOutput.size())
		, nullptr
		, 0
		, rasterizerStream
		, nullptr
		, &m_Shader));
}
