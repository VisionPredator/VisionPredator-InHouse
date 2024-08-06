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

	void Render();

	uint32_t GetID() { return m_ID; }
	uint32_t GetLayer() { return m_Info.Layer; }
	void SetID(const uint32_t& entityID) { m_ID = entityID; }
	void SetImageInfo(const ui::ImageInfo& info);

private:
	bool InitializeBuffers();
	void UpdateBuffers();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	uint32_t m_ID = 0;
	ui::ImageInfo m_Info;

	// ȭ�� ������� UI �Ŵ����� ������ �մ°� ������.
	uint32_t m_ScreenWidth = 0;		// �������� �� ��Ȯ�� ���� ��ġ�� �ʿ��ϱ� ������ ����.
	uint32_t m_ScreenHeight = 0;
	uint32_t m_BitmapWidth = 0;		// �ӽ�
	uint32_t m_BitmapHeight = 0;	// �ӽ�

	uint32_t m_PreviousPosX = -1;	// ���� �����Ӱ� ���Ͽ� ��ġ�� ������ �ʾҴٸ�
	uint32_t m_PreviousPosY = -1;	// ���� ���� ���۸� �ٲ��� �ʱ� ������ ������ ����� ���� �� �ִ�.

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	/*
	 * Constant Buffer
	 * �̹����� ���� ���� pixel shader �� �Ѱ��ֱ� ���� �ʿ��ϴ�.
	 */
	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;	

	int m_vertexCount = 0;
	int m_indexCount = 0;

	std::shared_ptr<ShaderResourceView> m_Texture;
};
