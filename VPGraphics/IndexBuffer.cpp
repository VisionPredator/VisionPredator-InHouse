#include "pch.h"

#include "IndexBuffer.h"
#include <d3d11.h>

IndexBuffer::IndexBuffer(Device* device, UINT count) : Buffer(device, count)
{

}

IndexBuffer::IndexBuffer()
{

}

IndexBuffer::IndexBuffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int drawindex) : Buffer(device, desc, data, drawindex)
{
	HRESULT hr;
	hr = m_Device->Get()->CreateBuffer(&m_Desc, &m_Data, &m_buffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"IndexBuffer Create Failed", 0, 0);
	}
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Update()
{

}
