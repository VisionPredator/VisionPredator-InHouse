#include "pch.h"
#include "UIManager.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"

#include "ImageObject.h"
//#include "TextObject.h"

void UIManager::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
}

void UIManager::Render()
{
	DrawAllImages();
}

void UIManager::CreateImageObject(const uint32_t& entityID, const ui::ImageInfo& info)
{
	auto imageObject = std::make_shared<ImageObject>(m_Device, m_ResourceManager, info);
	imageObject->SetID(entityID);

	m_ImageObjects.push_back(imageObject);
}

void UIManager::UpdateImageObject(const uint32_t& entityID, const ui::ImageInfo& info)
{
	for (const auto& ui : m_ImageObjects)
	{
		if (ui->GetID() == entityID)
		{
			ui->SetImageInfo(info);
			return;
		}
	}
}

void UIManager::DeleteImageObject(const uint32_t& entityId)
{
}

void UIManager::DrawAllImages()
{
	// Image 의 Layer 값에 따라 정렬하고 그린다.
	std::sort(m_ImageObjects.begin(), m_ImageObjects.end(),
		[&](const std::shared_ptr<ImageObject>& lhs, const std::shared_ptr<ImageObject>& rhs)
		{
			return lhs->GetLayer() > rhs->GetLayer();
		});

	for (const auto& image : m_ImageObjects)
	{
		image->Render();
	}
}
