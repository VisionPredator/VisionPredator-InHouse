#pragma once
#include <unordered_map>


#include "CBuffer.h"

class ResourceManager;

/// <summary>
/// 2024.06.04
/// 다양한 빛에 따라 늘어나는 데이터를 일괄적으로 관리해서 특정패스에 들어갈 상수버퍼를 업데이트한다
/// 리소스 매니저에서 하지 않는 이유는? 빛은 인게임에서 늘어나고 줄어들고 다양한 데이터가 변동된다 그런걸 리소스매니저에서 관리할 이유는 없다
/// 리소스 매니저는 상수버퍼만 관리하자
/// 굳이 매니저급으로 만드는 이유?
/// 그냥 map이나 vector여도 되는거 아님?
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

