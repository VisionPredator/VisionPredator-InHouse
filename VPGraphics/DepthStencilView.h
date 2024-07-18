#pragma once
#include <wrl/client.h>

#include "Resource.h"

class Texture2D;

enum class DepthStencilViewType
{
	Default,

	// 이후 HDR 용 RTV도 필요할 것이다.
};

class DepthStencilView : public Resource
{
public:
	DepthStencilView(std::shared_ptr<Device> device, D3D11_TEXTURE2D_DESC desc);
	DepthStencilView(std::shared_ptr<Device> device, D3D11_DEPTH_STENCIL_VIEW_DESC desc);
	DepthStencilView(std::shared_ptr<Device> device, D3D11_DEPTH_STENCIL_VIEW_DESC desc, Texture2D* texture);

	DepthStencilView(std::shared_ptr<Device> device, DepthStencilViewType type, const uint32_t& width, const uint32_t& height);
	~DepthStencilView();

	ID3D11DepthStencilView* Get() const;
	ID3D11DepthStencilView** GetAddress();

	virtual void Release() override;
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};

