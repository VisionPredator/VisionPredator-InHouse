#pragma once
#include "VPPhysicsStructs.h"
class CollisionManager
{
public: 
	CollisionManager();
	~CollisionManager();
	void Update();

	
	std::vector<VPPhysics::CollisionData> m_CollisionData;
};

