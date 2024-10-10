#pragma once
#include <System.h>
#include "VisPredComponents.h"
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,public IStartable,public IFixedUpdatable,public IRenderable,public ISoundable
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
	// IPhysicable을(를) 통해 상속됨
	void PhysicsLateUpdate(float deltaTime) override;
	void SearchingInterectives(PlayerComponent& playercomp);
	void SearchInterective(PlayerComponent& playercomp);
	void SearchedGun(PlayerComponent& playercomp);
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
#pragma region FSM Sound
	void FSM_Sound_FSM(PlayerComponent& playercomp, float deltaTime);
	void FSM_Sound_Idle(PlayerComponent& playercomp);
	void FSM_Sound_Walk(PlayerComponent& playercomp);
	void FSM_Sound_Run(PlayerComponent& playercomp);
	void FSM_Sound_Crouch(PlayerComponent& playercomp);
	void FSM_Sound_Slide(PlayerComponent& playercomp);
	void FSM_Sound_Jump(PlayerComponent& playercomp);
	void FSM_Sound_Attack(PlayerComponent& playercomp);
	void FSM_Sound_Die(PlayerComponent& playercomp);
	void FSM_Sound_Destroy(PlayerComponent& playercomp);
#pragma endregion

#pragma region Animation Change
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni index, float Speed, bool loop, bool Immidiate = false);
#pragma endregion

#pragma region Active_Logic
	void Active_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void Active_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp);
	void Active_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp);
	void Active_Slide(PlayerComponent& playercomp, float deltatime);
	void Active_Attack(PlayerComponent& playercomp);
#pragma endregion 


#pragma region Animation
	void PlayerAnimation(PlayerComponent& playercomp);
	void ReturnToIdle(AnimationComponent& anicomp);
	void ThrowFinished(PlayerComponent& playercomp);
#pragma endregion

#pragma region Gun Logic
	void PlayerInterect(PlayerComponent& playercomp);
	void Grab_Gun(PlayerComponent& playercomp);
	void Gun_Shoot(PlayerComponent& playercomp,GunComponent& guncomp);
	void Gun_Throw(PlayerComponent& playercomp, GunComponent& guncomp);
	void GunCooltime(PlayerComponent& playercomp, float deltatime);
#pragma region Shoot Logic
	void Shoot_Pistol(PlayerComponent& playercomp, GunComponent& guncomp);
	void Shoot_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp);
	void Shoot_Rifle(PlayerComponent& playercomp, GunComponent& guncomp);
#pragma endregion


#pragma region Shoot Logic
	void Throw_Pistol(PlayerComponent& playercomp, GunComponent& guncomp);
	void Throw_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp);
	void Throw_Rifle(PlayerComponent& playercomp, GunComponent& guncomp);
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
	// ISoundable을(를) 통해 상속됨
	void SoundUpdate(float deltaTime) override;
};

