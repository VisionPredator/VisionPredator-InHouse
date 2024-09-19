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

void UIManager::Create2DImageObject(uint32_t entityID, const ui::ImageInfo& info)
{
	m_TwoDImages.push_back(std::make_shared<ImageObject>(m_Device, m_ResourceManager, info, entityID));
}

void UIManager::Update2DImageObject(uint32_t entityID, const ui::ImageInfo& info)
{
	for (const auto& ui : m_TwoDImages)
	{
		if (ui->GetID() == entityID)
		{
			ui->SetImageInfo(info);
			return;
		}
	}
}

void UIManager::Delete2DImageObject(uint32_t entityId)
{
	auto it = std::remove_if(m_TwoDImages.begin(), m_TwoDImages.end(),
		[entityId](const std::shared_ptr<ImageObject>& obj)
		{
			return obj->GetID() == entityId;
		});

	if (it != m_TwoDImages.end())
	{
		m_TwoDImages.erase(it, m_TwoDImages.end());
	}
}

void UIManager::DrawAllImages()
{
	/// Render 2D Images
	// Image 의 Layer 값에 따라 정렬하고 그린다.
	std::sort(m_TwoDImages.begin(), m_TwoDImages.end(),
		[&](const std::shared_ptr<ImageObject>& lhs, const std::shared_ptr<ImageObject>& rhs)
		{
			return lhs->GetLayer() > rhs->GetLayer();
		});

	for (const auto& image : m_TwoDImages)
	{
		image->Render();
	}

	/// Render 3D Images
	// TODO
}

void UIManager::DrawAllTexts()
{

}
