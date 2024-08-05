#pragma once
#include "vpGraphics.h"
/*
 * 게임 UI에 쓰일
 * Image 와 Font를 관리하는 매니저
 */

class Bitmap;		// 2D 이미지 텍스처 리소스
class ImageObject;	// 비트맵 목록에서 텍스처 리소스 이름으로 파일을 찾아온다.
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
	std::unordered_map<std::wstring, Bitmap*> m_Bitmaps;	// ImageObject 의 ImagePath 를 키값으로 한다.
	//std::vector<ImageObject*> m_Images;
	std::vector<std::map<uint32_t, std::shared_ptr<ImageObject>>> m_Images;

	// Font
};

