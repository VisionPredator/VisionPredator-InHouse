#pragma once

using namespace physx;
class CollisionCallback :
    public PxSimulationEventCallback
{
public:
    CollisionCallback();
    ~CollisionCallback();
    // PxSimulationEventCallback��(��) ���� ��ӵ�
    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
    virtual void onWake(PxActor** actors, PxU32 count) override;
    virtual void onSleep(PxActor** actors, PxU32 count) override;
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
    virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;
    std::pair<uint32_t,uint32_t> SortEntityPair(const PxContactPair pairs);
    std::pair<uint32_t,uint32_t> SortEntityPair(const PxTriggerPair& pairs);
}; 
