#pragma once
#include "RenderPass.h"

class LightManager;

class DeferredPass : public RenderPass
{
public:
	// 일단 임시. 아래 생성자 없애면 이것도 삭제 해야한다.
	DeferredPass() = default;

	DeferredPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, std::shared_ptr<LightManager>lightmanager);
	~DeferredPass();

	void Initialize(const std::shared_ptr<Device>& device,
		const std::shared_ptr<ResourceManager>& resourceManager,
		std::shared_ptr<LightManager>& lightManager);
	void Render() override;
	void OnResize() override;

private:
	void PreDepth();
	void Geometry();
	void Light();

private:
	std::weak_ptr<LightManager> m_LightManager;
private:
	std::weak_ptr<DepthStencilView> m_DepthStencilView;

	//Geometry
	std::weak_ptr<PixelShader> m_GeometryPS;

	// Multi Render Target
	std::weak_ptr<RenderTargetView> m_AlbedoRTV;
	std::weak_ptr<RenderTargetView> m_NormalRTV;
	std::weak_ptr<RenderTargetView> m_PositionRTV;
	std::weak_ptr<RenderTargetView> m_DepthRTV;
	std::weak_ptr<RenderTargetView> m_MetalicRoughnessRTV;
	std::weak_ptr<RenderTargetView> m_AORTV;
	std::weak_ptr<RenderTargetView> m_EmissiveRTV;
	std::weak_ptr<RenderTargetView> m_LightMapRTV;

	//GBuffer Texture
	std::weak_ptr<ShaderResourceView> m_Albedo;
	std::weak_ptr<ShaderResourceView> m_Normal;
	std::weak_ptr<ShaderResourceView> m_Position;
	std::weak_ptr<ShaderResourceView> m_Depth;
	std::weak_ptr<ShaderResourceView> m_MetalicRoughness;
	std::weak_ptr<ShaderResourceView> m_AO;
	std::weak_ptr<ShaderResourceView> m_Emissive;
	std::weak_ptr<ShaderResourceView> m_LightMap;

	//Light - Quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;
	std::weak_ptr<PixelShader> m_QuadPS;
	std::weak_ptr<PixelShader> m_Deferred;

	//GBuffer Result
	std::weak_ptr<ShaderResourceView> m_GBuffer;
};

