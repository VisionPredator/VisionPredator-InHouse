#pragma once
#include "Buffer.h"
#include "Device.h"

class IndexBuffer :
	public Buffer
{
public:
	IndexBuffer();
	IndexBuffer(std::shared_ptr<Device> device, UINT count = 0);
	IndexBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int drawindex);

	IndexBuffer(const std::shared_ptr<Device>& device, const std::vector<unsigned int>& indices);

	~IndexBuffer() override = default;

	virtual void Update() override;


private:
};
