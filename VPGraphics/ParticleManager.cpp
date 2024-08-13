#include "pch.h"
#include "ParticleManager.h"

#include <memory>

#include "ParticleObject.h"
#include "Device.h"
#include "ResourceManager.h"
#include "TimeManager.h"



void ParticleManager::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, TimeManager* timeManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_TimeManager = timeManager;
}

void ParticleManager::Render()
{
	for (const auto& object : m_ParticleObjects)
	{
		object.second->Update(m_TimeManager->GetDeltaTime(), m_TimeManager->GetTotalGameTime());
		object.second->Draw();
	}
}

void ParticleManager::CreateParticleObject(const uint32_t& id, const effect::ParticleInfo& info)
{
	m_ParticleObjects[id] = std::make_shared<ParticleObject>(m_Device, m_ResourceManager, info);
	m_ParticleObjects[id]->SetParticleInfo(info);
}

void ParticleManager::DeleteParticleObjectByID(const uint32_t& id)
{
	auto it = m_ParticleObjects.find(id);
	if (it != m_ParticleObjects.end())
	{
		m_ParticleObjects.erase(it);
	}
}

void ParticleManager::UpdateParticleInfoByID(const uint32_t& id, const effect::ParticleInfo& info)
{
	m_ParticleObjects[id]->SetParticleInfo(info);
}
