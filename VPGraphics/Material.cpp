#include "pch.h"

#include "Material.h"

Material::Material(Device* device) : m_Device(device), m_DiffuseSRV(nullptr), m_NormalSRV(nullptr), m_SpecularSRV(nullptr)
{
	m_DiffuseSRV = new ShaderResourceView(device);
	m_NormalSRV = new ShaderResourceView(device);
}

Material::~Material()
{

}
