#include "pch.h"
#include "LightManager.h"
#include "ResourceManager.h"
#include "Desc.h"

LightManager::LightManager(std::weak_ptr<ResourceManager> manager) : m_ResourceManager(manager), m_BufferStruct()
{

}

LightManager::~LightManager()
{

}



void LightManager::Update(std::unordered_map<uint32_t, LightData>& usinglight)
{

	for (auto& curData : usinglight)
	{
		LightData curLight = curData.second;
		int type = static_cast<int>(curLight.type);
		if (type >= 0)
		{
			std::unordered_map <uint32_t, LightData> curLightMap = m_LightList[type];
			curLightMap[curData.first] = curLight;
		}
	}


	float& dirindex = m_BufferStruct.DirIndex;
	float& spotindex = m_BufferStruct.SpotIndex;

	for (int i = 0; i < m_LightList.size(); i++)
	{
		std::unordered_map <uint32_t, LightData> curLightMap = m_LightList[i];
		std::unordered_map <uint32_t, LightData>::iterator curData = curLightMap.begin();

		LightType kind = static_cast<LightType>(i);

		int size = curLightMap.size();

		switch (kind)
		{
			case LightType::Direction:
			{
				m_BufferStruct.DirIndex = static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{
					m_BufferStruct.array[i] = curData->second;
					curData++;
				}
			}
			break;
			case LightType::Spot:
			{
				m_BufferStruct.SpotIndex = static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{

					m_BufferStruct.array[static_cast<int>(dirindex) + i] = curData->second;
					curData++;
				}
			}
			break;
			case LightType::Point:
			{
				m_BufferStruct.PointIndex = static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{
					m_BufferStruct.array[static_cast<int>(dirindex + spotindex) + i] = curData->second;
					curData++;
				}
			}
			break;
			case LightType::End:
				break;
		}
	}

	std::shared_ptr<ConstantBuffer<LightArray>> cbuffer = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();
	cbuffer->Update(m_BufferStruct);
}