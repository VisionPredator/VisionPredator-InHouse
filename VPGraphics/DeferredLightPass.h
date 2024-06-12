#pragma once
#include "Pass.h"
#include "ResourceManager.h"



class ShaderResourceView;

class DeferredLightPass final : public Pass
{
public:
	DeferredLightPass() = default;
	DeferredLightPass(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager);
	virtual ~DeferredLightPass() = default;

	void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Render() override;

private:
	// Vertex Buffer
	// Index Buffer


	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;

	
	// Multi Render Target
	enum { GBufferSize = 4 };	// 상수. #define 보다 디버깅할때 더 편하다.
	std::shared_ptr<ShaderResourceView> m_Albedo;
	std::shared_ptr<ShaderResourceView> m_Normal;
	std::shared_ptr<ShaderResourceView> m_Position;
	std::shared_ptr<ShaderResourceView> m_Depth;
	
	std::shared_ptr<DepthStencilView> m_DepthStencilView;


	std::weak_ptr < PixelShader > m_PS;
	std::weak_ptr < VertexShader> m_VS;
};

