#pragma once
#include "VPPhysicsStructs.h"
#include "../VPEngine/EventSubscriber.h"
class CollisionManager :public EventSubscriber
{
public:
    CollisionManager();
    ~CollisionManager();
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

    std::vector<VPPhysics::CollisionData> m_CollisionData;

    std::multimap<uint32_t, uint32_t> CollisionEnter;
    std::multimap<uint32_t, uint32_t> CollisionContact;
    std::multimap<uint32_t, uint32_t> CollisionExit;

    std::multimap<uint32_t, uint32_t> TriggerEnter;
    std::multimap<uint32_t, uint32_t> TriggerContact;
    std::multimap<uint32_t, uint32_t> TriggerExit;
};

