#pragma once
#include "ConstantBuffer.h"
#include "RenderPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "VertexShader.h"

class DeferredGeometryPass final : public RenderPass
{
public:
	//void Initialize(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager, uint32_t width, uint32_t height);
	void Initialize(Device* device, ResourceManager* resourceManager, uint32_t width, uint32_t height);
	void Render() override;

private:
	//std::shared_ptr<Device> m_Device;
	//std::shared_ptr<ResourceManager> m_ResourceManager;
	Device* m_Device = nullptr;
	ResourceManager* m_ResourceManager = nullptr;

	D3D11_VIEWPORT m_Viewport = {};

	//std::shared_ptr<DepthStencilView> m_DepthStencilView;
	DepthStencilView* m_DepthStencilView = nullptr;

	// Shader
	//std::shared_ptr<VertexShader> m_StaticMeshVS;
	//std::shared_ptr<VertexShader> m_SkeletalMeshVS;
	VertexShader* m_StaticMeshVS = nullptr;
	VertexShader* m_SkeletalMeshVS = nullptr;

	//std::shared_ptr<PixelShader> m_GeometryPS;
	PixelShader* m_GeometryPS = nullptr;

	// Multi Render Target
	//std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	//std::shared_ptr<RenderTargetView> m_NormalRTV;
	//std::shared_ptr<RenderTargetView> m_PositionRTV;
	//std::shared_ptr<RenderTargetView> m_DepthRTV;
	RenderTargetView* m_AlbedoRTV = nullptr;
	RenderTargetView* m_NormalRTV = nullptr;
	RenderTargetView* m_PositionRTV = nullptr;
	RenderTargetView* m_DepthRTV = nullptr;

	// Constant Buffer
	//std::shared_ptr<ConstantBuffer<WorldTransformCB>> m_ModelTransformCB;
	//std::shared_ptr<ConstantBuffer<CameraCB>> m_CameraTransformCB;
	//std::shared_ptr<ConstantBuffer<MatrixPallete>> m_BoneTransformCB;
	//std::shared_ptr<ConstantBuffer<DirectionLightCB>> m_LightTransformCB;
	ConstantBuffer<WorldTransformCB>* m_ModelTransformCB = nullptr;
	ConstantBuffer<CameraCB>* m_CameraTransformCB = nullptr;
	ConstantBuffer<MatrixPallete>* m_BoneTransformCB = nullptr;
	ConstantBuffer<DirectionLightCB>* m_LightTransformCB = nullptr;
};
