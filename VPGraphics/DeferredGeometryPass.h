#pragma once
#include "ConstantBuffer.h"
#include "Pass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "VertexShader.h"

class DeferredGeometryPass final : public Pass
{
public:
	void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Render() override;

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	
	D3D11_VIEWPORT m_Viewport = {};

	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	// Shader
	std::shared_ptr<VertexShader> m_StaticMeshVS;
	std::shared_ptr<VertexShader> m_SkeletalMeshVS;
	
	std::shared_ptr<PixelShader> m_GeometryPS;

	// Multi Render Target
	enum { GBufferSize = 4 };	// 상수. 일단 만들어는 뒀는데 언제 쓸까.
	std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	std::shared_ptr<RenderTargetView> m_NormalRTV;
	std::shared_ptr<RenderTargetView> m_PositionRTV;
	std::shared_ptr<RenderTargetView> m_DepthRTV;

	// Constant Buffer
	std::shared_ptr<ConstantBuffer<WorldTransformCB>> m_ModelTransformCB;
	std::shared_ptr<ConstantBuffer<CameraCB>> m_CameraTransformCB;
	std::shared_ptr<ConstantBuffer<MatrixPallete>> m_BoneTransformCB;
	std::shared_ptr<ConstantBuffer<DirectionLightCB>> m_LightTransformCB;

	// VB 와 IB 는 여기서는 없어도 괜찮다. 어차피 MRT 에 그리는 것이 목적이기 때문.
};
