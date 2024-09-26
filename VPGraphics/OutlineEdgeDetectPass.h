#pragma once
#include "RenderPass.h"
#include "ConstantBuffer.h"

/*
 *	�ƿ����� ù��° �н�
 *	ObjectMask �н����� ����ŷ�� �̹����� 
 *	Laplacian Filter�� ����Ͽ� ������Ʈ�� ������ �����Ѵ�.
 */

class IndexBuffer;
class VertexBuffer;
class RenderTargetView;
class PixelShader;
class VertexShader;
class ShaderResourceView;

class OutlineEdgeDetectPass : public RenderPass
{
	struct ScreenSizeCB
	{
		unsigned int Width;
		unsigned int Height;
		float pad[2];
	};
	static_assert(sizeof(ScreenSizeCB) % 16 == 0, "must be align");

public:
	void Initialize(const std::shared_ptr<class Device>& device,
		const std::shared_ptr<class ResourceManager>& resourceManager);
	void Render();
	void OnResize();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	std::shared_ptr<RenderTargetView> m_OutlineEdgeDetectRTV;
	std::shared_ptr<ShaderResourceView> m_ObjectMaskSRV;

	std::shared_ptr<VertexShader> m_OutlineEdgeDetectVS;
	std::shared_ptr<PixelShader> m_OutlineEdgeDetectPS;

	std::shared_ptr<Sampler> m_LinearClampSS;

	std::shared_ptr<VertexBuffer> m_QuadVB;
	std::shared_ptr<IndexBuffer> m_QuadIB;
	std::shared_ptr<ConstantBuffer<ScreenSizeCB>> m_ScreenSizeCB;
	ScreenSizeCB m_ScreenSize = {};
};

