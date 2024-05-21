#pragma once
#include "ConstantBuffer.h"
#include "Pass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "VertexShader.h"

class DeferredGeometryPass final : public Pass
{
public:
	// TODO: 매개변수로 전달할때 생포인터로 전달 금지. 소유권을 명확히 할 수 없음. 무조건 참조로 전달.
	//void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Initialize(Device* device, ResourceManager* resourceManager, const uint32_t& width, const uint32_t& height);		// 포인터를 매개변수로 받는건 좀.. 바꿔야 한다.
	void Render() override;

private:
	// TODO: 스마트 포인터로 싹 바꾸어야 합니다~ (다 바꾸어야 한다는 뜻은 아님)

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
	VertexShader* m_SkinnedMeshVS = nullptr;

	//std::shared_ptr<PixelShader> m_GeometryPS;
	PixelShader* m_GeometryPS = nullptr;

	enum { GBufferSize = 4 };	// 상수. 일단 만들어는 뒀는데 언제 쓸까.

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
