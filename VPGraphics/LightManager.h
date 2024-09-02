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

class ShaderResourceView;

class LightManager
{
public:
	LightManager() = default;
	~LightManager() = default;

	void Initialize(std::weak_ptr<ResourceManager> manager);

	//void AddData(uint32_t EntityID, LightType kind, LightData data);
	void EraseData(uint32_t EntityID, LightType type);
	void Update(std::unordered_map<uint32_t, LightData>& usinglight);

	std::weak_ptr<ShaderResourceView> GetLightMap(int index);

private:
	std::weak_ptr<ResourceManager> m_ResourceManager;

	int m_DirCount = 0;
	int m_PointCount = 0;
	int m_SpotCount = 0;

	std::queue<LightData> m_UsingLights;
	LightArray m_BufferStruct;

	std::array<std::unordered_map<uint32_t, LightData>, static_cast<int>(LightType::End)> m_LightList;
	std::vector<std::weak_ptr<ShaderResourceView>> m_LightMap;
};

