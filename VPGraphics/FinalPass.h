#pragma once

/*
 *	�������� ������ũ���� �����ߴ� �������
 *	����ü�ΰ� ����� RTV�� �����Ͽ� �������ϴ� �н�
 */

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

