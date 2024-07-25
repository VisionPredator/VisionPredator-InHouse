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
	inline void SetIsFall(bool isfall);
	inline void SetVelocity(VPMath::Vector3 velocity);
	const VPMath::Vector3 GetPosition();
	inline VPMath::Vector3 GetVelocity();
	inline bool GetIsFall();
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

	physx::PxVec3 m_Velocity{};
	bool m_IsFall{};

};

inline uint32_t Controller::GetEntityID()
{
	return m_EntityID;
}

inline physx::PxControllerFilters* Controller::GetFilters()
{
	return m_Filters;
}
inline void Controller::SetIsFall(bool isfall)
{
	m_IsFall = isfall;
}

inline bool Controller::GetIsFall()
{
	return m_IsFall;
}
inline void Controller::SetVelocity(VPMath::Vector3 velocity)
{
	m_Velocity = { velocity .x,velocity .y,velocity .z};




}
inline VPMath::Vector3 Controller::GetVelocity()
{
	return { m_Velocity .x,m_Velocity .y,m_Velocity .z};
}

