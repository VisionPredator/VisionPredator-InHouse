#pragma once
#include "Buffer.h"

#include "Device.h"
#include "CBuffer.h"

template<typename T>
class ConstantBuffer :
	public Buffer
{
public:
	ConstantBuffer();
	ConstantBuffer(std::shared_ptr<Device> device);
	ConstantBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC Desc);
	ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, T data);
	~ConstantBuffer();


	virtual void Release() override;


	void Update();

	void Update(T cbstruct);

	T m_struct;

private:

};

template<typename T>
ConstantBuffer<T>::ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, T data)
{
	m_Device.lock()->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);

	m_struct = data;
}

template<typename T>
void ConstantBuffer<T>::Release()
{
	if (m_buffer == nullptr)
		return;
	m_buffer->Release();
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC Desc) : Buffer(device, Desc)
{
	m_Device.lock()->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);

	m_struct = T();
}

template<typename T>
ConstantBuffer<T>::ConstantBuffer() : Buffer()
{

}


template<typename T>
ConstantBuffer<T>::ConstantBuffer(std::shared_ptr<Device>device) : Buffer(device), m_struct()
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
	//몇번째 상수버퍼일지 카운트가 필요한데?

	m_Device.lock()->Context()->UpdateSubresource(m_buffer, 0, nullptr, &m_struct, 0, 0);

}

template<typename T>
void ConstantBuffer<T>::Update(T cbstruct)
{
	m_struct = cbstruct;

	m_Device.lock()->Context()->UpdateSubresource(m_buffer, 0, nullptr, &m_struct, 0, 0);
}