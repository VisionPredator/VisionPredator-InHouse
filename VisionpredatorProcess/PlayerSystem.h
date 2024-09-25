#pragma once
#include <System.h>
#include "VisPredComponents.h"
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,public IStartable,public IFixedUpdatable
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
	void RaycastTest(PlayerComponent& playercomp);

    void PlayerShoot(PlayerComponent& playercomp);
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
	void Action_FSM(PlayerComponent& playercomp, float deltaTime);
	void Action_Idle(PlayerComponent& playercomp);
	void Action_Walk(PlayerComponent& playercomp);
	void Action_Run(PlayerComponent& playercomp);
	void Action_Crouch(PlayerComponent& playercomp);
	void Action_Slide(PlayerComponent& playercomp,float deltatime);
	void Action_Jump(PlayerComponent& playercomp);
	void Action_Attack(PlayerComponent& playercomp);
	void Action_Die(PlayerComponent& playercomp);
	void Action_Destroy(PlayerComponent& playercomp);
#pragma endregion
#pragma region Animation Change
	void ChangeAniToAttack_Pistol	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack_Pistol	); };
	void ChangeAniToAttack_Rifle	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack_Rifle	); };
	void ChangeAniToAttack_ShotGun	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack_ShotGun	); };
	void ChangeAniToIdle01_Pistol	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle01_Pistol	); };
	void ChangeAniToIdle01_Rifle	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle01_Rifle	); };
	void ChangeAniToIdle01_ShotGun	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle01_ShotGun	); };
	void ChangeAniToIdle02_Pistol	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle02_Pistol	); };
	void ChangeAniToIdle02_Rifle	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle02_Rifle	); };
	void ChangeAniToIdle02_ShotGun	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle02_ShotGun	); };
	void ChangeAniTohook_Sword		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::Tohook_Sword		); };
	void ChangeAniTohook_Pistol		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::Tohook_Pistol	); };
	void ChangeAniTohook_Rifle		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::Tohook_Rifle		); };
	void ChangeAniTohook_ShotGun	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::Tohook_ShotGun	); };
	void ChangeAniTointeraction		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::Tointeraction	); };
	void ChangeAniToAttack1_Sword	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack1_Sword	); };
	void ChangeAniToAttack2_Sword	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack2_Sword	); };
	void ChangeAniToAttack3_Sword	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToAttack3_Sword	); };
	void ChangeAniToIdle01_Sword	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle01_Sword	); };
	void ChangeAniToIdle02_Sword	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToIdle02_Sword	); };
	void ChangeAniTothrow_Pistol	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToThrow_Pistol	); };
	void ChangeAniTothrow_Rifle		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToThrow_Rifle	); };
	void ChangeAniTothrow_ShotGun	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToThrow_ShotGun	); };
	void ChangeAniToVP1_attack_L	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_attack_L	); };
	void ChangeAniToVP1_attack_R	(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_attack_R	); };
	void ChangeAniToVP2_Idle		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_Idle		); };
	void ChangeAniTovp3_dash		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_dash		); };
	void ChangeAniTovp4_jump		(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_jump		); };
	void ChangeAniTovp5_run			(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_run		); };
	void ChangeAniTovp_draw			(PlayerComponent& PlayerComp){ ChangeAniToIndex(PlayerComp.GetEntityID(),VisPred::Game::PlayerAni::ToVP_draw		); };
	void ChangeAniToIndex(uint32_t, VisPred::Game::PlayerAni index,bool Immidiate=false) ;
#pragma endregion
#pragma region Animation
	void Animation(uint32_t entityid, float deltaTime);

#pragma endregion

#pragma region Shoot Logic
	void Shoot_Style(PlayerComponent& playercomp);
	void Shoot_Pistol(PlayerComponent& playercomp);
	void Shoot_ShotGun(PlayerComponent& playercomp);
	void Shoot_Rifle(PlayerComponent& playercomp);
	void GunCooltime(PlayerComponent& playercomp);
#pragma endregion

#pragma region Move Logic
	void Move_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void Move_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp);
	void Move_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp);
	void Move_Slide(PlayerComponent& playercomp);
#pragma endregion




	// IStartable을(를) 통해 상속됨
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override;

	void Finalize() override;


};

