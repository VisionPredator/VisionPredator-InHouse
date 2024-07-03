#pragma once
#include "VPPhysicsStructs.h"
class PhysxColliManager
{
public: 
	PhysxColliManager();
	~PhysxColliManager();


	std::vector<VPPhysics::CollisionData> m_CollisionData;
};

