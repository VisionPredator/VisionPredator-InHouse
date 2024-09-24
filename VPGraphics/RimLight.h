#pragma once
#include "RenderPass.h"




class RimLight : public RenderPass
{
public:
	RimLight(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> managr);
	~RimLight();

	virtual void Render() override;
	virtual void OnResize() override;
private:
	//Quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;

	std::weak_ptr<PixelShader> m_RimLightPS;
	std::weak_ptr<PixelShader> m_QuadPS;

	std::weak_ptr<ShaderResourceView> m_Gbuffer;
	std::weak_ptr<ShaderResourceView> m_Normal;
	std::weak_ptr<ShaderResourceView> m_Position;
	std::weak_ptr<ShaderResourceView> m_Mask;

	std::weak_ptr<ShaderResourceView> m_RimLightSRV;
	std::weak_ptr<RenderTargetView> m_RimLightRTV;
};

