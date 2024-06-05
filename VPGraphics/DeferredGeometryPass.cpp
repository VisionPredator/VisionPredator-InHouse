#include "pch.h"
#include "DeferredGeometryPass.h"

#include "Desc.h"

void DeferredGeometryPass::Initialize(std::shared_ptr<Device>& device,
	std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	m_Viewport.Width = static_cast<FLOAT>(width);
	m_Viewport.Height = static_cast<FLOAT>(height);

	m_DepthStencilView = std::make_shared<DepthStencilView>(device, DepthStencilViewType::Default, width, height);

	m_StaticMeshVS = std::make_shared<VertexShader>(device, L"../../../VPGraphics/TextureVS.hlsl", -1);
	m_SkeletalMeshVS = std::make_shared<VertexShader>(device, L"../../../VPGraphics/SkinningVS.hlsl", -1);
	m_GeometryPS = std::make_shared<PixelShader>(device, L"GeometryPass_PS");


}

void DeferredGeometryPass::Render()
{

}
