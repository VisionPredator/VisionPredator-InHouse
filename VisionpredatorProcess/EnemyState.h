#pragma once
#include "IState.h"
#include "VisPredStructs.h"

struct EnemyComponent;
struct TransformComponent;
class EnemyState : public virtual IState
{
public:
	/// <summary>
	/// �÷��̾ ���� ���� �ִ��� Ž���ϰ� ���¸� ����
	/// </summary>
	static void DetectTarget(EnemyComponent& enemyComp, float deltaTime);

	/// <summary>
	///	Enemy�� HP�� Ȯ���ϰ� 0 �����̸� true �� ��ȯ
	/// </summary>
	static bool CheckIsDead(const std::shared_ptr<EnemyComponent>& enemyComponent);

	/// <summary>
	/// �þ� ����, ���� ����, �߰� ������ �����Ͽ� ��ȯ
	/// </summary>
	static void CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput);

	/// <summary>
	/// �÷��̾ ���� ȸ���ϰ� ���� �� ȸ���Ͽ��ٸ� true�� ��ȯ
	/// </summary>
	static void RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime);

	/// <summary>
	/// ���� ���¸� ��ȯ�ϰ� Enter �Լ��� ȣ���Ѵ�. ���� ���´� Exit �Լ��� ȣ���Ѵ�.
	/// </summary>
	static void ChangeCurrentState(const std::shared_ptr<EnemyComponent>& enemyComponent, IState* newState);
	static void ChangeCurrentState(EnemyComponent& enemyComponent, IState* newState);

	/// <summary>
	/// ���� �ִϸ��̼� ����
	/// </summary>
	static void ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, float transitionTime = 0.f, bool isLoop = true, bool isImmediate = false);
	static void ChangeCurrentAnimation(const std::shared_ptr<EnemyComponent>& enemyComp, VisPred::Game::EnemyAni animation, float speed, float transitionTime = 0.f, bool isLoop = true, bool isImmediate = false);
};
