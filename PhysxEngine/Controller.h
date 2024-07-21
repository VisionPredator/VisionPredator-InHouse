#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;
class Controller
{
public:


	Controller();
	~Controller();

	bool ControllerInit(VPPhysics::ControllerInfo info,VPPhysics::PhysicsInfo physicsinfo);
	uint32_t m_EntityID;
	physx::PxFilterData* m_FilterData;
	physx::PxController* m_Controller;
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material;
};

