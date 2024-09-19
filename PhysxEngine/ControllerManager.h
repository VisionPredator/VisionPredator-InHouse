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
	uint32_t RaycastToHitActor(uint32_t entityID, VPMath::Vector3 dir, float distance);
	uint32_t RaycastToHitActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance);
	uint32_t RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
	/// <summary>
/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
/// </summary>
/// <param name="id">  </param>
/// <returns></returns>
	bool RemoveController(const unsigned int& id);

	bool HasController(uint32_t entityID);
	Controller* GetController(uint32_t entityID);
	std::unordered_map<uint32_t, std::shared_ptr<Controller> > m_CharectorMap{};
	physx::PxMaterial* m_Material{};
	uint32_t GetIDFromActor(physx::PxRigidActor* Actor);

	physx::PxPhysics* m_PxPhysics{};
	physx::PxScene* m_Scene = nullptr;

	CollisionManager* m_CollisionManager{};
	physx::PxControllerManager* m_PxControllerManager{};

};

