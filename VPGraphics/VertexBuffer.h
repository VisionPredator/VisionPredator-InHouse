#pragma once
#include "Buffer.h"
class VertexBuffer :
    public Buffer
{
public:
	VertexBuffer();
	VertexBuffer(Device* device, UINT count = 0);
	VertexBuffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, UINT sizeT);
	~VertexBuffer();

	UINT* Size();
	UINT* Offset();

	virtual void Update() override;
private:
	UINT m_stride = 0;
	UINT offset = 0;
};

