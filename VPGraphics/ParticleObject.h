#pragma once
#include <wrl/client.h>

#include "ConstantBuffer.h"
#include "d3d11.h"

class PixelShader;
class GeometryShader;
class VertexShader;
class ResourceManager;
class VertexBuffer;
class Device;

class ParticleObject
{
public:
	struct PerFrame
	{
		DirectX::SimpleMath::Matrix ViewProj;
		DirectX::SimpleMath::Vector3 EyePosW;
		float GameTime;
		DirectX::SimpleMath::Vector3 EmitPosW;
		float TimeStep;
		DirectX::SimpleMath::Vector3 EmitDirW;
		float Unused;
	};

	struct ParticleVertex
	{
		DirectX::SimpleMath::Vector3 InitialPos;
		DirectX::SimpleMath::Vector3 InitialVel;
		DirectX::SimpleMath::Vector2 Size;
		float Age;
		unsigned int Type;
	};

public:
	ParticleObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	~ParticleObject() = default;

	void Update(float deltaTime, float totalGameTime);
	void Draw(const std::shared_ptr<Device>& device, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;

	PerFrame m_PerFrame = {};

	//std::shared_ptr<VertexBuffer> m_InitVB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InitVB;		// emit	// 초기화용 방출기 입자
	//std::shared_ptr<VertexBuffer> m_StreamOutVB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamOutVB;	// simulate
	//std::shared_ptr<VertexBuffer> m_DrawVB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawVB;		// draw

	std::shared_ptr<ConstantBuffer<PerFrame>> m_FrameCB;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TexArraySRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RandomTexSRV;	// 쉐이더용 난수 텍스처

	std::shared_ptr<Sampler> m_SamLinear;

	std::shared_ptr<VertexShader> m_StreamOutVS;
	std::shared_ptr<VertexShader> m_DrawVS;
	std::shared_ptr<GeometryShader> m_StreamOutGS;
	std::shared_ptr<GeometryShader> m_DrawGS;
	std::shared_ptr<PixelShader> m_DrawPS;


	bool m_FirstRun = true;
	UINT m_MaxParticles = 0;

	float m_TotalGameTime = 0.f;	// 쉐이더에서 랜덤 값을 얻기 위해..  TODO: 다른 방법도 찾아보자.
	float m_DeltaTime = 0.f;
	float m_TimeStep = 0.f;
	float m_Age = 0.f;

	DirectX::SimpleMath::Vector3 m_EyePosW = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_EmitPosW = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_EmitDirW = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
};

