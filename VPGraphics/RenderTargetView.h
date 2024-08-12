#pragma once
#include "Resource.h"
#include <wrl/client.h>

class Texture2D;

enum class RenderTargetViewType
{
	Default,	// backbuffer
	OffScreen,	// for deferred rendering


	// ���� HDR �� RTV�� �ʿ��� ���̴�.

};

class RenderTargetView : public Resource
{
public:
	RenderTargetView(std::shared_ptr<Device> device, const RenderTargetViewType& type, const uint32_t& width, const uint32_t& height);
	~RenderTargetView() = default;

	ID3D11RenderTargetView* Get() const;
	ID3D11RenderTargetView** GetAddress();

	virtual void Release() override;

	std::weak_ptr<Texture2D> Texture();
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
	std::weak_ptr<Texture2D> m_tex;

};

