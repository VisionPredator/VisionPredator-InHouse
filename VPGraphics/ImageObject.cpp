#include "pch.h"
#include "ImageObject.h"

#include "Device.h"
#include "ResourceManager.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderResourceView.h"

// TODO: screen width 와 height 는 device 로 가져오고
// TODO: 이미지 크기는 쉐이더 리소스 만든 후 데이터를 뽑는다.


ImageObject::ImageObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager,
	const ui::ImageInfo& info)
		: m_Device(device), m_ResourceManager(resourceManager)
{
	m_ImagePath = info.ImagePath;
	// TODO: info.etc...
}
