#pragma once
#include "RenderPass.h"

/*
 *	Ư�� ������Ʈ�� ���� ����ũ�� ����
 *	�� Ư�� ������Ʈ�� ���� �÷��� �����Ѵ�.
 *	Outline�� �׸��� ��, ����Ʈ ���μ��� �ܰ迡 Ȱ���� �ؽ�ó�� �����ϴ� �н�
 */

class ObjectMaskPass : public RenderPass
{
public:
	ObjectMaskPass(std::shared_ptr<Device> device,
		std::shared_ptr<ResourceManager> resourceManager);
	void OnResize();
	void Render();

private:
	std::shared_ptr<RenderTargetView> m_ObjectMaskRTV;
	std::shared_ptr<DepthStencilView> m_DefaultDSV;
	std::shared_ptr<class DepthStencilState> m_DSS;

	std::shared_ptr<PixelShader> m_ObjectMaskPS;
	std::shared_ptr<VertexShader> m_ObjectMaskStaticMeshVS;
	std::shared_ptr<VertexShader> m_ObjectMaskSkeletalMeshVS;
};

