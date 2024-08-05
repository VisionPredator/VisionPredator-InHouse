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
		DirectX::SimpleMath::Vector3 Position;
		DirectX::SimpleMath::Vector2 TexCoord;
	};

	struct ColorCB
	{
		DirectX::SimpleMath::Vector4 Color;	// rgba
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

	uint32_t m_ScreenWidth = 0;		// 렌더링을 할 정확한 정점 위치가 필요하기 때문에 저장.
	uint32_t m_ScreenHeight = 0;
	uint32_t m_BitmapWidth = 0;
	uint32_t m_BitmapHeight = 0;

	uint32_t m_PreviousPosX = -1;	// 이전 프레임과 비교하여 위치가 변하지 않았다면
	uint32_t m_PreviousPosY = -1;	// 동적 정점 버퍼를 바꾸지 않기 때문에 성능의 향상을 꾀할 수 있다.

	uint32_t m_Layer = -1;	// 이미지 정렬을 위해 필요하다.

	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	/*
	 * Constant Buffer
	 * 이미지의 색상 값을 pixel shader 에 넘겨주기 위해 필요하다.
	 */
	std::shared_ptr<ConstantBuffer<ColorCB>> m_ColorCB;	

	int m_vertexCount = 0;
	int m_indexCount = 0;

	// 얘를 가지고 있지 말고 렌더할때 Bitmap 리소스들 중에서 가져오는걸로 할까?
	// 다른 image object 라도 같은 bitmap 을 가질 수 있으니..
	std::shared_ptr<ShaderResourceView> m_Texture;
	//TextureClass* m_Texture = nullptr;
};
