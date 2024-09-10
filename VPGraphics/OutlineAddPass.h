#pragma once

/*
 *	�ƿ����� ������ �н�
 *	������ ��ģ ���� �ܰ����� ����ȭ�� �ؽ��ĸ� �����Ѵ�.
 */

class DepthStencilView;
class IndexBuffer;
class VertexBuffer;
class Sampler;
class PixelShader;
class VertexShader;
class ShaderResourceView;
class RenderTargetView;
class ResourceManager;
class Device;

class OutlineAddPass
{
public:
	void Initialize(const std::shared_ptr<Device>& device,
		const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();
	void OnResize();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	std::shared_ptr<RenderTargetView> m_OffScreenRTV;
	std::shared_ptr<DepthStencilView> m_DefaultDSV;
	std::shared_ptr<ShaderResourceView> m_OutlineBlurSRV;

	std::shared_ptr<VertexShader> m_OutlineAddVS;
	std::shared_ptr<PixelShader> m_OutlineAddPS;

	std::shared_ptr<Sampler> m_DefaultSS;

	std::shared_ptr<VertexBuffer> m_QuadVB;
	std::shared_ptr<IndexBuffer> m_QuadIB;
};

