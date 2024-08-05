#pragma once
#include "VPPhysicsStructs.h"
#include "EventSubscriber.h"
class CollisionManager :public EventSubscriber
{
public:
    CollisionManager();
    ~CollisionManager();
    void RemoveEntity(uint32_t entityID);
    void RemoveMaps();
    void Update();

private:
    void AddEnter(std::pair<uint32_t, uint32_t> data, bool IsTrigger = false);
    std::multimap<uint32_t, uint32_t>::iterator MoveEnterToContact(std::multimap<uint32_t, uint32_t>::iterator it, bool IsTrigger = false);
    void MoveContactToExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger = false);
    std::multimap<uint32_t, uint32_t>::iterator EraseExit(std::multimap<uint32_t, uint32_t>::iterator it, bool IsTrigger = false);
    void EraseExit(std::pair<uint32_t, uint32_t> data, bool IsTrigger = false);
    void OnAddEnter_Collision(std::any entityPair);
    void OnAddEnter_Trigger(std::any entityPair);
    void OnMoveContactToExit_Collision(std::any entityPair);
    void OnMoveContactToExit_Trigger(std::any entityPair);


    std::multimap<uint32_t, uint32_t> m_CollisionEnter;
    std::multimap<uint32_t, uint32_t> m_CollisionContact;
    std::multimap<uint32_t, uint32_t> m_CollisionExit;

    std::multimap<uint32_t, uint32_t> m_TriggerEnter;
    std::multimap<uint32_t, uint32_t> m_TriggerContact;
    std::multimap<uint32_t, uint32_t> m_TriggerExit;
};

