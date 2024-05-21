#include "pch.h"

#include "PixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Device.h"
#include "Vertex.h"
#pragma comment (lib, "D3DCompiler.lib")

PixelShader::PixelShader(std::wstring filename) : Shader(filename)
{

}

PixelShader::PixelShader(std::shared_ptr<Device> device, std::wstring filename) : Shader(device, filename)
{
	m_filename = L"../x64/Debug/" + m_filename + L"PS.cso";

	ID3DBlob* PSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(m_filename.c_str(), &PSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"PS Load Fail", 0, 0);
	}

	hr = m_Device.lock()->Get()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &m_PS);
	if (FAILED(hr))
	{
		PSBlob->Release();
		MessageBox(0, L"CreatePS Fail", 0, 0);
	}
}


PixelShader::~PixelShader()
{
}


void PixelShader::Release()
{
	m_PS->Release();
}
