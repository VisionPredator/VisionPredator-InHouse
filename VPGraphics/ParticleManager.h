#pragma once

class TimeManager;
class ParticleObject;

class ParticleManager
{
public:
	void Initialize(const std::shared_ptr<class Device>& device, 
		const std::shared_ptr<class ResourceManager>& resourceManager, TimeManager* timeManager);

	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	TimeManager* m_TimeManager = nullptr;

	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
	 
	//std::vector<ParticleObject> m_ParticleObjects;

#pragma region TEST
	std::shared_ptr<ParticleObject> m_TestParticle;
#pragma endregion
};
