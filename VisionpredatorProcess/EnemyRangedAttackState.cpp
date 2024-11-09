#include "pch.h"
#include "EnemyRangedAttackState.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "../PhysxEngine/IPhysx.h"
#include  "Components.h"
#include  "VisPredComponents.h"
#include <random>
void EnemyRangedAttackState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter RangedAttackState");

	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyRangedAttackState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);	/// TEMP!!!!!!!
	const auto transform = enemyComp->GetComponent<TransformComponent>();
	const auto enemyPos = transform->World_Location;
	auto rotation = transform->World_Rotation;

	enemyComp->AttackCycleTimer += deltaTime;
	enemyComp->ConsecutiveAttackTimer += deltaTime;

	const auto playerTransform = enemyComp->Player->GetComponent<TransformComponent>();
	const auto playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};
	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	// 난수 생성기 초기화
	std::random_device rd;  // 하드웨어 엔트로피를 기반으로 시드 생성
	std::mt19937 gen(rd()); // Mersenne Twister 난수 생성기

	// 일정시간 마다 플레이어를 공격
	if (enemyComp->AttackCycleTimer >= enemyComp->AttackCycleDelay)
	{
		// 연속 발사 타이머가 충족되고 남은 연속 공격 횟수가 있을 때만 발사.
		if (enemyComp->ConsecutiveAttackTimer >= enemyComp->ConsecutiveAttackDelay && enemyComp->ShotCount > 0)
		{
			enemyComp->ConsecutiveAttackTimer = 0;
			enemyComp->ShotCount--;

			if (enemyComp->ShotCount <= 0)
			{
				// 타이머 초기화
				enemyComp->AttackCycleTimer = 0.f;
				if (enemyComp->MaxShotPerBurst <= 0)	// 예외처리
				{
					enemyComp->MaxShotPerBurst = 1;
				}
				std::uniform_int_distribution dist(1, enemyComp->MaxShotPerBurst);
				enemyComp->ShotCount = dist(gen);
			}

			const auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();

			// 공격 사운드 출력
			enemyComp->SceneManager.lock()->SpawnSoundEntity(
				soundComp->SoundKey_Attack, 
				soundComp->Volume_Attack, 
				false, 
				false, 
				enemyPos
			);

			// 공격 이펙트 출력
			const auto particle = enemyComp->SceneManager.lock()->GetChildEntityComp_HasComp<ParticleComponent>(enemyComp->GetEntityID());
			if (particle != nullptr)
			{
				particle->IsRender = true;
				particle->Restart = true;
			}

			// 명중률 계산
			enemyComp->AttackAccuracy = CalculateAccuracy(*enemyComp);

			std::random_device rd;  // 하드웨어 엔트로피를 기반으로 시드 생성
			std::mt19937 gen(rd()); // Mersenne Twister 난수 생성기
			std::uniform_int_distribution dist(0, 100);
			const float randomValue = static_cast<float>(dist(gen));

			// 명중률에 따라 사격
			if (randomValue <= enemyComp->AttackAccuracy)
			{
				const uint32_t detectedObjID = enemyComp->PhysicsManager->RaycastActorAtPose_Ignore(enemyComp->GetEntityID(), enemyPos, targetDir, enemyComp->FarZ).EntityID;
				if (detectedObjID == enemyComp->Player->GetEntityID())
				{
					EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair<uint32_t, int >(enemyComp->Player->GetEntityID(), enemyComp->AttackPower));
				}
			}
			ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
		}
	}

	if (enemyComp->ShotCount <= 0)
	{
		// 타이머 초기화
		enemyComp->AttackCycleTimer = 0.f;
		if (enemyComp->MaxShotPerBurst <= 0)	// 예외처리
		{
			enemyComp->MaxShotPerBurst = 1;
		}
		std::uniform_int_distribution dist(1, enemyComp->MaxShotPerBurst);
		enemyComp->ShotCount = dist(gen);
	}

}

void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

float EnemyRangedAttackState::CalculateAccuracy(const EnemyComponent& enemyComp)
{
	// 명중률에 관여하는 것
	// - 현재 플레이어가 이동중인지, WALk, RUN, JUMP
	// - distance 에 따라 감소.
	// 100% 에서 여러 명중률에 관여하는 수치에 따라 낮추거나 유지한다.

	float accuracy = 100.f; // 기본 명중률

	const bool isPlayerWalking = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::WALK;
	const bool isPlayerRunning = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::RUN;
	const bool isPlayerJumping = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::JUMP;

	// 거리별 명중률 감소
	if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeOne)
	{
		// 10m 이내일 때는 추가 감소 없음
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeTwo)
	{
		accuracy -= enemyComp.AccuracyPenaltyOne;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeThree)
	{
		accuracy -= enemyComp.AccuracyPenaltyTwo;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeFour)
	{
		accuracy -= enemyComp.AccuracyPenaltyThree;
	}
	else
	{
		accuracy -= enemyComp.AccuracyPenaltyFour;
	}

	// 플레이어가 이동 중일 때 명중률 감소
	if (isPlayerWalking || isPlayerJumping)
	{
		accuracy -= 20.0f;
	}
	else if (isPlayerRunning)
	{
		accuracy -= 40.0f;
	}

	// 명중률이 0 이하로 내려가지 않도록 보장
	if (accuracy < 0.0f)
	{
		accuracy = 0.0f;
	}

	return accuracy;
}
