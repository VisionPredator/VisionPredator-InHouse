#pragma once
#include "RenderPass.h"


//��ǻ� gbuffer���� forward�� ���� �׸��ͻ�
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

