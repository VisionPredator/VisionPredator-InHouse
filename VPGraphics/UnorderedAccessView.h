#pragma once
#include <wrl/client.h>
#include "Resource.h"

class UnorderedAccessView : public Resource
{
public:
	UnorderedAccessView(const std::shared_ptr<Device>& device);
	UnorderedAccessView(const std::shared_ptr<Device>& device, const std::shared_ptr<RenderTargetView>& renderTargetView);
	UnorderedAccessView(const std::shared_ptr<Device>& device, const std::shared_ptr<Texture2D>& texture2D);

	ID3D11UnorderedAccessView* Get() const { return m_View.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_View;
};
