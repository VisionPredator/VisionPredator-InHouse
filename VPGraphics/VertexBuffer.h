#pragma once
#include "Buffer.h"

enum class VertexBufferType
{
	Default,
	ParticleInit,
	ParticleDraw
};

class VertexBuffer :
    public Buffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(std::shared_ptr<Device> device, UINT count = 0);
	VertexBuffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, UINT sizeT);

	~VertexBuffer() override = default;

	UINT* Size();
	UINT* Offset();

	virtual void Update() override;
private:
	UINT m_stride = 0;
	UINT offset = 0;
};

