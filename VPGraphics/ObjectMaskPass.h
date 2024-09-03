#pragma once
#include "ConstantBuffer.h"
#include "RenderPass.h"

/*
 *	Ư�� ������Ʈ�� ���� ����ũ�� ����
 *	�� Ư�� ������Ʈ�� ���� �÷��� �����Ѵ�.
 *	Outline�� �׸��� ��, ����Ʈ ���μ��� �ܰ迡 Ȱ���� �ؽ�ó�� �����ϴ� �н�
 */

struct MaskColorCB
{
	VPMath::Color Color;
};
static_assert(sizeof(MaskColorCB) % 16 == 0, "must be align");

class ObjectMaskPass : public RenderPass
{
public:
	ObjectMaskPass(const std::shared_ptr<Device>& device,
		const std::shared_ptr<ResourceManager>& resourceManager);
	void OnResize();
	void Render();

private:
	std::shared_ptr<RenderTargetView> m_ObjectMaskRTV;
	std::shared_ptr<DepthStencilView> m_DefaultDSV;
	std::shared_ptr<class DepthStencilState> m_DSS;

	std::shared_ptr<PixelShader> m_ObjectMaskPS;
	std::shared_ptr<VertexShader> m_ObjectMaskStaticMeshVS;
	std::shared_ptr<VertexShader> m_ObjectMaskSkeletalMeshVS;

	std::shared_ptr<ConstantBuffer<MaskColorCB>> m_MaskColorCB;
};

