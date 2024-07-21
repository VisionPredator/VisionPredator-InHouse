#pragma once
#include "Controller.h"
class CapsuleController :
    public Controller
{
public:
    CapsuleController();
	void Initialize(CapsuleControllerInfo info, physx::PxControllerManager* CCTManager/*, CollisionData* collisionData*/,int CollisionMatrix );

    ~CapsuleController();
};

