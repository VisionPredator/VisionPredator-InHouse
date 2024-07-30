#pragma once
#include "RenderPass.h"
class GeoMetryPass :
    public RenderPass
{
public:
	GeoMetryPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger);
	~GeoMetryPass();

	virtual void Render() override;
	virtual void OnResize() override;
};

