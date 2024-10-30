#pragma once
#include <wrl/client.h>

#include "ConstantBuffer.h"
#include "d3d11.h"
#include "ParticleManager.h"
#include "vpGraphics.h"

class PixelShader;
class GeometryShader;
class VertexShader;
class ResourceManager;
class VertexBuffer;

class ParticleObject
{
public:
	friend ParticleManager;
public:
	struct PerFrame
	{
		VPMath::Matrix ViewProj;
		VPMath::Vector3 EyePosW;
		float GameTime;
		VPMath::Vector3 EmitPosW;
		float TimeStep;
		VPMath::Vector3 EmitDirW;
		float Unused;
	};
	static_assert(sizeof(PerFrame) % 16 == 0, "must be align");

	struct InfoCB
	{
		VPMath::Vector3 EmitPosW;	// Transform ���� ��ġ���� �̾ƿͼ� �ش�.
		VPMath::Vector3 EmitDirW;	// ������Ʈ�� Front Vector ���� �ش�.

		VPMath::Vector2 StartSizeA;
		VPMath::Vector2 StartSizeB;

		float Duration;
		int IsLoop;

		float StartLifetimeA;
		float StartLifetimeB;

		float Angle;
		float Radius;

		int ParticleShape;
		int RenderMode;

		float dummy1;
		float dummy2;
	};
	static_assert(sizeof(InfoCB) % 16 == 0, "must be align");

	struct ParticleVertex
	{
		VPMath::Vector3 InitialPos;
		VPMath::Vector3 InitialVel;
		VPMath::Vector2 Size;
		float Age;
		unsigned int Type;
	};

public:
	ParticleObject(const std::shared_ptr<class Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const effect::ParticleInfo& info);
	~ParticleObject() = default;

	void Update();
	void Draw(float deltaTime, float totalGameTime);
	
	void SetParticleInfo(const effect::ParticleInfo& info);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// CB structure
	PerFrame m_PerFrame = {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InitVB;		// emit	// �ʱ�ȭ�� ����� ����
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamOutVB;	// simulate
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawVB;		// draw

	std::shared_ptr<ConstantBuffer<PerFrame>> m_FrameCB;

	std::shared_ptr<ShaderResourceView> m_TextureSRV;	// ��ƼŬ 2d �ؽ���
	std::shared_ptr<ShaderResourceView> m_RandomTextureSRV;		// ���̴��� ���� �ؽ�ó
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TexArraySRV;		// TODO:					�ӽ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTexSRV;	// TODO: ���̴��� ���� �ؽ�ó	�ӽ�

	std::shared_ptr<Sampler> m_SamLinear;
	std::shared_ptr<VertexShader> m_StreamOutVS;
	std::shared_ptr<VertexShader> m_DrawVS;
	std::shared_ptr<GeometryShader> m_StreamOutGS;
	std::shared_ptr<GeometryShader> m_DrawGS;
	std::shared_ptr<PixelShader> m_DrawPS;

	bool m_FirstRun = true;

	float m_TotalGameTime = 0.f;	// ���̴����� ���� ���� ��� ����..
	float m_DeltaTime = 0.f;
	float m_TimeStep = 0.f;
	float m_Age = 0.f;

	// ��ġ ������
	effect::ParticleInfo m_Info;
	// TODO �Ʒ� �͵� ��ü��.
	VPMath::Vector3 m_EyePosW = VPMath::Vector3(0.0f, 0.0f, 0.0f);
	VPMath::Vector3 m_EmitPosW = VPMath::Vector3(0.0f, 0.0f, 0.0f);
	VPMath::Vector3 m_EmitDirW = VPMath::Vector3(0.0f, 1.0f, 0.0f);


	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
};

