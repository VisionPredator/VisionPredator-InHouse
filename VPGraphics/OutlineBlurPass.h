#pragma once

/*
 *	아웃라인 두번째 패스
 *	검출된 엣지에 블러를 적용하여 더 두껍고 부드럽게 만든다.
 */

class ViewPort;
class IndexBuffer;
class VertexBuffer;
class Sampler;
class PixelShader;
class VertexShader;
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
	std::shared_ptr<RenderTargetView> m_DownScaledRTV;
	std::shared_ptr<ShaderResourceView> m_OutlineEdgeDetectSRV;

	std::shared_ptr<VertexShader> m_OutlineBlurVS;
	std::shared_ptr<PixelShader> m_OutlineBlurPS;
	std::shared_ptr<PixelShader> m_OutlineAddPS;

	std::shared_ptr<Sampler> m_DefaultSS;

	std::shared_ptr<VertexBuffer> m_QuadVB;
	std::shared_ptr<IndexBuffer> m_QuadIB;
};
