#include "pch.h"
#include "CollisionManager.h"
#include "../VPEngine/EventManager.h"
CollisionManager::CollisionManager()
{
	EventManager::GetInstance().Subscribe("OnAddEnter_Collision", CreateSubscriber(&CollisionManager::OnAddEnter_Collision));
	EventManager::GetInstance().Subscribe("OnAddEnter_Trigger", CreateSubscriber(&CollisionManager::OnAddEnter_Trigger));
	EventManager::GetInstance().Subscribe("OnMoveContactToExit_Collision", CreateSubscriber(&CollisionManager::OnMoveContactToExit_Collision));
	EventManager::GetInstance().Subscribe("OnMoveContactToExit_Trigger", CreateSubscriber(&CollisionManager::OnMoveContactToExit_Trigger));
}


/// <summary>
///  �ٵ� �̰� �������ӿ� �ѹ� �Ͼ���ϴµ�.... ���� �����ϳ�? ���!!
/// </summary>
CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update()
{
    // simulation�� ��� �ݸ���Exit �߻���!
    for (auto it = CollisionExit.begin(); it != CollisionExit.end();)
    {
        auto pair = std::make_pair(it->first, it->second);
        EventManager::GetInstance().ImmediateEvent("OnCollisionExit", pair);
        it = EraseExit(it, false);
    }

    for (auto it = TriggerExit.begin(); it != TriggerExit.end();)
    {
        auto pair = std::make_pair(it->first, it->second);

        EventManager::GetInstance().ImmediateEvent("OnTriggerExit", pair);
        it = EraseExit(it, true);
    }

    // On�ݸ��� �߻���! ������ ���� �߻��ÿ� ���Ÿ��ϰ�, ��üǰ�� �����غ���
    for (auto& entitypair : CollisionContact)
        EventManager::GetInstance().ImmediateEvent("OnCollisionContact", entitypair);
    for (auto& entitypair : TriggerContact)
        EventManager::GetInstance().ImmediateEvent("OnTriggerContact", entitypair);

    // simulation�� ��� �ݸ���Enter �߻���!
    for (auto it = CollisionEnter.begin(); it != CollisionEnter.end();)
    {
        auto pair = std::make_pair(it->first, it->second);

        EventManager::GetInstance().ImmediateEvent("OnCollisionEnter", pair);
        it = MoveEnterToContact(it, false);
    }
    for (auto it = TriggerEnter.begin(); it != TriggerEnter.end();)
    {
        auto pair = std::make_pair(it->first, it->second);
        EventManager::GetInstance().ImmediateEvent("OnTriggerEnter", pair);
        it = MoveEnterToContact(it, true);
    }
}

void CollisionManager::AddEnter(std::pair<uint32_t, uint32_t> data, bool IsTrigger) 
{
    auto& enterSet = IsTrigger ? TriggerEnter : CollisionEnter;
    auto range = enterSet.equal_range(data.first);
    bool exists = false;
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == data.second)
        {
            exists = true;
            break;
        }
    }
    if (!exists)
    {
        enterSet.insert(data);
    }
}
std::multimap<uint32_t, uint32_t>::iterator CollisionManager::MoveEnterToContact(std::multimap<uint32_t, uint32_t>::iterator it, bool IsTrigger)
{
    if (!IsTrigger)
    {
        CollisionContact.insert(*it);
        return CollisionEnter.erase(it);
    }
    else
    {
        TriggerContact.insert(*it);
        return TriggerEnter.erase(it);
    }
}

void CollisionManager::MoveContactToExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger)
{
    auto& contactSet = IsTrigger ? TriggerContact : CollisionContact;
    auto& exitSet = IsTrigger ? TriggerExit : CollisionExit;
    auto range = contactSet.equal_range(data.first);
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == data.second)
        {
            contactSet.erase(it);
            exitSet.insert(data);
            break;
        }
    }
}

std::multimap<uint32_t, uint32_t>::iterator CollisionManager::EraseExit(std::multimap<uint32_t, uint32_t>::iterator it, bool IsTrigger)
{
    if (!IsTrigger)
    {
        return CollisionExit.erase(it);
    }
    else
    {
        return TriggerExit.erase(it);
    }
}

void CollisionManager::EraseExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger)
{
    auto& exitSet = IsTrigger ? TriggerExit : CollisionExit;
    auto range = exitSet.equal_range(data.first);
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == data.second)
        {
            exitSet.erase(it);
            break;
        }
    }
}

void CollisionManager::OnAddEnter_Collision(std::any entityPair)
{
    auto entitys = std::any_cast<std::pair<uint32_t, uint32_t>>(entityPair);
    AddEnter(entitys, false);
}

void CollisionManager::OnAddEnter_Trigger(std::any entityPair)
{
    auto entitys = std::any_cast<std::pair<uint32_t, uint32_t>>(entityPair);
    AddEnter(entitys, true);
}

void CollisionManager::OnMoveContactToExit_Collision(std::any entityPair)
{
    auto entitys = std::any_cast<std::pair<uint32_t, uint32_t>>(entityPair);
    MoveContactToExit(entitys, false);
}

void CollisionManager::OnMoveContactToExit_Trigger(std::any entityPair)
{
    auto entitys = std::any_cast<std::pair<uint32_t, uint32_t>>(entityPair);
    MoveContactToExit(entitys, true);
}
