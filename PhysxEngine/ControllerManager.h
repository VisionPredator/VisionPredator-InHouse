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
	/// 컨트롤러 매니저 생성 및 초기화 함수
	/// </summary>
	/// <returns></returns>
	bool Initialize(physx::PxScene* scene, physx::PxPhysics* physics, CollisionManager* collisionManager);


/// <summary>
/// 캐릭터 컨트롤러 생성 함수
/// </summary>
/// <param name="controllerInfo"> 캐릭터 컨트롤러 생성 데이터 </param>
/// <param name="movementInfo"> 캐릭터 무브먼트 생성 데이터 </param>
/// <returns></returns>
	bool CreatController();
	bool CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo, VPPhysics::PhysicsInfo physicsinfo);
	bool Update(float deltatime);
	/// <summary>
/// 캐릭터 컨트롤러 삭제 함수
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

