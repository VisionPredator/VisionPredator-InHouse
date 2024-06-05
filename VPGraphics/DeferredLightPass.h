#pragma once
#include "Pass.h"

class Device;
class ResourceManager;

class DeferredLightPass final : public Pass
{
public:
	DeferredLightPass() = default;
	virtual ~DeferredLightPass() = default;

	void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Render() override;

private:
	// Vertex Buffer
	// Index Buffer
};

