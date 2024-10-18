#pragma once
#include "RenderPass.h"
#include "ResourceManager.h"
#include "DecalManager.h"

class DecalPass :
    public RenderPass
{
public:
	DecalPass(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DecalManager> decalmanager);
	~DecalPass();

	virtual void Render() override;
	virtual void OnResize() override;

private:
	std::shared_ptr<DecalManager> m_DecalManager;
private:
	std::weak_ptr<VertexBuffer> m_BoundingVolume;	//depth buffer에서 확인한 영역
	std::weak_ptr<ShaderResourceView> m_Texture;	//어떤 텍스처를 데칼로 그려낼건지
	std::weak_ptr<ConstantBuffer<VPMath::XMFLOAT3>> m_VolumeSize;	//bounding volume 각 축에대한 사이즈

	//GBuffer Texture
	std::weak_ptr<ShaderResourceView> m_AlbedoSRV;
	std::weak_ptr<ShaderResourceView> m_NormalSRV;
	std::weak_ptr<ShaderResourceView> m_PositionSRV;
	std::weak_ptr<ShaderResourceView> m_DepthSRV;
	std::weak_ptr<ShaderResourceView> m_MetalicRoughnessSRV;

	//quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;
	std::weak_ptr<PixelShader> m_QuadPS;

	//GBuffer Result
	std::weak_ptr<ShaderResourceView> m_GBufferSRV;

	//
	std::weak_ptr<PixelShader> m_DecalPS;
};

