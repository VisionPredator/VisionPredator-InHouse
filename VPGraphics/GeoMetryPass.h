#pragma once
#include "RenderPass.h"
class GeoMetryPass :
    public RenderPass
{
public:
	GeoMetryPass(const std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager> manger);
	~GeoMetryPass();

	void Render() override;
	void OnResize() override;
};

