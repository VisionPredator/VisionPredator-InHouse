#pragma once
#include "vpGraphics.h"

class TimeManager;
class ParticleObject;

class ParticleManager
{
public:
	ParticleManager() = default;

	void Initialize(const std::shared_ptr<class Device>& device, 
		const std::shared_ptr<class ResourceManager>& resourceManager, TimeManager* timeManager);

	void Render();

	void CreateParticleObject(const uint32_t& id, const effect::ParticleInfo& info);
	void DeleteParticleObjectByID(const uint32_t& id);

	void UpdateParticleInfoByID(const uint32_t& id, const effect::ParticleInfo& info);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	TimeManager* m_TimeManager = nullptr;

	int a = 1;

	std::map<uint32_t, std::shared_ptr<ParticleObject>> m_ParticleObjects;
};
