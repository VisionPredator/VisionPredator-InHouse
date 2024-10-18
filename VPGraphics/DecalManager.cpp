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
	m_decals.push(info);
}

std::queue<decal::Info> DecalManager::GetTask() const
{
	return m_decals;
}
