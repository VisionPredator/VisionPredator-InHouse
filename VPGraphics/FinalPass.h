#pragma once

// ����ۿ� ������ �ξ��� SRV �� �о��
// ���� ȭ�鿡 ��µ� ����Ÿ�ٿ� �����ϴ� �н�

class Device;
class ResourceManager;
class RenderTargetView;

class FinalPass
{
public:
	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager);
	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	//std::shared_ptr<RenderTargetView> m_
};

