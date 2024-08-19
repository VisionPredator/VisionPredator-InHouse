#pragma once

/*
 *	�ƿ����� ù��° �н�
 *	Laplacian Filter�� ����Ͽ� ������Ʈ�� ������ �����Ѵ�.
 */

class OutlineEdgeDetectPass
{
public:
	OutlineEdgeDetectPass();

	void Initialize(const std::shared_ptr<class Device>& device,
		const std::shared_ptr<class ResourceManager>& resourceManager);
	void Render();

	void OnResize();

private:
	std::shared_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;


};

