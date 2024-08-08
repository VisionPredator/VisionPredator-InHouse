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


	physx::PxPhysics* m_PxPhysics{};
	CollisionManager* m_CollisionManager{};
	physx::PxControllerManager* m_PxControllerManager{};

};

