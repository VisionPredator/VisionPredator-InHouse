#include "pch.h"
#include "LightManager.h"
#include "ResourceManager.h"
#include "Desc.h"

void LightManager::Initialize(std::weak_ptr<ResourceManager> manager)
{
	m_ResourceManager = manager;
	m_LightMap.push_back(manager.lock()->Get<ShaderResourceView>(L"Lightmap-0_comp_light.png"));
}

void LightManager::EraseData(uint32_t EntityID, LightType type)
{
	int index = static_cast<int>(type);
	std::unordered_map<uint32_t, LightData>& curMap = m_LightList[index];

	if (curMap.find(EntityID) != curMap.end())
	{
		curMap.erase(EntityID);
	}
}

void LightManager::Update(std::unordered_map<uint32_t, LightData>& usinglight)
{
	int dirIndex = 0;
	int pointIndex = 0;
	int spotIndex = 0;

	std::vector<LightData> dir;
	std::vector<LightData> point;
	std::vector<LightData> spot;

	for (auto& curData : usinglight)
	{
		LightData curLight = curData.second;
		LightType type = static_cast<LightType>(curLight.type);

		switch (type)
		{
			case LightType::Direction:
			{
				dirIndex++;
				dir.push_back(curLight);
			}
			break;
			case LightType::Spot:
			{
				spotIndex++;
				spot.push_back(curLight);
			}
			break;
			case LightType::Point:
			{
				pointIndex++;
				point.push_back(curLight);
			}
			break;
			case LightType::End:
				break;
		}
	}

	m_BufferStruct.DirIndex = dirIndex;
	m_BufferStruct.PointIndex = pointIndex;
	m_BufferStruct.SpotIndex = spotIndex;

	for (int i = 0; i < dirIndex; i++)
	{
		m_BufferStruct.array[i] = dir[i];
	}

	for (int i = 0; i < spotIndex; i++)
	{
		m_BufferStruct.array[dirIndex + i] = spot[i];
	}

	for (int i = 0; i < pointIndex; i++)
	{
		m_BufferStruct.array[dirIndex+ spotIndex + i] = point[i];
	}

	/*

	for (auto& curData : usinglight)
	{
		LightData curLight = curData.second;
		int type = static_cast<int>(curLight.type);
		if (type < static_cast<int>(LightType::End))
		{
			std::unordered_map <uint32_t, LightData>& curLightMap = m_LightList[type];
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
	*/

	std::shared_ptr<ConstantBuffer<LightArray>> cbuffer = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();
	cbuffer->Update(m_BufferStruct);
}

std::weak_ptr<ShaderResourceView> LightManager::GetLightMap(int index)
{

	if (m_LightMap[index].lock() != nullptr)
	{
		return m_LightMap[index];
	}

	return {};
}
