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

	void Update(const float& deltaTime, const float& totalGameTime);
	void Draw();
	
	void SetParticleInfo(const effect::ParticleInfo& info) { m_Info = info; }

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// CB structure
	PerFrame m_PerFrame = {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InitVB;		// emit	// 초기화용 방출기 입자
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamOutVB;	// simulate
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawVB;		// draw

	std::shared_ptr<ConstantBuffer<PerFrame>> m_FrameCB;

	std::shared_ptr<ShaderResourceView> m_TextureSRV;	// 파티클 2d 텍스쳐
	std::shared_ptr<ShaderResourceView> m_RandomTextureSRV;		// 쉐이더용 난수 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TexArraySRV;		//						임시
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTexSRV;	// 쉐이더용 난수 텍스처,	임시

	std::shared_ptr<Sampler> m_SamLinear;
	std::shared_ptr<VertexShader> m_StreamOutVS;
	std::shared_ptr<VertexShader> m_DrawVS;
	std::shared_ptr<GeometryShader> m_StreamOutGS;
	std::shared_ptr<GeometryShader> m_DrawGS;
	std::shared_ptr<PixelShader> m_DrawPS;

	bool m_FirstRun = true;

	float m_TotalGameTime = 0.f;	// 쉐이더에서 랜덤 값을 얻기 위해..
	float m_DeltaTime = 0.f;
	float m_TimeStep = 0.f;
	float m_Age = 0.f;

	// 수치 조절용
	effect::ParticleInfo m_Info;
	// TODO 아래 것도 교체좀.
	VPMath::Vector3 m_EyePosW = VPMath::Vector3(0.0f, 0.0f, 0.0f);
	VPMath::Vector3 m_EmitPosW = VPMath::Vector3(0.0f, 0.0f, 0.0f);
	VPMath::Vector3 m_EmitDirW = VPMath::Vector3(0.0f, 1.0f, 0.0f);
};

