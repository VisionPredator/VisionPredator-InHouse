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
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TexCoord;
	};

	struct ColorCB
	{
		VPMath::Vector4 Color;	// rgba
	};

public:
	ImageObject(const std::shared_ptr<class Device>& device, 
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const ui::ImageInfo& info, const uint32_t& id);
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
	int m_ScreenWidth = 0;		// �������� �� ��Ȯ�� ���� ��ġ�� �ʿ��ϱ� ������ ����.
	int m_ScreenHeight = 0;
	uint32_t m_BitmapWidth = 0;		// �ӽ�
	uint32_t m_BitmapHeight = 0;	// �ӽ�

	float m_PreviousPosX = -1;	// ���� �����Ӱ� ���Ͽ� ��ġ�� ������ �ʾҴٸ�
	float m_PreviousPosY = -1;	// ���� ���� ���۸� �ٲ��� �ʱ� ������ ������ ����� ���� �� �ִ�.

	uint32_t m_PreviousWidth = -1;
	uint32_t m_PreviousHeight = -1;

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;		// �̹����� ���� ���� pixel shader �� �Ѱ��ֱ� ���� �ʿ��ϴ�.

	std::shared_ptr<ShaderResourceView> m_Texture;
	
	int m_vertexCount = 0;
	int m_indexCount = 0;
};
