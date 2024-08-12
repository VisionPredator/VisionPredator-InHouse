#pragma once
#include "Resource.h"
#include <string>
#include <wrl/client.h>

class ShaderResourceView : public Resource
{
public:
	ShaderResourceView(std::shared_ptr<Device>device);

	ShaderResourceView(std::shared_ptr<Device> device, std::wstring filename);
	ShaderResourceView(std::shared_ptr<Device> device, const std::shared_ptr<RenderTargetView>& rtv);

	
	//ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	//ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<RenderTargetView> rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	//ShaderResourceView(std::shared_ptr<Device> device, RenderTargetView* rtv);

	~ShaderResourceView() = default;

	ID3D11ShaderResourceView* Get() const;
	ID3D11ShaderResourceView** GetAddress();
	void Release() override;

	UINT GetWidth() { return m_Width; }
	UINT GetHeight() { return m_Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_view = nullptr;
	std::weak_ptr<Texture2D> m_tex;

	UINT m_Width = 0;
	UINT m_Height = 0;
};

