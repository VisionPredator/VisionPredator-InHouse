#pragma once
#include "Device.h"
#include "ShaderResourceView.h"

#include <string>
//Mesh�� �׸��� ��� �Ǵ� �÷��� �����ϱ� ���� ������ �����ϴ� Ŭ����
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