#pragma once
#include <unordered_map>
#include <queue>

#include "CBuffer.h"

class ResourceManager;
template<typename T>
class ConstantBuffer;

/// <summary>
/// 2024.06.04
/// 다양한 빛에 따라 늘어나는 데이터를 일괄적으로 관리해서 특정패스에 들어갈 상수버퍼를 업데이트한다
/// 리소스 매니저에서 하지 않는 이유는? 빛은 인게임에서 늘어나고 줄어들고 다양한 데이터가 변동된다 
/// 그런걸 리소스매니저에서 관리할 이유는 없다 
/// 리소스 매니저는 상수버퍼만 관리하자
/// 굳이 매니저급으로 만드는 이유?
/// 그냥 map이나 vector여도 되는거 아님?
/// 여기서 데이터를 관리하고 그걸 바탕으로 constantbuffer를 만들어야지 + 만들어진 다양한 빛 관련 constantbuffer를
/// 한번에 묶어서 보내줄려면?
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

