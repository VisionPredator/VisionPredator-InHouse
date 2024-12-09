#pragma once
#include "VPPhysicsStructs.h"
class CollisionManager;
class Controller;
class ControllerManager
{
public: 
	ControllerManager();
	~ControllerManager();

	/// <summary>
	/// ��Ʈ�ѷ� �Ŵ��� ���� �� �ʱ�ȭ �Լ�
	/// </summary>
	/// <returns></returns>
	bool Initialize(physx::PxScene* scene, physx::PxPhysics* physics, CollisionManager* collisionManager);


/// <summary>
/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
/// </summary>
/// <param name="controllerInfo"> ĳ���� ��Ʈ�ѷ� ���� ������ </param>
/// <param name="movementInfo"> ĳ���� �����Ʈ ���� ������ </param>
/// <returns></returns>
	bool CreatController();
	bool CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo, VPPhysics::PhysicsInfo physicsinfo);
	bool Update(float deltatime);
	bool LateUpdate();
	VPPhysics::RaycastData RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance);
	VPPhysics::RaycastData RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance);
	//void UpdateCapsuleSize(uint32_t entityID, const VPPhysics::CapsuleControllerInfo& info);
	void ResizeCapsuleControllerSize(uint32_t entityID, float radius, float height);
	/// <summary>
/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
/// </summary>
/// <param name="id">  </param>
/// <returns></returns>
	bool RemoveController(const unsigned int& id);

	bool HasController(uint32_t entityID);
	Controller* GetController(uint32_t entityID);
	physx::PxController* GetPxController(uint32_t entityID);
	std::unordered_map<uint32_t, std::shared_ptr<Controller> > m_CharectorMap{};
	physx::PxMaterial* m_Material{};
	uint32_t GetIDFromActor(physx::PxRigidActor* Actor);

	physx::PxPhysics* m_PxPhysics{};
	physx::PxScene* m_Scene = nullptr;

	CollisionManager* m_CollisionManager{};
	physx::PxControllerManager* m_PxControllerManager{};

};

