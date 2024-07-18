#pragma once
#include "VPPhysicsStructs.h"
class CharactorController
{
public:


	CharactorController();
	~CharactorController();
	uint32_t m_EntityID;

	physx::PxController* m_Controller;
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material;
};

