#pragma once
#include "../VPEngine/System.h"
struct EnemyComponent;
class EnemySystem :
	public System,public IFixedUpdatable,public IUpdatable
{
public:
    EnemySystem(std::shared_ptr<SceneManager> SceneMagener);
	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;
    // IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

#pragma region FSM Calculate
	void Calculate_FSM(EnemyComponent& enemycomp);
	void Calculate_Idle(EnemyComponent& enemycomp);
	void Calculate_Die(EnemyComponent& enemycomp);
	void Calculate_Attack(EnemyComponent& enemycomp);
	void Calculate_Move(EnemyComponent& enemycomp);
	void Calculate_Destroy(EnemyComponent& enemycomp);
#pragma endregion

#pragma region FSM Action
	void Action_FSM(EnemyComponent& enemycomp);
	void Action_Idle(EnemyComponent& enemycomp);
	void Action_Move(EnemyComponent& enemycomp);
	void Action_Attack(EnemyComponent& enemycomp);
	void Action_Die(EnemyComponent& enemycomp);
	void Action_Destroy(EnemyComponent& enemycomp);
#pragma endregion

#pragma region Shoot Logic
	void Shoot_Style(EnemyComponent& enemycomp);
	void Shoot_Pistol(EnemyComponent& enemycomp);
	void Shoot_ShotGun(EnemyComponent& enemycomp);
	void Shoot_Rifle(EnemyComponent& enemycomp);
#pragma endregion

};

