#pragma once
#include "vpGraphics.h"

/*
 * ���� UI�� ����
 * Image �� Font�� �����ϴ� �Ŵ���
 */

// TODO: ���� ������ ����� ���� ��������.

class ImageObject;	// ��Ʈ�� ��Ͽ��� �ؽ�ó ���ҽ� �̸����� ������ ã�ƿ´�.
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

	// �����ؼ� ����ִ� �����̳ʸ� ���� �̹����� 2D �������� 3D �������� �������´�.
	std::vector<std::shared_ptr<ImageObject>> m_TwoDImages;
	std::vector<std::shared_ptr<ImageObject>> m_ThreeDImages;

	// TODO: Font
};
