#pragma once
#include "pch.h"

#include "ShaderResourceView.h"


class Device;

//Mesh를 그리는 방법 또는 컬러를 결정하기 위한 정보를 소유하는 클래스
class Material
{
public:
	Material(std::shared_ptr<Device> device);
	~Material();

	std::wstring m_DiffuseFilePath;
	std::wstring m_NormalFilePath;
	std::wstring m_SpecularFilePath;

	std::weak_ptr <ShaderResourceView> m_DiffuseSRV;
	std::weak_ptr <ShaderResourceView> m_NormalSRV;
	std::weak_ptr <ShaderResourceView> m_SpecularSRV;

private:
	std::weak_ptr<Device> m_Device;
};