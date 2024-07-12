#include "pch.h"

#include "Buffer.h"

Buffer::Buffer() : Resource()
{
}

Buffer::Buffer(std::shared_ptr<Device>device, UINT count) : Resource(device), m_count(count), m_buffer(nullptr)
{

}

Buffer::Buffer(std::shared_ptr<Device>device) : Resource(device), m_count(0), m_buffer(nullptr)
{

}

Buffer::Buffer(std::shared_ptr<Device>device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data) : Resource(device), m_Desc(desc), m_Data(data)
{

}

Buffer::Buffer(std::shared_ptr<Device>device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int count) : Resource(device), m_Desc(desc), m_Data(data), m_count(count)
{

}

Buffer::Buffer(std::shared_ptr<Device>device, D3D11_BUFFER_DESC desc) : Resource(device), m_Desc(desc)
{

}

Buffer::~Buffer()
{
}


ID3D11Buffer* Buffer::Get() const
{
	return m_buffer.Get();
}

ID3D11Buffer** Buffer::GetAddress()
{
	return &m_buffer;
}

UINT Buffer::Count() const
{
	return m_count;
}

void Buffer::Release()
{
	m_buffer->Release();
}
