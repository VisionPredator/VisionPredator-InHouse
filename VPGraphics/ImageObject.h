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

	struct ImageTransformCB
	{
		VPMath::Matrix World;
		VPMath::Matrix View;
		VPMath::Matrix Projection;
	};

	struct ColorCB
	{
		VPMath::Color Color;	// rgba
	};
	static_assert(sizeof(ColorCB) % 16 == 0, "must be align");

public:
	ImageObject(const std::shared_ptr<Device>& device, 
		const std::shared_ptr<class ResourceManager>& resourceManager,
		const ui::ImageInfo& info, uint32_t id);
	~ImageObject() = default;
	void Render();

	void SetID(uint32_t entityID) { m_ID = entityID; }
	void SetImageInfo(const ui::ImageInfo& info);
	uint32_t GetID() const { return m_ID; }
	uint32_t GetLayer() const { return m_Info.Layer; }

private:
	bool InitializeBuffers();
	void UpdateBuffers();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;

	uint32_t m_ID = 0;
	ui::ImageInfo m_Info;

	// 화면 사이즈는 UI 매니저가 가지고 잇는게 나을듯.
	int m_CanvasWidth = 0;		// 렌더링을 할 정확한 정점 위치가 필요하기 때문에 저장.
	int m_CanvasHeight = 0;
	uint32_t m_BitmapWidth = 0;
	uint32_t m_BitmapHeight = 0;

	float m_PreviousPosXPercent = 0.f;	// 이전 프레임과 비교하여 위치가 변하지 않았다면
	float m_PreviousPosYPercent = 0.f;	// 동적 정점 버퍼를 바꾸지 않기 때문에 성능의 향상을 꾀할 수 있다.
	uint32_t m_PreviousWidth = -1;
	uint32_t m_PreviousHeight = -1;
	float m_PreviousScale = -1;
	uint32_t m_PreviousScreenWidth = 0;
	uint32_t m_PreviousScreenHeight = 0;

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	ImageTransformCB m_Transform;
	std::shared_ptr<ConstantBuffer<ImageTransformCB>> m_ImageTransformCB;
	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;		// 이미지의 색상 값을 pixel shader 에 넘겨주기 위해 필요하다.

	std::shared_ptr<ShaderResourceView> m_Texture;
	
	int m_vertexCount = 0;
	int m_indexCount = 0;

	float m_ImageCenterPosX = 0;
	float m_ImageCenterPosY = 0;
	float m_ImagePosX = 0;
	float m_ImagePosY = 0;
};
