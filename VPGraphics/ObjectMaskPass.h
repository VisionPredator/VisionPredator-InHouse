#pragma once
#include "RenderPass.h"

/*
 *	특정 오브젝트에 대한 마스크를 생성
 *	즉 특정 오브젝트를 단일 컬러로 렌더한다.
 *	Outline을 그리는 등, 포스트 프로세싱 단계에 활용할 텍스처를 생성하는 패스
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

