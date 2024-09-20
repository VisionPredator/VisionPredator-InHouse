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

void UIManager::CreateImageObject(uint32_t entityID, const ui::ImageInfo& info)
{
	m_Images.push_back(std::make_shared<ImageObject>(m_Device, m_ResourceManager, info, entityID));
}

void UIManager::UpdateImageObject(uint32_t entityID, const ui::ImageInfo& info)
{
	for (const auto& ui : m_Images)
	{
		if (ui->GetID() == entityID)
		{
			ui->SetImageInfo(info);
			return;
		}
	}
}

void UIManager::DeleteImageObject(uint32_t entityId)
{
	auto it = std::remove_if(m_Images.begin(), m_Images.end(),
		[entityId](const std::shared_ptr<ImageObject>& obj)
		{
			return obj->GetID() == entityId;
		});

	if (it != m_Images.end())
	{
		m_Images.erase(it, m_Images.end());
	}
}

void UIManager::DrawAllImages()
{
	// Image 의 Layer 값에 따라 정렬하고 그린다.
	std::sort(m_Images.begin(), m_Images.end(),
		[&](const std::shared_ptr<ImageObject>& lhs, const std::shared_ptr<ImageObject>& rhs)
		{
			return lhs->GetLayer() > rhs->GetLayer();
		});

	for (const auto& image : m_Images)
	{
		image->Render();
	}
}

void UIManager::DrawAllTexts()
{

}
