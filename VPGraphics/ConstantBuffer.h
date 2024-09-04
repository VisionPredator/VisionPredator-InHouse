#pragma once
#include "Buffer.h"

#include "Device.h"
#include "CBuffer.h"
#include "Defines.h"
#include "Desc.h"

enum class ConstantBufferType
{
	Default,
	Dynamic
};

template<typename T>
class ConstantBuffer :
	public Buffer
{
public:
	ConstantBuffer(std::shared_ptr<Device> device);
	ConstantBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC Desc);
	//ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, T data);

	ConstantBuffer(const std::shared_ptr<Device>& device, const ConstantBufferType& type);

	~ConstantBuffer();

	virtual void Release() override;

	void Update();

	void Update(T cbstruct);

	T m_struct;

private:

};

//template<typename T>
//ConstantBuffer<T>::ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, T data)
//{
//	m_Device.lock()->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);
//
//	m_struct = data;
//}

template<typename T>
void ConstantBuffer<T>::Release()
{
	m_buffer.Reset();
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC Desc)
	: Buffer(device, Desc)
{
	m_Device.lock()->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);

	m_struct = T();
}

template <typename T>
ConstantBuffer<T>::ConstantBuffer(const std::shared_ptr<Device>& device, const ConstantBufferType& type)
	: Buffer(device)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(T);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.MiscFlags = 0;

	switch (type)
	{
		case ConstantBufferType::Default:
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;
			break;
		}
		case ConstantBufferType::Dynamic:
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		}
		default:
		{
			return;
		}
	}

	HR_CHECK(device->Get()->CreateBuffer(&desc, nullptr, m_buffer.GetAddressOf()));
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(std::shared_ptr<Device> device)
	: Buffer(device), m_struct()
{

}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer()
{
	//Release();
}

template<typename T>
void ConstantBuffer<T>::Update()
{
	m_Device.lock()->Context()->UpdateSubresource(m_buffer.Get(), 0, nullptr, &m_struct, 0, 0);
}

template<typename T>
void ConstantBuffer<T>::Update(T cbstruct)
{
	m_struct = cbstruct;

	m_Device.lock()->Context()->UpdateSubresource(m_buffer.Get(), 0, nullptr, &m_struct, 0, 0);
}