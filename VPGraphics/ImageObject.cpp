#include "pch.h"
#include "ImageObject.h"

#include "Device.h"
#include "ResourceManager.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderResourceView.h"

// TODO: screen width �� height �� device �� ��������
// TODO: �̹��� ũ��� ���̴� ���ҽ� ���� �� �����͸� �̴´�.


ImageObject::ImageObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager,
	const ui::ImageInfo& info)
		: m_Device(device), m_ResourceManager(resourceManager)
{
	m_ImagePath = info.ImagePath;
	// TODO: info.etc...
}
