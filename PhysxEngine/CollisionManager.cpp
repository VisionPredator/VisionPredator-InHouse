#include "pch.h"
#include "CollisionManager.h"
#include "EventManager.h"
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
    for (auto it = m_CollisionExit.begin(); it != m_CollisionExit.end();)
    {
        auto pair = std::make_pair(it->first, it->second);
        EventManager::GetInstance().ImmediateEvent("OnCollisionExit", pair);
        it = EraseExit(it, false);
    }

    for (auto it = m_TriggerExit.begin(); it != m_TriggerExit.end();)
    {
        auto pair = std::make_pair(it->first, it->second);

        EventManager::GetInstance().ImmediateEvent("OnTriggerExit", pair);
        it = EraseExit(it, true);
    }

    // On�ݸ��� �߻���! ������ ���� �߻��ÿ� ���Ÿ��ϰ�, ��üǰ�� �����غ���
    //for (auto& entitypair : m_CollisionContact)
    //    EventManager::GetInstance().ImmediateEvent("OnCollisionContact", entitypair);
    //for (auto& entitypair : m_TriggerContact)
    //    EventManager::GetInstance().ImmediateEvent("OnTriggerContact", entitypair);

    // simulation�� ��� �ݸ���Enter �߻���!
    for (auto it = m_CollisionEnter.begin(); it != m_CollisionEnter.end();)
    {
        auto pair = std::make_pair(it->first, it->second);

        EventManager::GetInstance().ImmediateEvent("OnCollisionEnter", pair);
        it = MoveEnterToContact(it, false);
    }
    for (auto it = m_TriggerEnter.begin(); it != m_TriggerEnter.end();)
    {
        auto pair = std::make_pair(it->first, it->second);
        EventManager::GetInstance().ImmediateEvent("OnTriggerEnter", pair);
        it = MoveEnterToContact(it, true);
    }
}

void CollisionManager::AddEnter(std::pair<uint32_t, uint32_t> data, bool IsTrigger) 
{
    auto& enterSet = IsTrigger ? m_TriggerEnter : m_CollisionEnter;
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
        m_CollisionContact.insert(*it);
        return m_CollisionEnter.erase(it);
    }
    else
    {
        m_TriggerContact.insert(*it);
        return m_TriggerEnter.erase(it);
    }
}

void CollisionManager::MoveContactToExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger)
{
    auto& contactSet = IsTrigger ? m_TriggerContact : m_CollisionContact;
    auto& exitSet = IsTrigger ? m_TriggerExit : m_CollisionExit;
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
        return m_CollisionExit.erase(it);
    }
    else
    {
        return m_TriggerExit.erase(it);
    }
}

void CollisionManager::EraseExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger)
{
    auto& exitSet = IsTrigger ? m_TriggerExit : m_CollisionExit;
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

void CollisionManager::RemoveMaps()
{
    m_CollisionEnter.clear();
    m_CollisionContact.clear();
    m_CollisionExit.clear();
    m_TriggerEnter.clear();
    m_TriggerContact.clear();
    m_TriggerExit.clear();
}
void CollisionManager::RemoveEntity(uint32_t entityID) 
{
    auto removeEntityFromMap = [](auto& mmap, uint32_t id) {
        for (auto it = mmap.begin(); it != mmap.end(); ) {
            if (it->first == id || it->second == id) {
                it = mmap.erase(it);
            }
            else {
                ++it;
            }
        }
        };

    removeEntityFromMap(m_CollisionEnter, entityID);
    removeEntityFromMap(m_CollisionContact, entityID);
    removeEntityFromMap(m_CollisionExit, entityID);
    removeEntityFromMap(m_TriggerEnter, entityID);
    removeEntityFromMap(m_TriggerContact, entityID);
    removeEntityFromMap(m_TriggerExit, entityID);
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

bool CollisionManager::IsEntityInCollision(uint32_t entityID)
{
	// m_CollisionContact�� ��� �ִ� ��� false ��ȯ
	if (m_CollisionContact.empty())
		return false; // �浹 �����Ͱ� �����Ƿ� �ش� ID�� �浹 ���°� �ƴ�

	// m_CollisionContact���� entityID�� first �Ǵ� second�� �����ϴ��� Ȯ��
	for (const auto& pair : m_CollisionContact)
		if (pair.first == entityID || pair.second == entityID)
			return true; // �����ϸ� true ��ȯ
	return false; // �������� ������ false ��ȯ
}

bool CollisionManager::IsEntityInTrigger(uint32_t entityID)
{
    // m_TriggerContact�� ��� �ִ� ��� false ��ȯ
    if (m_TriggerContact.empty())
        return false; // �浹 �����Ͱ� �����Ƿ� �ش� ID�� �浹 ���°� �ƴ�

    // m_TriggerContact���� entityID�� first �Ǵ� second�� �����ϴ��� Ȯ��
    for (const auto& pair : m_TriggerContact)
        if (pair.first == entityID || pair.second == entityID)
            return true; // �����ϸ� true ��ȯ
    return false; // �������� ������ false ��ȯ
}
