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



void LightManager::Update(std::array<std::unordered_map<uint32_t, LightData>, static_cast<int>(LightType::End)>& usinglight)
{

	float& dirindex = m_BufferStruct.DirIndex;
	float& spotindex = m_BufferStruct.SpotIndex;


	for (int i = 0; i < usinglight.size(); i++)
	{
		std::unordered_map <uint32_t, LightData> curLightMap = usinglight[i];
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
				m_BufferStruct.PointIndex= static_cast<float>(size);
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