#pragma once
#include <System.h>
#include "VisPredComponents.h"
#include "EventSubscriber.h"
class PlayerSystem :
    public System, public IUpdatable,public IPhysicable,public IStartable,public IFixedUpdatable,public IRenderable,public ISoundable,public EventSubscriber
{

public:
    PlayerSystem(std::shared_ptr<SceneManager> sceneManager);
    ~PlayerSystem()=default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;
	bool ChangeArm(PlayerComponent& playercomp, bool IsVPmode);
	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override {};
	void PhysicsLateUpdate(float deltaTime) override {};

	// IRenderable을(를) 통해 상속됨
	void BeginRenderUpdate(float deltaTime) override {};
	void RenderUpdate(float deltaTime) override {};
	void LateRenderUpdate(float deltaTime) override {};
	void EditorRenderUpdate(float deltaTime) override {};

	// ISoundable을(를) 통해 상속됨
	void SoundUpdate(float deltaTime) override;
	void VPMode_Cooltime(PlayerComponent& playercomp, float deltatime);
	void Transformation_Time(PlayerComponent& playercomp, float deltatime);
	void NonDamage_Time(PlayerComponent& playercomp, float deltatime);
	void Gun_Cooltime(PlayerComponent& playercomp, float deltatime);

	void OnDamaged(std::any entityid_Damage);



	void SearchingInterectives(PlayerComponent& playercomp);
	void SearchInterective(PlayerComponent& playercomp);
	void SearchedGun(PlayerComponent& playercomp);
	void SearchedInterective(PlayerComponent& playercomp);
	//void Active_VPMode(PlayerComponent& playercomp);
	void CameraShake(PlayerComponent& playercomp,float deltatime);
	void PlayerMeleeAttack(PlayerComponent& playercomp);
	void Melee_Default(PlayerComponent& playercomp);
	void Melee_VPMode(PlayerComponent& playercomp);
#pragma region Physics Setting
	void UpdateCharDataToController(PlayerComponent& playercomp);
	void UpdateControllerSize(PlayerComponent& playercomp);
	void OnCrouchModeController(std::any playercomp);
	void OnSlideModeController(std::any playercomp);
	void OnDefalutModeController(std::any playercomp);


	void CrouchModeController(PlayerComponent& playercomp);
	void SlideModeController(PlayerComponent& playercomp);
	void DefalutModeController(PlayerComponent& playercomp);
	void DownCamera(PlayerComponent& playercomp,float deltatime);
	void UpCamera(PlayerComponent& playercomp, float deltatime);
	void CarmeraPosChange(PlayerComponent& playercomp,float deltatime);
#pragma endregion 



#pragma region FSM Action
	void Action_FSM(PlayerComponent& playercomp, float deltaTime);
	void Action_Idle(PlayerComponent& playercomp);
	void Action_Walk(PlayerComponent& playercomp);
	void Action_Run(PlayerComponent& playercomp);
	void Action_Crouch(PlayerComponent& playercomp);
	void Action_Slide(PlayerComponent& playercomp,float deltatime);
	void Action_Jump(PlayerComponent& playercomp);
	void Action_Die(PlayerComponent& playercomp, float deltatime);
	void Action_Destroy(PlayerComponent& playercomp);
	void Action_Transformation(PlayerComponent& playercomp,float deltatime);
#pragma endregion

#pragma region FSM Sound
	void Sound_FSM(PlayerComponent& playercomp, float deltaTime);
	void Sound_Idle(PlayerComponent& playercomp) {};
	void Sound_Walk(PlayerComponent& playercomp);
	void Sound_Run(PlayerComponent& playercomp);
	void Sound_Crouch(PlayerComponent& playercomp) {};
	void Sound_Slide(PlayerComponent& playercomp) {};
	void Sound_Transformation(PlayerComponent& playercomp) {};
	void Sound_Jump(PlayerComponent& playercomp) {};
	void Sound_Die(PlayerComponent& playercomp) {};
	void Sound_Destroy(PlayerComponent& playercomp) {};
#pragma endregion

#pragma region Animation Change
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni index, float Speed, float transition, bool loop, bool Immidiate = false);
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::VPAni index, float Speed, float transition, bool loop, bool Immidiate = false);
#pragma endregion

#pragma region Active_Logic
	void Active_Interect(PlayerComponent& playercomp);
	void Active_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void Active_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp);
	void Active_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp);
	void Active_Slide(PlayerComponent& playercomp, float deltatime);
	void Active_Attack(PlayerComponent& playercomp);
#pragma endregion 

#pragma region Animation
	void PlayerAnimation(PlayerComponent& playercomp);
	void ReturnToIdle(AnimationComponent& anicomp);
	void ReturnToVPIdle(PlayerComponent& player, AnimationComponent& anicomp);
	void AnimationFinished(PlayerComponent& playercomp, AnimationComponent& anicomp);
	void VPAnimationFinished(PlayerComponent& playercomp, AnimationComponent& anicomp);
#pragma endregion
#pragma region Interection Logic
	/// <summary>
	/// VPMODEChange
	/// </summary>
	/// <param name="playercomp"></param>
	void OnDrop_Gun(std::any playercomp);
	void Drop_Gun(PlayerComponent& playercomp);
#pragma endregion

#pragma region Gun Logic

	void Gun_RecoilingToMiddle(PlayerComponent& playercomp, float deltatime);

	bool Gun_Shoot(PlayerComponent& playercomp,GunComponent& guncomp);
	bool Gun_Throw(PlayerComponent& playercomp, GunComponent& guncomp);
	void Gun_RecoilSetting(PlayerComponent& playercomp, GunComponent& guncomp);
#pragma region Shoot Logic
	bool Shoot_Common(PlayerComponent& playercomp, GunComponent& guncomp, VisPred::Game::PlayerAni idleAnimation, VisPred::Game::PlayerAni attackAnimation);
	bool Shoot_Pistol(PlayerComponent& playercomp, GunComponent& guncomp, TransformComponent& TransformComp);
	bool Shoot_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp, TransformComponent& TransformComp);
	bool Shoot_Rifle(PlayerComponent& playercomp, GunComponent& guncomp,TransformComponent& TransformComp);

#pragma endregion


#pragma region Shoot Logic
	bool Throw_Pistol(PlayerComponent& playercomp, GunComponent& guncomp);
	bool Throw_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp);
	bool Throw_Rifle(PlayerComponent& playercomp, GunComponent& guncomp);
	bool Throw_Setting(PlayerComponent& playercomp);
#pragma endregion

	// IStartable을(를) 통해 상속됨
	void Initialize() override;
	void Start(uint32_t gameObjectId) override;
	void Finish(uint32_t gameObjectId) override {};
	void Finalize() override;
	float Randomfloat(float min, float max);
	double RecoilPercent(double curtime, double time, double percent);
	bool RecoilReturn(double curtime, double time, double percent);
};

