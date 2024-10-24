#pragma once
#include <System.h>
class PlayerFSMSystem :
    public System, public IUpdatable, public IPhysicable, public IStartable, public ILateUpdatable, public IRenderable, public ISoundable
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

//#pragma region FSM Calculate
//	void Calculate_FSM(PlayerComponent& playercomp);
//	void Calculate_Idle(PlayerComponent& playercomp);
//	void Calculate_Transformation(PlayerComponent& playercomp);
//	void Calculate_Walk(PlayerComponent& playercomp);
//	void Calculate_Run(PlayerComponent& playercomp);
//	void Calculate_Crouch(PlayerComponent& playercomp);
//	void Calculate_Slide(PlayerComponent& playercomp);
//	void Calculate_Jump(PlayerComponent& playercomp);
//	void Calculate_Die(PlayerComponent& playercomp);
//	void Calculate_Die_end(PlayerComponent& playercomp);
//#pragma endregion

#pragma region FSM Sound
	void Enter_Sound_FSM(PlayerComponent& playercomp, float deltaTime);
	void Enter_Sound_Idle(PlayerComponent& playercomp);
	void Enter_Sound_Walk(PlayerComponent& playercomp);
	void Enter_Sound_Run(PlayerComponent& playercomp);
	void Enter_Sound_Crouch(PlayerComponent& playercomp);
	void Enter_Sound_Dash_Slide(PlayerComponent& playercomp);
	void Enter_Sound_Transformation(PlayerComponent& playercomp);
	void Enter_Sound_Jump(PlayerComponent& playercomp);
	void Enter_Sound_Die(PlayerComponent& playercomp);
	void Enter_Sound_Destroy(PlayerComponent& playercomp);
	void Enter_Sound_Die_end(PlayerComponent& playercomp);
#pragma endregion

};

