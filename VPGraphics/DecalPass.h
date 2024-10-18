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
	std::weak_ptr<VertexBuffer> m_BoundingVolume;	//depth buffer���� Ȯ���� ����
	std::weak_ptr<ShaderResourceView> m_Texture;	//� �ؽ�ó�� ��Į�� �׷�������
	std::weak_ptr<ConstantBuffer<VPMath::XMFLOAT3>> m_VolumeSize;	//bounding volume �� �࿡���� ������

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

