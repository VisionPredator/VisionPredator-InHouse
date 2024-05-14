#include "pch.h"

#include "Texture2D.h"
#include <d3d11.h>

Texture2D::Texture2D(Device* device, D3D11_TEXTURE2D_DESC desc) : Resource(device), m_desc(desc), m_tex(nullptr)
{
	device->Get()->CreateTexture2D(&m_desc, 0, &m_tex);
}

Texture2D::~Texture2D()
{

}

void Texture2D::Release()
{
	m_tex->Release();
}

ID3D11Texture2D* Texture2D::Get()
{
	return m_tex;
}

ID3D11Texture2D** Texture2D::GetAddress()
{
	return &m_tex;
}
