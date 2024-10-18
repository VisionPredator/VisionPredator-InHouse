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

// 로우 포인터로 받은 객체를 스마트 포인터로 변환하였을 때 메모리 해제 동작을 하지 않도록 설정하기 위한 클래스
struct null_deleter
{
	void operator()(void const *) const {}
};

void EnemySystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<EnemyComponent>(gameObjectId))
		return;

	const auto enemyCompRawPtr = GetSceneManager()->GetComponent<EnemyComponent>(gameObjectId);
	const std::shared_ptr<EnemyComponent> enemyComp(enemyCompRawPtr, null_deleter{});	// null_deleter를 사용해 메모리 해제가 되지 않도록 스마트 포인터 생성

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

		// 상태 변경 예시
		enemycomp.MovementState = &EnemyMovementState::s_Jumping;

		Log::GetClientLogger()->info("Current State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));

		enemycomp.MovementState = &EnemyMovementState::s_Idle;

		Log::GetClientLogger()->info("Current State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));

	}
}

void EnemySystem::CalculateFSM(EnemyComponent& enemyComp)
{
	// 하위 State 들을 만드는데.
	// 이 State 들을 컨테이너에 넣어놓고
	// 각 상위 State 들 마다 이것들을 확인하여 그에 맞는 행동을 한다.
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
	// 현재 Scene에 플레이어가 없다면 아무것도 하지 않음
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
	// TODO: 애니메이션 출력이 끝났다면 일정 시간 후 씬에서 객체 삭제 또는 ObjectPool에 객체 반환

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

void EnemySystem::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
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

	// 범위 debug draw
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

	// 플레이어 위치 정보 얻어오기
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

	/// 시야 범위, 소음 범위, 추격 범위 별로 플레이어 감지 수행.

	// 각 범위에 들어 왔는지를 확인하고
	// 세부 조건을 확인한다.
	bool isInViewRange = false, isInNoiseRange = false, isInChaseRange = false;

	isInViewRange = viewRange.Contains(playerPos);
	isInNoiseRange = noiseRange.Contains(playerPos) && (m_PlayerComp->CurrentFSM == VisPred::Game::EFSM::RUN) || (m_PlayerComp->CurrentFSM == VisPred::Game::EFSM::JUMP) ? true : false;	// 최적화 필요.

	// 플레이어가 Enemy의 시야 범위 안에 들어와있을 때에만 레이캐스트 수행
	//if (viewRange.Contains(playerPos) || noiseRange.Contains(playerPos))	// TODO: 소음감지 범위 안에서 플레이어가 "움직였을 때" 를 확인해야 한다. 지금은 원안에 가만히 있기만 해도 감지됨.
	if (isInViewRange || isInNoiseRange)	// TODO: 소음감지 범위 안에서 플레이어가 "움직였을 때" 를 확인해야 한다. 지금은 원안에 가만히 있기만 해도 감지됨.
	{
		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);
		const uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActorFromLocation_Ignore(enemyID, enemyPos, targetDir, enemyComp.FarZ);
		if (detectedObjID == playerID)
		{
			ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Chase);
			RotateToTarget(transform, targetDir, deltaTime);
		}
	}
	else  // TODO: 뭔가.. 수정이 필요.
	{
		ChangeCurrentStateEnumValue(enemyComp, VisPred::Game::EnemyStates::Idle);
	}
}

void EnemySystem::CreateDetectionAreas(const EnemyComponent& enemyComp, const TransformComponent* transform, DirectX::BoundingFrustum& viewRangeOutput, DirectX::BoundingSphere& noiseRangeOutput, DirectX::BoundingSphere& chaseRangeOutput)
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

void EnemySystem::RotateToTarget(TransformComponent* transform, VisPred::SimpleMath::Vector3 targetDir, float deltaTime)
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
