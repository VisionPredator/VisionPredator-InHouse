#pragma once
#include "vpGraphics.h"
/*
 * 게임 UI에 쓰일
 * Image 와 Font를 관리하는 매니저
 */

class ImageObject;	// 비트맵 목록에서 텍스처 리소스 이름으로 파일을 찾아온다.
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

