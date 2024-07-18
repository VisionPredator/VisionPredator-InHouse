#include "pch.h"
#include "ParticleManager.h"
#include "ParticleObject.h"
#include "Device.h"
#include "ResourceManager.h"
#include "TimeManager.h"


void ParticleManager::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, TimeManager* timeManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_TimeManager = timeManager;

#pragma region TEST
	// 하나만 생성하고 되는지 한번 보자.
	m_TestParticle = std::make_shared<ParticleObject>(m_Device, m_ResourceManager);
#pragma endregion
}

void ParticleManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_View = view;
	m_Proj = proj;

	m_TestParticle->Update(m_TimeManager->GetDeltaTime(), m_TimeManager->GetTotalGameTime());
	m_TestParticle->Draw(m_Device, m_View, m_Proj);
}
