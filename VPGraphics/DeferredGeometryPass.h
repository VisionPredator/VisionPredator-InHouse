#pragma once
#include "ConstantBuffer.h"
#include "Device.h"
#include "VertexShader.h"
#include "ModelData.h"

#include <string>
#include <queue>

enum class PassState : unsigned;
class Mesh;
class RenderState;
class ResourceManager;

class DeferredGeometryPass
{
public:
	DeferredGeometryPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager);
	~DeferredGeometryPass();

	void Render(const std::shared_ptr<ModelData>& model);

private:
	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
	
	D3D11_VIEWPORT m_Viewport = {};

	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	// Shader
	std::shared_ptr<VertexShader> m_StaticMeshVS;
	std::shared_ptr<VertexShader> m_SkeletalMeshVS;
	
	std::shared_ptr<PixelShader> m_GeometryPS;
	std::shared_ptr<PixelShader> m_StaticPS;
	std::shared_ptr<PixelShader> m_SkinningPS;

	// Multi Render Target
	enum { GBufferSize = 4 };	// 상수. #define 보다 디버깅할때 더 편하다.
	std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	std::shared_ptr<RenderTargetView> m_NormalRTV;
	std::shared_ptr<RenderTargetView> m_PositionRTV;
	std::shared_ptr<RenderTargetView> m_DepthRTV;

};
