#include "pch.h"
#include "Material.h"
#include "Device.h"


Material::Material(std::shared_ptr<Device> device) : m_Device(device), m_DiffuseSRV(), m_NormalSRV(), m_SpecularSRV()
{
	m_DiffuseSRV =  std::make_shared<ShaderResourceView>(device);
	m_NormalSRV =  std::make_shared<ShaderResourceView>(device);
	
}

Material::~Material()
{

}
