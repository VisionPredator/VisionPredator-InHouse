#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;
class Controller
{
public:


	Controller();
	~Controller();

	bool InfoInit(VPPhysics::ControllerInfo info);
	uint32_t m_EntityID;

	physx::PxController* m_Controller;
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material;
};

