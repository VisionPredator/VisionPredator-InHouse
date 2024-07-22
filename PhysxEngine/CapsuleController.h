#pragma once
#include "Controller.h"
class ControllerHitCallback;
class CapsuleController :
    public Controller
{
public:
    CapsuleController();
	bool Initialize(CapsuleControllerInfo info, physx::PxControllerManager* CCTManager, physx::PxMaterial* material/*, CollisionData* collisionData*/, PhysicsInfo physicinfo );

    ~CapsuleController();

private:
    ControllerHitCallback* m_ControllerHitCallback{};

};

