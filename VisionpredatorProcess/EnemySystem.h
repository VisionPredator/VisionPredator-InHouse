#pragma once
#include "../VPEngine/System.h"
struct EnemyComponent;
struct PlayerComponent;
class EnemySystem :
	public System
	, public IFixedUpdatable
	, public IUpdatable
	, public IPhysicable
	, public IRenderable
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
	void Calculate_Chase(EnemyComponent& enemycomp);
	void Calculate_Patrol(EnemyComponent& enemycomp);
	void Calculate_Die(EnemyComponent& enemycomp);

	void Calculate_Attack(EnemyComponent& enemycomp);		// 삭제
	void Calculate_Walk(EnemyComponent& enemycomp);			// 삭제
	void Calculate_Attacked(EnemyComponent& enemycomp);		// 삭제
	void Calculate_BackWalk(EnemyComponent& enemycomp);		// 삭제

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


	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override;
	void PhysicsLateUpdate(float deltaTime) override;


private:
	void Die(EnemyComponent& enemycomp);
	
	/// <summary>
	///	시야 범위 안에 플레이어가 있는지 확인한후 Enemy의 상태를 처리한다.
	/// </summary>
	void DetectTarget(EnemyComponent& enemyComp);

public:
	// TODO: 그래픽스 이용한 메소드 호출하려고 일단 상속해 놨는데. 수정해야함.
    void BeginRenderUpdate(float deltaTime) override {};
    void RenderUpdate(float deltaTime) override {};
    void LateRenderUpdate(float deltaTime) override {};
    void EditorRenderUpdate(float deltaTime) override {};
};

