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
	physx::PxController* GetPxController() { return m_Controller; }
	virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<Controller>(); }
	inline uint32_t GetEntityID();
	inline physx::PxControllerFilters* GetFilters();
	//uint32_t m_EntityID{};
	USERDATA m_UserData{};
	physx::PxController* m_Controller{};
	VPPhysics::EPhysicsLayer m_LayerNum{};
	physx::PxMaterial* m_Material{};
	std::shared_ptr<PxFilterData> m_FilterData{};
	std::shared_ptr<ControllerQueryFilterCallback> m_ControllerQueryFilterCallback{};
	std::shared_ptr<PxControllerFilters> m_Filters{};

	physx::PxVec3 m_Velocity{};
	bool m_IsFall{};

};

inline uint32_t Controller::GetEntityID()
{
	return m_UserData.entityID;
}

inline physx::PxControllerFilters* Controller::GetFilters()
{
	return m_Filters.get();
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

