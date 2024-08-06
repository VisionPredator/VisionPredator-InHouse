#include "pch.h"
#include "UIPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "UIManager.h"

void UIPass::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager,
	const std::shared_ptr<UIManager>& uiManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_UIManager = uiManager;

	ui::ImageInfo testInfo = {};
	m_UIManager->CreateImageObject(518, testInfo);
}

void UIPass::Render()
{
	m_UIManager->Render();
}
