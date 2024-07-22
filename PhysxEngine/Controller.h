#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;
class ControllerQueryFilterCallback;

class Controller
{
public:


	Controller();
	~Controller();

	bool ControllerInit(VPPhysics::ControllerInfo info, physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo);

	inline uint32_t GetEntityID();
	uint32_t m_EntityID;
	physx::PxFilterData* m_FilterData{};
	physx::PxController* m_Controller{};
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material{};
	ControllerQueryFilterCallback* m_ControllerQueryFilterCallback{};
	physx::PxControllerFilters* mFilters{};

};

inline uint32_t Controller::GetEntityID()
{
	return m_EntityID;
}
