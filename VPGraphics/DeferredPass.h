#pragma once
#include "RenderPass.h"

class DeferredPass : public RenderPass
{
public:
	DeferredPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~DeferredPass();
	virtual void Render() override;
	void OnResize() override;

private:

	void Light();
	void Geometry();

private:
	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	//Geometry
	std::shared_ptr<PixelShader> m_GeometryPS;

	// Multi Render Target
	std::weak_ptr<RenderTargetView> m_AlbedoRTV;
	std::weak_ptr<RenderTargetView> m_NormalRTV;
	std::weak_ptr<RenderTargetView> m_PositionRTV;
	std::weak_ptr<RenderTargetView> m_DepthRTV;
	std::weak_ptr<RenderTargetView> m_MetalicRTV;
	std::weak_ptr<RenderTargetView> m_RoughnessRTV;
	std::weak_ptr<RenderTargetView> m_AORTV;
	std::weak_ptr<RenderTargetView> m_EmissiveRTV;

	//GBuffer Texture
	std::weak_ptr<ShaderResourceView> m_Albedo;
	std::weak_ptr<ShaderResourceView> m_Normal;
	std::weak_ptr<ShaderResourceView> m_Position;
	std::weak_ptr<ShaderResourceView> m_Depth;
	std::weak_ptr<ShaderResourceView> m_Metalic;
	std::weak_ptr<ShaderResourceView> m_Roughness;
	std::weak_ptr<ShaderResourceView> m_AO;
	std::weak_ptr<ShaderResourceView> m_Emissive;

	//Light - Quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;
	std::weak_ptr<PixelShader> m_QuadPS;
	std::weak_ptr<PixelShader> m_Deferred;

	//GBuffer Result
	std::weak_ptr<ShaderResourceView> m_GBuffer;

	std::unique_ptr<DirectX::DX11::CommonStates> m_States;
};

