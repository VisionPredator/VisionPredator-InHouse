#include "pch.h"
#include "ParticleManager.h"
#include "ParticleObject.h"
#include "Device.h"
#include "ResourceManager.h"

void ParticleManager::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager)
	: m_Device(device), m_ResourceManager(resourceManager)
{

}

void ParticleManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{

}
