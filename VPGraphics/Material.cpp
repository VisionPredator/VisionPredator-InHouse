#include "pch.h"
#include "Material.h"
#include "Device.h"
#include "CBuffer.h"

Material::Material(std::shared_ptr<Device> device) : m_Device(device), m_AlbedoSRV(), m_NormalSRV(), m_MetalicSRV(), m_RoughnessSRV(), m_AOSRV()
{
	m_AlbedoSRV =  std::make_shared<ShaderResourceView>(device);
	m_NormalSRV =  std::make_shared<ShaderResourceView>(device);
	m_MetalicSRV =  std::make_shared<ShaderResourceView>(device);
	m_RoughnessSRV =  std::make_shared<ShaderResourceView>(device);
	m_AOSRV =  std::make_shared<ShaderResourceView>(device);
	
	m_Data =  std::make_shared<MaterialData>();
}

Material::~Material()
{

}
