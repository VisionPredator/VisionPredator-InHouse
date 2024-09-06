#pragma once

/*
 *	이전까지 오프스크린에 렌더했던 결과물을
 *	스왑체인과 연결된 RTV에 렌더하여 마무리하는 패스
 */

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

