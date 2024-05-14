#pragma once
#include "Resource.h"

class RenderState :public Resource
{
public:
	RenderState(Device* device, D3D11_RASTERIZER_DESC desc);
	~RenderState();

	virtual void Release() override;
	ID3D11RasterizerState* Get() const;
private:

	ID3D11RasterizerState* m_RS;
};