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
	/// �þ� ����, ���� ����, �߰� ������ �����Ͽ� ��ȯ
	/// </summary>
	static void CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput);

	/// <summary>
	/// �÷��̾ ���� ȸ��
	/// </summary>
	static void RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime);

	/// <summary>
	/// ���� �ִϸ��̼� ����
	/// </summary>
	void ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop = true, bool isImmediate = false);
};
