#pragma once
#include "vpGraphics.h"
/*
 * ���� UI�� ����
 * Image �� Font�� �����ϴ� �Ŵ���
 */

class Bitmap;		// 2D �̹��� �ؽ�ó ���ҽ�
class ImageObject;	// ��Ʈ�� ��Ͽ��� �ؽ�ó ���ҽ� �̸����� ������ ã�ƿ´�.
// class TextObject;

class UIManager
{
public:
	UIManager() = default;
	~UIManager() = default;

	void Initialize(const std::shared_ptr<class Device>& device
		, const std::shared_ptr<class ResourceManager>& resourceManager);

	void Render();

	// Image
	void CreateImageObject(const uint32_t& entityID, const ui::ImageInfo& info);
	void UpdateImageObject(const uint32_t& entityID, const ui::ImageInfo& info);
	void DeleteImageObject(const uint32_t& entityId);

	// TODO
	// Font
	//void CreateTextObject(const uint32_t& entityID, const ui::TextInfo& info);
	//void UpdateTextObject(const uint32_t& entityID, const ui::TextInfo& info);
	//void DeleteTextObject(const uint32_t& entityId);

private:
	void AddImage();

	void DrawAllImages();
	void DrawAllTexts();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// Image
	std::unordered_map<std::wstring, Bitmap*> m_Bitmaps;	// ImageObject �� ImagePath �� Ű������ �Ѵ�.
	//std::vector<ImageObject*> m_Images;
	std::vector<std::map<uint32_t, std::shared_ptr<ImageObject>>> m_Images;

	// Font
};

