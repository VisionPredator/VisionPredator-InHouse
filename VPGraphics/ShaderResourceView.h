#pragma once
#include "Resource.h"
#include <string>

class ShaderResourceView : public Resource
{
public:
	ShaderResourceView(std::shared_ptr<Device>device);
	ShaderResourceView(std::shared_ptr<Device>device, std::wstring filename);
	ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<RenderTargetView> rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	ShaderResourceView(std::shared_ptr<Device> device, RenderTargetView* rtv);
	~ShaderResourceView();

	ID3D11ShaderResourceView* Get() const;
	ID3D11ShaderResourceView** GetAddress();
	virtual void Release() override;

private:
	ID3D11ShaderResourceView* m_view;
	ID3D11Texture2D* m_tex;
};

