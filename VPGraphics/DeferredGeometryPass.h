#pragma once
#include "ConstantBuffer.h"
#include "Pass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "VertexShader.h"

#include <map>
#include <utility>
#include <string>
#include <memory>

#include "MeshFilter.h"

class Mesh;
class ModelData;
class RenderState;

class DeferredGeometryPass final : public Pass
{
public:
	void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Render() override;
	//void TestRender(const std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& renderList);

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
	enum { GBufferSize = 4 };	// 상수. #define 보다 디버깅할때 더 편하다.
	std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	std::shared_ptr<RenderTargetView> m_NormalRTV;
	std::shared_ptr<RenderTargetView> m_PositionRTV;
	std::shared_ptr<RenderTargetView> m_DepthRTV;

	// Constant Buffer
	std::shared_ptr<ConstantBuffer<TransformData>> m_ModelTransformCB;
	std::shared_ptr<ConstantBuffer<MatrixPallete>> m_BoneTransformCB;

	//std::shared_ptr<ConstantBuffer<WorldTransformCB>> m_ModelTransformCB;
	//std::shared_ptr<ConstantBuffer<CameraData>> m_CameraTransformCB;
	//std::shared_ptr<ConstantBuffer<MatrixPallete>> m_BoneTransformCB;
	//std::shared_ptr<ConstantBuffer<DirectionLightCB>> m_LightTransformCB;
};
