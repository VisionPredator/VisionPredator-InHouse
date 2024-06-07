#pragma once
#include <unordered_map>
#include <queue>

#include "CBuffer.h"

class ResourceManager;
template<typename T>
class ConstantBuffer;

/// <summary>
/// 2024.06.04
/// �پ��� ���� ���� �þ�� �����͸� �ϰ������� �����ؼ� Ư���н��� �� ������۸� ������Ʈ�Ѵ�
/// ���ҽ� �Ŵ������� ���� �ʴ� ������? ���� �ΰ��ӿ��� �þ�� �پ��� �پ��� �����Ͱ� �����ȴ� 
/// �׷��� ���ҽ��Ŵ������� ������ ������ ���� 
/// ���ҽ� �Ŵ����� ������۸� ��������
/// ���� �Ŵ��������� ����� ����?
/// �׳� map�̳� vector���� �Ǵ°� �ƴ�?
/// ���⼭ �����͸� �����ϰ� �װ� �������� constantbuffer�� �������� + ������� �پ��� �� ���� constantbuffer��
/// �ѹ��� ��� �����ٷ���?
/// 
/// 
/// </summary>



class LightManager
{
public:
	LightManager(std::weak_ptr<ResourceManager> manager);
	~LightManager();

	void Update(std::array<std::unordered_map<std::wstring, LightData>, static_cast<int>(Kind_of_Light::End)>& usinglight);


private:
	std::weak_ptr<ResourceManager> m_ResourceManager;

	int m_DirCount = 0;
	int m_PointCount = 0;
	int m_SpotCount = 0;

	std::queue<LightData> m_UsingLights;
	LightArray m_BufferStruct;
};

