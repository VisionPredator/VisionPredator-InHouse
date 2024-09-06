#pragma once
#include "Resource.h"
#include <wrl/client.h>

class Texture2D;

enum class RenderTargetViewType
{
	BackBuffer,	// backbuffer
	OffScreen,	// for deferred rendering
	ObjectMask,
	OutlineEdgeDetect,
	// 이후 HDR 용 RTV도 필요할 것이다.

};

class RenderTargetView : public Resource
{
public:
	RenderTargetView(const std::shared_ptr<Device>& device, const RenderTargetViewType& type, const uint32_t& width, const uint32_t& height);
	~RenderTargetView() = default;

	ID3D11RenderTargetView* Get() const;
	ID3D11RenderTargetView** GetAddress();

	void OnResize(const std::shared_ptr<Device>& device, const RenderTargetViewType& type, const uint32_t& width, const uint32_t& height);

	virtual void Release() override;

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
};
