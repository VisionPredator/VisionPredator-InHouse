#pragma once
#include "Device.h"
#include "ShaderResourceView.h"

#include <string>
//Mesh를 그리는 방법 또는 컬러를 결정하기 위한 정보를 소유하는 클래스
class Material
{
public:
	Material(Device* device);
	~Material();



	std::wstring m_DiffuseFilePath;
	std::wstring m_NormalFilePath;
	std::wstring m_SpecularFilePath;

	ShaderResourceView* m_DiffuseSRV;
	ShaderResourceView* m_NormalSRV;
	ShaderResourceView* m_SpecularSRV;

private:
	Device* m_Device;
};