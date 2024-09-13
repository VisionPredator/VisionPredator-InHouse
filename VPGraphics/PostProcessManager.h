#pragma once


// SRV 를 주면 이펙트 처리하고 돌려주기.

class ComputeShader;
class ShaderResourceView;
class ResourceManager;
class Device;

class PostProcessManager
{
public:
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();
	void OnResize();

	std::shared_ptr<ShaderResourceView> Blur(const std::shared_ptr<ShaderResourceView>& inputSRV, const int& blurCount);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	std::shared_ptr<ComputeShader> m_HorizontalBlurCS;
	std::shared_ptr<ComputeShader> m_VerticalBlurCS;



};

