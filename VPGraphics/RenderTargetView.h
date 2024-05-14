#pragma once
#include "Resource.h"

class Texture2D;

class RenderTargetView : public Resource
{
public:
	RenderTargetView(Device* device);
	RenderTargetView(Device* device, Texture2D* backbuffer);
	RenderTargetView(Device* device, Texture2D* backbuffer, D3D11_RENDER_TARGET_VIEW_DESC desc);
	~RenderTargetView();

	ID3D11RenderTargetView* Get() const;
	ID3D11RenderTargetView** GetAddress();

	virtual void Release() override;

	Texture2D* Texture();

private:
	ID3D11RenderTargetView* m_RTV;
	Texture2D* m_tex;

};

