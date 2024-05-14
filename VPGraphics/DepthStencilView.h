#pragma once
#include "Resource.h"

class Texture2D;

class DepthStencilView : public Resource
{
public:
	DepthStencilView(Device* device, D3D11_TEXTURE2D_DESC desc);
	DepthStencilView(Device* device, D3D11_DEPTH_STENCIL_VIEW_DESC desc);
	DepthStencilView(Device* device, D3D11_DEPTH_STENCIL_VIEW_DESC desc, Texture2D* texture);
	~DepthStencilView();

	ID3D11DepthStencilView* Get() const;
	ID3D11DepthStencilView** GetAddress();

	virtual void Release() override;
private:
	ID3D11DepthStencilView* m_DSV;
};

