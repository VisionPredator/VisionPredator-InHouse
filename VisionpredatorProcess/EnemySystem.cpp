#include "pch.h"
#include "EnemySystem.h"

#include <memory>

#include "VisPredComponents.h"
#include "EngineStructs.h"
#include "../VPGraphics/D3DUtill.h"

#include "EnemyMovementState.h"
#include "EnemyJumpState.h"
#include "StatesInclude.h"

//#include "SceneManager.h"

void EnemySystem::Initialize()
{
	COMPLOOP(PlayerComponent, comp)
	{

		//Start(comp.GetEntityID());
		uint32_t playerID = comp.GetEntityID();
		if (GetSceneManager()->HasComponent<PlayerComponent>(playerID))
		{
			m_PlayerComp = GetSceneManager()->GetComponent<PlayerComponent>(playerID);
			break;
		}
	}

	for (auto& iter : m_SceneManager.lock()->GetComponentPool<IdentityComponent>())
	{
		iter.get().UUID;
	}

	COMPLOOP(EnemyComponent, comp)
	{
		Start(comp.GetEntityID());

		comp.Player = m_PlayerComp;
	}
}

// �ο� �����ͷ� ���� ��ü�� ����Ʈ �����ͷ� ��ȯ�Ͽ��� �� �޸� ���� ������ ���� �ʵ��� �����ϱ� ���� Ŭ����
struct null_deleter
{
	void operator()(void const *) const {}
};

void EnemySystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<EnemyComponent>(gameObjectId))
		return;

	const auto enemyCompRawPtr = GetSceneManager()->GetComponent<EnemyComponent>(gameObjectId);
	const std::shared_ptr<EnemyComponent> enemyComp(enemyCompRawPtr, null_deleter{});	// null_deleter�� ����� �޸� ������ ���� �ʵ��� ����Ʈ ������ ����

	enemyCompRawPtr->SceneManager = m_SceneManager;
	enemyCompRawPtr->PhysicsManager = m_PhysicsEngine;
	enemyComp->MovementState->Enter(enemyComp);
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, enemycomp)
	{
		//HPCheck(enemycomp);
		DetectTarget(enemycomp, deltaTime);
		CalculateFSM(enemycomp);

		//enemycomp.testState->Update(deltaTime);

		// ���� ���� ����
		enemycomp.MovementState = &EnemyMovementState::s_Jumping;

		Log::GetClientLogger()->info("Current State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));

		enemycomp.MovementState = &EnemyMovementState::s_Idle;

		Log::GetClientLogger()->info("Current State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));

	}
}

void EnemySystem::CalculateFSM(EnemyComponent& enemyComp)
{
	// ���� State ���� ����µ�.
	// �� State ���� �����̳ʿ� �־����
	// �� ���� State �� ���� �̰͵��� Ȯ���Ͽ� �׿� �´� �ൿ�� �Ѵ�.
	// ex) 

	switch (enemyComp.CurrentFSM)
	{
		case VisPred::Game::EnemyStates::Idle:
			CalculateIdle(enemyComp);
			break;
		case VisPred::Game::EnemyStates::Chase:
			CalculateChase(enemyComp);
			break;
		case VisPred::Game::EnemyStates::Patrol:
			CalculatePatrol(enemyComp);
			break;
		case VisPred::Game::EnemyStates::Dead:
			CalculateDie(enemyComp);
			break;
		default:
			break;
	}
}

void EnemySystem::CalculateIdle(EnemyComponent& enemyComp)
{
	// Disable Player Chase
	uint32_t enemyID = enemyComp.GetEntityID();
	if (true == m_SceneManager.lock()->HasComponent<NavAgentComponent>(enemyID))
	{
		auto navComp = m_SceneManager.lock()->GetComponent<NavAgentComponent>(enemyID);
		navComp->IsChase = false;
	}

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::IDLE, 1.f);
}

void EnemySystem::CalculateChase(EnemyComponent& enemyComp)
{
	// ���� Scene�� �÷��̾ ���ٸ� �ƹ��͵� ���� ����
	if (!m_PlayerComp)
		return;

	// Enable Player Chase
	uint32_t enemyID = enemyComp.GetEntityID();
	if (true == m_SceneManager.lock()->HasComponent<NavAgentComponent>(enemyID))
	{
		auto navComp = m_SceneManager.lock()->GetComponent<NavAgentComponent>(enemyID);
		navComp->IsChase = true;
	}

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::CHASE, 2.f);
}

void EnemySystem::CalculatePatrol(EnemyComponent& enemyComp)
{
}

void EnemySystem::CalculateDie(EnemyComponent& enemyComp)
{
	// TODO: �ִϸ��̼� ����� �����ٸ� ���� �ð� �� ������ ��ü ���� �Ǵ� ObjectPool�� ��ü ��ȯ

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::DIE, 2.f, false);
}

void EnemySystem::Die(EnemyComponent& enemyComp)
{
	if (enemyComp.HasComponent<AnimationComponent>())
	{
		auto ani = enemyComp.GetComponent<AnimationComponent>();
		ani->isLoop = false;
	}

	m_PhysicsEngine->RemoveController(enemyComp.GetEntityID());
}

void EnemySystem::ChangeCurrentAnimation(EnemyComponent& enemyComp, VisPred::Game::EnemyAni animation, float speed, bool isLoop, bool isImmediate)
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

void EnemySystem::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
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

	// ���� debug draw
#ifdef _DEBUG 
	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum = viewRange;
	frustumInfo.Color = VisPred::SimpleMath::Color{ 1, 1, 0, 1 };
	m_Graphics->DrawFrustum(frustumInfo);

	debug::SphereInfo sphereInfo;
	sphereInfo.Sphere = noiseRange;
	sphereInfo.Color = VPMath::Color{ 1, 0, 1, 1 };
	m_Graphics->DrawSphere(sphereInfo);
#endif _DEBUG

	if (!m_PlayerComp)
		return;

	for (auto it : m_SceneManager.lock()->GetComponentPool<IdentityComponent>())
	{
		
	}

	// �÷��̾� ��ġ ���� ������
	const uint32_t playerID = m_PlayerComp->GetEntityID();
	const auto playerTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(playerID);
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
	isInNoiseRange = noiseRange.Contains(playerPos) && (m_PlayerComp->CurrentFSM == VisPred::Game::EFSM::RUN) || (m_PlayerComp->CurrentFSM == VisPred::Game::EFSM::JUMP) ? true : false;	// ����ȭ �ʿ�.

	// �÷��̾ Enemy�� �þ� ���� �ȿ� �������� ������ ����ĳ��Ʈ ����
	//if (viewRange.Contains(playerPos) || noiseRange.Contains(playerPos))	// TODO: �������� ���� �ȿ��� �÷��̾ "�������� ��" �� Ȯ���ؾ� �Ѵ�. ������ ���ȿ� ������ �ֱ⸸ �ص� ������.
	if (isInViewRange || isInNoiseRange)	// TODO: �������� ���� �ȿ��� �÷��̾ "�������� ��" �� Ȯ���ؾ� �Ѵ�. ������ ���ȿ� ������ �ֱ⸸ �ص� ������.
	{
		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);
		const uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActorFromLocation_Ignore(enemyID, enemyPos, targetDir, enemyComp.FarZ);
		if (detectedObjID == playerID)
		{
			ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Chase);
			RotateToTarget(transform, targetDir, deltaTime);
		}
	}
	else  // TODO: ����.. ������ �ʿ�.
	{
		ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Idle);
	}
}

void EnemySystem::CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput)
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

void EnemySystem::RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime)
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
