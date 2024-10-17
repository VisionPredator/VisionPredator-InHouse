#pragma once
#include "../VPEngine/System.h"
#include "VisPredComponents.h"

class EnemySystem :
	public System
	, public IFixedUpdatable
	, public IUpdatable
	, public IPhysicable
	, public IRenderable
	, public IStartable
{
public:
    EnemySystem(const std::shared_ptr<SceneManager>& sceneManager) : System(sceneManager) {}
	
public:
	// IStartable
	void Initialize();
	void Start(uint32_t gameObjectId);
	void Finish(uint32_t gameObjectId) {}
	void Finalize() {}

	// IRenderable
	// TODO: �׷��Ƚ� �̿��� �޼ҵ� ȣ���Ϸ��� �ϴ� ����� ���µ�. �����ؾ���.
	void BeginRenderUpdate(float deltaTime) override {}
	void RenderUpdate(float deltaTime) override {}
	void LateRenderUpdate(float deltaTime) override {}
	void EditorRenderUpdate(float deltaTime) override {}

	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override {}
    // IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

	// IPhysicable��(��) ���� ��ӵ�
	void PhysicsUpdate(float deltaTime) override {}
	void PhysicsLateUpdate(float deltaTime) override {}

private:
	void CalculateFSM(EnemyComponent& enemyComp);
	void CalculateIdle(EnemyComponent& enemyComp);
	void CalculateChase(EnemyComponent& enemyComp);
	void CalculatePatrol(EnemyComponent& enemyComp);
	void CalculateDie(EnemyComponent& enemyComp);

private:
	/// <summary>
	///	�þ� ����, ���� ����, �߰� ���� �ȿ� �÷��̾ �ִ��� Ȯ������ Enemy�� ���¸� ó��
	/// </summary>
	void DetectTarget(EnemyComponent& enemyComp, float deltaTime);

	/// <summary>
	/// �þ� ����, ���� ����, �߰� ������ �����Ͽ� ��ȯ
	/// </summary>
    static void CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput);

	/// <summary>
	/// �÷��̾ ���� ȸ��
	/// </summary>
    static void RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime);

	/// <summary>
	///	��� ó�� �ִϸ��̼� ��� �� ��ü ���� (ObjectPool�� �����Ǿ� �ִٸ� Pool�� ������Ʈ ��ȯ)
	/// </summary>
	void Die(EnemyComponent& enemyComp);

	/// <summary>
	/// ���� ���� Enum �� ���� for FSM
	/// </summary>
    static void ChangeCurrentStateEnumValue(EnemyComponent& enemyComp, VisPred::Game::EnemyState state) { enemyComp.CurrentFSM = state; }

	/// <summary>
	/// ���� �ִϸ��̼� ����
	/// </summary>
	void ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop = true, bool isImmediate = false);

private:
	PlayerComponent* m_PlayerComp = nullptr;
};

