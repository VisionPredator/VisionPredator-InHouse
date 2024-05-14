#include "pch.h"

#include "Mesh.h"

Mesh::Mesh() : m_VB(nullptr), m_IB(nullptr), m_primitive(), m_node(nullptr), Matrix_Pallete(new MatrixPallete())
{

}

ID3D11Buffer* Mesh::VB()
{
	return m_VB->Get();
}

ID3D11Buffer* Mesh::IB()
{
	return m_IB->Get();
}

ID3D11Buffer** Mesh::GetAddressVB()
{
	return m_VB->GetAddress();
}

UINT* Mesh::VBSize()
{
	return m_VB->Size();
}

UINT* Mesh::VBOffset()
{
	return m_VB->Offset();
}

ID3D11Buffer** Mesh::GetAddressIB()
{
	return m_IB->GetAddress();
}

UINT Mesh::IBCount()
{
	return m_IB->Count();
}


void Mesh::Release()
{

}
