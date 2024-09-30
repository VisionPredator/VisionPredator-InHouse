#pragma once
#include <System.h>
#include "VisPredComponents.h"
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,public IStartable,public IFixedUpdatable,public IRenderable
{

public:
    PlayerSystem(std::shared_ptr<SceneManager> sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;
	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override;
	void SearchingInterectiveItems(PlayerComponent& playercomp);
	void SearchingGun(PlayerComponent& playercomp);
	void ToVPMode(PlayerComponent& playercomp);



#pragma region Physics Setting
	void UpdateCharDataToController(PlayerComponent& playercomp);
	void UpdateControllerSize(PlayerComponent& playercomp);
	void CrouchModeController(PlayerComponent& playercomp);
	void SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void DefalutModeController(PlayerComponent& playercomp);
	void DownCamera(PlayerComponent& playercomp,float deltatime);
	void UpCamera(PlayerComponent& playercomp, float deltatime);
	void CarmeraPosChange(PlayerComponent& playercomp,float deltatime);
#pragma endregion 
#pragma region FSM Calculate
	void Calculate_FSM(PlayerComponent& playercomp);
	void Calculate_Idle(PlayerComponent& playercomp);
	void Calculate_Die(PlayerComponent& playercomp);
	void Calculate_Attack(PlayerComponent& playercomp);
	void Calculate_Walk(PlayerComponent& playercomp);
	void Calculate_Run(PlayerComponent& playercomp);
	void Calculate_Crouch(PlayerComponent& playercomp);
	void Calculate_Slide(PlayerComponent& playercomp);
	void Calculate_Jump(PlayerComponent& playercomp);
	void Calculate_Destroy(PlayerComponent& playercomp);
#pragma endregion

#pragma region FSM Action
	void FSM_Action_FSM(PlayerComponent& playercomp, float deltaTime);
	void FSM_Action_Idle(PlayerComponent& playercomp);
	void FSM_Action_Walk(PlayerComponent& playercomp);
	void FSM_Action_Run(PlayerComponent& playercomp);
	void FSM_Action_Crouch(PlayerComponent& playercomp);
	void FSM_Action_Slide(PlayerComponent& playercomp,float deltatime);
	void FSM_Action_Jump(PlayerComponent& playercomp);
	void FSM_Action_Attack(PlayerComponent& playercomp);
	void FSM_Action_Die(PlayerComponent& playercomp);
	void FSM_Action_Destroy(PlayerComponent& playercomp);
#pragma endregion
#pragma region Animation Change
	void ChangeAni_Attack_Pistol(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack_Pistol, speed, loop, Immidiate); };
	void ChangeAni_Attack_Rifle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack_Rifle, speed, loop, Immidiate); };
	void ChangeAni_Attack_ShotGun(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack_ShotGun, speed, loop, Immidiate); };
	void ChangeAni_Idle01_Pistol(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle01_Pistol, speed, loop, Immidiate); };
	void ChangeAni_Idle01_Rifle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle01_Rifle, speed, loop, Immidiate); };
	void ChangeAni_Idle01_ShotGun(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle01_ShotGun, speed, loop, Immidiate); };
	void ChangeAni_Idle02_Pistol(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle02_Pistol, speed, loop, Immidiate); };
	void ChangeAni_Idle02_Rifle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle02_Rifle, speed, loop, Immidiate); };
	void ChangeAni_Idle02_ShotGun(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle02_ShotGun, speed, loop, Immidiate); };
	void ChangeAni_hook_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::Tohook_Sword, speed, loop, Immidiate); };
	void ChangeAni_hook_Pistol(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::Tohook_Pistol, speed, loop, Immidiate); };
	void ChangeAni_hook_Rifle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::Tohook_Rifle, speed, loop, Immidiate); };
	void ChangeAni_hook_ShotGun(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::Tohook_ShotGun, speed, loop, Immidiate); };
	void ChangeAni_interaction(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::Tointeraction, speed, loop, Immidiate); };
	void ChangeAni_Attack1_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack1_Sword, speed, loop, Immidiate); };
	void ChangeAni_Attack2_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack2_Sword, speed, loop, Immidiate); };
	void ChangeAni_Attack3_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToAttack3_Sword, speed, loop, Immidiate); };
	void ChangeAni_Idle01_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle01_Sword, speed, loop, Immidiate); };
	void ChangeAni_Idle02_Sword(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToIdle02_Sword, speed, loop, Immidiate); };
	void ChangeAni_throw_Pistol(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToThrow_Pistol, speed, loop, Immidiate); };
	void ChangeAni_throw_Rifle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToThrow_Rifle, speed, loop, Immidiate); };
	void ChangeAni_throw_ShotGun(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToThrow_ShotGun, speed, loop, Immidiate); };
	void ChangeAni_VP1_attack_L(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_attack_L, speed, loop, Immidiate); };
	void ChangeAni_VP1_attack_R(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_attack_R, speed, loop, Immidiate); };
	void ChangeAni_VP2_Idle(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_Idle, speed, loop, Immidiate); };
	void ChangeAni_vp3_dash(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_dash, speed, loop, Immidiate); };
	void ChangeAni_vp4_jump(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_jump, speed, loop, Immidiate); };
	void ChangeAni_vp5_run(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_run, speed, loop, Immidiate); };
	void ChangeAni_vp_draw(uint32_t entityID, float speed, bool loop, bool Immidiate = false) { ChangeAni_Index(entityID, VisPred::Game::PlayerAni::ToVP_draw, speed, loop, Immidiate); };
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni index, float Speed, bool loop, bool Immidiate = false);
#pragma endregion
#pragma region Animation
	void Animation(uint32_t entityid, float deltaTime);
	void PlayerAnime(PlayerComponent& playercomp);
	void ReturnToIdle(AnimationComponent& anicomp);
#pragma endregion

#pragma region Gun Logic
	void PlayerInterect(PlayerComponent& playercomp);
	void Grab_Gun(PlayerComponent& playercomp);
	void Gun_Shoot(PlayerComponent& playercomp,GunComponent& guncomp);
	void Gun_Throw(PlayerComponent& playercomp, GunComponent& guncomp);
	void PlayerShoot(PlayerComponent& playercomp);
	void GunCooltime(PlayerComponent& playercomp, float deltatime);

#pragma region Shoot Logic
	void Shoot_Pistol(PlayerComponent& playercomp, GunComponent& guncomp);
	void Shoot_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp);
	void Shoot_Rifle(PlayerComponent& playercomp, GunComponent& guncomp);
#pragma endregion
#pragma endregion 

#pragma region Move Logic
	void Move_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void Move_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp);
	void Move_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp);
	void Move_Slide(PlayerComponent& playercomp);
#pragma endregion
#pragma region Attack Logic
	void Attack(PlayerComponent& playercomp);
#pragma endregion



	// IStartable을(를) 통해 상속됨
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override;

	void Finalize() override;



	// IRenderable을(를) 통해 상속됨
	void BeginRenderUpdate(float deltaTime) override;

	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;

	void EditorRenderUpdate(float deltaTime) override;

};

