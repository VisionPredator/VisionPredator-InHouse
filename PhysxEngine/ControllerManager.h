#pragma once
class CharacterController;
class CollisionManager;
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
	bool CreatCapsuleController();

	/// <summary>
/// 캐릭터 컨트롤러 삭제 함수
/// </summary>
/// <param name="id">  </param>
/// <returns></returns>
	bool RemoveController(const unsigned int& id);

	bool HasController(uint32_t entityID);
	CharacterController* GetController(uint32_t entityID);
	std::unordered_map<uint32_t, CharacterController*> m_CharectorMap;

	physx::PxPhysics* m_PxPhysics{};
	CollisionManager* m_CollisionManager{};
	physx::PxControllerManager* m_PxControllerManager{};

};

