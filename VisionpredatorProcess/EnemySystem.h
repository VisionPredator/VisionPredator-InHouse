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
	// TODO: 그래픽스 이용한 메소드 호출하려고 일단 상속해 놨는데. 수정해야함.
	void BeginRenderUpdate(float deltaTime) override {}
	void RenderUpdate(float deltaTime) override {}
	void LateRenderUpdate(float deltaTime) override {}
	void EditorRenderUpdate(float deltaTime) override {}

	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override {}
    // IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

	// IPhysicable을(를) 통해 상속됨
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
	///	시야 범위, 소음 범위, 추격 범위 안에 플레이어가 있는지 확인한후 Enemy의 상태를 처리
	/// </summary>
	void DetectTarget(EnemyComponent& enemyComp, float deltaTime);

	/// <summary>
	/// 시야 범위, 소음 범위, 추격 범위를 생성하여 반환
	/// </summary>
    static void CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput);

	/// <summary>
	/// 플레이어를 향해 회전
	/// </summary>
    static void RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime);

	/// <summary>
	///	사망 처리 애니메이션 출력 후 객체 삭제 (ObjectPool이 구현되어 있다면 Pool에 오브젝트 반환)
	/// </summary>
	void Die(EnemyComponent& enemyComp);

	/// <summary>
	/// 현재 상태 Enum 값 변경 for FSM
	/// </summary>
    static void ChangeCurrentStateEnumValue(EnemyComponent& enemyComp, VisPred::Game::EnemyState state) { enemyComp.CurrentFSM = state; }

	/// <summary>
	/// 현재 애니메이션 변경
	/// </summary>
	void ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop = true, bool isImmediate = false);

private:
	PlayerComponent* m_PlayerComp = nullptr;
};

