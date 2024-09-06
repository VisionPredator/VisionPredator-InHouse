#pragma once

/*
 *	�ƿ����� �ι�° �н�
 *	����� ������ ���� �����Ͽ� �� �β��� �ε巴�� �����.
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

