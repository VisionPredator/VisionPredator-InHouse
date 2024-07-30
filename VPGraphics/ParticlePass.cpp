#include "pch.h"
#include "ParticlePass.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include "TimeManager.h"

#include "Device.h"

void ParticlePass::Initialize(const std::shared_ptr<Device>& device,
                              const std::shared_ptr<ResourceManager>& resourceManager, 
                              const std::shared_ptr<ParticleManager>& particleManager,
                              TimeManager* timeManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_ParticleManager = particleManager;
	m_TimeManager = timeManager;
}

void ParticlePass::Render()
{
	m_ParticleManager->Render();
}
