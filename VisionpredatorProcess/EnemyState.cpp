#include "pch.h"
#include "EnemyState.h"
#include "EngineStructs.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "../VPGraphics/D3DUtill.h"

#include "StatesInclude.h"

struct null_deleter
{
	void operator()(void const*) const {}
};

void EnemyState::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
{
	// ���� Enemy�� Dead ���¶�� Ž���� �������� ����
	if (enemyComp.BehaviorState == &EnemyBehaviorState::s_Dead)
		return;

	const uint32_t enemyID = enemyComp.GetEntityID();
	const auto transform = enemyComp.GetComponent<TransformComponent>();

	// �þ� ����, ��������, �߰� ���� ����
	DirectX::BoundingFrustum viewRange;
	DirectX::BoundingSphere noiseRange, chaseRange;
	CreateDetectionAreas(enemyComp, transform, viewRange, noiseRange, chaseRange);

	// draw debug circle, frustum
#ifdef _DEBUG 
	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum = viewRange;
	frustumInfo.Color = VisPred::SimpleMath::Color{ 1, 1, 0, 1 };
	enemyComp.Graphics->DrawFrustum(frustumInfo);

	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere = noiseRange;
	sphereInfo.Color = VPMath::Color{ 1, 0, 1, 1 };
	enemyComp.Graphics->DrawSphere(sphereInfo);
#endif _DEBUG

	if (!enemyComp.Player)
		return;

	// �÷��̾� ��ġ ���� ������
	const uint32_t playerID = enemyComp.Player->GetEntityID();
	const auto playerTransform = enemyComp.Player->GetComponent<TransformComponent>();
	const VisPred::SimpleMath::Vector3 playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};

	constexpr float offsetY = 1.8f;
	const VPMath::Vector3 enemyPos =
	{ transform->WorldTransform._41, transform->WorldTransform._42 + offsetY, transform->WorldTransform._43 };

	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	/// �þ� ����, ���� ����, �߰� ���� ���� �÷��̾� ���� ����.

// �� ������ ��� �Դ����� Ȯ���ϰ�
// ���� ������ Ȯ���Ѵ�.
	bool isInViewRange = false, isInNoiseRange = false, isInChaseRange = false;

	isInViewRange = viewRange.Contains(playerPos);
	isInNoiseRange = noiseRange.Contains(playerPos) && (enemyComp.Player->CurrentFSM == VisPred::Game::EFSM::RUN) || (enemyComp.Player->CurrentFSM == VisPred::Game::EFSM::JUMP) ? true : false;	// ����ȭ �ʿ�.

	// �÷��̾ Enemy�� �þ� ���� �ȿ� �������� ������ ����ĳ��Ʈ ����
	if (isInViewRange || isInNoiseRange)	// TODO: �������� ���� �ȿ��� �÷��̾ "�������� ��" �� Ȯ���ؾ� �Ѵ�.
	{
		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);

		const uint32_t detectedObjID = enemyComp.PhysicsManager->RaycastToHitActorFromLocation_Ignore(enemyID, enemyPos, targetDir, enemyComp.FarZ);
		if (detectedObjID == playerID)
		{
			if (enemyComp.BehaviorState != &EnemyBehaviorState::s_Chase)
			{
				//enemyComp.BehaviorState = &EnemyBehaviorState::s_Chase;
				//const std::shared_ptr<EnemyComponent> temp(&enemyComp, null_deleter{});
				//enemyComp.BehaviorState->Enter(temp);
				ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Chase);
			}
			RotateToTarget(transform, targetDir, deltaTime);	// TODO: MovementState ������ �Űܾ� �ҵ�..?

		}
	}
	else  // TODO: ����.. ������ �ʿ�.	 // ���� ���¸� ���� -> ���� ���¸� �Ű������� �޾ƿ���.
	{
		if (enemyComp.BehaviorState != &EnemyBehaviorState::s_Idle)
		{
			//enemyComp.BehaviorState = &EnemyBehaviorState::s_Idle;
			//const std::shared_ptr<EnemyComponent> temp(&enemyComp, null_deleter{});
			//enemyComp.BehaviorState->Enter(temp);
			ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Idle);
		}

	}
}

void EnemyState::CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform,
                                      DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput,
                                      DirectX::BoundingSphere& chaseRangeOutput)
{
	constexpr float offsetY = 1.8f;
	VPMath::Vector3 enemyPos = { transform->WorldTransform._41, transform->WorldTransform._42 + offsetY, transform->WorldTransform._43 };

	// �þ� ���� ����
	D3DUtill::CreateBoundingFrustum(
		enemyPos,
		transform->FrontVector,
		transform->UpVector,
		enemyComp.HorizontalFOV,
		enemyComp.VerticalFOV,
		enemyComp.NearZ,
		enemyComp.FarZ,
		viewRangeOutput,
		enemyComp.IsModelFlipped
	);

	enemyPos.y -= offsetY;

	// ���� ���� ���� ����
	noiseRangeOutput.Center = enemyPos;
	noiseRangeOutput.Radius = enemyComp.NoiseRangeRadius;

	// �߰� ���� ����
	chaseRangeOutput.Center = enemyPos;
	chaseRangeOutput.Radius = enemyComp.ChaseRangeRadius;
}

void EnemyState::RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime)
{
	const float distance = targetDir.Length();
	targetDir.y = 0.f;

	const auto targetRotation = VPMath::Quaternion::LookRotation(targetDir, VPMath::Vector3::Up);
	const auto worldQuaternion = transform->World_Quaternion;

	constexpr float baseSpeed = 1.5f;
	const float speed = baseSpeed + (1.0f / distance) * 10.0f;	// �Ÿ��� ����� ȸ�� �ӵ� ����

	const auto rotation = VPMath::Quaternion::Slerp(worldQuaternion, targetRotation, deltaTime * speed);	// �ε巴�� ȸ��

	transform->SetWorldQuaternion(rotation);
}

void EnemyState::ChangeCurrentState(const std::shared_ptr<EnemyComponent>& enemyComponent, IState* newState)
{
	if (auto movementState = dynamic_cast<EnemyMovementState*>(newState)) {
		if (enemyComponent->MovementState != movementState)
			enemyComponent->MovementState->Exit(enemyComponent);

		enemyComponent->MovementState = movementState;
		enemyComponent->MovementState->Enter(enemyComponent);
	}
	else if (auto behaviorState = dynamic_cast<EnemyBehaviorState*>(newState)) {
		if (enemyComponent->BehaviorState != behaviorState)
			enemyComponent->BehaviorState->Exit(enemyComponent);

		enemyComponent->BehaviorState = behaviorState;
		enemyComponent->BehaviorState->Enter(enemyComponent);
	}
	//else if (auto combatState = dynamic_cast<EnemyCombatState*>(newState)) {
	//	enemyComponent->CombatState = combatState;
	//	enemyComponent->CombatState->Enter(enemyComponent);
	//}
	else {
		Log::GetClientLogger()->error("Unknown state type passed to ChangeState.");
	}
}

void EnemyState::ChangeCurrentState(EnemyComponent& enemyComponent, IState* newState)
{
	const std::shared_ptr<EnemyComponent> temp(&enemyComponent, null_deleter{});

	if (auto movementState = dynamic_cast<EnemyMovementState*>(newState)) {
		if (enemyComponent.MovementState != movementState)
			enemyComponent.MovementState->Exit(temp);

		enemyComponent.MovementState = movementState;
		enemyComponent.MovementState->Enter(temp);
	}
	else if (auto behaviorState = dynamic_cast<EnemyBehaviorState*>(newState)) {
		if (enemyComponent.BehaviorState != behaviorState)
			enemyComponent.BehaviorState->Exit(temp);

		enemyComponent.BehaviorState = behaviorState;
		enemyComponent.BehaviorState->Enter(temp);
	}
	//else if (auto combatState = dynamic_cast<EnemyCombatState*>(newState)) {
	//	enemyComponent->CombatState = combatState;
	//	enemyComponent->CombatState->Enter(temp);
	//}
	else {
		Log::GetClientLogger()->error("Unknown state type passed to ChangeState.");
	}
}

void EnemyState::ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop, bool isImmediate)
{
	// �����Ϸ��� �ִϸ��̼ǰ� ���� �ִϸ��̼��� ���ٸ� �ִϸ��̼� ������ �������� ����
	if (enemyComp.CurrentAni == animation)
		return;

	enemyComp.CurrentAni = animation;

	VisPred::Engine::AniBlendData temp{ enemyComp.GetEntityID(), static_cast<int>(animation), speed, isLoop };

	std::any data = temp;
	if (true == isImmediate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}
