#pragma once
#include "vpGraphics.h"
/*
 * ���� UI�� ����
 * Image �� Font�� �����ϴ� �Ŵ���
 */

class ImageObject;	// ��Ʈ�� ��Ͽ��� �ؽ�ó ���ҽ� �̸����� ������ ã�ƿ´�.
// class TextObject;

class UIManager
{
public:
	UIManager() = default;

	void Initialize(const std::shared_ptr<class Device>& device
		, const std::shared_ptr<class ResourceManager>& resourceManager);

	void Render();

	// Image
	void CreateImageObject(const uint32_t& entityID, const ui::ImageInfo& info);
	void UpdateImageObject(const uint32_t& entityID, const ui::ImageInfo& info);
	void DeleteImageObject(const uint32_t& entityId);

	// TODO: Create Font
	//void CreateTextObject(const uint32_t& entityID, const ui::TextInfo& info);
	//void UpdateTextObject(const uint32_t& entityID, const ui::TextInfo& info);
	//void DeleteTextObject(const uint32_t& entityId);

private:
	void DrawAllImages();
	void DrawAllTexts();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// Image
	std::vector<std::shared_ptr<ImageObject>> m_ImageObjects;

	// TODO: Font

};
