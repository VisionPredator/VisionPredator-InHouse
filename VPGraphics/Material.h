#pragma once
#include "pch.h"

#include "ShaderResourceView.h"


class Device;
class MaterialData;

//Mesh를 그리는 방법 또는 컬러를 결정하기 위한 정보를 소유하는 클래스
class Material
{
public:
	Material(std::shared_ptr<Device> device);
	~Material();

	std::wstring AlbeoPath;
	std::wstring NormalPath;
	std::wstring MetalicPath;
	std::wstring RoughnessPath;
	std::wstring AOPath;

	std::weak_ptr <ShaderResourceView> m_AlbedoSRV;
	std::weak_ptr <ShaderResourceView> m_NormalSRV;
	std::weak_ptr <ShaderResourceView> m_MetalicSRV;
	std::weak_ptr <ShaderResourceView> m_RoughnessSRV;
	std::weak_ptr <ShaderResourceView> m_AOSRV;

	std::shared_ptr<MaterialData> m_Data;

private:
	std::weak_ptr<Device> m_Device;
};