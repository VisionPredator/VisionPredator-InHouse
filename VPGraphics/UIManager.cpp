#include "pch.h"
#include "UIManager.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"

#include "Bitmap.h"
#include "ImageObject.h"
//#include "TextObject.h"

void UIManager::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
		: m_Device(device), m_ResourceManager(resourceManager)
{
}

void UIManager::CreateImageObject(const uint32_t& entityID, const ui::ImageInfo& info)
{
    std::shared_ptr<ImageObject> imageObject = std::make_shared<ImageObject>(m_Device, m_ResourceManager, info);

	std::map<uint32_t, std::shared_ptr<ImageObject>> imageMap;
	imageMap[entityID] = imageObject;

	m_Images.push_back(imageMap);
}

void UIManager::UpdateImageObject(const uint32_t& entityID, const ui::ImageInfo& info)
{
}

void UIManager::DeleteImageObject(const uint32_t& entityId)
{
}

void UIManager::DrawAllImages()
{
	// TODO: Image 의 Layer 값에 따라 정렬하고 그린다.

}
