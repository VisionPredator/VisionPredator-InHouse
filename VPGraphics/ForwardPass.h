#pragma once
#include "RenderPass.h"

class ForwardPass :
    public RenderPass
{
public:
	ForwardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~ForwardPass();

	virtual void Render() override;

private:
};

