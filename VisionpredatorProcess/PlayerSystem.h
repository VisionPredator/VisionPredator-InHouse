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
	void RaycastTest(PlayerComponent& playercomp);
    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;
    void PlayerShoot(PlayerComponent& playercomp);
#pragma region Physics Setting
	void UpdateCharDataToController(PlayerComponent& playercomp);
	void UpdateControllerSize(PlayerComponent& playercomp);
	void CrouchModeController(PlayerComponent& playercomp);
	void DefalutModeController(PlayerComponent& playercomp);
	void DownCamera(PlayerComponent& playercomp,float deltatime);
	void UpCamera(PlayerComponent& playercomp, float deltatime);
	void CarmeraPosChange(PlayerComponent& playercomp,float deltatime);
	void SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp);
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


	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

};

