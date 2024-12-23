#include "pch.h"
#include "EnemyState.h"
#include "EngineStructs.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "../VPGraphics/D3DUtill.h"
#include "SceneManager.h"
#include "ParticleOwnerComponent.h"
#include "StatesInclude.h"

struct null_deleter
{
	void operator()(void const*) const {}
};

float EnemyState::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
{
	if (enemyComp.BehaviorState == &EnemyBehaviorState::s_Dead)
		return -1.f;

	const uint32_t enemyID = enemyComp.GetEntityID();
	const auto transform = enemyComp.GetComponent<TransformComponent>();

	DirectX::BoundingFrustum viewRange;
	DirectX::BoundingSphere noiseRange, chaseRange;
	CreateDetectionAreas(enemyComp, transform, viewRange, noiseRange, chaseRange);

	DrawDebugDraw(enemyComp, viewRange, noiseRange, chaseRange);

	if (!enemyComp.Player)
		return -1.f;

	const uint32_t playerID = enemyComp.Player->GetEntityID();
	const auto playerTransform = enemyComp.Player->GetComponent<TransformComponent>();
	const VisPred::SimpleMath::Vector3 playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};

	constexpr float offsetY = 1.8f;
	VPMath::Vector3 enemyPos = transform->World_Location;
	enemyPos.y += 1.2f;

	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	/// 시야 범위, 소음 범위, 추격 범위 별로 플레이어 감지 수행.
	// 각 범위에 들어 왔는지를 확인하고 세부 조건을 확인한다.
	bool isPlayerDetectedInViewRange = false, isPlayerDetectedInNoiseRange = false, isDetectedInChaseRange = false;

	isPlayerDetectedInViewRange = viewRange.Contains(playerPos);
	const bool isInNoiseRange = noiseRange.Contains(playerPos);
	bool isPlayerMoving = enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::IDLE && enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::CROUCH;

	// 추격 범위 안에서 플레이어가 총을 쏘면 플레이어를 감지.
	isDetectedInChaseRange = chaseRange.Contains(playerPos);
	const bool isPlayerGunShoot = isDetectedInChaseRange && enemyComp.Player->IsAttacking;

	if (enemyComp.BehaviorState == &EnemyBehaviorState::s_Chase)
	{
		isPlayerMoving = true;
	}
	isPlayerDetectedInNoiseRange = isInNoiseRange && isPlayerMoving;

	isPlayerDetectedInNoiseRange |= isPlayerGunShoot;

	// TODO: 코드 정리 필요.
	// 플레이어가 추격 범위 내에서 공격을 하면 플레이어 방향으로 회전. 
	if (isPlayerGunShoot)
	{
		RotateToTarget(transform, targetDir, deltaTime);
	}

	// 플레이어로부터 공격을 받으면 플레이어 방향으로 회전한다.
	bool isHit = false;
	if (enemyComp.OnHit)
	{
		isHit = RotateToTarget(transform, targetDir, deltaTime);	// 거의 다 회전하였다면 OnHit 를 false로

		if (true == isHit)
		{

			const std::shared_ptr<EnemyComponent> temp(&enemyComp, null_deleter{});

			if (enemyComp.MovementState == &EnemyMovementState::s_HitReaction)
			//if (enemyComp.CurrentAni == VisPred::Game::EnemyAni::ATTACKED)
				enemyComp.MovementState->Enter(temp);
			else
				ChangeCurrentState(enemyComp, &EnemyMovementState::s_HitReaction);

			enemyComp.OnHit = false;
		}
		//else
		//{
		//	ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
		//}
	}

	// 감지 범위 내에서 플레이어가 감지되었다면 Chase 상태로 변환한다.
	if (isPlayerDetectedInViewRange || isPlayerDetectedInNoiseRange || isHit)
	{
		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);
		VPMath::Vector3 pose{};
		VPMath::Vector3 dir{};
	auto poscomp= 	enemyComp.SceneManager.lock()->GetChildEntityComp_HasComp<ParticleOwnerComponent>(enemyComp.GetEntityID());
	if (poscomp)
	{
		pose = poscomp->GetComponent<TransformComponent>()->World_Location;
		dir= playerPos - pose;
	}
	else
	{
		pose = enemyPos;
		dir = targetDir;
	}
		const uint32_t detectedObjID = enemyComp.PhysicsManager->RaycastActorAtPose_Ignore(enemyID, pose, dir, enemyComp.FarZ).EntityID;
		if (detectedObjID == playerID)
		{
			enemyComp.DistanceToPlayer = (playerPos - enemyPos).Length();

			if (enemyComp.BehaviorState != &EnemyBehaviorState::s_Chase)
			{
				ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Chase);
			}
			RotateToTarget(transform, targetDir, deltaTime);

			// 플레이어까지의 거리를 계산하고 반환
			return (playerPos - enemyPos).Length();
		}
	}
	else
	{
		if (enemyComp.BehaviorState != &EnemyBehaviorState::s_Idle)
		{
			ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Idle);
		}
	}

	return -1;
}

bool EnemyState::CheckIsDead(const std::shared_ptr<EnemyComponent>& enemyComponent)
{
	if (enemyComponent->HP > 0)
		return false;

	ChangeCurrentState(enemyComponent, &EnemyBehaviorState::s_Dead);
	return true;
}

void EnemyState::CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform,
	DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput,
	DirectX::BoundingSphere& chaseRangeOutput)
{
	constexpr float offsetY = 1.8f;
	VPMath::Vector3 enemyPos = { transform->WorldTransform._41, transform->WorldTransform._42 + offsetY, transform->WorldTransform._43 };

	// 시야 범위 생성
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

	// 소음 감지 범위 세팅
	noiseRangeOutput.Center = enemyPos;
	noiseRangeOutput.Radius = enemyComp.NoiseRangeRadius;

	// 추격 범위 세팅
	chaseRangeOutput.Center = enemyPos;
	chaseRangeOutput.Radius = enemyComp.ChaseRangeRadius;
}

float QuaternionDot(const VPMath::Quaternion& q1, const VPMath::Quaternion& q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

bool EnemyState::RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime)
{
	const float distance = targetDir.Length();
	targetDir.y = 0.f;

	const auto targetRotation = VPMath::Quaternion::LookRotation(targetDir, VPMath::Vector3::Up);
	const auto worldQuaternion = transform->World_Quaternion;

	constexpr float baseSpeed = 1.5f;
	const float speed = baseSpeed + (1.0f / distance) * 10.0f;	// 거리에 비례해 회전 속도 증가

	const auto rotation = VPMath::Quaternion::Slerp(worldQuaternion, targetRotation, deltaTime * speed);	// 부드럽게 회전

	transform->SetWorldQuaternion(rotation);

	float dot = QuaternionDot(rotation, targetRotation);

	return dot >= 0.95f;
}

void EnemyState::ChangeCurrentState(const std::shared_ptr<EnemyComponent>& enemyComponent, IState* newState)
{
	if (const auto movementState = dynamic_cast<EnemyMovementState*>(newState)) {
		if (enemyComponent->MovementState == movementState)
			return;

		if (enemyComponent->MovementState != movementState)
			enemyComponent->MovementState->Exit(enemyComponent);

		enemyComponent->MovementState = movementState;
		enemyComponent->MovementState->Enter(enemyComponent);
	}
	else if (const auto behaviorState = dynamic_cast<EnemyBehaviorState*>(newState)) {
		if (enemyComponent->BehaviorState == behaviorState)
			return;

		if (enemyComponent->BehaviorState != behaviorState)
			enemyComponent->BehaviorState->Exit(enemyComponent);

		enemyComponent->BehaviorState = behaviorState;
		enemyComponent->BehaviorState->Enter(enemyComponent);
	}
	else if (const auto combatState = dynamic_cast<EnemyCombatState*>(newState)) {
		if (enemyComponent->CombatState == combatState)
			return;

		if (enemyComponent->CombatState != combatState)
			enemyComponent->CombatState->Exit(enemyComponent);

		enemyComponent->CombatState = combatState;
		enemyComponent->CombatState->Enter(enemyComponent);
	}
	else {
		Log::GetClientLogger()->error("Unknown state type passed to ChangeState.");
	}
}

void EnemyState::ChangeCurrentState(EnemyComponent& enemyComponent, IState* newState)
{
	const std::shared_ptr<EnemyComponent> temp(&enemyComponent, null_deleter{});

	if (const auto movementState = dynamic_cast<EnemyMovementState*>(newState)) {
		if (enemyComponent.MovementState == movementState)
			return;

		if (enemyComponent.MovementState != movementState)
			enemyComponent.MovementState->Exit(temp);

		enemyComponent.MovementState = movementState;
		enemyComponent.MovementState->Enter(temp);
	}
	else if (const auto behaviorState = dynamic_cast<EnemyBehaviorState*>(newState)) {
		if (enemyComponent.BehaviorState == behaviorState)
			return;

		if (enemyComponent.BehaviorState != behaviorState)
			enemyComponent.BehaviorState->Exit(temp);

		enemyComponent.BehaviorState = behaviorState;
		enemyComponent.BehaviorState->Enter(temp);
	}
	else if (const auto combatState = dynamic_cast<EnemyCombatState*>(newState)) {
		if (enemyComponent.CombatState == combatState)
			return;

		if (enemyComponent.CombatState != combatState)
			enemyComponent.CombatState->Exit(temp);

		enemyComponent.CombatState = combatState;
		enemyComponent.CombatState->Enter(temp);
	}
	else {
		Log::GetClientLogger()->error("Unknown state type passed to ChangeState.");
	}
}

void EnemyState::ChangeCurrentAnimation(const std::shared_ptr<EnemyComponent>& enemyComp, VisPred::Game::EnemyAni animation, float speed, float transitionTime, bool isLoop, bool isAgain, bool isImmediate)
{
	if (enemyComp->CurrentAni == animation && !isAgain)
		return;

	enemyComp->CurrentAni = animation;

	VisPred::Engine::AniBlendData temp{ enemyComp->GetEntityID(), static_cast<int>(animation), speed, transitionTime, isLoop, isAgain };

	const std::any data = temp;
	if (true == isImmediate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}

void EnemyState::DrawDebugDraw(const EnemyComponent& enemyComp, DirectX::BoundingFrustum& viewRange,
	DirectX::BoundingSphere& noiseRange, DirectX::BoundingSphere& chaseRange)
{
	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum = viewRange;
	frustumInfo.Color = VisPred::SimpleMath::Color{ 1, 1, 0, 1 };
	enemyComp.Graphics->DrawFrustum(frustumInfo);

	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere = noiseRange;
	sphereInfo.Color = VPMath::Color{ 1, 0, 1, 1 };
	enemyComp.Graphics->DrawSphere(sphereInfo);

	DirectX::BoundingSphere temp = noiseRange;
	temp.Radius = enemyComp.AccuracyRangeOne;
	debug::SphereInfo sphereInfo1;
	sphereInfo1.Sphere = temp;
	sphereInfo1.Color = VPMath::Color{ 1.0f, 1.0f, 0.0f, 1.0f }; // 진한 노란색
	enemyComp.Graphics->DrawSphere(sphereInfo1);

	temp.Radius = enemyComp.AccuracyRangeTwo;
	debug::SphereInfo sphereInfo2;
	sphereInfo2.Sphere = temp;
	sphereInfo2.Color = VPMath::Color{ 1.0f, 0.9f, 0.2f, 1.0f }; // 약간 연한 노란색
	enemyComp.Graphics->DrawSphere(sphereInfo2);

	temp.Radius = enemyComp.AccuracyRangeThree;
	debug::SphereInfo sphereInfo3;
	sphereInfo3.Sphere = temp;
	sphereInfo3.Color = VPMath::Color{ 1.0f, 0.8f, 0.4f, 1.0f }; // 더 연한 노란색
	enemyComp.Graphics->DrawSphere(sphereInfo3);

	temp.Radius = enemyComp.AccuracyRangeFour;
	debug::SphereInfo sphereInfo4;
	sphereInfo4.Sphere = temp;
	sphereInfo4.Color = VPMath::Color{ 1.0f, 0.7f, 0.6f, 1.0f }; // 가장 연한 노란색
	enemyComp.Graphics->DrawSphere(sphereInfo4);
}


void EnemyState::ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, float transitionTime, bool isLoop, bool isAgain, bool isImmediate)
{
	if (enemyComp.CurrentAni == animation && !isAgain)
		return;

	enemyComp.CurrentAni = animation;

	VisPred::Engine::AniBlendData temp{ enemyComp.GetEntityID(), static_cast<int>(animation), speed, transitionTime, isLoop };

	const std::any data = temp;
	if (true == isImmediate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}
