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
	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;
    // IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

#pragma region FSM Calculate
	void Calculate_FSM(EnemyComponent& enemycomp);
	void Calculate_Attack(EnemyComponent& enemycomp);
	void Calculate_Idle(EnemyComponent& enemycomp);
	void Calculate_Chase(EnemyComponent& enemycomp);
	void Calculate_Walk(EnemyComponent& enemycomp);
	void Calculate_Die(EnemyComponent& enemycomp);
	void Calculate_Attacked(EnemyComponent& enemycomp);
	void Calculate_BackWalk(EnemyComponent& enemycomp);
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


	// IPhysicable��(��) ���� ��ӵ�
	void PhysicsUpdate(float deltaTime) override;
	void PhysicsLateUpdate(float deltaTime) override;


private:
	void Die(EnemyComponent& enemycomp);

	// �þ� ���� �ȿ� �÷��̾ �ִ��� Ȯ��
	void DetectTarget(EnemyComponent& enemycomp);

public:
    void BeginRenderUpdate(float deltaTime) override {};
    void RenderUpdate(float deltaTime) override {};
    void LateRenderUpdate(float deltaTime) override {};
    void EditorRenderUpdate(float deltaTime) override {};
};

