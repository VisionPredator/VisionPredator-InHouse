#pragma once
#include "Resource.h"
#include <string>

class ShaderResourceView : public Resource
{
public:
	ShaderResourceView(Device* device);
	ShaderResourceView(Device* device, std::wstring filename, D3D11_SAMPLER_DESC sampler);
	ShaderResourceView(Device* device, Texture2D* texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	ShaderResourceView(Device* device, RenderTargetView* rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	ShaderResourceView(Device* device, RenderTargetView* rtv);
	~ShaderResourceView();

	ID3D11ShaderResourceView* Get() const;
	ID3D11ShaderResourceView** GetAddress();
	ID3D11SamplerState* GetSampler() const;
	ID3D11SamplerState** GetSamplerAddress();

	void SetSampler(ID3D11SamplerState* sampler);
	virtual void Release() override;

	static void Create();

private:
	ID3D11ShaderResourceView* m_view = nullptr;
	ID3D11Texture2D* m_tex = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;

};

