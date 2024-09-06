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

	// ȭ�� ũ�� ����
	m_ScreenWidth = m_Device->GetWndSize().right - m_Device->GetWndSize().left;
	m_ScreenHeight = m_Device->GetWndSize().bottom - m_Device->GetWndSize().top;

	// ���� �ؽ�ó �ε�
	if (m_Info.ImagePath.empty())	// �ؽ�ó ��ΰ� ����ִٸ� �⺻ �ؽ�ó ��������.
	{
		m_Texture = m_ResourceManager->Get<ShaderResourceView>(L"DefaultUI").lock();
		m_Info.ImagePath = "DefaultUI.png";
	}
	else
		m_Texture = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.ImagePath.c_str()), Util::ToWideChar(m_Info.ImagePath.c_str())).lock();

	// �������� ��Ʈ���� �ȼ��� ũ�⸦ ����
	m_BitmapWidth = m_Texture->GetWidth();
	m_BitmapHeight = m_Texture->GetHeight();

	// ���� ������ ������ ������ �ʱ�ȭ
	m_PreviousWidth = -1;
	m_PreviousHeight = -1;
	m_PreviousScale = -1;

	// ���� �� �ε��� ���� �ʱ�ȭ
	InitializeBuffers();

	// Initialize Constant Buffers
	m_ColorCB = m_ResourceManager->Create<ConstantBuffer<ColorCB>>(L"ImageColorCB", ConstantBufferType::Default).lock();
}

void ImageObject::Render()
{
	// ȭ���� �ٸ� ��ġ�� �������ϱ� ���� ���� ���� ���۸� �ٽ� �����Ѵ�.
	UpdateBuffers();

	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� ���ؽ��� �ε��� ���۸� ���ε��Ѵ�.
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

	// ���� �ؽ�ó �ε�
	if (m_Info.ImagePath.empty())	// �ؽ�ó ��ΰ� ����ִٸ� �⺻ �ؽ�ó ��������.
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
	// ���� �迭�� ���� ���� �ε��� �迭�� �ε��� ���� �����Ѵ�.
	// �� ���� �ﰢ���� ������ �ϱ� ������ ���� ���� 6���� �����Ѵ�.
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

	// ���� ���� ������ ����
	m_VertexBuffer = std::make_shared<VertexBuffer>(m_Device, vertices, true);

	// �ε��� ���� ����
	m_IndexBuffer = std::make_shared<IndexBuffer>(m_Device, indices);

	return true;
}

void ImageObject::UpdateBuffers()
{
	std::vector<ImageVertex> vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	m_ScreenWidth = m_Device->GetWndWidth();
	m_ScreenHeight = m_Device->GetWndHeight();

	// �̹����� ��ġ�� ������ ���Ͽ� �޶����� �ʾҴٸ� ���۸� ������Ʈ���� �ʴ´�.
	if ((m_Info.PosXPercent == m_PreviousPosXPercent && m_Info.PosYPercent == m_PreviousPosYPercent)
		&& (m_BitmapWidth == m_PreviousWidth && m_BitmapHeight == m_PreviousHeight)
		&& (m_Info.Scale == m_PreviousScale)
		&& (m_ScreenWidth == m_PreviousScreenWidth) && (m_ScreenHeight == m_PreviousScreenHeight))
		return;

	// �ۼ�Ʈ ����� ���� ������ ��ȯ (0.0 ~ 1.0)
	float relPosX = m_Info.PosXPercent / 100.0f;  // 1%�� 0.01�� ��ȯ
	float relPosY = m_Info.PosYPercent / 100.0f;  // 1%�� 0.01�� ��ȯ

	// ���ο� �ػ󵵿� ���� �̹��� �߽��� ��ġ�� ����մϴ�.
	float centerX = relPosX * m_ScreenWidth;  // �̹��� �߽��� X ��ġ
	float centerY = relPosY * m_ScreenHeight; // �̹��� �߽��� Y ��ġ
	m_ImageCenterPosX = centerX;
	m_ImageCenterPosY = centerY;

	// �̹����� �����ϸ��� ũ�� ���
	float scaledWidth = m_BitmapWidth * m_Info.Scale;
	float scaledHeight = m_BitmapHeight * m_Info.Scale;

	// �̹��� �߽� �������� ���� ��� ��ǥ�� ���
	float calculatedPosX = centerX - (scaledWidth / 2.0f);
	float calculatedPosY = centerY - (scaledHeight / 2.0f);
	m_ImagePosX = calculatedPosX;
	m_ImagePosY = calculatedPosY;

	// ������ �Ǵ� ��ġ�� ũ�⸦ ������Ʈ�Ѵ�.
	m_PreviousPosXPercent = m_Info.PosXPercent;
	m_PreviousPosYPercent = m_Info.PosYPercent;
	m_PreviousWidth = m_BitmapWidth;
	m_PreviousHeight = m_BitmapHeight;
	m_PreviousScale = m_Info.Scale;
	m_PreviousScreenWidth = m_ScreenWidth;
	m_PreviousScreenHeight = m_ScreenHeight;

	// ��Ʈ���� ��ǥ ���
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

	// ���� ���� �迭�� �����͸� �ε��Ѵ�.
	// ù ��° �ﰢ��
	vertices[0].Position = DirectX::XMFLOAT4(left, top, 0.0f, 1.0f);		// Top left.
	vertices[0].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	vertices[1].Position = DirectX::XMFLOAT4(right, bottom, 0.0f, 1.0f);	// Bottom right.
	vertices[1].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);
	vertices[2].Position = DirectX::XMFLOAT4(left, bottom, 0.0f, 1.0f);		// Bottom left.
	vertices[2].TexCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
	// �� ��° �ﰢ��
	vertices[3].Position = DirectX::XMFLOAT4(left, top, 0.0f, 1.0f);		// Top left.
	vertices[3].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	vertices[4].Position = DirectX::XMFLOAT4(right, top, 0.0f, 1.0f);		// Top right.
	vertices[4].TexCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
	vertices[5].Position = DirectX::XMFLOAT4(right, bottom, 0.0f, 1.0f);	// Bottom right.
	vertices[5].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��ٴ�.
	const HRESULT result = m_Device->Context()->Map(m_VertexBuffer->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return;

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	ImageVertex* verticesPtr = static_cast<ImageVertex*>(mappedResource.pData);

	// �����͸� ���� ���ۿ� �����Ѵ�.
	memcpy(verticesPtr, vertices.data(), sizeof(ImageVertex) * m_vertexCount);

	// ���� ���� ������ ����� �����Ѵ�.
	m_Device->Context()->Unmap(m_VertexBuffer->Get(), 0);
}
