#pragma once

/*
 *	아웃라인 첫번째 패스
 *	Laplacian Filter를 사용하여 오브젝트의 엣지를 검출한다.
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

