#pragma once

#include "MeshFilter.h"
#include "ModelData.h"

#include <map>
#include <string>
#include <memory>
#include <queue>


class ShaderResourceView;
class RenderTargetView;
class ResourceManager;
class Device;

class DeferredShadingPipeline
{
public:
	DeferredShadingPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager);
	~DeferredShadingPipeline();

	void Update(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& RenderList);


	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// Passes
	std::shared_ptr<class DeferredGeometryPass> m_DeferredGeometryPass;
	std::shared_ptr<class DeferredLightPass> m_DeferredLightPass;

	// Multi Render Target
	enum { GBufferSize = 4 };	// 상수. 일단 만들어는 뒀는데 언제 쓸까.
	std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	std::shared_ptr<RenderTargetView> m_NormalRTV;
	std::shared_ptr<RenderTargetView> m_PositionRTV;
	std::shared_ptr<RenderTargetView> m_DepthRTV;

	// Shader Resource Views
	std::shared_ptr<ShaderResourceView> m_AlbedoSRV;
	std::shared_ptr<ShaderResourceView> m_NormalSRV;
	std::shared_ptr<ShaderResourceView> m_PositionSRV;
	std::shared_ptr<ShaderResourceView> m_DepthSRV;



	std::queue<std::shared_ptr<ModelData>> m_RenderQueue;


};
