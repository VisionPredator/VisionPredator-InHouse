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

PixelShader::PixelShader(Device* device, std::wstring filename) : Shader(device, filename)
{
	m_filename = L"../x64/Debug/" + m_filename + L"PS.cso";

	ID3DBlob* PSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(m_filename.c_str(), &PSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"PS Load Fail", 0, 0);
	}

	hr = m_Device->Get()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &m_PS);
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

Resource* PixelShader::Create(Device* device, std::wstring filename)
{
	filename = L"../x64/Debug/" + filename + L"PS.cso";

	ID3DBlob* PSBlob = nullptr;

	HRESULT hr;
	hr = D3DReadFileToBlob(filename.c_str(), &PSBlob);

	if (FAILED(hr))
	{
		MessageBox(0, L"PS Load Fail", 0, 0);
		return nullptr;
	}

	PixelShader* newPS = new PixelShader(filename);


	hr = device->Get()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, newPS->GetPSAddress());
	if (FAILED(hr))
	{
		delete newPS;

		PSBlob->Release();
		MessageBox(0, L"CreatePS Fail", 0, 0);
		return nullptr;
	}

	return newPS;
}

