#pragma once
#include "IState.h"
#include "VisPredStructs.h"

struct EnemyComponent;
struct TransformComponent;
class EnemyState : public virtual IState
{
public:
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
	/// 현재 상태를 변환하고 Enter함수를 호출한다. 이전 상태는 Exit 함수를 호출한다.
	/// </summary>
	static void ChangeCurrentState(const std::shared_ptr<EnemyComponent>& enemyComponent, IState* newState);
	static void ChangeCurrentState(EnemyComponent& enemyComponent, IState* newState);

	/// <summary>
	/// 현재 애니메이션 변경
	/// </summary>
	void ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop = true, bool isImmediate = false);
};
