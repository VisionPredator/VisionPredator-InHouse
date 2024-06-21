#pragma once
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <queue>
#include <directxtk/CommonStates.h>
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/Effects.h>
#include <directxtk/VertexTypes.h>
#include <wrl/client.h>

#include "vpGraphics.h"

using namespace DirectX;

class Camera;
class Device;

class DebugDrawManager
{
public:
	void Initialize(const std::shared_ptr<Device>& device);

	void Execute(const std::shared_ptr<Device>& device, const std::shared_ptr<Camera>& camera);

	void AddTask(const debug::SphereInfo& info) { m_SphereInfos.push(info); }
	void AddTask(const debug::BoxInfo& info);
	void AddTask(const debug::OBBInfo& info);
	void AddTask(const debug::FrustumInfo& info);
	void AddTask(const debug::GridInfo& info);
	void AddTask(const debug::RingInfo& info);
	void AddTask(const debug::TriangleInfo& info);
	void AddTask(const debug::QuadInfo& info);
	void AddTask(const debug::RayInfo& info);

private:
	void Draw(const debug::SphereInfo& info);
	void Draw(const debug::BoxInfo& info);
	void Draw(const debug::OBBInfo& info);
	void Draw(const debug::FrustumInfo& info);
	void Draw(const debug::TriangleInfo& info);
	void Draw(const debug::QuadInfo& info);
	void Draw(const debug::RayInfo& info);

	void DrawRing(const debug::RingInfo& info);
	void DrawCube(const SimpleMath::Matrix& worldTransform, const SimpleMath::Color& color);

private:
	std::unique_ptr<CommonStates> m_States;
	std::unique_ptr<BasicEffect> m_BatchEffect;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>> m_Batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_BatchInputLayout;

	// Task Container
	std::queue<debug::SphereInfo> m_SphereInfos;
	// etc...
	
};

