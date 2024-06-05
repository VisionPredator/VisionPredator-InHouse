#pragma once
#include <unordered_map>


#include "CBuffer.h"

class ResourceManager;

/// <summary>
/// 2024.06.04
/// �پ��� ���� ���� �þ�� �����͸� �ϰ������� �����ؼ� Ư���н��� �� ������۸� ������Ʈ�Ѵ�
/// ���ҽ� �Ŵ������� ���� �ʴ� ������? ���� �ΰ��ӿ��� �þ�� �پ��� �پ��� �����Ͱ� �����ȴ� �׷��� ���ҽ��Ŵ������� ������ ������ ����
/// ���ҽ� �Ŵ����� ������۸� ��������
/// ���� �Ŵ��������� ����� ����?
/// �׳� map�̳� vector���� �Ǵ°� �ƴ�?
/// 
/// </summary>



class LightManager
{
public:
	LightManager(std::weak_ptr<ResourceManager> manager);
	~LightManager();

	void AddLight(Kind_of_Light kind, LightData data);
	void Update(float dt);

private:

	std::map<Kind_of_Light, std::shared_ptr<LightData>> m_LightObjects;


	std::weak_ptr<ResourceManager> m_ResourceManager;
};

