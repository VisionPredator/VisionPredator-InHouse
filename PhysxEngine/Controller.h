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
	void SetPosition(const VPMath::Vector3& position);
	const VPMath::Vector3 GetPosition();

	virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<Controller>(); }
	inline uint32_t GetEntityID();
	inline physx::PxControllerFilters* GetFilters();
	uint32_t m_EntityID;
	physx::PxFilterData* m_FilterData{};
	physx::PxController* m_Controller{};
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material{};
	ControllerQueryFilterCallback* m_ControllerQueryFilterCallback{};
	physx::PxControllerFilters* m_Filters{};

};

inline uint32_t Controller::GetEntityID()
{
	return m_EntityID;
}

inline physx::PxControllerFilters* Controller::GetFilters()
{
	return m_Filters;
}
