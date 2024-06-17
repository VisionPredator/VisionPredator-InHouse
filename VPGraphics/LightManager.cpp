#include "pch.h"
#include "LightManager.h"
#include "ResourceManager.h"
#include "Desc.h"

LightManager::LightManager(std::weak_ptr<ResourceManager> manager) : m_ResourceManager(manager)
{

}

LightManager::~LightManager()
{

}



void LightManager::Update(std::array<std::unordered_map<std::wstring, LightData>, static_cast<int>(Kind_of_Light::End)>& usinglight)
{
	for (int i = 0; i < usinglight.size(); i++)
	{
		std::unordered_map <std::wstring, LightData> curLightMap = usinglight[i];
		std::unordered_map <std::wstring, LightData>::iterator curData = curLightMap.begin();

		Kind_of_Light kind = static_cast<Kind_of_Light>(i);

		int size = curLightMap.size();

		switch (kind)
		{
			case Kind_of_Light::Direction:
			{
				m_BufferStruct.DirIndex = static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{
					m_BufferStruct.Dir[i] = curData->second;
					curData++;
				}
			}
			break;
			case Kind_of_Light::Spot:
			{
				m_BufferStruct.SpotIndex = static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{
					m_BufferStruct.Spot[i] = curData->second;
					curData++;
				}
			}
			break;
			case Kind_of_Light::Point:
			{
				m_BufferStruct.PointIndex= static_cast<float>(size);
				for (int i = 0; i < size; i++)
				{
					m_BufferStruct.Point[i] = curData->second;
					curData++;
				}
			}
			break;
			case Kind_of_Light::End:
				break;
		}		
	}

	std::shared_ptr<ConstantBuffer<LightArray>> cbuffer = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();
	cbuffer->Update(m_BufferStruct);
}