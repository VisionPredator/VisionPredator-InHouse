#pragma once
#include "Controller.h"
class ControllerHitCallback;
class CapsuleController :
    public Controller
{
public:
    CapsuleController();
	bool Initialize(CapsuleControllerInfo info, physx::PxControllerManager* CCTManager, physx::PxMaterial* material/*, CollisionData* collisionData*/, PhysicsInfo physicinfo );
    void ResizeCapsuleControllerSize(float radius, float height);

    void UpdateCapsuleSize(const CapsuleControllerInfo& newInfo);
    void SetShapeOffset(ControllerPivot pivot,VPMath::Vector3 offset);
    void UpdateCapsuleDimensions(CapsuleControllerInfo info);
    ~CapsuleController();
	entt::id_type GetTypeID() const override { return Reflection::GetTypeID<CapsuleController>(); }
private:
    std::shared_ptr<ControllerHitCallback> m_ControllerHitCallback;


};

