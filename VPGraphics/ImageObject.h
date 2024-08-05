#pragma once
#include "ConstantBuffer.h"
#include "vpGraphics.h"
// 2D �̹��� ������Ʈ

class ShaderResourceView;
class IndexBuffer;
class VertexBuffer;

class ImageObject
{
private:
	struct ImageVertex
	{
		VPMath::Vector3 Position;
		VPMath::Vector2 TexCoord;
	};

	struct ColorCB
	{
		VPMath::Vector4 Color;	// rgba
	};

public:
	ImageObject(const std::shared_ptr<class Device>& device, 
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const ui::ImageInfo& info);
	~ImageObject() = default;

	void Initialize() {}

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	std::string m_ImagePath;	

	uint32_t m_ScreenWidth = 0;		// �������� �� ��Ȯ�� ���� ��ġ�� �ʿ��ϱ� ������ ����.
	uint32_t m_ScreenHeight = 0;
	uint32_t m_BitmapWidth = 0;
	uint32_t m_BitmapHeight = 0;

	uint32_t m_PreviousPosX = -1;	// ���� �����Ӱ� ���Ͽ� ��ġ�� ������ �ʾҴٸ�
	uint32_t m_PreviousPosY = -1;	// ���� ���� ���۸� �ٲ��� �ʱ� ������ ������ ����� ���� �� �ִ�.

	uint32_t m_Layer = -1;	// �̹��� ������ ���� �ʿ��ϴ�.

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	/*
	 * Constant Buffer
	 * �̹����� ���� ���� pixel shader �� �Ѱ��ֱ� ���� �ʿ��ϴ�.
	 */
	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;	

	int m_vertexCount = 0;
	int m_indexCount = 0;

	// �긦 ������ ���� ���� �����Ҷ� Bitmap ���ҽ��� �߿��� �������°ɷ� �ұ�?
	// �ٸ� image object �� ���� bitmap �� ���� �� ������..
	std::shared_ptr<ShaderResourceView> m_Texture;
	//TextureClass* m_Texture = nullptr;
};
