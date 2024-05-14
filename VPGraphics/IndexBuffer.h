#pragma once
#include "Buffer.h"
#include "Device.h"

class IndexBuffer :
	public Buffer
{
public:
	IndexBuffer();
	IndexBuffer(Device* device, UINT count = 0);
	IndexBuffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int drawindex);

	//����� �迭�� T�� �޾Ƽ� �Ѵٸ�?
	//IndexBuffer(Device* device, D3D11_BUFFER_DESC desc, T* databuffer,int drawindex);

	~IndexBuffer();

	//template<typename T>
	//bool Load(T* buffer);

	virtual void Update() override;


private:
};
