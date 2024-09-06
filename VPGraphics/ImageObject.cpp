#include "pch.h"
#include "ImageObject.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderResourceView.h"
#include "Util.h"

ImageObject::ImageObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager,
	const ui::ImageInfo& info, const uint32_t& id)
		: m_Device(device), m_ResourceManager(resourceManager), m_Info(info)
{
	m_ID = id;

	// 화면 크기 저장
	m_ScreenWidth = m_Device->GetWndSize().right - m_Device->GetWndSize().left;
	m_ScreenHeight = m_Device->GetWndSize().bottom - m_Device->GetWndSize().top;

	// 모델의 텍스처 로드
	if (m_Info.ImagePath.empty())	// 텍스처 경로가 비어있다면 기본 텍스처 가져오기.
	{
		m_Texture = m_ResourceManager->Get<ShaderResourceView>(L"DefaultUI").lock();
		m_Info.ImagePath = "DefaultUI.png";
	}
	else
		m_Texture = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.ImagePath.c_str()), Util::ToWideChar(m_Info.ImagePath.c_str())).lock();

	// 렌더링할 비트맵의 픽셀의 크기를 저장
	m_BitmapWidth = m_Texture->GetWidth();
	m_BitmapHeight = m_Texture->GetHeight();

	// 이전 렌더링 정보를 음수로 초기화
	m_PreviousWidth = -1;
	m_PreviousHeight = -1;
	m_PreviousScale = -1;

	// 정점 및 인덱스 버퍼 초기화
	InitializeBuffers();

	// Initialize Constant Buffers
	m_ColorCB = m_ResourceManager->Create<ConstantBuffer<ColorCB>>(L"ImageColorCB", ConstantBufferType::Default).lock();
}

void ImageObject::Render()
{
	// 화면의 다른 위치로 렌더링하기 위해 동적 정점 버퍼를 다시 빌드한다.
	UpdateBuffers();

	// 그리기를 준비하기 위해 그래픽 파이프 라인에 버텍스와 인덱스 버퍼를 바인드한다.
	UINT stride = sizeof(ImageVertex);
	UINT offset = 0;

	// CB Update;
	ColorCB alpha;
	alpha.Color = m_Info.Color;
	m_ColorCB->Update(alpha);

	m_Device->Context()->IASetVertexBuffers(0, 1, m_VertexBuffer->GetAddress(), &stride, &offset);
	m_Device->Context()->IASetIndexBuffer(m_IndexBuffer->Get(), DXGI_FORMAT_R32_UINT, 0);
	m_Device->Context()->PSSetShaderResources(0, 1, m_Texture->GetAddress());
	m_Device->Context()->PSSetConstantBuffers(0, 1, m_ColorCB->GetAddress());
	m_Device->Context()->DrawIndexed(m_indexCount, 0, 0);
}

void ImageObject::SetImageInfo(const ui::ImageInfo& info)
{
	m_Info.PosXPercent = info.PosXPercent;
	m_Info.PosYPercent = info.PosYPercent;
	m_Info.Layer = info.Layer;
	m_Info.ImagePath = info.ImagePath;
	m_Info.Color = info.Color;
	m_Info.Scale = info.Scale;

	// 모델의 텍스처 로드
	if (m_Info.ImagePath.empty())	// 텍스처 경로가 비어있다면 기본 텍스처 가져오기.
	{
		m_Texture = m_ResourceManager->Get<ShaderResourceView>(L"DefaultUI").lock();
		m_Info.ImagePath = "DefaultUI";
	}
	else
		m_Texture = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.ImagePath.c_str()), Util::ToWideChar(m_Info.ImagePath.c_str())).lock();

	m_BitmapWidth = m_Texture->GetWidth();
	m_BitmapHeight = m_Texture->GetHeight();
}

bool ImageObject::InitializeBuffers()
{
	// 정점 배열의 정점 수와 인덱스 배열의 인덱스 수를 지정한다.
	// 두 개의 삼각형을 만들어야 하기 때문의 정점 수를 6으로 설정한다.
	m_indexCount = m_vertexCount = 6;

	std::vector<ImageVertex> vertices;
	vertices.resize(m_vertexCount);
	if (vertices.empty())
		return false;

	std::fill(vertices.begin(), vertices.end(), ImageVertex{});

	std::vector<unsigned int> indices;
	indices.resize(m_indexCount);
	if (indices.empty())
		return false;

	for (size_t i = 0; i < m_indexCount; ++i)
	{
		indices[i] = static_cast<unsigned int>(i);
	}

	// 동적 정점 버퍼의 생성
	m_VertexBuffer = std::make_shared<VertexBuffer>(m_Device, vertices, true);

	// 인덱스 버퍼 생성
	m_IndexBuffer = std::make_shared<IndexBuffer>(m_Device, indices);

	return true;
}

void ImageObject::UpdateBuffers()
{
	std::vector<ImageVertex> vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	m_ScreenWidth = m_Device->GetWndWidth();
	m_ScreenHeight = m_Device->GetWndHeight();

	// 이미지의 위치가 이전과 비교하여 달라지지 않았다면 버퍼를 업데이트하지 않는다.
	if ((m_Info.PosXPercent == m_PreviousPosXPercent && m_Info.PosYPercent == m_PreviousPosYPercent)
		&& (m_BitmapWidth == m_PreviousWidth && m_BitmapHeight == m_PreviousHeight)
		&& (m_Info.Scale == m_PreviousScale)
		&& (m_ScreenWidth == m_PreviousScreenWidth) && (m_ScreenHeight == m_PreviousScreenHeight))
		return;

	// 퍼센트 기반의 값을 비율로 변환 (0.0 ~ 1.0)
	float relPosX = m_Info.PosXPercent / 100.0f;  // 1%는 0.01로 변환
	float relPosY = m_Info.PosYPercent / 100.0f;  // 1%는 0.01로 변환

	// 새로운 해상도에 맞춰 이미지 중심의 위치를 계산합니다.
	float centerX = relPosX * m_ScreenWidth;  // 이미지 중심의 X 위치
	float centerY = relPosY * m_ScreenHeight; // 이미지 중심의 Y 위치
	m_ImageCenterPosX = centerX;
	m_ImageCenterPosY = centerY;

	// 이미지의 스케일링된 크기 계산
	float scaledWidth = m_BitmapWidth * m_Info.Scale;
	float scaledHeight = m_BitmapHeight * m_Info.Scale;

	// 이미지 중심 기준으로 좌측 상단 좌표를 계산
	float calculatedPosX = centerX - (scaledWidth / 2.0f);
	float calculatedPosY = centerY - (scaledHeight / 2.0f);
	m_ImagePosX = calculatedPosX;
	m_ImagePosY = calculatedPosY;

	// 렌더링 되는 위치와 크기를 업데이트한다.
	m_PreviousPosXPercent = m_Info.PosXPercent;
	m_PreviousPosYPercent = m_Info.PosYPercent;
	m_PreviousWidth = m_BitmapWidth;
	m_PreviousHeight = m_BitmapHeight;
	m_PreviousScale = m_Info.Scale;
	m_PreviousScreenWidth = m_ScreenWidth;
	m_PreviousScreenHeight = m_ScreenHeight;

	// 비트맵의 좌표 계산
	//const float left = (float)((m_ScreenWidth / 2) * (-1)) + m_Info.PosXPercent;
	//const float left = static_cast<float>((m_ScreenWidth / 2) * (-1)) + calculatedPosX;
	const float left = static_cast<float>((m_ScreenWidth / 2) * (-1)) + calculatedPosX;
	const float right = left + scaledWidth;
	//const float top = (float)(m_ScreenHeight / 2) - m_Info.PosYPercent;
	const float top = static_cast<float>(m_ScreenHeight / 2) - calculatedPosY;
	const float bottom = top - scaledHeight;

	vertices.resize(m_vertexCount);
	if (vertices.empty())
		return;

	// 동적 정점 배열에 데이터를 로드한다.
	// 첫 번째 삼각형
	vertices[0].Position = DirectX::XMFLOAT4(left, top, 0.0f, 1.0f);		// Top left.
	vertices[0].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	vertices[1].Position = DirectX::XMFLOAT4(right, bottom, 0.0f, 1.0f);	// Bottom right.
	vertices[1].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);
	vertices[2].Position = DirectX::XMFLOAT4(left, bottom, 0.0f, 1.0f);		// Bottom left.
	vertices[2].TexCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
	// 두 번째 삼각형
	vertices[3].Position = DirectX::XMFLOAT4(left, top, 0.0f, 1.0f);		// Top left.
	vertices[3].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	vertices[4].Position = DirectX::XMFLOAT4(right, top, 0.0f, 1.0f);		// Top right.
	vertices[4].TexCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
	vertices[5].Position = DirectX::XMFLOAT4(right, bottom, 0.0f, 1.0f);	// Bottom right.
	vertices[5].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	// 버텍스 버퍼를 쓸 수 있도록 잠근다.
	const HRESULT result = m_Device->Context()->Map(m_VertexBuffer->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return;

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	ImageVertex* verticesPtr = static_cast<ImageVertex*>(mappedResource.pData);

	// 데이터를 정점 버퍼에 복사한다.
	memcpy(verticesPtr, vertices.data(), sizeof(ImageVertex) * m_vertexCount);

	// 동적 정점 버퍼의 잠금을 해제한다.
	m_Device->Context()->Unmap(m_VertexBuffer->Get(), 0);
}
