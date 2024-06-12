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
	// 작업 디렉토리 기준으로 cso 파일을 찾는다.
	// 프로젝트 설정으로 작업 디렉토리를 Binaries 폴더 안으로 바꾸었으므로
	// 하드코딩으로 상세 폴더 경로를 지정해 줄 필요 없음.
	//m_filename = L"../x64/Debug/" + m_filename + L"PS.cso";
	m_filename = m_filename + L"PS.cso";


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
