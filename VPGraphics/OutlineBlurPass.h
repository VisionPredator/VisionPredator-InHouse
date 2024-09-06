#pragma once

/*
 *	아웃라인 두번째 패스
 *	검출된 엣지에 블러를 적용하여 더 두껍고 부드럽게 만든다.
 */

class ShaderResourceView;
class RenderTargetView;
class ResourceManager;
class Device;

class OutlineBlurPass
{
public:
	void Initialize(const std::shared_ptr<Device>& device,
		const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();
	void OnResize();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	std::shared_ptr<RenderTargetView> m_OutlineBlurRTV;
	std::shared_ptr<ShaderResourceView> m_OutlineEdgeDetectSRV;


};

