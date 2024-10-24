#pragma once
#include "ResourceManager.h"


class DeferredLightPass
{
public:
	DeferredLightPass() = default;
	DeferredLightPass(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager);
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);

	void Render();



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
	std::shared_ptr<ShaderResourceView> m_Metalic;
	std::shared_ptr<ShaderResourceView> m_Roughness;
	std::shared_ptr<ShaderResourceView> m_AO;
	std::shared_ptr<ShaderResourceView> m_Emissive;

	std::shared_ptr<ShaderResourceView> m_GBuffer;
	
	std::shared_ptr<DepthStencilView> m_DepthStencilView;


	std::weak_ptr < PixelShader > m_PS;
	std::weak_ptr < VertexShader> m_VS;

	std::weak_ptr <VertexBuffer> m_QuadVB;
	std::weak_ptr < IndexBuffer > m_QuadIB;
	std::weak_ptr<PixelShader> m_QuadPS;


	/// TEST
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DSS_Null;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BS_Null;
};

