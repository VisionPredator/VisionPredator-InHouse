#pragma once
#include "Buffer.h"

#include "Device.h"
#include "CBuffer.h"



enum class CBFILTER
{
	TRANSFORM = 0,
	LIGHT,
	CAMERA,



	END

};


template<typename T>
class ConstantBuffer :
	public Buffer
{
public:
	ConstantBuffer();
	ConstantBuffer(Device* device);
	ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc);
	//ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, CBFILTER kind_of_CB);
	~ConstantBuffer();


	virtual void Release() override;


	void Update();

	void Update(T cbstruct);

	T m_struct;

private:
	CBFILTER m_Kind_of_CB;

};

template<typename T>
void ConstantBuffer<T>::Release()
{
	m_buffer->Release();
}

//template<typename T>
//ConstantBuffer<T>::ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc, CBFILTER kind_of_CB)
//{
//	m_Device->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);
//
//	m_struct = T();
//}

template<typename T>
ConstantBuffer<T>::ConstantBuffer(Device* device, D3D11_BUFFER_DESC Desc) : Buffer(device, Desc), m_Kind_of_CB(CBFILTER::END)
{
	m_Device->Get()->CreateBuffer(&m_Desc, nullptr, &m_buffer);

	m_struct = T();

}

template<typename T>
ConstantBuffer<T>::ConstantBuffer() : Buffer()
{

}


template<typename T>
ConstantBuffer<T>::ConstantBuffer(Device* device) : Buffer(device), m_struct()
{

}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer()
{

}



template<typename T>
void ConstantBuffer<T>::Update()
{
	//몇번째 상수버퍼일지 카운트가 필요한데?
	m_Device->Context()->UpdateSubresource(m_buffer, 0, nullptr, &m_struct, 0, 0);

}

template<typename T>
void ConstantBuffer<T>::Update(T cbstruct)
{
	m_struct = cbstruct;

	m_Device->Context()->UpdateSubresource(m_buffer, 0, nullptr, &m_struct, 0, 0);
}