#pragma once


// SRV �� �ָ� ����Ʈ ó���ϰ� �����ֱ�.

class ResourceManager;
class Device;

class PostProcessManager
{
public:
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();
	void OnResize();


private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;



};

