#pragma once

class UIPass
{
public:
	void Initialize(const std::shared_ptr<class Device>& device,
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const std::shared_ptr<class UIManager>& uiManager);

	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<UIManager> m_UIManager;


};

