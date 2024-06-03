#pragma once
#include "pch.h"

#include "ShaderResourceView.h"


class Device;

//Mesh�� �׸��� ��� �Ǵ� �÷��� �����ϱ� ���� ������ �����ϴ� Ŭ����
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