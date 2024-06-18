#pragma once
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <directxtk/CommonStates.h>
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/Effects.h>
#include <directxtk/VertexTypes.h>
#include <wrl/client.h>

class Device;

class DebugDrawManager
{
public:
	void Initialize(const std::shared_ptr<Device>& device);

	//void Draw(const )


private:
	std::unique_ptr<DirectX::CommonStates> m_States;
	std::unique_ptr<DirectX::BasicEffect> m_BatchEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_Batch;




};

