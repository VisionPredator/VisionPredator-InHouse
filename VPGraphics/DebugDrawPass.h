#pragma once

class ResourceManager;
class DebugDrawManager;
class Device;

class DebugDrawPass
{
public:
	DebugDrawPass(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);

	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<DebugDrawManager>& debugDrawManager);

	void Render();


private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<DebugDrawManager> m_DebugDrawManager;


};

