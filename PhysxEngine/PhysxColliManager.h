#pragma once
#include "VPPhysicsStructs.h"
class CollisionManager
{
public: 
	CollisionManager();
	~CollisionManager();


	std::vector<VPPhysics::CollisionData> m_CollisionData;
};

