#include "pch.h"
#include "EnemyState.h"
#include "EngineStructs.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "../VPGraphics/D3DUtill.h"

#include "EnemyMovementState.h"
#include "EnemyCombatState.h"
#include "EnemyBehaviorState.h"
#include "EnemyJumpState.h"
#include "EnemyRunState.h"
#include "EnemyIdleState.h"
#include "EnemyChaseState.h"

void EnemyState::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
{
	// 현재 Enemy가 Dead 상태라면 탐지를 수행하지 않음
	if (enemyComp.CurrentFSM == VisPred::Game::EnemyStates::Dead)
		return;

	const uint32_t enemyID = enemyComp.GetEntityID();
	const auto transform = enemyComp.GetComponent<TransformComponent>();

	// 시야 범위, 소음범위, 추격 범위 생성
	DirectX::BoundingFrustum viewRange;
	DirectX::BoundingSphere noiseRange, chaseRange;
	CreateDetectionAreas(enemyComp, transform, viewRange, noiseRange, chaseRange);

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

	// 플레이어 위치 정보 얻어오기
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

	/// 시야 범위, 소음 범위, 추격 범위 별로 플레이어 감지 수행.

// 각 범위에 들어 왔는지를 확인하고
// 세부 조건을 확인한다.
	bool isInViewRange = false, isInNoiseRange = false, isInChaseRange = false;

	isInViewRange = viewRange.Contains(playerPos);
	isInNoiseRange = noiseRange.Contains(playerPos) && (enemyComp.Player->CurrentFSM == VisPred::Game::EFSM::RUN) || (enemyComp.Player->CurrentFSM == VisPred::Game::EFSM::JUMP) ? true : false;	// 최적화 필요.

	// 플레이어가 Enemy의 시야 범위 안에 들어와있을 때에만 레이캐스트 수행
	//if (viewRange.Contains(playerPos) || noiseRange.Contains(playerPos))	// TODO: 소음감지 범위 안에서 플레이어가 "움직였을 때" 를 확인해야 한다. 지금은 원안에 가만히 있기만 해도 감지됨.
	if (isInViewRange || isInNoiseRange)	// TODO: 소음감지 범위 안에서 플레이어가 "움직였을 때" 를 확인해야 한다. 지금은 원안에 가만히 있기만 해도 감지됨.
	{
		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);

		const uint32_t detectedObjID = enemyComp.PhysicsManager->RaycastToHitActorFromLocation_Ignore(enemyID, enemyPos, targetDir, enemyComp.FarZ);
		if (detectedObjID == playerID)
		{
			//ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Chase);
			enemyComp.BehaviorState = &EnemyBehaviorState::s_Chase;
			RotateToTarget(transform, targetDir, deltaTime);
		}
	}
	else  // TODO: 뭔가.. 수정이 필요.
	{
		enemyComp.BehaviorState = &EnemyBehaviorState::s_Idle;
		//ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Idle);
	}
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

void EnemyState::RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime)
{
	const float distance = targetDir.Length();
	targetDir.y = 0.f;

	const auto targetRotation = VPMath::Quaternion::LookRotation(targetDir, VPMath::Vector3::Up);
	const auto worldQuaternion = transform->World_Quaternion;

	constexpr float baseSpeed = 1.5f;
	const float speed = baseSpeed + (1.0f / distance) * 10.0f;	// 거리에 비례해 회전 속도 증가

	const auto rotation = VPMath::Quaternion::Slerp(worldQuaternion, targetRotation, deltaTime * speed);	// 부드럽게 회전

	transform->SetWorldQuaternion(rotation);
}

void EnemyState::ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop, bool isImmediate)
{
	// 변경하려는 애니메이션과 현재 애니메이션이 같다면 애니메이션 변경을 수행하지 않음
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
