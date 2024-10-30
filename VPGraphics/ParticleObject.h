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
		float TimeStep;
		float dummy1;
		float dummy2;
		float dummy3;
	};
	static_assert(sizeof(PerFrame) % 16 == 0, "must be align");

	struct DataCB
	{
		VPMath::Vector3 EmitPosW;	// Transform 에서 위치값을 뽑아와서 준다.
		float dummy1;
		VPMath::Vector3 EmitDirW;	// 오브젝트이 Front Vector 값을 준다.
		float dummy2;

		VPMath::Vector2 StartSizeA;
		VPMath::Vector2 StartSizeB;

		float Duration;
		int IsLoop;

		float StartLifetimeA;
		float StartLifetimeB;

		float Angle;
		float Radius;

		unsigned int ParticleShape;
		unsigned int RenderMode;

	};
	static_assert(sizeof(DataCB) % 16 == 0, "must be align");

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
	DataCB m_Data = {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InitVB;		// emit	// 초기화용 방출기 입자
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamOutVB;	// simulate
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawVB;		// draw

	std::shared_ptr<ConstantBuffer<PerFrame>> m_FrameCB;
	std::shared_ptr<ConstantBuffer<DataCB>> m_DataCB;

	std::shared_ptr<ShaderResourceView> m_TextureSRV;	// 파티클 2d 텍스쳐
	std::shared_ptr<ShaderResourceView> m_RandomTextureSRV;		// 쉐이더용 난수 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TexArraySRV;		// TODO:					임시
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTexSRV;	// TODO: 쉐이더용 난수 텍스처	임시

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
};

