#pragma once
#include "RenderPass.h"
#include "BlendState.h"

class TransparencyPass :
    public RenderPass
{
public:
	TransparencyPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~TransparencyPass();

	virtual void Render() override;
	virtual void OnResize() override;

private:
	std::weak_ptr<BlendState> m_BlendState;

};

