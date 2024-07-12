#pragma once


class ResourceManager;
class Device;
class ParticleObject;

class ParticleManager
{
public:
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;

	std::vector<ParticleObject> m_ParticleObjects;
};
