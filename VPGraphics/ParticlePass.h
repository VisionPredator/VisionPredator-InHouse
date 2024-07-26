#pragma once

class TimeManager;

class ParticlePass
{
public:

	void Initialize(const std::shared_ptr<class Device>& device,
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const std::shared_ptr<class ParticleManager>& particleManager,
		TimeManager* timeManager);

	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager = nullptr;
};
