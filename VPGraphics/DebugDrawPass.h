#pragma once

class Camera;
class DepthStencilView;
class RenderTargetView;
class ResourceManager;
class DebugDrawManager;
class Device;

class DebugDrawPass
{
public:

	void Initialize(const std::shared_ptr<Device>& device, 
		const std::shared_ptr<DebugDrawManager>& debugDrawManager,
		const std::shared_ptr<ResourceManager>& resourceManager,
		const std::shared_ptr<Camera>& camera);

	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<DebugDrawManager> m_DebugDrawManager;
	std::shared_ptr<Camera> m_Camera;

	std::shared_ptr<RenderTargetView> m_FullScreenRTV;
	std::shared_ptr<DepthStencilView> m_DepthStencilView;
};

