#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;
class ControllerQueryFilterCallback;
class MyControllerFilterCallback : public physx::PxControllerFilterCallback 
{
public:
	virtual bool filter(const physx::PxController& a, const physx::PxController& b) override {
		// Get filter data from both controllers
		physx::PxShape* shapeA = nullptr;
		physx::PxShape* shapeB = nullptr;
		a.getActor()->getShapes(&shapeA, 1);  // Assuming each controller has one shape
		b.getActor()->getShapes(&shapeB, 1);

		if (!shapeA || !shapeB) {
			// If either shape is null, do not allow interaction
			return false;
		}

		// Extract filter data
		physx::PxFilterData filterDataA = shapeA->getSimulationFilterData();
		physx::PxFilterData filterDataB = shapeB->getSimulationFilterData();

		// Filtering logic similar to CustomSimulationFilterShader
		bool shouldInteract = (((1 << filterDataA.word0) & filterDataB.word1) > 0) &&
			(((1 << filterDataB.word0) & filterDataA.word1) > 0);

		// Return true if controllers 'a' and 'b' should be allowed to interact
		return shouldInteract;
	}
};

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
	std::shared_ptr<ControllerQueryFilterCallback> m_PxQueryFilterCallback{};
	std::shared_ptr<MyControllerFilterCallback> m_PxControllerFilterCallback{};

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

