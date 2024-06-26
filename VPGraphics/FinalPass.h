#pragma once

// 백버퍼에 렌더해 두었던 SRV 를 읽어와
// 최종 화면에 출력될 렌더타겟에 렌더하는 패스

class Device;
class ResourceManager;
class RenderTargetView;

class FinalPass
{
public:
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	//std::shared_ptr<RenderTargetView> m_
};

