#include "pch.h"
#include "DecalManager.h"

DecalManager::DecalManager()
{

}

void DecalManager::Initialize()
{

}

void DecalManager::AddTask(decal::Info info)
{
	if (m_decals.find(info.TexturePath) != m_decals.end())
	{
		m_decals[info.TexturePath].push_back(info);
		return;
	}
	else
	{
		std::vector < decal::Info > temp;
		temp.push_back(info);
		m_decals.insert(std::pair<std::string, std::vector<decal::Info>>(info.TexturePath,temp));
	}
}

void DecalManager::ClearDecals()
{
	m_decals.clear();
}

std::map<std::string, std::vector<decal::Info>>& DecalManager::GetDecals()
{
	return  m_decals;
}
