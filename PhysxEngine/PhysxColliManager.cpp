#include "pch.h"
#include "PhysxColliManager.h"
#include "../VPEngine/EventManager.h"
CollisionManager::CollisionManager()
{
	EventManager::GetInstance().Subscribe("OnAddEnterData", CreateSubscriber(&CollisionManager::OnAddEnterData));
	EventManager::GetInstance().Subscribe("OnAddContactData", CreateSubscriber(&CollisionManager::OnAddContactData));
	EventManager::GetInstance().Subscribe("OnAddExitData", CreateSubscriber(&CollisionManager::OnAddExitData));
	EventManager::GetInstance().Subscribe("OnEraseData", CreateSubscriber(&CollisionManager::OnEraseData));
	EventManager::GetInstance().Subscribe("OnFindData", CreateSubscriber(&CollisionManager::OnFindData));

}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update()
{
	for (auto& entity : Enter)
	{
		//어떤 이벤트 호출	
	}

	for (auto& entity : Contact)
	{

	}

	for (auto& entity : Exit)
	{

	}
}

void CollisionManager::OnAddEnterData(std::any entitypair)
{m
	std::pair<uint32_t, uint32_t> pair = std::any_cast<std::pair<uint32_t, uint32_t>>(entitypair);
	auto range = Enter.equal_range(pair.first);

	if (range.first == Enter.end())
	{
		Enter.insert(pair);
	}
	else
	{
		for (auto i = range.first; i != range.second; i++)
		{
			if (pair.second != i->second)
			{
				Enter.insert(pair);
			}
		}
	}
}

void CollisionManager::OnAddContactData(std::any entitypair)
{
	std::pair<uint32_t, uint32_t> pair = std::any_cast<std::pair<uint32_t, uint32_t>>(entitypair);

	auto range = Enter.equal_range(pair.first);

	for (auto i = range.first; i != range.second; i++)
	{
		if (pair.second == i->second)
		{
			Contact.insert(pair);
			Enter.erase(i);
			break;
		}
	}

}

void CollisionManager::OnAddExitData(std::any entitypair)
{
	std::pair<uint32_t, uint32_t> pair = std::any_cast<std::pair<uint32_t, uint32_t>>(entitypair);

	auto range = Contact.equal_range(pair.first);

	for (auto i = range.first; i != range.second; i++)
	{
		if (pair.second == i->second)
		{
			Exit.insert(pair);
			Contact.erase(i);
			break;
		}
	}
}

void CollisionManager::OnEraseData(std::any entitypair)
{
}

void CollisionManager::OnFindData(std::any entitypair)
{

}
