#include "pch.h"

#include "RenderState.h"

RenderState::RenderState(Device* device, D3D11_RASTERIZER_DESC desc) : Resource(device)
{
	m_Device->Get()->CreateRasterizerState(&desc, &m_RS);
}

RenderState::~RenderState()
{

}


void RenderState::Release()
{
	m_RS->Release();
}

ID3D11RasterizerState* RenderState::Get() const
{
	return m_RS;
}
