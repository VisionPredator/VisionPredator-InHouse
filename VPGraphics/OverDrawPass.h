#pragma once
#include "RenderPass.h"


//사실상 gbuffer위에 forward로 덮어 그린것뿐
class OverDrawPass :
    public RenderPass
{
public:
	OverDrawPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~OverDrawPass();

	virtual void Render() override;
	virtual void OnResize() override;
private:
	//Quad
	std::weak_ptr<PixelShader> m_meshPS;

	std::weak_ptr<ShaderResourceView> m_Gbuffer;

};

