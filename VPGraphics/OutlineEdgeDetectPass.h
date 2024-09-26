#pragma once
#include "RenderPass.h"
#include "ConstantBuffer.h"

/*
 *	아웃라인 첫번째 패스
 *	ObjectMask 패스에서 마스킹한 이미지를 
 *	Laplacian Filter를 사용하여 오브젝트의 엣지를 검출한다.
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

