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
	// ���� Enemy�� Dead ���¶�� Ž���� �������� ����
	if (enemyComp.CurrentFSM == VisPred::Game::EnemyStates::Dead)
		return;

	const uint32_t enemyID = enemyComp.GetEntityID();
	const auto transform = enemyComp.GetComponent<TransformComponent>();

	// �þ� ����, ��������, �߰� ���� ����
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
	//if (viewRange.Contains(playerPos) || noiseRange.Contains(playerPos))	// TODO: �������� ���� �ȿ��� �÷��̾ "�������� ��" �� Ȯ���ؾ� �Ѵ�. ������ ���ȿ� ������ �ֱ⸸ �ص� ������.
	if (isInViewRange || isInNoiseRange)	// TODO: �������� ���� �ȿ��� �÷��̾ "�������� ��" �� Ȯ���ؾ� �Ѵ�. ������ ���ȿ� ������ �ֱ⸸ �ص� ������.
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
	else  // TODO: ����.. ������ �ʿ�.
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
