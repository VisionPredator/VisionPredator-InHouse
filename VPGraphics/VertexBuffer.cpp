#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Device* device, UINT count) : Buffer(device, count)
{

}

VertexBuffer::VertexBuffer()
{

}

VertexBuffer::VertexBuffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, UINT sizeT) : Buffer(device, desc, data), m_stride(sizeT)
{
	HRESULT hr;
	hr = m_Device->Get()->CreateBuffer(&m_Desc, &m_Data, &m_buffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"VertexBuffer Create Failed", 0, 0);
	}
}

VertexBuffer::~VertexBuffer()
{
}

UINT* VertexBuffer::Size()
{
	return &m_stride;
}

UINT* VertexBuffer::Offset()
{
	return &offset;
}

void VertexBuffer::Update()
{

}
