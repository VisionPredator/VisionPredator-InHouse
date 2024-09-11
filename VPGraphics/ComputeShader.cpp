#include "pch.h"
#include "ComputeShader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Defines.h"
#include "Device.h"
#include "Vertex.h"
#pragma comment (lib, "D3DCompiler.lib")

ComputeShader::ComputeShader(std::shared_ptr<Device> device, std::wstring filename /*= L"need name"*/) : Shader(device,filename)
{
	m_filename = m_filename + L".cso";


	ID3DBlob* PSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(m_filename.c_str(), &PSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"CS Load Fail", 0, 0);
	}
	else
	{
		hr = m_Device.lock()->Get()->CreateComputeShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &m_CS);
		if (FAILED(hr))
		{
			PSBlob->Release();
			MessageBox(0, L"CreateCS Fail", 0, 0);
		}
	}
}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::Release()
{

}
