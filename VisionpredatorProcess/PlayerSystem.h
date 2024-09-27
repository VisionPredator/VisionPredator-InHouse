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
	void SearchingItem(PlayerComponent& playercomp);
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
	void ChangeAni_Attack_Pistol(PlayerComponent& PlayerComp,bool Immidiate =false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_Pistol, Immidiate); };
	void ChangeAni_Attack_Rifle(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_Rifle, Immidiate); };
	void ChangeAni_Attack_ShotGun(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_ShotGun, Immidiate); };
	void ChangeAni_Idle01_Pistol(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_Pistol, Immidiate); };
	void ChangeAni_Idle01_Rifle(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_Rifle, Immidiate); };
	void ChangeAni_Idle01_ShotGun(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_ShotGun, Immidiate); };
	void ChangeAni_Idle02_Pistol(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle02_Pistol, Immidiate); };
	void ChangeAni_Idle02_Rifle(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle02_Rifle, Immidiate); };
	void ChangeAni_Idle02_ShotGun(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle02_ShotGun, Immidiate); };
	void ChangeAni_hook_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::Tohook_Sword, Immidiate); };
	void ChangeAni_hook_Pistol(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::Tohook_Pistol, Immidiate); };
	void ChangeAni_hook_Rifle(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::Tohook_Rifle, Immidiate); };
	void ChangeAni_hook_ShotGun(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::Tohook_ShotGun, Immidiate); };
	void ChangeAni_interaction(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::Tointeraction, Immidiate); };
	void ChangeAni_Attack1_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack1_Sword, Immidiate); };
	void ChangeAni_Attack2_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack2_Sword, Immidiate); };
	void ChangeAni_Attack3_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack3_Sword, Immidiate); };
	void ChangeAni_Idle01_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_Sword, Immidiate); };
	void ChangeAni_Idle02_Sword(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToIdle02_Sword, Immidiate); };
	void ChangeAni_throw_Pistol(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Pistol, Immidiate); };
	void ChangeAni_throw_Rifle(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Rifle, Immidiate); };
	void ChangeAni_throw_ShotGun(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_ShotGun, Immidiate); };
	void ChangeAni_VP1_attack_L(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_attack_L, Immidiate); };
	void ChangeAni_VP1_attack_R(PlayerComponent& PlayerComp, bool Immidiate = false)	{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_attack_R, Immidiate); };
	void ChangeAni_VP2_Idle(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_Idle, Immidiate); };
	void ChangeAni_vp3_dash(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_dash, Immidiate); };
	void ChangeAni_vp4_jump(PlayerComponent& PlayerComp, bool Immidiate = false)		{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_jump, Immidiate); };
	void ChangeAni_vp5_run(PlayerComponent& PlayerComp, bool Immidiate = false)			{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_run, Immidiate); };
	void ChangeAni_vp_draw(PlayerComponent& PlayerComp, bool Immidiate = false)			{ ChangeAni_Index(PlayerComp.GetEntityID(), VisPred::Game::PlayerAni::ToVP_draw, Immidiate); };
	void ChangeAni_Index(uint32_t, VisPred::Game::PlayerAni index, bool Immidiate = false);
#pragma endregion
#pragma region Animation
	void Animation(uint32_t entityid, float deltaTime);

#pragma endregion

#pragma region Gun Logic
	void Gun_Grab(PlayerComponent& playercomp);
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

