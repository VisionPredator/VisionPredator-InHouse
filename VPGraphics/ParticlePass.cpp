#include "pch.h"
#include "ParticlePass.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include "TimeManager.h"

#include "Device.h"

void ParticlePass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager, 
	const std::shared_ptr<ParticleManager>& particleManager,
	TimeManager* timeManager,
	const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_ParticleManager = particleManager;
	m_TimeManager = timeManager;
	m_View = view;
	m_Proj = proj;
}

void ParticlePass::Render()
{
	m_View = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock()->m_struct.view;
	m_Proj = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock()->m_struct.proj;

	m_ParticleManager->Render(m_View.Transpose(), m_Proj.Transpose());
}
