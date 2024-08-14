#pragma once
#include "ConstantBuffer.h"
#include "vpGraphics.h"
// 2D 이미지 오브젝트

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

	// 화면 사이즈는 UI 매니저가 가지고 잇는게 나을듯.
	int m_ScreenWidth = 0;		// 렌더링을 할 정확한 정점 위치가 필요하기 때문에 저장.
	int m_ScreenHeight = 0;
	uint32_t m_BitmapWidth = 0;		// 임시
	uint32_t m_BitmapHeight = 0;	// 임시

	float m_PreviousPosX = -1;	// 이전 프레임과 비교하여 위치가 변하지 않았다면
	float m_PreviousPosY = -1;	// 동적 정점 버퍼를 바꾸지 않기 때문에 성능의 향상을 꾀할 수 있다.

	uint32_t m_PreviousWidth = -1;
	uint32_t m_PreviousHeight = -1;

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;		// 이미지의 색상 값을 pixel shader 에 넘겨주기 위해 필요하다.

	std::shared_ptr<ShaderResourceView> m_Texture;
	
	int m_vertexCount = 0;
	int m_indexCount = 0;
};
