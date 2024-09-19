#pragma once
#include "RenderPass.h"




class FadeInFadeOut : public RenderPass
{
public:
	FadeInFadeOut(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> managr);
	~FadeInFadeOut();

	virtual void Render() override;
	virtual void OnResize() override;
private:
	//Quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;
	std::weak_ptr<PixelShader> m_FadeInFadeOut;

	std::weak_ptr<ShaderResourceView> m_Gbuffer;

};

