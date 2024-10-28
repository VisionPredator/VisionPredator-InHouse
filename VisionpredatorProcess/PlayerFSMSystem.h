#pragma once
#include <System.h>

class PlayerFSMSystem :
    public System, public IUpdatable, public ILateUpdatable, public ISoundable, public EventSubscriber
{
public:
    PlayerFSMSystem(std::shared_ptr<SceneManager> sceneManager);
    ~PlayerFSMSystem() = default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
    // ISoundable을(를) 통해 상속됨
	void SoundUpdate(float deltaTime) override;


	// ILateUpdatable을(를) 통해 상속됨
	void LateUpdate(float deltaTime) override;

#pragma region FSM Calculate
	void Calculate_FSM(PlayerComponent& playercomp);
	void Calculate_Idle(PlayerComponent& playercomp);
	void Calculate_Transformation(PlayerComponent& playercomp);
	void Calculate_Walk(PlayerComponent& playercomp);
	void Calculate_Run(PlayerComponent& playercomp);
	void Calculate_Crouch(PlayerComponent& playercomp);
	void Calculate_Slide(PlayerComponent& playercomp);
	void Calculate_Jump(PlayerComponent& playercomp);
	void Calculate_Die(PlayerComponent& playercomp);
	void Calculate_Die_end(PlayerComponent& playercomp);
#pragma endregion
#pragma region FSM Enter
	void Enter_FSM(PlayerComponent& playercomp);
	void Enter_Idle(PlayerComponent& playercomp);
	void Enter_Walk(PlayerComponent& playercomp);
	void Enter_Run(PlayerComponent& playercomp);
	void Enter_Jump(PlayerComponent& playercomp);
	void Enter_Crouch(PlayerComponent& playercomp);
	void Enter_Dash_Slide(PlayerComponent& playercomp);
	void Enter_Transformation(PlayerComponent& playercomp);
	void Enter_Die(PlayerComponent& playercomp);
	void Enter_Die_end(PlayerComponent& playercomp);
	//void Enter_Transformation(PlayerComponent& playercomp, bool VPMode);
#pragma endregion
#pragma region FSM Sound
	void Enter_Sound_FSM(PlayerComponent& playercomp, float deltaTime);
	void Enter_Sound_Idle(PlayerSoundComponent& playercomp);
	void Enter_Sound_Walk(PlayerSoundComponent& playercomp);
	void Enter_Sound_Run(PlayerSoundComponent& playercomp);
	void Enter_Sound_Crouch(PlayerSoundComponent& playercomp);
	void Enter_Sound_Dash_Slide(PlayerSoundComponent& playercomp);
	void Enter_Sound_Transformation(PlayerSoundComponent& playercomp);
	void Enter_Sound_Jump(PlayerSoundComponent& playercomp);
	void Enter_Sound_Die(PlayerSoundComponent& playercomp);
	void Enter_Sound_Destroy(PlayerSoundComponent& playercomp);
	void Enter_Sound_Die_end(PlayerSoundComponent& playercomp);
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni index, float Speed, float transition, bool loop, bool Immidiate = false);
	void ChangeAni_Index(uint32_t entityID, VisPred::Game::VPAni index, float Speed, float transition, bool loop, bool Immidiate = false);
#pragma endregion
	void SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp);
	void Drop_Gun(PlayerComponent& playercomp);
	void SlideModeController(PlayerComponent& playercomp);
	void CrouchModeController(PlayerComponent& playercomp);
	void DefalutModeController(PlayerComponent& playercomp);
};

