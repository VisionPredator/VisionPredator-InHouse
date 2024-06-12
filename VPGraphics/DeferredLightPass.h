#pragma once
#include "Pass.h"
#include "ResourceManager.h"



class ShaderResourceView;

class DeferredLightPass final : public Pass
{
public:
	DeferredLightPass() = default;
	virtual ~DeferredLightPass() = default;

	void Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height);
	void Render() override;

private:
	// Vertex Buffer
	// Index Buffer


	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;

	
	std::weak_ptr<ShaderResourceView> m_Albedo;
	std::weak_ptr<ShaderResourceView> m_Normal;
	std::weak_ptr<ShaderResourceView> m_Position;
	std::weak_ptr<ShaderResourceView> m_Depth;
	std::weak_ptr<ShaderResourceView> m_Tangent;

	std::weak_ptr < PixelShader > m_PS;
	std::weak_ptr < VertexShader> m_VS;
};

