#include "pch.h"
#include "Object.h"
#include "RenderState.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace DirectX;
using namespace SimpleMath;


Object::Object()
{
}


Object::~Object()
{

}

std::vector<Mesh*>& Object::Meshes()
{
	return m_LoadData->m_Meshes;
}

std::vector<Animation*>& Object::Animations()
{
	return m_LoadData->m_Animations;
}

std::vector<Material*>& Object::Materials()
{
	return m_LoadData->m_Materials;
}

ID3D11RasterizerState* Object::RS()
{
	return m_renderState->Get();
}

ID3D11InputLayout* Object::InputLayOut()
{
	return m_VS->InputLayout();
}

ID3D11PixelShader* Object::PS()
{
	return m_PS->GetPS();
}

ID3D11PixelShader** Object::AddressPS()
{
	return m_PS->GetPSAddress();
}

ID3D11VertexShader* Object::VS()
{
	return m_VS->GetVS();
}

ID3D11VertexShader** Object::AddressVS()
{
	return m_VS->GetVSAddress();
}

ShaderResourceView* Object::DiffuseSRV(int index)
{
	return m_LoadData->m_Materials[index]->m_DiffuseSRV;
}

ShaderResourceView* Object::NormalSRV(int index)
{

	return m_LoadData->m_Materials[index]->m_NormalSRV;
}

ShaderResourceView* Object::SpecularSRV(int index)
{
	return m_LoadData->m_Materials[index]->m_SpecularSRV;
}

Node* Object::RootNode()
{
	return m_LoadData->m_RootNode;
}

Animation* Object::CurAnimation(int index)
{
	return m_LoadData->m_Animations[index];
}
