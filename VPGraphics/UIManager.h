#pragma once
#include "vpGraphics.h"

/*
 * 게임 UI에 쓰일
 * Image 와 Font를 관리하는 매니저
 */

// TODO: 글자 렌더링 기능을 마저 구현하자.

class ImageObject;	// 비트맵 목록에서 텍스처 리소스 이름으로 파일을 찾아온다.
// class TextObject;

class UIManager
{
public:
	UIManager() = default;

	void Initialize(const std::shared_ptr<class Device>& device
		, const std::shared_ptr<class ResourceManager>& resourceManager);
	void Render();

	// Images
	void Create2DImageObject(uint32_t entityID, const ui::ImageInfo& info);
	void Update2DImageObject(uint32_t entityID, const ui::ImageInfo& info);
	void Delete2DImageObject(uint32_t entityId);
	//void Create3DImageObject(uint32_t entityID, const ui::ImageInfo& info);
	//void Update3DImageObject(uint32_t entityID, const ui::ImageInfo& info);
	//void Delete3DImageObject(uint32_t entityId);

	// TODO: Create Font
	//void CreateTextObject(uint32_t entityID, const ui::TextInfo& info);
	//void UpdateTextObject(uint32_t entityID, const ui::TextInfo& info);
	//void DeleteTextObject(uint32_t entityId);

private: 
	void DrawAllImages();
	void DrawAllTexts();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	// 생성해서 집어넣는 컨테이너를 통해 이미지의 2D 렌더링과 3D 렌더링을 구분짓는다.
	std::vector<std::shared_ptr<ImageObject>> m_TwoDImages;
	std::vector<std::shared_ptr<ImageObject>> m_ThreeDImages;

	// TODO: Font
};
