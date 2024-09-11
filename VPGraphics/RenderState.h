#pragma once
#include <wrl/client.h>
#include "Resource.h"

class RenderState :public Resource
{
public:
	RenderState(std::shared_ptr<Device> device, D3D11_RASTERIZER_DESC desc);
	~RenderState();

	void Release() override;
	ID3D11RasterizerState* Get() const;

private:

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RS;
};