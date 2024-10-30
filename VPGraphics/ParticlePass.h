#pragma once

/*
 *	Geometry Shader 를 사용해 Stream Output 기능을 이용하여
 *	파티클을 생성하고 렌더하는 패스
 */

class TimeManager;
class ParticlePass
{
public:

	void Initialize(const std::shared_ptr<class Device>& device,
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const std::shared_ptr<class ParticleManager>& particleManager,
		TimeManager* timeManager);

	void Render(float deltaTime);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager = nullptr;
};
