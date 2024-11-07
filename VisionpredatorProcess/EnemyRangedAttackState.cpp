#include "pch.h"
#include "EnemyRangedAttackState.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "../PhysxEngine/IPhysx.h"
#include  "Components.h"
#include  "VisPredComponents.h"
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

	enemyComp->currentAttackTime += deltaTime;

	const auto playerTransform = enemyComp->Player->GetComponent<TransformComponent>();
	const auto playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};
	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	enemyComp->GetComponent<NavAgentComponent>()->IsChase = false;

	// TODO: 공격 쿨타임 조절할수있게 하기
	// 일정시간 마다 플레이어를 공격
	if ( enemyComp->currentAttackTime >= 1.f)
	//if (enemyComp->GetComponent<AnimationComponent>()->IsFinished && enemyComp->currentAttackTime >= 1.f)
	{
		enemyComp->currentAttackTime = 0.f;

		auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();
		// 사운드 출력
		//enemyComp->SceneManager.lock()->SpawnSoundEntity("Pistol",10,false,false , enemyPos);
		enemyComp->SceneManager.lock()->SpawnSoundEntity(soundComp->SoundKey_Attack,soundComp->Volume_Attack,false,false , enemyPos);

		// 이펙트 출력
		const auto particle = enemyComp->SceneManager.lock()->GetChildEntityComp_HasComp<ParticleComponent>(enemyComp->GetEntityID());
		if (particle != nullptr)
		{
			particle->IsRender = true;
			particle->Restart = true;
		}

		// 명중률 계산
		enemyComp->AttackAccuracy = CalculateAccuracy(*enemyComp);
		const float randomValue = static_cast<float>(rand() % 101);

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
		return;
	}
}

void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

float EnemyRangedAttackState::CalculateAccuracy(EnemyComponent& enemyComp)
{
	// 명중률에 관여하는 것
	// - 현재 플레이어가 이동중인지, WALk, RUN, JUMP
	// - distance 에 따라 감소.
	// 100% 에서 여러 명중률에 관여하는 수치에 따라 낮추거나 유지한다.

	float accuracy = 100.f; // 기본 명중률

	bool isPlayerMoving = enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::IDLE && enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::CROUCH;

	// 플레이어가 이동 중일 때 명중률 감소
	if (isPlayerMoving)
	{
		accuracy -= 20.0f;
	}

	// 거리별 명중률 감소
	if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeOne)
	{
		// 10m 이내일 때는 추가 감소 없음
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeTwo)
	{
		accuracy -= 10.0f;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeThree)
	{
		accuracy -= 20.0f;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeFour)
	{
		accuracy -= 30.0f;
	}
	else
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
